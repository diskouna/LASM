#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "lasm_translate.h"
#include "lasm_instruction.h"
#include "lasm_string.h"
#include "lasm_file.h"


translateStatus (*translateInstructionIntoOutputFile[])(char *arguments) = {
    &translateAddInstructionIntoOutputFile,
    &translateAndInstructionIntoOutputFile,
    &translateBrInstructionIntoOutputFile,   
    &translateBrnInstructionIntoOutputFile,   
    &translateBrnpInstructionIntoOutputFile,   
    &translateBrnzInstructionIntoOutputFile,   
    &translateBrnzpInstructionIntoOutputFile,   
    &translateBrpInstructionIntoOutputFile,   
    &translateBrzInstructionIntoOutputFile,   
    &translateBrzpInstructionIntoOutputFile, 
    &translateGetcInstructionIntoOutputFile,
    &translateHaltInstructionIntoOutputFile,
    &translateInInstructionIntoOutputFile,
    &translateJmpInstructionIntoOutputFile,
    &translateJsrInstructionIntoOutputFile,
    &translateJsrrInstructionIntoOutputFile,
    &translateLdInstructionIntoOutputFile,
    &translateLdiInstructionIntoOutputFile,
    &translateLdrInstructionIntoOutputFile,
    &translateLeaInstructionIntoOutputFile,
    &translateNotInstructionIntoOutputFile,
    &translateOutInstructionIntoOutputFile,
    &translatePutsInstructionIntoOutputFile,
    &translatePutspInstructionIntoOutputFile,
    &translateRetInstructionIntoOutputFile,
    &translateRtiInstructionIntoOutputFile,
    &translateStInstructionIntoOutputFile,
    &translateStiInstructionIntoOutputFile,
    &translateStrInstructionIntoOutputFile,
    &translateTrapInstructionIntoOutputFile
};

translateStatus (*translateDirectiveIntoOutputFile[])(char *arguments) = {
    // theses functions alter the program counter in different way 
    &translateBlkwDirectiveIntoOutputFile,
    &translateEndDirectiveIntoOutputFile,
    &translateFillDirectiveIntoOutputFile,
    &translateOrigDirectiveIntoOutputFile,
    &translateStringzDirectiveIntoOutputFile
};


translateStatus translateOrigDirectiveIntoOutputFile(char *arguments)
{
    arguments = arguments; // avoid compiler unused arguments warning
    
    programCounter = programCounterStart;  
    // programCounterStart is already computed during the first pass
    uint16_t value = (uint16_t)programCounterStart; 
    return writeWordToObjectFile(output, value);
}
translateStatus translateFillDirectiveIntoOutputFile(char *arguments){
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
            printErrorMessage("Undefined reference to label [%s] \n", token);
            return TRANSLATE_FAILURE;
        }
    }
    return writeWordToObjectFile(output, value);
}

translateStatus translateBlkwDirectiveIntoOutputFile(char *arguments)
{
    uint16_t numberOfBlocks = convertStringToNumber(arguments);  
    // set aside "numberOfBlocks" words = (2*numberOfBlocks) Bytes
    programCounter += (numberOfBlocks - 1);
    fseek(output, 2 * numberOfBlocks, SEEK_CUR); 
    return TRANSLATE_SUCCESS;         
}

translateStatus translateStringzDirectiveIntoOutputFile(char *arguments)
{
    char *s = strpbrk(arguments, "\"");
    s++;
    uint16_t value;
    while (*s != '\"') {
        value = (uint16_t)*s;
        programCounter++;
        if (!writeWordToObjectFile(output, value))
            return TRANSLATE_FAILURE;
        s++;
    }
    value = 0x0000; // null character
    return writeWordToObjectFile(output, value);
}
translateStatus translateEndDirectiveIntoOutputFile(char *arguments){
    arguments = arguments; // avoid compiler unused arguments warning
    return TRANSLATE_SUCCESS;
}

