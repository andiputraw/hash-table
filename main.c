#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct String
{
    int len;
    int cap;
    char *val;
} String;

// Initialise a string with c-style string
void StringInit(String *string, char *cstr);
// Compare two string
bool StringCmp(String *s1, String *s2);
// get hash value of a string
int StringHash(String *const s);
// to assert to two string. for testing purpose
void StringAssert(String *s1, String *s2);

// HashTable method
// set item to hash table
void HashTableSetItem(HashTable *ht, Item *item);
// get item from hash table
Item *HashTableGetItem(HashTable *ht, String *const key);

void StringInit(String *string, char *strlit)
{
    int l = strlen(strlit);
    string->len = l;
    string->cap = l;
    string->val = strlit;
}

bool StringCmp(String *s1, String *s2)
{
    if (s1 == NULL || s2 == NULL)
    {
        return false;
    }
    if (s1->len != s2->len)
    {
        return false;
    }
    return strcmp(s1->val, s2->val) == 0;
}

int StringHash(String *const s)
{
    int byte = 0;
    for (int i = 0; i < s->len; i++)
    {
        byte += s->val[i];
    }
    return byte;
}

void StringAssert(String *s1, String *s2)
{
    // if both is null. it is ok
    if (s1 == NULL && s2 == NULL)
    {
        return;
    }
    if (!StringCmp(s1, s2))
    {
        printf("Expected %s get %s", s1->val, s2->val);
        exit(1);
    }
}

typedef struct Item
{
    String *key;
    String *val;
    struct Item *next;
} Item;

typedef struct HashTable
{
    int len;
    Item **bucket;
} HashTable;

// int HashTableInit(HashTable *ht, int len)
// {
//     ht->len = len;
//     Item **bucket = (Item **)malloc(sizeof(Item *) * len);
//     if (bucket == NULL)
//     {
//         return -1;
//     }
//     return 0;
// }

void HashTableSetItem(HashTable *ht, Item *item)
{
    int index = StringHash(item->key) % ht->len;

    Item *curItem = ht->bucket[index];
    // check if the item is not set
    if (curItem == NULL)
    {
        ht->bucket[index] = item;
        return;
    }
    // Check if the key equal
    if (StringCmp(curItem->key, item->key))
    {
        ht->bucket[index] = item;
        return;
    }
    // hash collition happened
    while (curItem->next != NULL)
    {
        // check if the next item key is equal
        if (StringCmp(curItem->next->key, item->key))
        {
            // set the next next item to retain the linked list;
            Item *nextItem = curItem->next->next;
            item->next = nextItem;
            curItem->next = item;
            break;
        }
        else
        {
            // contine walk to next link
            curItem = curItem->next;
        }
    }
    // No same key on collision
    if (curItem->next == NULL)
    {
        curItem->next = item;
    }
}

Item *HashTableGetItem(HashTable *ht, String *const key)
{
    int index = StringHash(key) % ht->len;
    Item *item = ht->bucket[index];
    // check until the key equal or the item is null
    while (item != NULL && !StringCmp(item->key, key))
    {
        item = item->next;
    }
    return item;
}

int main()
{
    String foo = {0};
    StringInit(&foo, "foo");
    String bar = {0};
    StringInit(&bar, "bar");
    Item *bucket[10] = {0};
    HashTable ht = {0};
    ht.len = 10;
    ht.bucket = bucket;

    // set an item
    Item item = {0};
    item.key = &foo;
    item.val = &bar;
    HashTableSetItem(&ht, &item);

    // Get an item
    Item *result = HashTableGetItem(&ht, &foo);
    StringAssert(result->val, &bar);

    // nonexistent key
    String baz = {0};
    StringInit(&baz, "baz");
    Item *result2 = HashTableGetItem(&ht, &baz);
    if (result2 != NULL)
    {
        printf("key %s should return NULL", baz.val);
        return 1;
    }

    // re set an item
    Item item2 = {0};
    item2.key = &foo;
    item2.val = &baz;
    HashTableSetItem(&ht, &item2);
    Item *result3 = HashTableGetItem(&ht, &foo);
    StringAssert(result3->val, &baz);

    // hash collision
    String BD = {0};
    StringInit(&BD, "BD"); // both is 134 total byte
    String CC = {0};
    StringInit(&CC, "CC"); // both is 134 total byte

    Item item3 = {0};
    item3.key = &BD;
    item3.val = &foo;

    Item item4 = {0};
    item4.key = &CC;
    item4.val = &bar;

    HashTableSetItem(&ht, &item3);
    HashTableSetItem(&ht, &item4);

    Item *CCResult = HashTableGetItem(&ht, &CC);
    Item *BDResult = HashTableGetItem(&ht, &BD);

    StringAssert(BDResult->val, &foo);
    StringAssert(CCResult->val, &bar);

    printf("Test ok\n");

    return 0;
}