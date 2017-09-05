//
// Created by evolution on 17-8-29.
//

#ifndef HASH_TABLE_MAIN_H
#define HASH_TABLE_MAIN_H

#define HASH_TABLE_INIT_SIZE 3
#define HASH_INDEX(ht, key) (hash_str((key)) % (ht)->size)
#define ECHO(val) printf("%d \n", val)

#define FAILED -1
#define SUCCESSS 0

#define DEBUG 0

#if defined(DEBUG)
#   define LOG_MSG printf
#else
#   define LOG_MSG(...)
#endif

typedef struct _Bucket
{
    char *key;
    void *value;
    struct _Buckey * next;
} Bucket;

typedef struct _HashTable
{
    int size;
    int elem_num;
    Bucket** buckets;
} HashTable;

int hash_init(HashTable *ht);
int hash_lookup(HashTable *ht, char *key, void **result);
int hash_insert(HashTable *ht, char *key, void *value);
int hash_remove(HashTable *ht, char *key);
int hash_destroy(HashTable *ht);

#endif //HASH_TABLE_MAIN_H
