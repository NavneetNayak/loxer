#ifndef HT_H
#define HT_H

#include <stdlib.h>

typedef struct {
	const char * key;
	int val;
} ht_entry;

typedef struct {
	ht_entry * entries;

	size_t capacity;
	size_t length;
} ht;

ht * ht_init (void);
void ht_del (ht * table);
int * ht_get (ht * table, const char * key);
const char * ht_set (ht * table, const char * key, int val);
size_t ht_len (ht * table); 

#endif