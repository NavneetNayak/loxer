#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include "../include/ht.h"

static int hash_key (const char * key);
static const char * ht_set_entry (ht_entry * entries, size_t capacity, const char * key, int val, size_t * length);
static bool ht_expand (ht * table);

static int hash_key (const char * key) {
	int len = strlen(key);
	int hash;

	while (len--) {
		hash = *key * pow(31, len);
	}

	return hash;
}

#define INITIAL_CAPACITY 20

ht * ht_init (void) {
	ht * table = malloc (sizeof(ht));
	if (table == NULL) {
		return NULL;
	}

	table->entries = calloc(INITIAL_CAPACITY, sizeof(ht_entry));
	if (table->entries == NULL) {
		free(table);
		return NULL;
	}

	table->capacity = INITIAL_CAPACITY;
	table->length = 0;

	return table;
}

void ht_del (ht * table) {
	if (table == NULL) {
		return;
	}

	if (table->entries == NULL) {
		free(table);
		return;
	}
    
	free(table->entries);
	free(table);

	return;
}

int * ht_get (ht * table, const char * key) {
	assert(table != NULL && table->entries != NULL);
	if (table == NULL) {
		return NULL;
	}
	if (table->entries == NULL) {
		return NULL;
	}

	assert(key != NULL);
	if (key == NULL) {
		return NULL;
	}

	int hash = hash_key(key);
	size_t index = (size_t) (hash % table->capacity);

	while (table->entries[index].key != NULL) {
		if (strcmp(table->entries[index].key, key) == 0) {
			return &(table->entries[index].val);
		}

		index++;
		if (index > table->capacity) {
			index = 0;
		}
	}

	return NULL;
}

const char * ht_set (ht * table, const char * key, int val) {
	assert(key != NULL);
	if (key == NULL) {
		return NULL;
	}

	assert(table != NULL && table->entries != NULL);
	if (table == NULL) {
		return NULL;
	}
	if (table->entries == NULL) {
		return NULL;
	}

	if (table->length > table->capacity / 2) {
		if (ht_expand(table) == false) {
			return NULL;
		}
	}

	return ht_set_entry(table->entries, table->capacity, key, val, &(table->length));
}

const char *
ht_set_entry (ht_entry * entries, size_t capacity, const char * key, int val, size_t * length) {
	int hash = hash_key(key);
	size_t index = (size_t) (hash % capacity);

	while (entries[index].key != NULL) {
		if (strcmp(entries[index].key, key) == 0) {
			entries[index].val = val;
			return entries[index].key;
		}

		index++;
		if (index > capacity) {
			index = 0;
		}
	}

	if (length == NULL) {
		return NULL;
	}
	(*length)++;

	entries[index].key = strdup(key);
	entries[index].val = val;
	
	return entries[index].key;
}

static bool ht_expand (ht * table) {
	assert(table != NULL && table->entries != NULL);
	if (table == NULL) {
		return false;
	}
	if (table->entries == NULL) {
		return false;
	}

	size_t new_capacity = table->capacity * 2;
	if (new_capacity < table->capacity) {
		return false;
	}

	ht_entry * new_entries = calloc(new_capacity, sizeof(ht_entry));
	if (new_entries == NULL) {
		return false;
	}

	for (size_t i = 0; i < table->capacity; i++) {
		if (table->entries[i].key == NULL) {
			continue;
		}

		ht_entry entry = table->entries[i];

		ht_set_entry(new_entries, new_capacity, entry.key, entry.val, NULL);
	}

	free(table->entries);

	table->entries = new_entries;
	table->capacity = new_capacity;

	return true;
}

size_t ht_len (ht * table) {
	return table->length;
}