#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "hashtable.h"

static void resize_hash_table_if_needed(HashTable *ht);
static int hash_str(char *key);

static int hash_str(char *key)
{
    int hash = 0;
    char *cur = key;
    while (*cur != '\0') {
        hash += *cur;
//        printf("--%d--", hash);
        ++cur;
    }

    return hash;
}

/**
 * Hash table init
 * @param ht
 * @return
 */
int hash_init(HashTable *ht)
{
    ht->size = HASH_TABLE_INIT_SIZE;
    ht->elem_num = 0;
    ht->buckets = (Bucket **)calloc(ht->size, sizeof(Bucket *));

    if (ht->buckets == NULL) return FAILED;

    LOG_MSG("[init]\t size: %i\n", ht->size);

    return SUCCESSS;
}

/**
 * lookup a key
 * @param ht
 * @param key
 * @param result
 * @return
 */
int hash_lookup(HashTable *ht, char *key, void **result)
{
    int index = HASH_INDEX(ht, key);
    Bucket *bucket = ht->buckets[index];
    if (bucket == NULL) {
        LOG_MSG("[lookup]\t key:%s\tfailed\t\n", key);
        return FAILED;
    }

    while (bucket)
    {
        if (strcmp(bucket->key, key) == 0) {
            LOG_MSG("[lookup]\t found %s \t index:%i  value %s, addr:%p\n", key, index, bucket->value, bucket->value);

            *result = bucket->value;

            return SUCCESSS;
        }

        bucket = bucket->next;
    }

}

/**
 * 向hash表中插入元素
 * @param ht
 * @param key
 * @param value
 * @return
 */
int hash_insert(HashTable *ht, char *key, void *value)
{
//    ECHO(ht->size);
//    ECHO(ht->elem_num);

    //check if we need to resize the hashtable
    resize_hash_table_if_needed(ht);

    int index = HASH_INDEX(ht, key);

    Bucket *org_bucket = ht->buckets[index];
    Bucket *tmp_bucket = org_bucket;

    //check if the key exists already
    while (tmp_bucket)
    {
        if (strcmp(key, tmp_bucket->key) == 0)
        {
            LOG_MSG("[update]\tkey: %s\n", key);
            tmp_bucket->value =  value;

            return SUCCESSS;
        }

        tmp_bucket = tmp_bucket->next;
    }

    Bucket *bucket = (Bucket *)malloc(sizeof(Bucket));

    bucket->key = key;
    bucket->value = value;
    bucket->next = NULL;

    ht->elem_num += 1;

    if (org_bucket != NULL) {
        LOG_MSG("[collision]\t index:%d key:%s\n", index, key);
        bucket->next = org_bucket;
    }

    ht->buckets[index] = bucket;

    LOG_MSG("[insert]\t index:%d key:%s \t ht(num:%d)\n", index, key, ht->elem_num);

    return SUCCESSS;
}

/**
 * 在hash table 中移除元素
 * @param ht
 * @param key
 * @return
 */
int hash_remove(HashTable *ht, char *key)
{
    int index = HASH_INDEX(ht, key);
    Bucket *bucket = ht->buckets[index];
    Bucket *prev = NULL;

    if (bucket == NULL) return FAILED;

    //find the right bucket from the link list
    while (bucket)
    {
        if (strcmp(bucket->key, key) == 0)
        {
            LOG_MSG("[remove]\t key:(%s) index: %d\n", key, index);

            if (prev == NULL)
            {
                ht->buckets[index] = bucket->next;
            } else {
                prev->next = bucket->next;
            }

            free(bucket);

            return SUCCESSS;
        }

        prev = bucket;
        bucket = bucket->next;

    }

    LOG_MSG("[remove]\t key:%s not found remove \t failed\t\n", key);
    return FAILED;
}

/**
 * 销毁hash table
 * @param ht
 * @return
 */
int hash_destroy(HashTable *ht)
{
    int i;
    Bucket *cur = NULL;
    Bucket *tmp = NULL;

    for (int j = 0; j < ht->size; ++j) {
        cur = ht->buckets[i];
        while (cur)
        {
            tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }

    free(ht->buckets);

    return SUCCESSS;
}

//todo 验证 sizeof(Bucket **) sizeof(Bucket *) sizeof(Bucket)

/**
 * 扩充hash table double
 * @param ht
 * @return
 */
static int hash_resize(HashTable *ht)
{
    //double the hashtable size
    int org_size = ht->size;
    ht->size = ht->size * 2;
    ht->elem_num = 0;

    LOG_MSG("[resize]\t org size: %i\t new size: %i\n", org_size, ht->size);

    Bucket **buckets = (Bucket **)calloc(ht->size, sizeof(Bucket **));
    Bucket **org_buckets = ht->buckets;
    ht->buckets = buckets;

    for (int i = 0; i < org_size; ++i) {
        Bucket *cur = org_buckets[i];
        Bucket *tmp = NULL;
        while (cur)
        {
            hash_insert(ht, cur->key, cur->value);
            tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }

    free(org_buckets);

    LOG_MSG("[resize] done \n");

    return SUCCESSS;

}

//if the elem_num is almost as large as the capacity of the hashtable
//we new to resize the hashtable to contain enough elements
static void resize_hash_table_if_needed(HashTable *ht)
{
    if (ht->size - ht->elem_num < 1)
    {
        ECHO(111);
        hash_resize(ht);
    }
}