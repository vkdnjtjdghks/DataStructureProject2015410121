#include "Head.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef Hash_h
#define Hash_h
typedef char* Hash_Data;
typedef int Hash_Int;
typedef struct _ElemT
{
	Hash_Data    HData;
	Hash_Int  HInt;

	enum ElementStatus Status;
} ElemT;
enum ElementStatus { EMPTY, OCCUPIED };
typedef struct tagHashTable
{
	int           FullNum;
	int           TableCapacity;

	ElemT*  Table;
} HashTable;

void       Hash_Set(HashTable** HT, Hash_Data HData, Hash_Int HInt);
Hash_Int  Hash_Get(HashTable* HT, Hash_Data HData);
int        Hash_Utilize(Hash_Data HData, int KeyLength, int TableCapacity);
int        Hash_Utilize_Second(Hash_Data HData, int KeyLength, int TableCapacity);
HashTable * CreateHashTable(int TableCapacity);
void       DestroyHashTable(HashTable* HT);
void       EmptyElement(ElemT* Element);
#endif

