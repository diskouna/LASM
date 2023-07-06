#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "lasm_translate.h"
#include "lasm_instruction.h"
#include "lasm_string.h"
#include "lasm_file.h"


void (*addInstructionToOutputFile[])(char *arguments) = {
    &addAddInstructionToOutputFile,
    &addAndInstructionToOutputFile,
    &addBrInstructionToOutputFile,   
    &addBrnInstructionToOutputFile,   
    &addBrnpInstructionToOutputFile,   
    &addBrnzInstructionToOutputFile,   
    &addBrnzpInstructionToOutputFile,   
    &addBrpInstructionToOutputFile,   
    &addBrzInstructionToOutputFile,   
    &addBrzpInstructionToOutputFile, 
    &addGetcInstructionToOutputFile,
    &addHaltInstructionToOutputFile,
    &addInInstructionToOutputFile,
    &addJmpInstructionToOutputFile,
    &addJsrInstructionToOutputFile,
    &addJsrrInstructionToOutputFile,
    &addLdInstructionToOutputFile,
    &addLdiInstructionToOutputFile,
    &addLdrInstructionToOutputFile,
    &addLeaInstructionToOutputFile,
    &addNotInstructionToOutputFile,
    &addOutInstructionToOutputFile,
    &addPutsInstructionToOutputFile,
    &addPutspInstructionToOutputFile,
    &addRetInstructionToOutputFile,
    &addRtiInstructionToOutputFile,
    &addStInstructionToOutputFile,
    &addStiInstructionToOutputFile,
    &addStrInstructionToOutputFile,
    &addTrapInstructionToOutputFile
};

void (*addDirectiveToOutputFile[])(char *arguments) = {
    // theses functions alter the program counter in different way 
    &addBlkwDirectiveToOutputFile,
    &addEndDirectiveToOutputFile,
    &addFillDirectiveToOutputFile,
    &addOrigDirectiveToOutputFile,
    &addStringzDirectiveToOutputFile
};


void addOrigDirectiveToOutputFile(char *arguments)
{
    arguments = arguments; // avoid compiler unused arguments warning
    
    programCounter = programCounterStart;  
    // programCounterStart is already computed during the first pass
    uint16_t value = (uint16_t)programCounterStart; 
    writeWordToBinaryFile(output, value);
}
void addFillDirectiveToOutputFile(char *arguments){
    char *token = getFirstToken(arguments);
    // token is either a number or a label 
    uint16_t value;
    if (isStringValidWord(token)) {
        value = (uint16_t)convertStringToNumber(token);
    } else {
        hashSearchResult result = searchLabelInHashTable(token);
        if (result.status == HASH_SEARCH_SUCCESS) {
            value = result.address;
        } else {
            printErrorMessage("Invalid Number Or Undefined reference to label [%s] \n", token);
            fclose(output);
            deleteHashTable();
            exit(EXIT_FAILURE);
        }
    }
    writeWordToBinaryFile(output, value);
}

void addBlkwDirectiveToOutputFile(char *arguments)
{
    uint16_t numberOfBlocks = convertStringToNumber(arguments);  
    // set aside "numberOfBlocks" words = (2*numberOfBlocks) Bytes
    programCounter += (numberOfBlocks - 1);
    fseek(output, 2 * numberOfBlocks, SEEK_CUR);          
}

void addStringzDirectiveToOutputFile(char *arguments)
{
    char *s = strpbrk(arguments, "\"");
    s++;
    uint16_t value;
    while (*s != '\"') {
        value = (uint16_t)*s;
        programCounter++;
        writeWordToBinaryFile(output, value);
        s++;
    }
    value = 0x0000; // null character
    writeWordToBinaryFile(output, value);
}
void addEndDirectiveToOutputFile(char *arguments){
    arguments = arguments; // avoid compiler unused arguments warning
    // ??? use TRAP x25 ??? 
    fclose(output);
    deleteHashTable();
}

