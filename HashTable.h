#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct HashNode
{
    char * key;
    void * data;
    HashNode* next;
};

struct HashTable
{
    int nodes_size;
    HashNode ** nodes;
};

HashTable * HashTable_Make(int size)
{
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->nodes_size = size;
    table->nodes = (HashNode**) malloc(sizeof(HashNode*) * size);
    for (int n = 0; n < size; n++)
        table->nodes[n] = NULL;

    return table;
}

void HashNode_Free(HashNode* node, int free_data)
{
    if (node == NULL) return;

    if (free_data) free(node->data);
    free(node->key);

    if (node->next != NULL)
        HashNode_Free(node->next, free_data);

    free(node);

}

void HashTable_Free(HashTable* table, int free_data)
{
    for (int n = 0; n < table->nodes_size; n++)
    {
        HashNode* node = table->nodes[n];
        HashNode_Free(node, free_data);
    }

    free(table->nodes);
    free(table);
}

// djb2 hash function
unsigned long hash(const char * str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

HashNode* HashTable_GetNode(HashTable * table, const char * key)
{
    int idx = hash(key) % table->nodes_size;
    HashNode * iter = table->nodes[idx];

    while (iter != NULL)
    {
        if (strcmp(key, iter->key) == 0) break;
        iter = iter->next;
    }

    return iter;
}


void HashTable_Print(HashTable* table)
{
    for (int i = 0; i < table->nodes_size; i++)
    {
        printf("--- Index %d ---\n", i);
        HashNode* iter = table->nodes[i];
        if (iter == NULL)
        {
            printf(" <void> \n");
            continue;
        }

        while (iter != NULL)
        {
            printf(" * %s\n", iter->key);
            iter = iter->next;
        }
    }
}

HashNode* HashTable_InsertNode(HashTable* table, const char * key, void * data)
{

    int idx = hash(key) % table->nodes_size;
    if (HashTable_GetNode(table, key) != NULL)
    {
       fprintf(stderr, "The node with key %s already exists, use HashTable_Update instead\n",
               key);
       return NULL;
    }

    // Create new node
    HashNode* new_node = (HashNode*) malloc(sizeof(HashNode));
    new_node->data = data;
    new_node->key = (char *) malloc((strlen(key) + 1) * sizeof(char));
    strcpy(new_node->key, key);

    if (table->nodes[idx] == NULL)
    {
        table->nodes[idx] = new_node;
        new_node->next = NULL;
    }
    else
    {
        HashNode * aux = table->nodes[idx];
        new_node->next = aux;
        table->nodes[idx] = new_node;
    }

    return new_node;
}

#endif // HASHTABLE_H
