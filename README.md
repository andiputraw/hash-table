# Hash Table
Hash table implementation in c.

this project is created to test the owner of this repositories in C and algorithm and data structure

## Feature

1. No dynamic memory allocation.
2. Hash collision handling.

## Drawback

1. Slow.
2. Lack of feature. such as resizeable

## Implementation

there's a custom struct called `String` as a wrapper to c-style string. 

this string has a several function associated with it.

```c
// Initialise a string with c-style string
void StringInit(String *string, char *cstr);
// Compare two string
bool StringCmp(String *s1, String *s2);
// get hash value of a string
int StringHash(String *const s);
// to assert to two string. for testing purpose
void StringAssert(String *s1, String *s2);
```

this string can be used as a key to the hashtable. and (for now ad probably forever) as a value to hash table.  
for now, the hash implementations is simply to combine all bytes in string. 

and for the hash table. there's this function associated with it.  

```c
// set item to hash table
void HashTableSetItem(HashTable *ht, Item *item);
// get item from hash table
Item *HashTableGetItem(HashTable *ht, String *const key);
```

the `HashTableSetItem` will call the hash function, modulo it with the length to get the index. that index can be used to set item to the `bucket`. if the index unfortunately already taken, that's mean there's a collission happened. that's why the type `Item` is actually linked list.

```c
typedef struct Item
{
    String *key;
    String *val;
    struct Item *next;
} Item;
```

if the index is taken, we simply set item to the next properties in the struct. it is just a linked list. 

if the set item have a same key string with the hash table. we override the string key, that unfortunately will be a problem in dynamically allocated Item. unless you are using arena allocator or something similar.

