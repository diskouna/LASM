#include <stdlib.h>

#include "lasm_parse.h"
#include "lasm_file.h"
#include "lasm_instruction.h"
#include "lasm_hash.h"
#include "lasm_string.h"

extern  uint16_t programCounter;
extern int programCounterStart;
extern unsigned int instructionCount;
extern char line[LINE_MAX];

parseStatus (*parseDirective[])(char *arguments) = {
    &parseBlkwDirective,
    &parseEndDirective,
    &parseFillDirective,
    &parseOrigDirective,
    &parseStringzDirective
};

parseStatus (*parseInstruction[])(char *arguments) = {
    &parseAddInstruction,
    &parseAndInstruction,
    &parseBrInstruction,   // BR
    &parseBrInstruction,   // BRn
    &parseBrInstruction,   // BRnp
    &parseBrInstruction,   // BRnz
    &parseBrInstruction,   // BRnzp
    &parseBrInstruction,   // BRp
    &parseBrInstruction,   // BRz
    &parseBrInstruction,   // BRzp  
    &parseGetcInstruction,
    &parseHaltInstruction,
    &parseInInstruction,
    &parseJmpInstruction,
    &parseJsrInstruction,
    &parseJsrrInstruction,
    &parseLdInstruction,
    &parseLdiInstruction,
    &parseLdrInstruction,
    &parseLeaInstruction,
    &parseNotInstruction,
    &parseOutInstruction,
    &parsePutsInstruction,
    &parsePutspInstruction,
    &parseRetInstruction,
    &parseRtiInstruction,
    &parseStInstruction,
    &parseStiInstruction,
    &parseStrInstruction,
    &parseTrapInstruction
};

// parseLine() alters the "line" string 
parseStatus parseLine(char *instruction)  // don't need lineCount
{ 
    instructionCount++; // move it up 
    //static uint16_t current MemoryLocation = 0;
    char *token = getFirstToken(instruction);  // Label/Direct/Opcode/undefined token
    char *arguments;
    while (token != NULL) {        
        if (isStringValidAsmDirective(token))  {
            directiveType type = getDirectiveType(token);
            arguments = getArguments();
            return parseDirective[type](arguments);
        }    
        if (isStringValidOpcode(token)) {
            instructionType type = getInstructionType(token);
            arguments = getArguments();
            return parseInstruction[type](arguments);                        
        } 
        if (isStringValidLabel(token)) {
            if (programCounterStart == -1) 
                return (parseStatus){.type=LABEL_BEFORE_PROGRAM_STARTING_ADDRESS, 
                                     .token=token };
            hashAddStatus status = addLabelToSymbolTable(token);
            if (status == HASH_ADD_DUPPLICATE_LABEL)
                return (parseStatus){.type=DUPPLICATE_LABEL, 
                                     .token=token };
            if (status == HASH_ADD_MEMORY_ALLOCATION_ERROR)
                return (parseStatus){.type=CANNOT_ADD_LABEL_TO_SYMBOL_TABLE, 
                                     .token=token };                         
            char *label = token;
            token = getNextToken();
            if (token == NULL) 
                return  (parseStatus){.type=INUTILE_LABEL, 
                                      .token=label };
            if (!isStringValidOpcode(token) && !isStringValidAsmDirective(token))
                return (parseStatus){.type=UNDEFINED_TOKEN, 
                                     .token=token };
            continue;
        } else {
            return (parseStatus){.type= INVALID_LABEL, .token=token};
        }    
    }         
    return (parseStatus){.type=NULL_INSTRUCTION, .token=NULL};    // handle "NULL" instruction
}