void addAddInstructionToOutputFile(char *arguments)
{
    uint16_t instruction = ADD_OPCODE << 12;
    int r0, r1, r2, imm5;
    char *token = getFirstToken(arguments);
    r0 = convertRegisterToNumber(token);
    instruction |= (r0 << 9);
    token = getNextToken();      
    r1 = convertRegisterToNumber(token);
    instruction |= (r1 << 6);
    token = getNextToken();
    if (isStringValidRegister(token)) {
        r2 = convertRegisterToNumber(token);
        instruction |= r2;
    } else {
        imm5 = convertStringToNumber(token);
        instruction |= (1 << 5);
        instruction |= (0x001F & imm5);
    }
    writeWordToBinaryFile(output, instruction);
}
void addAndInstructionToOutputFile(char *arguments)
{
    uint16_t instruction = AND_OPCODE << 12;
    int r0, r1, r2, imm5;
    char *token = getFirstToken(arguments);
    r0 = convertRegisterToNumber(token);
    instruction |= (r0 << 9);
    token = getNextToken();      
    r1 = convertRegisterToNumber(token);
    instruction |= (r1 << 6);
    token = getNextToken();
    if (isStringValidRegister(token)) {
        r2 = convertRegisterToNumber(token);
        instruction |= r2;
    } else {
        imm5 = convertStringToNumber(token);
        instruction |= (1 << 5);
        instruction |= (0x001F & imm5);
    }
    writeWordToBinaryFile(output, instruction);
}

void addBrsInstructionToOutputFile(char *arguments, uint16_t nzpValue)
{
    uint16_t instruction =  BR_OPCODE;
    instruction |=  nzpValue;
    char *token = getFirstToken(arguments); // important : discard trailing spaces
    hashSearchResult result = searchLabelInHashTable(token);
    if (result.status == HASH_SEARCH_SUCCESS) {
        uint16_t labelAddress = result.address;
        instruction |= 0x001FF & (labelAddress - programCounter);    //PCOffset9
        writeWordToBinaryFile(output, instruction);
    } else {   
        printErrorMessage("Undefined reference to label [%s] \n", token);
        fclose(output);
        deleteHashTable();
        exit(EXIT_FAILURE);
    }
}

void addBrInstructionToOutputFile(char *arguments)
{
    uint16_t nzpValue = (0x7 << 9); // make it macro
    addBrsInstructionToOutputFile(arguments, nzpValue);
}
void addBrnInstructionToOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 11); // make it macro
    addBrsInstructionToOutputFile(arguments, nzpValue);
}

void addBrzInstructionToOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 10); // make it macro
    addBrsInstructionToOutputFile(arguments, nzpValue);
}
void addBrpInstructionToOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 9); // make it macro
    addBrsInstructionToOutputFile(arguments, nzpValue);
}
void addBrnpInstructionToOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 11) | (1 << 9); // make it macro
    addBrsInstructionToOutputFile(arguments, nzpValue);
}
void addBrnzInstructionToOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 11) | (1 << 10); // make it macro
    addBrsInstructionToOutputFile(arguments, nzpValue);
}
void addBrzpInstructionToOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 10) | (1 << 9); // make it macro
    addBrsInstructionToOutputFile(arguments, nzpValue);
}
void addBrnzpInstructionToOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 11) | (1 << 10) | (1 << 9); // make it macro
    addBrsInstructionToOutputFile(arguments, nzpValue);
}

void addJsrInstructionToOutputFile(char *arguments)
{
    uint16_t instruction = (JSR_OPCODE << 12);
    char *token = getFirstToken(arguments); // important : discard trailing space in arguments
    hashSearchResult result = searchLabelInHashTable(token);
    if (result.status == HASH_SEARCH_SUCCESS) {
        uint16_t labelAddress = result.address;
        instruction |= (1 << 11);
        instruction |= 0x7FF & (labelAddress - programCounter);   // PCoffset 11 ??
        writeWordToBinaryFile(output, instruction);
    } else {   
        printErrorMessage("Undefined reference to label [%s] \n", token);
        fclose(output);
        deleteHashTable();
        exit(EXIT_FAILURE);
    } 
}
void addJsrrInstructionToOutputFile(char *arguments)
{
    uint16_t instruction = (JSR_OPCODE << 12);
    char *token = getFirstToken(arguments); // important : discard trailing spaces
    int r0 = convertRegisterToNumber(token); 
    instruction |= (r0 << 6);
    writeWordToBinaryFile(output, instruction);
}

