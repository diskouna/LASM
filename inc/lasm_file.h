#ifndef __LASM_FILE__
#define __LASM_FILE__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "lasm_hash.h"

// LINE BUFFER 
#define LINE_MAX 200
extern char line[LINE_MAX]; // buffer line

extern FILE *input, *symbolTable, *intermediateFile, *output;
extern unsigned int instructionCount;
extern int programCounterStart;
extern uint16_t programCounter;  // address of the **next** instruction to be executed
                                 // not the current one 
  
size_t writeWordToBinaryFile(FILE *binaryFile, uint16_t value);

void writeSymbolTableHeader();
void writeSymbolTableFooter();
void writeErrorMessageInFile();

int isEndOfFileReach(FILE *file);
void createIntermediateFile();
void createSymbolTableFile();
void openFile(char *fileName);
void closeAllFiles();
void printErrorMessage(const char *message,...);

void getNextLineFromFile(FILE *file);
//void removeCommentInLine(char *line);
void addLineToFile(FILE *file, char *line);


hashAddStatus addLabelToSymbolTable(char *label);

#endif // __LASM_FILE__
