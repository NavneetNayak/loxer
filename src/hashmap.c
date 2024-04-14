#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/hashmap.h"

static int hash_code (const char * key);
static void add_node (struct bucket ** first, const char * key, int val);

void init_hashmap () {
    for (int i = 0; i < MAP_SIZE; i++) hashmap[i] = NULL;
}

static int hash_code (const char * key) {
    int len = strnlen(key, KEY_SIZE);
    int hash;

    while (len--) {
        hash = *key + pow(31, len);
    }

    return hash % MAP_SIZE;
}

static void add_node (struct bucket ** first, const char * key, int val) {
    struct bucket * new_node;

    new_node = malloc(sizeof(struct bucket));
    if (new_node == NULL) {
        printf("Allocation Error.");
        return;
    }

    strncpy(new_node->key, key, KEY_SIZE);
    new_node->val = val;
    new_node->next = (*first);

    (*first) = new_node;
}

void add_to_map (const char * key, int val) {
    if (key == NULL) return;

    int index = hash_code(key);

    if (hashmap[index] == NULL) {
        hashmap[index] = malloc(sizeof(struct bucket));

        if (hashmap[index] == NULL) {
            printf("Allocation Error.");
            return;
        }

        strncpy(hashmap[index]->key, key, KEY_SIZE);
        hashmap[index]->val = val;
        hashmap[index]->next = NULL;

        return;
    }

    add_node(&hashmap[index], key, val);
}

struct bucket * search_map (const char * key) {
    int index = hash_code(key);

    if (hashmap[index] == NULL) {
        return NULL;
    }

    struct bucket * list = hashmap[index];
    while (list != NULL) {
        if (strncmp(list->key, key, KEY_SIZE) == 0) return list;
        list = list->next;
    }

    return NULL;
}

