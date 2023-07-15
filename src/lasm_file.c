#include <stdarg.h>

#include "lasm_file.h"
#include "lasm_hash.h"

// Global variables used by different file
char line[LINE_MAX]; // buffer line

char *programName, *inputFileName, *outputFileName;
FILE *input, *symbolTable, *intermediateFile, *output;

int programCounterStart = UNDEFINED_ADDRESS;
unsigned int instructionCount;
uint16_t programCounter;  // address of the **next** instruction to be executed
                          // not the current one 


// A word is a 16 bit value
static inline uint16_t swapByteInWord(uint16_t n)
{
    return ((n << 8) | (n >> 8) );
}
static inline char firstByteStoredInUint16_t(uint16_t value)
{
    return *(char*)&value;
}
static inline int isLittleEndian() // not function ---> macro
{
    return (firstByteStoredInUint16_t((uint16_t)1) == 0x01);
}

void printSyntaxErrorMessage(int instructionLine)
{
    fprintf(stderr, "invalid syntax at line %d  \n", instructionLine);
}
void writeSymbolTableHeader()
{
    fprintf(symbolTable, " _____________________________________________ \n");
    fprintf(symbolTable, "|   %12s         |      address       |\n", "symbol");
    fprintf(symbolTable, "|------------------------|--------------------|\n");
}
void writeSymbolTableFooter()
{
    fprintf(symbolTable, "|________________________|____________________|\n");
}

void openFile(char *fileName)
{
    input = fopen(fileName, "r");
    if (input == NULL) {
        fprintf(stderr, "cannot open the file [%s] \n", fileName);
        exit(EXIT_FAILURE);    
    }
}
void createSymbolTableFile() 
{
    symbolTable = fopen("../build/symbol_table.txt", "w");
    if (symbolTable == NULL) {
        fprintf(stderr, "Failed to create the symbol table \n");
        exit(EXIT_FAILURE);
    } 
}

void createObjectFile(char *outputFileName) 
{
    output = fopen(outputFileName, "wb");
    if (output == NULL) {
        fprintf(stderr, "Cannot create the objet file : %s", outputFileName);
        exit(EXIT_FAILURE);
    }
}

void writeErrorMessageInFile() {
    fprintf(intermediateFile, "    Oops!!!    An error occurs   \n");
    fprintf(symbolTable, "|         Oops!!!    An error occurs          |\n");
}

// must be in first/second pass   
void closeAllFiles() // out ??? 
{
    writeSymbolTableFooter();
    fclose(symbolTable);
    fclose(intermediateFile);      
    fclose(input);
}

// 1 word == 16 bits
int writeWordToObjectFile(FILE *binaryFile, uint16_t value)
{                                                      // the representation of 1 in 16 bits
    if (isLittleEndian())
        value = swapByteInWord(value);
    return (fwrite(&value, sizeof(uint16_t), 1, binaryFile) == 1);
}

int isEndOfFileReach(FILE *file)
{
    return feof(file);
}

// TODO : absolute path for file

void createIntermediateFile(/* todo : char *inputFileName*/)
{
    intermediateFile = fopen("../build/intermediate_file.asm", "w+");
    if (intermediateFile == NULL) {
        fprintf(stderr, "preprocessor failed to create the intermediateFile.asm file \n");
        exit(EXIT_FAILURE);
    }
} 

void addLineToFile(FILE *file, char *line) 
{
    fprintf(file, "%s\n", line);
}

void getNextLineFromFile(FILE *file)
{
    static unsigned int lineCount = 0;
    lineCount++;

    char *c = line;
    int len = 0;
    while ((len < LINE_MAX) && ((*c = getc(file)) != EOF) && (*c != '\n')) {
        c++;
        len++;
    }
    *c = '\0';   
}

void printErrorMessage(const char *message,...)
{
    va_list formatOptions;
    va_start(formatOptions, message);
    vfprintf(stdout, message, formatOptions);
    va_end(formatOptions);
}

hashAddStatus addLabelToSymbolTable(char *label)
{
    hashAddStatus status = addItemToHashTable(label, instructionCount + programCounterStart - 1);
    if (status == HASH_ADD_SUCCESS)
        fprintf(symbolTable, "|   %-20s |       0x%04X       |\n", label, instructionCount + programCounterStart -1);
    return status;
}
