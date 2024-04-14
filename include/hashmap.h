#ifndef HASHMAP_H
#define HASHMAP_H

#define MAP_SIZE 25
#define KEY_SIZE 256

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

struct bucket {
    char key[KEY_SIZE];
    int val;
    struct bucket * next;
};

struct bucket * hashmap[MAP_SIZE];

struct bucket * search_map (const char * key);
void add_to_map (const char * key, int val);
void init_hashmap ();

#endif