parseStatus parseOrigDirective(char *arguments)
{
    if (arguments == NULL) 
        return (parseStatus){.type=FEW_ARGUMENT, .token=NULL};
    
    if (programCounterStart != -1)  
        return  (parseStatus){.type=MULTIPLE_PROGRAM_STARTING_ADDRESS, 
                              .token=NULL};  

    char *token = getFirstToken(arguments);
    if (token == NULL) 
        return (parseStatus){.type=FEW_ARGUMENT, 
                             .token=NULL}; 
    if (!isStringValidPC(token)) 
        return (parseStatus){.type=INVALID_PROGRAM_STARTING_ADDRESS, 
                             .token=token};           
    programCounterStart = convertStringToNumber(token);
    token = getNextToken();      
    if (token != NULL)  
        return (parseStatus){.type=TOO_MANY_ARGUMENTS, 
                             .token=token};
    instructionCount = 0;             //beginning of the source code
    return (parseStatus){.type=SUCCESS, .token=NULL};        
}
parseStatus parseFillDirective(char *arguments)
{
    if (arguments == NULL) 
        return (parseStatus){.type=FEW_ARGUMENT, .token=NULL};

    char *token = getFirstToken(arguments);
    if (token == NULL) 
        return (parseStatus){.type=FEW_ARGUMENT, 
                             .token=NULL};     

    if (!isStringValidWord(token) && !isStringValidLabel(token)) 
        return (parseStatus){.type= INVALID_LABEL_OR_NUMBER,
                             .token=token};
    token = getNextToken();      
    if (token != NULL)              
        return (parseStatus){.type=TOO_MANY_ARGUMENTS, 
                            .token=token};
    return (parseStatus){.type=SUCCESS, .token=NULL};
}
parseStatus parseBlkwDirective(char *arguments)
{
    if (arguments == NULL) return (parseStatus){.type=FEW_ARGUMENT, 
                                                .token=NULL};
    char *token = getFirstToken(arguments);
    if (token == NULL) return (parseStatus){.type=FEW_ARGUMENT, 
                                            .token=NULL};
    if (!isStringValidWord(token)) 
        return (parseStatus){.type= INVALID_NUMBER, 
                             .token=token};

    // isValidNumberOfBlock() ?
    int n = convertStringToNumber(token);   // number of block of words to set aside
    if (n <= 0) 
        return (parseStatus){.type= INVALID_NUMBER, .token=token};      // number of blocks > 0 
    instructionCount += n - 1;      // set aside memory --> check overflow ?? 

    token = getNextToken();      
    if (token != NULL)              
        return (parseStatus){.type=TOO_MANY_ARGUMENTS, 
                             .token=token};                               
    return (parseStatus){.type=SUCCESS, .token=NULL};
}
parseStatus parseStringzDirective(char *arguments)
{  
    if (arguments == NULL) return (parseStatus){.type=FEW_ARGUMENT,
                                                .token=NULL};
    if (!isQuotedString(arguments)) 
        return (parseStatus){.type=INVALID_STRING, 
                             .token=arguments};  
    instructionCount += lengthOfQuotedString(arguments);
    // check overflows ---> ?  
    return (parseStatus){.type=SUCCESS, .token=NULL};
}
parseStatus parseEndDirective(char *arguments)
{
    if (arguments != NULL) return (parseStatus){.type=FEW_ARGUMENT, 
                                                .token=NULL};                                
    if (programCounterStart == -1) 
        return (parseStatus){.type=UNDEFINED_PROGRAM_STARTING_ADDRESS, 
                             .token=NULL};                 
    return (parseStatus){.type=END_OF_FILE_REACH, .token=NULL}; 
}

// Type 1 instruction : instr = OPCODE R0,R1,R2|IMM5 
parseStatus parseType1Instruction(char *arguments)
{
    if (arguments == NULL) 
        return (parseStatus){.type=FEW_ARGUMENT, 
                             .token=NULL};

    char *token = getFirstToken(arguments);        
    if (!isStringValidRegister(token))
        return (parseStatus){.type=INVALID_REGISTER, 
                             .token=token};             
    token = getNextToken();        
    if (!isStringValidRegister(token))
        return (parseStatus){.type=INVALID_REGISTER, 
                             .token=token}; 
                
    token = getNextToken(); 
    if (!isStringValidImm5(token) && !isStringValidRegister(token))
        return (parseStatus){.type=INVALID_REGISTER_OR_NUMBER, 
                             .token=token};
    
    token = getNextToken();      
    if (token != NULL)              
        return (parseStatus){.type=TOO_MANY_ARGUMENTS, 
                             .token=token};

    return (parseStatus){.type=SUCCESS, .token=NULL};
}
// Type 2 instruction : instr = OPCODE LABEL
parseStatus parseType2Instruction(char *arguments)
{
    if (arguments == NULL) 
        return (parseStatus){.type=FEW_ARGUMENT, 
                             .token=NULL};    
    char *token = getFirstToken(arguments);
    if (!isStringValidLabel(token))
        return (parseStatus){.type=INVALID_LABEL, 
                             .token=token};; 
    token = getNextToken();      
    if (token != NULL)              
        return (parseStatus){.type=TOO_MANY_ARGUMENTS, 
                             .token=token};
    return (parseStatus){.type=SUCCESS, .token=NULL};
}

