#ifndef __LASM_HASH__
#define __LASM_HASH__

#include <stdint.h>

#define HASH_TABLE_SIZE 257

struct item {
    struct item *next;
    char *label;           // key 
    uint16_t address;      // value 
};
extern struct item *hashTable[HASH_TABLE_SIZE];

typedef enum {
    HASH_ADD_MEMORY_ALLOCATION_ERROR,
    HASH_ADD_DUPPLICATE_LABEL,
    HASH_ADD_SUCCESS
} hashAddStatus;

typedef struct {
    enum {
        HASH_SEARCH_FAILURE,
        HASH_SEARCH_SUCCESS,
    } status; 
    uint16_t address;
} hashSearchResult;

uint16_t hashString(char *str);
hashAddStatus addItemToHashTable(char *label, uint16_t address);
hashSearchResult searchLabelInHashTable(char *label);
void deleteHashTable();
void printHashTable();

#endif // __LASM_HASH__
