#include "Hash.h"

HashTable* CreateHashTable(int TableCapacity)
{
	HashTable* HT = (HashTable*)malloc(sizeof(HashTable));
	HT->Table = (ElemT*)malloc(sizeof(ElemT) * TableCapacity);

	memset(HT->Table, 0, sizeof(ElemT) * TableCapacity);

	HT->TableCapacity = TableCapacity;
	HT->FullNum = 0;

	return HT;
}

void Hash_Set(HashTable** HT, Hash_Data HData, Hash_Int HInt)
{
	int    KeyLen, Address, StepSize;

	KeyLen = strlen(HData);
	Address = Hash_Utilize(HData, KeyLen, (*HT)->TableCapacity);
	StepSize = Hash_Utilize_Second(HData, KeyLen, (*HT)->TableCapacity);

	while ((*HT)->Table[Address].Status != EMPTY &&
		strcmp((*HT)->Table[Address].HData, HData) != 0)
	{

		Address = (Address + StepSize) % (*HT)->TableCapacity;
	}

	(*HT)->Table[Address].HData = (char*)malloc(sizeof(char) * (KeyLen + 1));
	strcpy((*HT)->Table[Address].HData, HData);

	(*HT)->Table[Address].HInt = HInt;

	(*HT)->Table[Address].Status = OCCUPIED;

	(*HT)->FullNum++;

}

Hash_Int Hash_Get(HashTable* HT, Hash_Data HData)
{
	int KeyLen = strlen(HData);

	int Address = Hash_Utilize(HData, KeyLen, HT->TableCapacity);
	int StepSize = Hash_Utilize_Second(HData, KeyLen, HT->TableCapacity);

	while (HT->Table[Address].Status != EMPTY &&
		strcmp(HT->Table[Address].HData, HData) != 0)
	{
		Address = (Address + StepSize) % HT->TableCapacity;
	}

	return HT->Table[Address].HInt;
}

void EmptyElement(ElemT* Element)
{
	if (Element->Status == EMPTY)
		return;

	free(Element->HData);
}

void DestroyHashTable(HashTable* HT)
{

	int i = 0;
	for (i = 0; i<HT->TableCapacity; i++)
	{
		EmptyElement(&(HT->Table[i]));
	}


	free(HT->Table);
	free(HT);
}

int Hash_Utilize(Hash_Data HData, int KeyLength, int TableCapacity)
{
	int i = 0;
	long long HashValue = 0;

	for (i = 0; i<KeyLength; i++)
	{
		int Abs = (HData[i] > 0) ? HData[i] : -HData[i];
		HashValue = (HashValue << 3) + Abs;
		HashValue = HashValue % TableCapacity;
	}


	return (int)HashValue;
}

int Hash_Utilize_Second(Hash_Data HData, int KeyLength, int TableCapacity)
{
	int i = 0;
	long long HashValue = 0;

	for (i = 0; i<KeyLength; i++)
	{
		int Abs = (HData[i] > 0) ? HData[i] : -HData[i];
		HashValue = (HashValue << 2) + Abs;
		HashValue = HashValue % (TableCapacity - 3);
	}

	return (int)HashValue + 1;
}
