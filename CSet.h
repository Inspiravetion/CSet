#ifndef CSET_H
#define CSET_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct _CSet {

   uint32_t Capacity;    // dimension of the set's array
   uint32_t Usage;       // number of elements in the set
   int32_t* Data;        // pointer to the set's array
};

typedef struct _CSet CSet;

bool CSet_Init(CSet* const pSet, uint32_t Sz);

bool CSet_Load(CSet* const pSet, uint32_t Sz, const int32_t* const Data, uint32_t DSz);

bool CSet_Insert(CSet* const pSet, int32_t Value);

bool CSet_Copy(CSet* const pTarget, const CSet* const pSource);

bool CSet_Contains(const CSet* const pSet, int32_t Value);

bool CSet_Remove(CSet* const pSet, int32_t Value);

bool CSet_Equals(const CSet* const pA, const CSet* const pB);

bool CSet_isSubsetOf(const CSet* const pA, const CSet* const pB);

bool CSet_Union(CSet* const pUnion, const CSet* const pA, const CSet* const pB);

bool CSet_Intersection(CSet* const pIntersection, const CSet* const pA, const CSet* const pB);

bool CSet_Difference(CSet* const pDifference, const CSet* const pA, const CSet* const pB);

uint32_t CSet_Size(const CSet* const pSet);

bool CSet_isEmpty(const CSet* const pSet);

bool CSet_isFull(const CSet* const pSet);

void CSet_makeEmpty(CSet* const pSet);

#endif