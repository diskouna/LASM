#ifndef __LASM_TRANSLATE__
#define __LASM_TRANSLATE__

#include "lasm_file.h"

enum {
    BR_OPCODE = 0x0000,        
    ADD_OPCODE,        
    LD_OPCODE,         
    ST_OPCODE,         
    JSR_OPCODE,         
    AND_OPCODE,        
    LDR_OPCODE,        
    STR_OPCODE,
    RTI_OPCODE,
    NOT_OPCODE,
    LDI_OPCODE, 
    STI_OPCODE, 
    JMP_OPCODE, 
    RES_OPCODE,    // reserved
    LEA_OPCODE,
    TRAP_OPCODE
};

void addAddInstructionToOutputFile(char *arguments);
void addAndInstructionToOutputFile(char *arguments);
void addBrsInstructionToOutputFile(char *arguments, uint16_t nzpValue);
void addBrInstructionToOutputFile(char *arguments);
void addBrnInstructionToOutputFile(char *arguments);
void addBrzInstructionToOutputFile(char *arguments);
void addBrpInstructionToOutputFile(char *arguments);
void addBrnpInstructionToOutputFile(char *arguments);
void addBrnzInstructionToOutputFile(char *arguments);
void addBrzpInstructionToOutputFile(char *arguments);
void addBrnzpInstructionToOutputFile(char *arguments);
void addJsrInstructionToOutputFile(char *arguments);
void addJsrrInstructionToOutputFile(char *arguments);
void addJmpInstructionToOutputFile(char *arguments);
void addRetInstructionToOutputFile(char *arguments);
void addRtiInstructionToOutputFile(char *arguments);
void addLdInstructionToOutputFile(char *arguments);
void addLdiInstructionToOutputFile(char *arguments);
void addLeaInstructionToOutputFile(char *arguments);
void addStInstructionToOutputFile(char *arguments);
void addStiInstructionToOutputFile(char *arguments);
void addNotInstructionToOutputFile(char *arguments);
void addLdrInstructionToOutputFile(char *arguments);
void addStrInstructionToOutputFile(char *arguments);
void addTrapInstructionToOutputFile(char *arguments);
void addGetcInstructionToOutputFile(char *arguments);
void addHaltInstructionToOutputFile(char *arguments);
void addInInstructionToOutputFile(char *arguments);
void addOutInstructionToOutputFile(char *arguments);
void addPutsInstructionToOutputFile(char *arguments);
void addPutspInstructionToOutputFile(char *arguments);
extern void (*addInstructionToOutputFile[])(char *arguments);

void addOrigDirectiveToOutputFile(char *arguments);
void addFillDirectiveToOutputFile(char *arguments);
void addBlkwDirectiveToOutputFile(char *arguments);
void addStringzDirectiveToOutputFile(char *arguments);
void addEndDirectiveToOutputFile(char *arguments);
extern void (*addDirectiveToOutputFile[])(char *arguments); 

#endif // __LASM_TRANSLATE__