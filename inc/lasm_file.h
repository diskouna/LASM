#ifndef __LASM_FILE__
#define __LASM_FILE__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "lasm_hash.h"



extern char *programName, *inputFileName, *outputFileName;
extern FILE *input, *symbolTable, *intermediateFile, *output;

#define UNDEFINED_ADDRESS -1
extern int programCounterStart;
extern uint16_t programCounter;  // address of the **next** instruction to be executed
                                 // not the current one 
extern unsigned int instructionCount;

void openFile(char *fileName);
int isEndOfFileReach(FILE *file);
void closeAllFiles();
void createSymbolTableFile();
void writeSymbolTableHeader();
void writeSymbolTableFooter();
hashAddStatus addLabelToSymbolTable(char *label);
void createObjectFile(char *outputFileName);
int writeWordToObjectFile(FILE *binaryFile, uint16_t value);
void createIntermediateFile();
void writeErrorMessageInFile();
void printErrorMessage(const char *message,...);


// LINE BUFFER 
#define LINE_MAX 200
extern char line[LINE_MAX]; 

// write into "line" buffer 
void getNextLineFromFile(FILE *file);
// write into "line" buffer
void addLineToFile(FILE *file, char *line);

#endif // __LASM_FILE__