// Type 3 instruction :  instr : OPCODE R0
parseStatus parseType3Instruction(char *arguments)
{
    if (arguments == NULL) 
        return (parseStatus){.type=FEW_ARGUMENT, 
                             .token=NULL}; 
    char *token = getFirstToken(arguments);        
    if (!isStringValidRegister(token))
        return (parseStatus){.type=INVALID_REGISTER, 
                             .token=token}; 
    token = getNextToken();      
    if (token != NULL)              
        return (parseStatus){.type=TOO_MANY_ARGUMENTS, 
                             .token=token};
    return (parseStatus){.type=SUCCESS, .token=NULL};
}
// Type 4 instruction : instr : OPCODE
parseStatus parseType4Instruction(char *arguments)
{
    char *token = getFirstToken(arguments);
    if (token != NULL) 
        return (parseStatus){.type=TOO_MANY_ARGUMENTS, 
                             .token=token};    
    return (parseStatus){.type=SUCCESS, .token=NULL};
}
// Type 5 instruction : instr: OPCODE R0 LABEL
parseStatus parseType5Instruction(char *arguments)
{
    if (arguments == NULL) 
        return (parseStatus){.type=FEW_ARGUMENT, 
                             .token=NULL};    
    char *token = getFirstToken(arguments);        
    if (!isStringValidRegister(token))
        return (parseStatus){.type=INVALID_REGISTER, 
                             .token=token}; 
    token = getNextToken();
    if (!isStringValidLabel(token))
        return (parseStatus){.type=INVALID_LABEL, 
                             .token=token};   
    token = getNextToken();      
    if (token != NULL)              
        return (parseStatus){.type=TOO_MANY_ARGUMENTS, 
                             .token=token};
    return (parseStatus){.type=SUCCESS, .token=NULL};
}

// Type 6 instruction : instr: OPCODE R0,R1
parseStatus parseType6Instruction(char *arguments)
{
    if (arguments == NULL) 
        return (parseStatus){.type=FEW_ARGUMENT, 
                             .token=NULL};    
    char *token = getFirstToken(arguments);        
    if (!isStringValidRegister(token))
        return (parseStatus){.type=INVALID_REGISTER, 
                             .token=token};             
    token = getNextToken();        
    if (!isStringValidRegister(token))
        return (parseStatus){.type=INVALID_REGISTER, 
                             .token=token};   

    token = getNextToken();      
    if (token != NULL)              
        return (parseStatus){.type=TOO_MANY_ARGUMENTS, 
                             .token=token};
    return (parseStatus){.type=SUCCESS, .token=NULL};
}

// Type 7 instruction : instr: OPCODE R0,R1,offset6
parseStatus parseType7Instruction(char *arguments)
{
    if (arguments == NULL) 
        return (parseStatus){.type=FEW_ARGUMENT, 
                             .token=NULL};    
    char *token = getFirstToken(arguments);        
    if (!isStringValidRegister(token))
        return (parseStatus){.type=INVALID_REGISTER, 
                             .token=token};             
    token = getNextToken();        
    if (!isStringValidRegister(token))
        return (parseStatus){.type=INVALID_REGISTER, 
                             .token=token}; 

    token = getNextToken();
    if (!isStringValidOffset6(token))
        return (parseStatus){.type=INVALID_NUMBER, 
                             .token=token};

    token = getNextToken();      
    if (token != NULL)              
        return (parseStatus){.type=TOO_MANY_ARGUMENTS, 
                             .token=token};
    return (parseStatus){.type=SUCCESS, .token=NULL};
}

// Type 8 instruction : instr : OPCODE TRAPVECTOR8
parseStatus parseType8Instruction(char *arguments)
{
    if (arguments == NULL) 
        return (parseStatus){.type=FEW_ARGUMENT, 
                             .token=NULL};    
    char *token = getFirstToken(arguments);
    if (!isStringValidTrapvector8(token))
        return (parseStatus){.type=INVALID_NUMBER, 
                             .token=token};  

    token = getNextToken();      
    if (token != NULL)              
        return (parseStatus){.type=TOO_MANY_ARGUMENTS, 
                             .token=token};
    return (parseStatus){.type=SUCCESS, .token=NULL};
}
parseStatus parseAddInstruction(char *arguments)
{
   return parseType1Instruction(arguments);
}
parseStatus parseAndInstruction(char *arguments)
{
    return parseType1Instruction(arguments);
}
parseStatus parseBrInstruction(char *arguments)
{
    return parseType2Instruction(arguments);
}
parseStatus parseJsrInstruction(char *arguments)
{
    return parseType2Instruction(arguments);
}
parseStatus parseJsrrInstruction(char *arguments)
{
    return parseType3Instruction(arguments);
}
parseStatus parseJmpInstruction(char *arguments)
{
    return parseType3Instruction(arguments);
}
parseStatus parseRetInstruction(char *arguments)
{
    return parseType4Instruction(arguments);
}
parseStatus parseRtiInstruction(char *arguments)
{
    return parseType4Instruction(arguments);
}
parseStatus parseLdInstruction(char *arguments)
{
    return parseType5Instruction(arguments);
}
parseStatus parseLdiInstruction(char *arguments)
{
    return parseType5Instruction(arguments);
}
parseStatus parseLeaInstruction(char *arguments)
{
    return parseType5Instruction(arguments);
}
parseStatus parseStInstruction(char *arguments)
{
    return parseType5Instruction(arguments);
}
parseStatus parseStiInstruction(char *arguments)
{
    return parseType5Instruction(arguments);
}
parseStatus parseNotInstruction(char *arguments)
{
    return parseType6Instruction(arguments);
}
parseStatus parseLdrInstruction(char *arguments)
{
    return parseType7Instruction(arguments);
}
parseStatus parseStrInstruction(char *arguments)
{
    return parseType7Instruction(arguments);
}

