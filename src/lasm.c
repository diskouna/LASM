#include <stdio.h>
#include <stdlib.h>

#include "lasm_file.h"
#include "lasm_string.h"
#include "lasm_instruction.h"
#include "lasm_parse.h"
#include "lasm_translate.h"

static inline int  checkSyntax(int argc);
static inline void printCorrectSyntax(char *programName);

int firstPass(char *inputFileName);
int secondPass(char *outputFileName);

int main(int argc, char * argv[])
{
    int isSyntaxCorrect = checkSyntax(argc);
    if (isSyntaxCorrect) {
        int doesFirstPassSucceed = firstPass(argv[1]);
        if (doesFirstPassSucceed) {      
            secondPass(argv[2]);
        } else {                    
            printErrorMessage("First pass failed\n"); 
            writeErrorMessageInFile(); // ?? not here 
            // reportError() 
        }
        closeAllFiles();   // not here  
    } else {
        printErrorMessage("Incorrect syntax\n"); 
        printCorrectSyntax(argv[0]);
    }
    return EXIT_SUCCESS;
}

static inline void printCorrectSyntax(char *programName)
{
    printf("syntax : %s input.asm output.obj\n", programName);
}
static inline int checkSyntax(int argumentsCount)
{
    return (argumentsCount == 3);
}

int firstPass(char *inputFileName)
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
                printErrorMessage("at line %d : ", lineCount);
                printParseErrorMessage(status);
                return 0;
            }
            break;    
        }
    }  
    if (status.type != END_OF_FILE_REACH) {
        printErrorMessage("End of file [.END] not reach \n");
        return 0;
    }
    return 1;
}

// execute secondPass() if only if firstPass() was succesfull
int secondPass(char *outputFileName)
{

    output = fopen(outputFileName, "wb");
    if (output == NULL) {
        fprintf(stderr, "Cannot create the objet file : %s", outputFileName);
        exit(EXIT_FAILURE);
    }  
    rewind(intermediateFile);
    while (!feof(intermediateFile)) {
        getNextLineFromFile(intermediateFile);
        programCounter++;
        // TODO
        // verbose translation --> option 
        // printf("%04hx : %s \n", programCounter, line);
        /* translate the line */
        //uint16_t instruction = 0;
        char *token = getFirstToken(line);
        char *arguments;
        while (token != NULL) {
            //TODO : deep analysis of this loop 
            // make sure it not exit too soon
            if (isStringValidAsmDirective(token))  {
                directiveType type = getDirectiveType(token);
                arguments = getArguments();
                addDirectiveToOutputFile[type](arguments);
                // TODO : status --> label 
            } else if (isStringValidOpcode(token)) {
                instructionType type = getInstructionType(token);
                arguments = getArguments();
                addInstructionToOutputFile[type](arguments);
                // TODO : status 
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
