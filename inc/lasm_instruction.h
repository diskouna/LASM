#ifndef __LASM_INSTRUCTION__
#define __LASM_INSTRUCTION__

#define OPCODE_COUNTS  30 
#define DIRECTIVE_COUNTS  5

typedef enum { 
    UNDEFINED_INSTRUCTION = -1,
    /* sorted in alphabetical order */
    ADD_INSTRUCTION, 
    AND_INSTRUCTION, 
    BR_INSTRUCTION, 
    BRn_INSTRUCTION, 
    BRnp_INSTRUCTION,
    BRnz_INSTRUCTION,
    BRnzp_INSTRUCTION,
    BRp_INSTRUCTION, 
    BRz_INSTRUCTION, 
    BRzp_INSTRUCTION, 
    GETC_INSTRUCTION,
    HALT_INSTRUCTION,
    IN_INSTRUCTION,
    JMP_INSTRUCTION, 
    JSR_INSTRUCTION, 
    JSRR_INSTRUCTION,
    LD_INSTRUCTION, 
    LDI_INSTRUCTION, 
    LDR_INSTRUCTION, 
    LEA_INSTRUCTION, 
    NOT_INSTRUCTION, 
    OUT_INSTRUCTION,
    PUTS_INSTRUCTION,
    PUTSP_INSTRUCTION,
    RET_INSTRUCTION, 
    RTI_INSTRUCTION,
    ST_INSTRUCTION, 
    STI_INSTRUCTION, 
    STR_INSTRUCTION, 
    TRAP_INSTRUCTION
} instructionType;

instructionType getInstructionType(char *opcode);

typedef enum {
    UNDEFINED_DIRECTIVE = -1,
    /* sorted in alphabetical order */
    BLKW_DIRECTIVE,
    END_DIRECTIVE,
    FILL_DIRECTIVE,
    ORIG_DIRECTIVE,
    STRINGZ_DIRECTIVE
} directiveType;

directiveType getDirectiveType(char *directive);

#endif // __LASM_INSTRUCTION__
