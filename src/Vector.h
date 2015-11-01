#ifndef DEF_VECTOR
#define DEF_VECTOR

#include <stddef.h>


#define VECTOR_DEFAULT_ALLOC 8

extern const size_t VECTOR_NEGATIVE_SEARCH;
typedef void * VectorItem;

struct Vector
{
	size_t size;
	size_t allocSize;
	VectorItem *items;
};
typedef struct Vector Vector;


Vector *VectorNew();
Vector *VectorNewWithReserve(size_t n);
Vector *VectorNewFromArray(VectorItem *arr, size_t size);
Vector *VectorNewWithSize(size_t size);
Vector *VectorNewWithValue(VectorItem val, size_t size);
Vector *VectorNewFromVector(Vector *v);
void VectorFree(Vector *v);
void VectorFreeAll(Vector *v);

size_t VectorSize(Vector *v);
int VectorEmpty(Vector *v);
VectorItem *VectorData(Vector *v);
VectorItem VectorGet(Vector *v, size_t pos);
VectorItem VectorGetFront(Vector *v);
VectorItem VectorGetBack(Vector *v);
void VectorSet(Vector *v, size_t pos, VectorItem e);
void VectorInsert(Vector *v, size_t pos, VectorItem e);
void VectorInsertArray(Vector *v, size_t pos, VectorItem *a, size_t size);
void VectorRemove(Vector *v, size_t pos, size_t size);
void VectorResize(Vector *v, size_t size);
void VectorClear(Vector *v);
void VectorReserve(Vector *v, size_t size);
void VectorShrinkToFit(Vector *v);
size_t VectorIndexOf(Vector *v, VectorItem e);
size_t VectorLastIndexOf(Vector *v, VectorItem e);
void VectorInsertFront(Vector *v, VectorItem e);
void VectorInsertArrayFront(Vector *v, VectorItem *a, size_t size);
void VectorInsertBack(Vector *v, VectorItem e);
void VectorInsertArrayBack(Vector *v, VectorItem *a, size_t size);
void VectorRemoveFront(Vector *v, size_t size);
void VectorRemoveBack(Vector *v, size_t size);

VectorItem *VectorToArray(Vector *v);
VectorItem *VectorToSubArray(Vector *v, size_t start, size_t size);

void VectorPrintInt(Vector *v);

#endif
