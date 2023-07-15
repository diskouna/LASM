#ifndef __LASM_STRING__
#define __LASM_STRING__

#include <stdint.h>
// TODO : define word = int16_t 

void removeCommentInLine(char *line);

// use "line" buffer
char *getFirstToken(char *instruction);
// use "line" buffer
char *getNextToken();
// use "line" buffer 
char *getArguments();

int isBlankSubstring(char *start, char *end);
int isBlankString(char *str);
int isQuotedString(char *str);
int isAlphaNumericString(char *str);

size_t lengthOfQuotedString(char *str);
size_t lengthOfSubstring(char *star, char *end);

int isStringValidLabel(char* token);
int isStringValidOpcode(char *str);
int isStringValidAsmDirective(char *str);

int isStringValidRegister(char *str);
int convertRegisterToNumber(char *reg);

int isStringValidWord(char *str);
int32_t convertStringToNumber(char *str);

int isNBitsWidthSignedValue(int16_t number, uint8_t n);
int isNBitsWidthUnsignedValue(uint16_t number, uint8_t n);
int isStringValidPC(char *token);
int isNumberValidPCoffset9(int number);
int isStringValidPCoffset9(char *token);
int isNumberValidPCoffset11(int number);
int isStringValidPCoffset11(char *token);
int isNumberValidImm5(int number);
int isStringValidImm5(char *token);
int isNumberValidOffset6(int number);
int isStringValidOffset6(char *token);
int isNumberValidTrapvector8(int number);
int isStringValidTrapvector8(char *token);
int isNumberValidPC(int number);

#endif // __LASM_STRING__