void addJmpInstructionToOutputFile(char *arguments)
{
    uint16_t instruction  = (JMP_OPCODE << 12);
    char *token = getFirstToken(arguments); // important : discard trailing spaces
    int r0 = convertRegisterToNumber(token); 
    instruction |= (r0 << 6);
    writeWordToBinaryFile(output, instruction);
}
void addRetInstructionToOutputFile(char *arguments)
{  
    arguments = "R7";
    addJmpInstructionToOutputFile(arguments);
}
void addRtiInstructionToOutputFile(char *arguments)
{
    arguments = arguments; // avoid compiler unused arguments warning
    uint16_t instruction  = (RTI_OPCODE << 12);
    writeWordToBinaryFile(output, instruction);
}
void addLdInstructionToOutputFile(char *arguments)
{
    uint16_t instruction  = (LD_OPCODE << 12);
    char *token = getFirstToken(arguments);
    int r0 = convertRegisterToNumber(token);
    instruction |= (r0 << 9);
    token = getNextToken();
    hashSearchResult result = searchLabelInHashTable(token);
    if (result.status == HASH_SEARCH_SUCCESS) {
        uint16_t labelAddress = result.address;
        instruction |= 0x1FF & (labelAddress - programCounter);  // PCoffset 9??
        writeWordToBinaryFile(output, instruction);
    } else {   
        printErrorMessage("Undefined reference to label [%s] \n", token);
        fclose(output);
        deleteHashTable();
        exit(EXIT_FAILURE);
    }
}
void addLdiInstructionToOutputFile(char *arguments)
{
    uint16_t instruction  = (LDI_OPCODE << 12);
    char *token = getFirstToken(arguments);
    int r0 = convertRegisterToNumber(token);
    instruction |= (r0 << 9);
    token = getNextToken();
    hashSearchResult result = searchLabelInHashTable(token);
    if (result.status == HASH_SEARCH_SUCCESS) {
        uint16_t labelAddress = result.address;
        instruction |= 0x1FF & (labelAddress - programCounter); // PCoffset9
        writeWordToBinaryFile(output, instruction);
    } else {   
        printErrorMessage("Undefined reference to label [%s] \n", token);
        fclose(output);
        deleteHashTable();
        exit(EXIT_FAILURE);
    }
}
void addLeaInstructionToOutputFile(char *arguments)
{
    uint16_t instruction  = (LEA_OPCODE << 12);
    char *token = getFirstToken(arguments);
    int r0 = convertRegisterToNumber(token);
    instruction |= (r0 << 9);
    token = getNextToken();
    hashSearchResult result = searchLabelInHashTable(token);
    if (result.status == HASH_SEARCH_SUCCESS) {
        uint16_t labelAddress = result.address;
        instruction |= 0x1FF & (labelAddress - programCounter); 
        writeWordToBinaryFile(output, instruction);
    } else {   
        printErrorMessage("Undefined reference to label [%s] \n", token);
        fclose(output);
        deleteHashTable();
        exit(EXIT_FAILURE);
    }
}
void addStInstructionToOutputFile(char *arguments)
{
    uint16_t instruction  = (ST_OPCODE << 12);
    char *token = getFirstToken(arguments);
    int r0 = convertRegisterToNumber(token);
    instruction |= (r0 << 9);
    token = getNextToken();
    hashSearchResult result = searchLabelInHashTable(token);
    if (result.status == HASH_SEARCH_SUCCESS) {
        uint16_t labelAddress = result.address;
        instruction |= 0x1FF & (labelAddress - programCounter);  // PCoffset9 ??
        writeWordToBinaryFile(output, instruction);
    } else {   
        printErrorMessage("Undefined reference to label [%s] \n", token);
        fclose(output);
        deleteHashTable();
        exit(EXIT_FAILURE);
    }
}
void addStiInstructionToOutputFile(char *arguments)
{
    uint16_t instruction  = (STI_OPCODE << 12);
    char *token = getFirstToken(arguments);
    int r0 = convertRegisterToNumber(token);
    instruction |= (r0 << 9);
    token = getNextToken();
    hashSearchResult result = searchLabelInHashTable(token);
    if (result.status == HASH_SEARCH_SUCCESS) {
        uint16_t labelAddress = result.address;
        instruction |= 0x1FF & (labelAddress - programCounter); // PCoffset9 ??
        writeWordToBinaryFile(output, instruction);
    } else {   
        printErrorMessage("Undefined reference to label [%s] \n", token);
        fclose(output);
        deleteHashTable();
        exit(EXIT_FAILURE);
    }
}
void addNotInstructionToOutputFile(char *arguments)
{
    uint16_t instruction  = (NOT_OPCODE << 12);  
    char *token = getFirstToken(arguments);
    int r0 = convertRegisterToNumber(token);
    instruction |= (r0 << 9);
    token = getNextToken();
    int r1 = convertRegisterToNumber(token);
    instruction |= (r1 << 6);
    instruction |= (0x3F);
    writeWordToBinaryFile(output, instruction);
}
void addLdrInstructionToOutputFile(char *arguments)
{
    uint16_t instruction  = (LDR_OPCODE << 12);
    int r0, r1, off;
    char *token = getFirstToken(arguments);
    r0 = convertRegisterToNumber(token);
    instruction |= (r0 << 9);
    token = getNextToken();
    r1 = convertRegisterToNumber(token);
    instruction |= (r1 << 6);
    token = getNextToken();
    off = convertStringToNumber(token);
    instruction |= (0x3F & off); // offset 6 ??
    writeWordToBinaryFile(output, instruction);
}

