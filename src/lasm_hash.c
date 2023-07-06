#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "lasm_hash.h"

struct item *hashTable[HASH_TABLE_SIZE] = {NULL};

// dupplicateString() : allocate memory in the heap. 
// Deallocation is done in deleteHashTable()
static inline char *dupplicateString(char *s)
{
    char *d = (char*)malloc((strlen(s) + 1) * sizeof(char));
    if (d == NULL)
        exit(EXIT_FAILURE);
    strcpy(d, s);
    return d;
}
// maximum length supported ?? 
uint16_t hashString(char *str)
{
    unsigned long int hashvalue = 0UL;
    for (; *str != '\0'; str++) 
        hashvalue = *str + 31 * hashvalue;
    return (uint16_t)(hashvalue % HASH_TABLE_SIZE);
}
// addItemToHashTable() : allocate memory in the heap.
// Deallocation is done in deleteHashTable()
hashAddStatus addItemToHashTable(char *label, uint16_t address)
{
    uint16_t hashvalue = hashString(label);
    struct item *item = (struct item*)malloc(sizeof(*item));
    if (item == NULL) return HASH_ADD_MEMORY_ALLOCATION_ERROR;    
    item->label = dupplicateString(label);
    item->address = address;
    item->next = NULL;
    if (hashTable[hashvalue] == NULL) {   // No collision 
        hashTable[hashvalue] = item;
    } else {                              // Collision 
        struct item *tail;
        for (tail = hashTable[hashvalue]; tail->next != NULL; tail = tail->next) 
            if (strcmp(tail->label, label) == 0)  return HASH_ADD_DUPPLICATE_LABEL;
        if (strcmp(tail->label, label) == 0)      return HASH_ADD_DUPPLICATE_LABEL;    
        tail->next = item;
    }
    return HASH_ADD_SUCCESS;
}

hashSearchResult searchLabelInHashTable(char *label)
{
    struct item *item= hashTable[hashString(label)];
    for (; item!= NULL; item= item->next) 
        if (strcmp(item->label, label) == 0) 
            return (hashSearchResult){.status = HASH_SEARCH_SUCCESS,
                                      .address = item->address};
    return (hashSearchResult){.status = HASH_SEARCH_FAILURE};
}
//  deleteHashTable() frees all memory allocated 
//  by hash table's utilities functions   
void deleteHashTable() {
    struct item *current, *next;
    for (int i = 0; i  < HASH_TABLE_SIZE; i++) {
        current = hashTable[i];
        while (current != NULL) {
            next = current->next;
            free(current->label);
            free(current);
            current = next;
        }
    }
}

void printHashTable()
{
    struct item *current;
    for (int i = 0; i  < HASH_TABLE_SIZE; i++) {
        current = hashTable[i];
        if (current == NULL) continue;
        printf("%d : ", i);
        while (current != NULL) {
            printf("(%s, %hu)", current->label, current->address);
            printf("--->");
            current = current->next;
        }
        printf("(END)\n");
    }
}

