#include <stdio.h>
#include <stdlib.h>

#include "lasm_file.h"
#include "lasm_string.h"
#include "lasm_instruction.h"
#include "lasm_parse.h"
#include "lasm_translate.h"

static inline int  checkSyntax(int argumentsCount, char *arguments[]);
static inline void printCorrectSyntax();

int firstPass(void);
int secondPass(void);
// TOFO : add extern variable in .c files that need it
int main(int argc, char * argv[])
{
    int isSyntaxCorrect = checkSyntax(argc, argv);
    if (isSyntaxCorrect) {
        int doesFirstPassSucceed = firstPass();
        if (doesFirstPassSucceed) {      
            secondPass();
        } else {                    
            printErrorMessage("First pass failed\n"); 
            writeErrorMessageInFile(); // ?? not here 
            // reportError() 
        }
        closeAllFiles();   // not here  
    } else {
        printErrorMessage("incorrect syntax\n"); 
        printCorrectSyntax();
    }
    return EXIT_SUCCESS;
}

static inline void printCorrectSyntax()
{
    printf("usage : %s input.asm [output.obj]\n", programName);
}

static inline int checkSyntax(int argumentsCount, char *arguments[])
{
    programName = arguments[0];
    if (argumentsCount == 2 || argumentsCount == 3) {
        inputFileName = arguments[1];
        if (argumentsCount == 3) 
            outputFileName = arguments[2];
        else 
           // default output file name
            outputFileName = "a.obj"; 
        return 1;
    } 
    return 0;
}

int firstPass(void)
{
    openFile(inputFileName);
    createSymbolTableFile();
    writeSymbolTableHeader();
    createIntermediateFile();
    /* parse File */
    unsigned int lineCount = 0;
    parseStatus status = {.type = UNDEFINED_PROGRAM_STARTING_ADDRESS, .token = NULL};
    while (!isEndOfFileReach(input)) {
        getNextLineFromFile(input);
        lineCount++;
        removeCommentInLine(line);
        if (!isBlankString(line)) {
            addLineToFile(intermediateFile, line);   // must precede parseLine() because
                                                     // parseLine() alters the "line" string       
            status = parseLine(line);
            if (status.type == SUCCESS) continue;
            if (status.type != END_OF_FILE_REACH) {
                printErrorMessage("%s:%d: ", inputFileName, lineCount);
                printParseErrorMessage(status);
                return 0;
            }
            break;    
        }
    }  
    if (status.type != END_OF_FILE_REACH) {
        printErrorMessage("End of file [.END] not reach\n");
        return 0;
    }
    return 1;
}

// execute secondPass() if only if firstPass() was succesfull
int secondPass(void)
{
    createObjectFile(outputFileName);
    rewind(intermediateFile);
    while (!feof(intermediateFile)) {
        getNextLineFromFile(intermediateFile);
        programCounter++;
        translateStatus status; 
        // TODO
        // verbose translation --> option 
        // translateLine() function
        char *token = getFirstToken(line);
        char *arguments;
        while (token != NULL) {
            if (isStringValidAsmDirective(token))  {
                directiveType type = getDirectiveType(token);
                arguments = getArguments();
                status = translateDirectiveIntoOutputFile[type](arguments);
                if (status==TRANSLATE_FAILURE) 
                    return 0;
            } else if (isStringValidOpcode(token)) {
                instructionType type = getInstructionType(token);
                arguments = getArguments();
                status = translateInstructionIntoOutputFile[type](arguments);
                if (status==TRANSLATE_FAILURE) 
                    return 0; 
            } 
            token = getNextToken();   
        } 
    }
    return 1;
}


// TODO ;
//  modify isPCOffset --> include PC in parameter 
// currentMemoryLocation
// translateInstructionIntoMachineCode
// getLineFromFile()
// writeToFile()
// openFile()
