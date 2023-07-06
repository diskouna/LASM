#ifndef __LASM_PARSE__
#define __LASM_PARSE__

typedef struct 
{
    enum {
        END_OF_FILE_REACH = -1,
        FAILURE,  // expand failure ?? illegal litteral/ 
        INVALID_NUMBER,  // expands--> out of range
        INVALID_REGISTER,
        INVALID_REGISTER_OR_NUMBER, // IMM5 value
        // INVALID LABEL 
        INVALID_LABEL,    
        INVALID_LABEL_OR_NUMBER,      
        DUPPLICATE_LABEL,
        UNDEFINED_LABEL,
        INUTILE_LABEL,
        CANNOT_ADD_LABEL_TO_SYMBOL_TABLE,
        // INVALID NUMBER OF ARGUMENTS 
        TOO_MANY_ARGUMENTS,
        FEW_ARGUMENT,
        INVALID_PROGRAM_STARTING_ADDRESS,
        MULTIPLE_PROGRAM_STARTING_ADDRESS,
        UNDEFINED_PROGRAM_STARTING_ADDRESS,

        INVALID_STRING, 
        UNDEFINED_TOKEN,
        LABEL_BEFORE_PROGRAM_STARTING_ADDRESS,
        NULL_INSTRUCTION,
        SUCCESS
    } type;

    char *token;
} parseStatus;

parseStatus parseLine(char *instruction);

// TODO : ADD EXTERNAL DIRECTIVE
parseStatus parseOrigDirective(char *arguments);
parseStatus parseFillDirective(char *arguments);
parseStatus parseBlkwDirective(char *arguments);
parseStatus parseStringzDirective(char *arguments);
parseStatus parseEndDirective(char *arguments);

parseStatus parseType1Instruction(char *arguments);
parseStatus parseType2Instruction(char *arguments);
parseStatus parseType3Instruction(char *arguments);
parseStatus parseType4Instruction(char *arguments);
parseStatus parseType5Instruction(char *arguments);
parseStatus parseType6Instruction(char *arguments);
parseStatus parseType7Instruction(char *arguments);
parseStatus parseType8Instruction(char *arguments);

parseStatus parseAddInstruction(char *arguments);
parseStatus parseAndInstruction(char *arguments);
parseStatus parseBrInstruction(char *arguments);
parseStatus parseJsrInstruction(char *arguments);
parseStatus parseJsrrInstruction(char *arguments);
parseStatus parseJmpInstruction(char *arguments);
parseStatus parseRetInstruction(char *arguments);
parseStatus parseRtiInstruction(char *arguments);
parseStatus parseLdInstruction(char *arguments);
parseStatus parseLdiInstruction(char *arguments);
parseStatus parseLeaInstruction(char *arguments);
parseStatus parseStInstruction(char *arguments);
parseStatus parseStiInstruction(char *arguments);
parseStatus parseNotInstruction(char *arguments);
parseStatus parseLdrInstruction(char *arguments);
parseStatus parseStrInstruction(char *arguments);
parseStatus parseTrapInstruction(char *arguments);
parseStatus parseGetcInstruction(char *arguments);
parseStatus parseOutInstruction(char *arguments);
parseStatus parsePutsInstruction(char *arguments);
parseStatus parseInInstruction(char *arguments);
parseStatus parsePutspInstruction(char *arguments);
parseStatus parseHaltInstruction(char *arguments);

void printParseErrorMessage(parseStatus status);



#endif // __LASM_PARSE__