parseStatus parseTrapInstruction(char *arguments)
{
    return parseType8Instruction(arguments);
}
parseStatus parseGetcInstruction(char *arguments)
{
    return parseType4Instruction(arguments);
}
parseStatus parseOutInstruction(char *arguments)
{
    return parseType4Instruction(arguments);
}
parseStatus parsePutsInstruction(char *arguments)
{
    return parseType4Instruction(arguments);
}
parseStatus parseInInstruction(char *arguments)
{
    return parseType4Instruction(arguments);
}
parseStatus parsePutspInstruction(char *arguments)
{
    return parseType4Instruction(arguments);
}
parseStatus parseHaltInstruction(char *arguments)
{
    return parseType4Instruction(arguments);
}

 
void printParseErrorMessage(parseStatus status)
{
    // array of function 
    // priority in error report
    switch(status.type){
        case END_OF_FILE_REACH                 :      ;
        case SUCCESS                           : break;
        case INVALID_NUMBER             : printErrorMessage("INVALID_NUMERIC_VALUE ");
                                            break;
        case INVALID_REGISTER                  : printErrorMessage("INVALID_REGISTER ");
                                            break; 
        case INVALID_REGISTER_OR_NUMBER : printErrorMessage("INVALID_REGISTER_OR_IMM5_VALUE ");
                                            break;
        case INVALID_LABEL                     : printErrorMessage("INVALID_LABEL ");
                                            break;          
        case INVALID_LABEL_OR_NUMBER           : printErrorMessage("INVALID_LABEL_OR_NUMBER ");
                                            break;
        case DUPPLICATE_LABEL                  : printErrorMessage("DUPPLICATE_LABEL ");
                                            break;
        case UNDEFINED_LABEL                   : printErrorMessage("UNDEFINED_LABEL ");
                                            break;
        case INUTILE_LABEL                     : printErrorMessage("INUTILE_LABEL ");
                                            break;
        case TOO_MANY_ARGUMENTS                : printErrorMessage("TOO_MANY_ARGUMENTS ");
                                            break;
        case FEW_ARGUMENT                      : printErrorMessage("FEW_ARGUMENT");
                                            break;
        case INVALID_PROGRAM_STARTING_ADDRESS  : printErrorMessage("INVALID_PROGRAM_STARTING_ADDRESS");
                                            break;
        case MULTIPLE_PROGRAM_STARTING_ADDRESS : printErrorMessage(" MULTIPLE_PROGRAM_STARTING_ADDRESS");
                                            break; 
        case INVALID_STRING                    : printErrorMessage(" INVALID_STRING");
                                            break;  
        case UNDEFINED_PROGRAM_STARTING_ADDRESS: printErrorMessage(" \".ORIG\" is not found! ");
                                            break; 
        case UNDEFINED_TOKEN              : printErrorMessage("UNDEFINED_TOKEN_FOUND");
                                            break;  
        case LABEL_BEFORE_PROGRAM_STARTING_ADDRESS : printErrorMessage("\".ORIG\" is not found yet!"
                                        "\".ORIG\" must be at the beginning of the source file ");
                                            break;  
        case CANNOT_ADD_LABEL_TO_SYMBOL_TABLE :  printErrorMessage(" CANNOT_ADD_LABEL_TO_SYMBOL_TABLE ");
                                            break;                                                                                        
        default                           : printErrorMessage("INVALID ERROR %d", status.type);
    } 
    if (status.token != NULL)
        printf("[%s]", status.token);
    printf("\n");
}