translateStatus translateAddInstructionIntoOutputFile(char *arguments)
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
    return writeWordToObjectFile(output, instruction);
}
translateStatus translateAndInstructionIntoOutputFile(char *arguments)
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
    return writeWordToObjectFile(output, instruction);
}

translateStatus translateBrsInstructionIntoOutputFile(char *arguments, uint16_t nzpValue)
{
    uint16_t instruction =  BR_OPCODE;
    instruction |=  nzpValue;
    char *token = getFirstToken(arguments); // important : discard trailing spaces
    hashSearchResult result = searchLabelInHashTable(token);
    if (result.status == HASH_SEARCH_SUCCESS) {
        uint16_t labelAddress = result.address;
        instruction |= 0x001FF & (labelAddress - programCounter);    //PCOffset9
        return writeWordToObjectFile(output, instruction);
    }  
    printErrorMessage("undefined reference to label [%s] \n", token);
    return TRANSLATE_FAILURE;
}

translateStatus translateBrInstructionIntoOutputFile(char *arguments)
{
    uint16_t nzpValue = (0x7 << 9); // make it macro
    return translateBrsInstructionIntoOutputFile(arguments, nzpValue);
}
translateStatus translateBrnInstructionIntoOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 11); // make it macro
    return translateBrsInstructionIntoOutputFile(arguments, nzpValue);
}

translateStatus translateBrzInstructionIntoOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 10); // make it macro
    return translateBrsInstructionIntoOutputFile(arguments, nzpValue);
}
translateStatus translateBrpInstructionIntoOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 9); // make it macro
    return translateBrsInstructionIntoOutputFile(arguments, nzpValue);
}
translateStatus translateBrnpInstructionIntoOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 11) | (1 << 9); // make it macro
    return translateBrsInstructionIntoOutputFile(arguments, nzpValue);
}
translateStatus translateBrnzInstructionIntoOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 11) | (1 << 10); // make it macro
    return translateBrsInstructionIntoOutputFile(arguments, nzpValue);
}
translateStatus translateBrzpInstructionIntoOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 10) | (1 << 9); // make it macro
    return translateBrsInstructionIntoOutputFile(arguments, nzpValue);
}
translateStatus translateBrnzpInstructionIntoOutputFile(char *arguments)
{
    uint16_t nzpValue = (1 << 11) | (1 << 10) | (1 << 9); // make it macro
    return translateBrsInstructionIntoOutputFile(arguments, nzpValue);
}

translateStatus translateJsrInstructionIntoOutputFile(char *arguments)
{
    uint16_t instruction = (JSR_OPCODE << 12);
    char *token = getFirstToken(arguments); // important : discard trailing space in arguments
    hashSearchResult result = searchLabelInHashTable(token);
    if (result.status == HASH_SEARCH_SUCCESS) {
        uint16_t labelAddress = result.address;
        instruction |= (1 << 11);
        instruction |= 0x7FF & (labelAddress - programCounter);   // PCoffset 11 ??
        return writeWordToObjectFile(output, instruction);
    }  
    printErrorMessage("undefined reference to label [%s] \n", token);
    return TRANSLATE_FAILURE;
}
translateStatus translateJsrrInstructionIntoOutputFile(char *arguments)
{
    uint16_t instruction = (JSR_OPCODE << 12);
    char *token = getFirstToken(arguments); // important : discard trailing spaces
    int r0 = convertRegisterToNumber(token); 
    instruction |= (r0 << 6);
    return writeWordToObjectFile(output, instruction); 
}

