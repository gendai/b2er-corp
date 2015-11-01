#include "Vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const size_t VECTOR_NEGATIVE_SEARCH = (size_t) -1;

Vector *VectorNew()
{
	return VectorNewWithReserve(VECTOR_DEFAULT_ALLOC);
}

Vector *VectorNewWithReserve(size_t n)
{
	Vector *v = malloc(sizeof(Vector));
	v->size = 0;
	v->allocSize = n;
	v->items = malloc(sizeof(VectorItem) * n);
	return v;
}

Vector *VectorNewFromArray(VectorItem *arr, size_t size)
{
	size_t allocSize = VECTOR_DEFAULT_ALLOC;
	while(allocSize < size)
		allocSize *= 2;

	Vector *v = VectorNewWithReserve(allocSize);
	memcpy(v->items, arr, sizeof(VectorItem) * size);
	v->size = size;
	return v;
}

Vector *VectorNewWithSize(size_t size)
{
	size_t allocSize = VECTOR_DEFAULT_ALLOC;
	while(size > allocSize)
		allocSize *= 2;

	Vector *v = VectorNewWithReserve(allocSize);
	v->size = size;
	return v;
}

Vector *VectorNewWithValue(VectorItem val, size_t size)
{
	size_t allocSize = VECTOR_DEFAULT_ALLOC;
	while(allocSize < size)
		allocSize *= 2;

	Vector *v = VectorNewWithReserve(allocSize);
	for(size_t i = 0; i < size; ++i)
		v->items[i] = val;
	v->size = size;
	return v;
}

Vector *VectorNewFromVector(Vector *v)
{
	Vector *clone = VectorNewWithReserve(v->allocSize);
	clone->size = v->size;
	memcpy(clone->items, v->items, v->size * sizeof(VectorItem));
	return clone;
}

void VectorFree(Vector *v)
{
	free(v->items);
	free(v);
}

void VectorFreeAll(Vector *v)
{
	for(size_t i = 0; i < v->size; ++i)
		free(v->items[i]);
	VectorFree(v);
}



size_t VectorSize(Vector *v)
{
	return v->size;
}

int VectorEmpty(Vector *v)
{
	return !v->size;
}

VectorItem *VectorData(Vector *v)
{
	return v->items;
}

VectorItem VectorGet(Vector *v, size_t pos)
{
	return v->items[pos];
}

VectorItem VectorGetFront(Vector *v)
{
	return v->items[0];
}

VectorItem VectorGetBack(Vector *v)
{
	return v->items[v->size-1];
}

void VectorSet(Vector *v, size_t pos, VectorItem e)
{
	v->items[pos] = e;
}

void VectorInsert(Vector *v, size_t pos, VectorItem e)
{
	size_t newSize = v->size+1;
	int resize = 0;
	VectorItem *src = v->items;
	
	if(newSize > v->allocSize)
	{
		v->allocSize *= 2;
		v->items = malloc(v->allocSize * sizeof(VectorItem));
		memmove(v->items, src, pos * sizeof(VectorItem));
		resize = 1;
	}

	memmove(v->items + pos + 1, src + pos, (v->size-pos) * sizeof(VectorItem));
	v->items[pos] = e;
	v->size = newSize;

	if(resize)
		free(src);
}

void VectorInsertArray(Vector *v, size_t pos, VectorItem *a, size_t size)
{
	size_t newSize = v->size+size;
	int resize = 0;
	VectorItem *src = v->items;
	while(newSize > v->allocSize)
	{
		v->allocSize *= 2;
		resize = 1;
	}

	if(resize)
	{
		v->items = malloc(v->allocSize * sizeof(VectorItem));
		memmove(v->items, src, pos * sizeof(VectorItem));
	}

	memmove(v->items + pos + size, src + pos, (v->size-pos) * sizeof(VectorItem));
	memcpy(v->items + pos, a, size * sizeof(VectorItem));
	v->size = newSize;

	if(resize)
		free(src);
}

void VectorRemove(Vector *v, size_t pos, size_t size)
{
	memmove(v->items + pos, v->items + pos +size, (v->size - pos - size) * sizeof(VectorItem));
	v->size -= size;
}

void VectorResize(Vector *v, size_t size)
{
	if(size > v->allocSize)
	{
		while(size > v->allocSize)
			v->allocSize *= 2;

		VectorItem *temp = v->items;
		v->items = malloc(v->allocSize * sizeof(VectorItem));
		memmove(v->items, temp, v->size * sizeof(VectorItem));
		free(temp);
	}

	v->size = size;
}

void VectorClear(Vector *v)
{
	v->size = 0;
}

void VectorReserve(Vector *v, size_t size)
{
	if(size <= v->allocSize)
		return;

	v->allocSize = size;
	VectorItem *temp = v->items;
	v->items = malloc(v->allocSize * sizeof(VectorItem));
	memmove(v->items, temp, v->size * sizeof(VectorItem));
	free(temp);
}

void VectorShrinkToFit(Vector *v)
{
	VectorItem *temp = v->items;
	v->items = malloc(v->size * sizeof(VectorItem));
	memmove(v->items, temp, v->size * sizeof(VectorItem));
	v->allocSize = v->size;
	free(temp);
}

size_t VectorIndexOf(Vector *v, VectorItem e)
{
	for(size_t i = 0; i < v->size; ++i)
		if(v->items[i] == e)
			return i;
	return VECTOR_NEGATIVE_SEARCH;
}

size_t VectorLastIndexOf(Vector *v, VectorItem e)
{
	for(size_t i = v->size - 1; i < v->size; --i)
	{
		if(v->items[i] == e)
			return i;
	}
		return VECTOR_NEGATIVE_SEARCH;
}


void VectorInsertFront(Vector *v, VectorItem e)
{
	VectorInsert(v, 0, e);
}

void VectorInsertArrayFront(Vector *v, VectorItem *a, size_t size)
{
	VectorInsertArray(v, 0, a, size);
}

void VectorInsertBack(Vector *v, VectorItem e)
{
	VectorInsert(v, v->size, e);
}

void VectorInsertArrayBack(Vector *v, VectorItem *a, size_t size)
{
	VectorInsertArray(v, v->size, a, size);
}

void VectorRemoveFront(Vector *v, size_t size)
{
	VectorRemove(v, 0, size);
}

void VectorRemoveBack(Vector *v, size_t size)
{
	VectorRemove(v, v->size-size, size);
}



VectorItem *VectorToArray(Vector *v)
{
	VectorItem *a = malloc(v->size * sizeof(VectorItem));
	memcpy(a, v->items, v->size * sizeof(VectorItem));
	return a;
}

VectorItem *VectorToSubArray(Vector *v, size_t start, size_t size)
{
	VectorItem *a = malloc(size * sizeof(VectorItem));
	memcpy(a, v->items + start, size * sizeof(VectorItem));
	return a;
}



void VectorPrintInt(Vector *v)
{
	printf("Size: %zu (Allocated: %zu); {", v->size, v->allocSize);
	for(size_t i = 0; i < v->size; ++i)
	{
		int *val = v->items[i];
		printf("%d", *val);
		if(i+1 < v->size)
			printf(", ");
	}
	printf("}\n");
}