void addStrInstructionToOutputFile(char *arguments)
{
    uint16_t instruction  = (STR_OPCODE << 12);
    int r0, r1, off;
    char *token = getFirstToken(arguments);
    r0 = convertRegisterToNumber(token);
    instruction |= (r0 << 9);
    token = getNextToken();
    r1 = convertRegisterToNumber(token);
    instruction |= (r1 << 6);
    token = getNextToken();
    off = convertStringToNumber(token);
    instruction |= (0x3F & off);
    writeWordToBinaryFile(output, instruction);
}

void addTrapInstructionToOutputFile(char *arguments)
{
    uint16_t instruction = (TRAP_OPCODE << 12);     
    char *token = getFirstToken(arguments); // important : discard trailing spaces
    int trapv8 = convertStringToNumber(token);   
    instruction |= trapv8;
    writeWordToBinaryFile(output, instruction);
}
void addGetcInstructionToOutputFile(char *arguments)
{
    arguments = "x20";
    addTrapInstructionToOutputFile(arguments);
}
void addHaltInstructionToOutputFile(char *arguments)
{
    arguments = "x25";
    addTrapInstructionToOutputFile(arguments);
}
void addInInstructionToOutputFile(char *arguments)
{
    arguments = "x23";
    addTrapInstructionToOutputFile(arguments);
}
void addOutInstructionToOutputFile(char *arguments)
{
    arguments = "x21";
    addTrapInstructionToOutputFile(arguments);
}
void addPutsInstructionToOutputFile(char *arguments)
{
    arguments = "x22";
    addTrapInstructionToOutputFile(arguments);
}
void addPutspInstructionToOutputFile(char *arguments)
{
    arguments = "x24";
    addTrapInstructionToOutputFile(arguments);
}