translateStatus translateJmpInstructionIntoOutputFile(char *arguments)
{
    uint16_t instruction  = (JMP_OPCODE << 12);
    char *token = getFirstToken(arguments); // important : discard trailing spaces
    int r0 = convertRegisterToNumber(token); 
    instruction |= (r0 << 6);
    return writeWordToObjectFile(output, instruction);
}
translateStatus translateRetInstructionIntoOutputFile(char *arguments)
{  
    arguments = "R7";
    return translateJmpInstructionIntoOutputFile(arguments);
}
translateStatus translateRtiInstructionIntoOutputFile(char *arguments)
{
    arguments = arguments; // avoid compiler unused arguments warning
    uint16_t instruction  = (RTI_OPCODE << 12);
    return writeWordToObjectFile(output, instruction);
}
translateStatus translateLdInstructionIntoOutputFile(char *arguments)
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
        return writeWordToObjectFile(output, instruction);
    }  
    printErrorMessage("undefined reference to label [%s] \n", token);
    return TRANSLATE_FAILURE;
}
translateStatus translateLdiInstructionIntoOutputFile(char *arguments)
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
        return writeWordToObjectFile(output, instruction);
    }  
    printErrorMessage("undefined reference to label [%s] \n", token);
    return TRANSLATE_FAILURE;
}
translateStatus translateLeaInstructionIntoOutputFile(char *arguments)
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
        return writeWordToObjectFile(output, instruction);
    }  
    printErrorMessage("undefined reference to label [%s] \n", token);
    return TRANSLATE_FAILURE;
}
translateStatus translateStInstructionIntoOutputFile(char *arguments)
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
        return writeWordToObjectFile(output, instruction);
    }  
    printErrorMessage("undefined reference to label [%s] \n", token);
    return TRANSLATE_FAILURE;
}
translateStatus translateStiInstructionIntoOutputFile(char *arguments)
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
        return writeWordToObjectFile(output, instruction);
    }  
    printErrorMessage("undefined reference to label [%s] \n", token);
    return TRANSLATE_FAILURE;
}
translateStatus translateNotInstructionIntoOutputFile(char *arguments)
{
    uint16_t instruction  = (NOT_OPCODE << 12);  
    char *token = getFirstToken(arguments);
    int r0 = convertRegisterToNumber(token);
    instruction |= (r0 << 9);
    token = getNextToken();
    int r1 = convertRegisterToNumber(token);
    instruction |= (r1 << 6);
    instruction |= (0x3F);
    return writeWordToObjectFile(output, instruction);
}
translateStatus translateLdrInstructionIntoOutputFile(char *arguments)
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
    return writeWordToObjectFile(output, instruction);
}

translateStatus translateStrInstructionIntoOutputFile(char *arguments)
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
    return writeWordToObjectFile(output, instruction);
}

translateStatus translateTrapInstructionIntoOutputFile(char *arguments)
{
    uint16_t instruction = (TRAP_OPCODE << 12);     
    char *token = getFirstToken(arguments); // important : discard trailing spaces
    int trapv8 = convertStringToNumber(token);   
    instruction |= trapv8;
    return writeWordToObjectFile(output, instruction);
}
translateStatus translateGetcInstructionIntoOutputFile(char *arguments)
{
    arguments = "x20";
    return translateTrapInstructionIntoOutputFile(arguments);
}
translateStatus translateHaltInstructionIntoOutputFile(char *arguments)
{
    arguments = "x25";
    return translateTrapInstructionIntoOutputFile(arguments);
}
translateStatus translateInInstructionIntoOutputFile(char *arguments)
{
    arguments = "x23";
    return translateTrapInstructionIntoOutputFile(arguments);
}
translateStatus translateOutInstructionIntoOutputFile(char *arguments)
{
    arguments = "x21";
    return translateTrapInstructionIntoOutputFile(arguments);
}
translateStatus translatePutsInstructionIntoOutputFile(char *arguments)
{
    arguments = "x22";
    return translateTrapInstructionIntoOutputFile(arguments);
}
translateStatus translatePutspInstructionIntoOutputFile(char *arguments)
{
    arguments = "x24";
    return translateTrapInstructionIntoOutputFile(arguments);
}
