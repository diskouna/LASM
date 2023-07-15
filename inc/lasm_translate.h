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

typedef enum {
    TRANSLATE_FAILURE = 0,
    TRANSLATE_SUCCESS,
} translateStatus;

translateStatus translateAddInstructionIntoOutputFile(char *arguments);
translateStatus translateAndInstructionIntoOutputFile(char *arguments);
translateStatus translateBrsInstructionIntoOutputFile(char *arguments, uint16_t nzpValue);
translateStatus translateBrInstructionIntoOutputFile(char *arguments);
translateStatus translateBrnInstructionIntoOutputFile(char *arguments);
translateStatus translateBrzInstructionIntoOutputFile(char *arguments);
translateStatus translateBrpInstructionIntoOutputFile(char *arguments);
translateStatus translateBrnpInstructionIntoOutputFile(char *arguments);
translateStatus translateBrnzInstructionIntoOutputFile(char *arguments);
translateStatus translateBrzpInstructionIntoOutputFile(char *arguments);
translateStatus translateBrnzpInstructionIntoOutputFile(char *arguments);
translateStatus translateJsrInstructionIntoOutputFile(char *arguments);
translateStatus translateJsrrInstructionIntoOutputFile(char *arguments);
translateStatus translateJmpInstructionIntoOutputFile(char *arguments);
translateStatus translateRetInstructionIntoOutputFile(char *arguments);
translateStatus translateRtiInstructionIntoOutputFile(char *arguments);
translateStatus translateLdInstructionIntoOutputFile(char *arguments);
translateStatus translateLdiInstructionIntoOutputFile(char *arguments);
translateStatus translateLeaInstructionIntoOutputFile(char *arguments);
translateStatus translateStInstructionIntoOutputFile(char *arguments);
translateStatus translateStiInstructionIntoOutputFile(char *arguments);
translateStatus translateNotInstructionIntoOutputFile(char *arguments);
translateStatus translateLdrInstructionIntoOutputFile(char *arguments);
translateStatus translateStrInstructionIntoOutputFile(char *arguments);
translateStatus translateTrapInstructionIntoOutputFile(char *arguments);
translateStatus translateGetcInstructionIntoOutputFile(char *arguments);
translateStatus translateHaltInstructionIntoOutputFile(char *arguments);
translateStatus translateInInstructionIntoOutputFile(char *arguments);
translateStatus translateOutInstructionIntoOutputFile(char *arguments);
translateStatus translatePutsInstructionIntoOutputFile(char *arguments);
translateStatus translatePutspInstructionIntoOutputFile(char *arguments);
extern translateStatus (*translateInstructionIntoOutputFile[])(char *arguments);

translateStatus translateOrigDirectiveIntoOutputFile(char *arguments);
translateStatus translateFillDirectiveIntoOutputFile(char *arguments);
translateStatus translateBlkwDirectiveIntoOutputFile(char *arguments);
translateStatus translateStringzDirectiveIntoOutputFile(char *arguments);
translateStatus translateEndDirectiveIntoOutputFile(char *arguments);
extern translateStatus (*translateDirectiveIntoOutputFile[])(char *arguments); 

#endif // __LASM_TRANSLATE__