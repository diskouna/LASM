#include <string.h>
#include <stdlib.h> 

#include "lasm_instruction.h"

// OPCODES sorted in alphabetical increasing order
char *opcodes[] = {"ADD", "AND", "BR", "BRn", "BRnp", "BRnz", "BRnzp", "BRp", "BRz", "BRzp", "GETC", "HALT", "IN", "JMP", "JSR", "JSRR",
 "LD", "LDI", "LDR", "LEA", "NOT", "OUT", "PUTS", "PUTSP", "RET", "RTI", "ST", "STI", "STR" , "TRAP"}; 
// ASM Directives sorted in alphabetical increasing order
char *asmDirectives[] = {".BLKW", ".END", ".FILL", ".ORIG", ".STRINGZ"}; 

//case insensitive comparaison utility for binary search
static inline int compareStringUtility(const void *str1, const void *str2)
{
    return strcasecmp(*(char **)str1, *(char **)str2);
}

instructionType getInstructionType(char *opcode)
{
    char **p = bsearch(&opcode, opcodes, OPCODE_COUNTS, sizeof(char *), compareStringUtility);
    if (p == NULL)
        return UNDEFINED_INSTRUCTION;
    return (p - (char**)opcodes);
}

directiveType getDirectiveType(char *directive)
{
    char **p = bsearch(&directive, asmDirectives, DIRECTIVE_COUNTS, sizeof(char *), compareStringUtility);
    if (p == NULL)
        return UNDEFINED_DIRECTIVE;
    return (p - (char**)asmDirectives);
}
