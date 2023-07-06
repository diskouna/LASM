#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lasm_string.h"
#include "lasm_instruction.h"

void removeCommentInLine(char *line)
{
    if (line != NULL) {
        char *startOfComment = strchr(line, ';');
        if (startOfComment != NULL)
            *startOfComment = '\0';
    }
}

int isStringValidLabel(char *str)
{
    if (str == NULL || strlen(str) > 20)
        return 0; 
    if (!isalpha(str[0]) || (strlen(str) > 1 && !isAlphaNumericString(str + 1))) 
        return 0;    
    if (isStringValidRegister(str) || isStringValidOpcode(str) || isStringValidAsmDirective(str)) 
        /* str is a reserved word */ 
        return 0;
    return 1;
}

int isAlphaNumericString(char *str)
{
    if (str == NULL)
        return 0;
    for (char *c = str + 1; *c != '\0'; c++)
        if (!isalnum(*c))
            return 0;
    return 1;
}

int isStringValidOpcode(char *str)
{
    if (str == NULL)
        return 0; 
    return (getInstructionType(str) != UNDEFINED_INSTRUCTION);
}

int isStringValidAsmDirective(char *str)
{
    if (str == NULL)
        return 0; 
    return (getDirectiveType(str) != UNDEFINED_DIRECTIVE);
}

int isStringValidRegister(char *str)
{
    if (str == NULL || strlen(str) != 2)
        return 0;
    if (toupper(str[0]) != 'R') 
        return 0;
    return ('0' <= str[1] && str[1] <= '7');
}

int isQuotedString(char *str)
{
    if (str == NULL || strlen(str) < 2)
        return 0;
        
    char *firstQuote = strpbrk(str, "\"");
    if (firstQuote == NULL || strlen(firstQuote) < 2)
        return 0;
    if (!isBlankSubstring(str, firstQuote - 1))
        return 0;

    char *secondQuote = strpbrk(firstQuote+1, "\"");
    if (secondQuote == NULL)
        return 0; 
    if (strlen(secondQuote) > 1 && !isBlankString(secondQuote + 1))
        return 0;
        
    return 1;
}

int isBlankSubstring(char *start, char *end)
{
    if (start == NULL || end == NULL)
        return 0;
    char *c = start;
    while (c <= end) {
        if (!isblank(*c))   
            return 0;
        c++;
    }
    return 1; 
}
int isBlankString(char *str)
{
    if (str == NULL)
        return 0;   
    char *end = str + strlen(str) - 1;
    return isBlankSubstring(str, end);
}

size_t lengthOfSubstring(char *star, char *end)
{
    if (star == NULL || end == NULL)
        return 0;
    size_t len = 0;
    char *c = star;
    while (c <= end)
    {
        if (*c == '\0') 
            break;
        len++;
        c++;
    }
    return len;
}

size_t lengthOfQuotedString(char *str)
{
    if (str == NULL || !isQuotedString(str))
        return 0;
    char *firstQuote  = strchr(str, '\"');
    char *secondQuote = strrchr(str, '\"');
    return lengthOfSubstring(firstQuote + 1, secondQuote - 1);
}

/* LC-3 supports 3 types of number representation :
 *   xNumber ---> hexadecimal
 *   #Number ---> decimal
 *   bNumber ---> binary
 *   |Value(Number)| < MAXIMUM_VALUE =  2^16
 */

/*
 * Maximum number of characters in  different
 * representations (without leading zeros : #0000000123)
 */                                                                      // sign ('+'/'-'/' ')
#define MAXIMUM_BINARY_WIDTH        17      // ceil(log2(MAXIMUM_VALUE))  +  1  
#define MAXIMUM_DECIMAL_WIDTH       6       // ceil(log10(MAXIMUM_VALUE)) +  1
#define MAXIMUM_HEXADECIMAL_WIDTH   5       // ceil(log16(MAXIMUM_VALUE)) +  1

// A word is a 16 bit value. 
// Either signed or unsigned
int isStringValidWord(char *str)
{
    if (str == NULL || strlen(str) < 2 ) return 0;
    int base;
    char c = tolower(str[0]);  
    //int sign = ((str[0] == '-') ? -1 : 1);
    char *eff = strpbrk(str + 1, "123456789abcdefABCDEF"); //[1-9a-fA-F]
    if (c == 'x') {
        base = 16;
        if (eff != NULL && strlen(eff) > MAXIMUM_HEXADECIMAL_WIDTH) return 0;
    } else if (c == '#') {
        base = 10;
        if (eff != NULL && strlen(eff) > MAXIMUM_DECIMAL_WIDTH) return 0;
    } else if (c == 'b') {
        base = 2;
        if (eff != NULL && strlen(eff) > MAXIMUM_BINARY_WIDTH) return 0;
    } else return 0;

    char *p = &c;
    long int value = strtol(str + 1, &p, base);
    // only digit in string and converted value in range
    return (*p == '\0' && value <= UINT16_MAX && value >= INT16_MIN);        
}

/*
 * invoke isStringValidWord() function before using
 * convertStringToNumber() function
 */
int32_t convertStringToNumber(char *str)
{
    if (isStringValidWord(str)) { 
        int base = 10;
        char c = tolower(str[0]);  
        if (c == 'x')  base = 16;
        else if (c == 'b') base = 2;
        return (int32_t)strtol(str + 1, NULL, base);
    }    
    return -1;     
}

int isNBitsWidthSignedValue(int16_t number, uint8_t n)
{
    if (n >= 16)    return 1;  
    // Range[min, max] of n bit width signed number
    int16_t max = (1 << (n-1)) - 1;
    int16_t min = ~max; // -(1 << (n-1))
    return (min <= number && number <= max);
}

int isNBitsWidthUnsignedValue(uint16_t number, uint8_t n)
{
    if (n >= 16)    return 1;
    // Range[0, max] of n bit width unsigned number
    uint16_t max = (1 << n) - 1; 
    return (number <= max);
}

int isNumberValidPCoffset9(int number)
{
    return isNBitsWidthSignedValue(number, 9);
}
int isStringValidPCoffset9(char *token)
{
    if (isStringValidWord(token)) {
        int pc = convertStringToNumber(token);
        return isNumberValidPCoffset9(pc);
    }
    return 0;
}

int isNumberValidPCoffset11(int number)
{
    return isNBitsWidthSignedValue(number, 11);
}
int isStringValidPCoffset11(char *token)
{
    if (isStringValidWord(token)) {
        int pc = convertStringToNumber(token);
        return isNumberValidPCoffset11(pc);
    }
    return 0;
}

int isNumberValidImm5(int number)
{
    return isNBitsWidthSignedValue(number, 5);
}
int isStringValidImm5(char *token)
{
    if (isStringValidWord(token)) {
        int imm5 = convertStringToNumber(token);
        return isNumberValidImm5(imm5);
    }
    return 0;
}

int isNumberValidOffset6(int number)
{
    return isNBitsWidthSignedValue(number, 6);
}
int isStringValidOffset6(char *token)
{
    if (isStringValidWord(token)) {
        int off = convertStringToNumber(token);
        return isNumberValidOffset6(off);
    }
    return 0;
}

int isNumberValidTrapvector8(int number)
{
    return isNBitsWidthUnsignedValue(number, 8);
}
int isStringValidTrapvector8(char *token)
{
    if (isStringValidWord(token)) {
        int trapv8 = convertStringToNumber(token);
        return isNumberValidTrapvector8(trapv8);
    }
    return 0;
}

int isNumberValidPC(int number)
{
    // >= x3000 
    return isNBitsWidthUnsignedValue(number, 16);
}
int isStringValidPC(char *token)
{
    if (isStringValidWord(token)) {
        int pc = convertStringToNumber(token);
        return isNumberValidPC(pc);
    }
    return 0;
}

/*
 * invoke isStringValidRegister() function before using
 * convertRegisterToNumber() function
 */
int convertRegisterToNumber(char *reg)
{
    if (isStringValidRegister(reg))
        return (reg[1] - '0');
    return -1;
}

char *getFirstToken(char *instruction)
{
    return strtok(instruction, " ,");
}
// call must be precede by at least one "getFirstToken()" call
char *getArguments()
{
    return strtok(NULL, "\0");
}
// call must be precede by at least one "getFirstToken()" call
char *getNextToken()
{
    return strtok(NULL, " ,");
}

