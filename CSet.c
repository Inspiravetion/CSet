#include "CSet.h"

// CSet provides an implementation of a set type for storing signed
// 32-bit integer values (int32_t).
//
// The implementation imposes the following constraints:
//  - storage is array-based
//  - duplicate elements are not allowed in a CSet
//  - logically empty cells are set to INT32_MAX
//  - sets can contain up to UINT32_MAX elements
//  - unless noted to the contrary, the amortized cost of operations is O(N),
//    where N is the number of elements in the CSet
//  - amortized cost of search is O(log N)
//  - amortized cost of union, intersection and difference is O(N)
//  - cost of checking size, and full and empty tests are O(1)
//  - there are no memory leaks during any of the supported operations
//
// Every initialized CSet object A satisfies the following contract:
//  1.  A.Data points to an array of dimension A.Capacity,
//      or is NULL if A.Capacity == 0
//  2.  A.Data[0 : A.Usage-1] are the values stored in the set 
//      (in unspecified order)
//  3.  A.Data[A.Usage : A.Capacity-1] equal INT32_MAX
//
// This applies to CSet objects yielded by any of the support functions
// in this file.
//

/* struct _CSet {

   uint32_t Capacity;    // dimension of the set's array
   uint32_t Usage;       // number of elements in the set
   int32_t* Data;        // pointer to the set's array
};

typedef struct _CSet CSet;*/

/**
 * Initializes an empty pSet object, with capacity Sz.
 *
 * Pre:
 *    *pSet satisfies the CSet contract
 *    Sz   has been initialized
 * Post:  
 *    If successful:
 *       pSet->Capacity == Sz
 *       pSet->Usage == 0
 *       pSet->Data points to an array of dimension Sz, or
 *          is NULL if Sz == 0
 *       if Sz != 0, pSet->Data[0 : Sz-1] == INT32_MAX
 *    else:
 *       pSet->Capacity == 0
 *       pSet->Usage == 0
 *       pSet->Data == NULL
 * Returns:
 *    true if successful, false otherwise
 */
bool CSet_Init(CSet* const pSet, uint32_t Sz);

/**
 * Loads specified values into a pSet object, replacing any previous contents.
 *
 * Pre:
 *    *pSet satisfies the CSet contract
 *    Sz   has been initialized
 *    Data points to an array of dimension >= DSz, or
 *          is NULL if Sz == 0
 *    Data[0:DSz-1] have been initialized
 *    DSz   has been initialized, DSz <= Sz
 * Post:  
 *    If successful:
 *       pSet->Capacity == Sz
 *       pSet->Usage == DSz
 *       pSet->Data points to an array of dimension Sz
 *       pSet->Data[0:DSz-1] == Data[0:DSz-1]
 *    else:
 *       *pSet is unchanged
 * Returns:
 *    true if successful, false otherwise
 */
bool CSet_Load(CSet* const pSet, uint32_t Sz, const int32_t* const Data, uint32_t DSz);

/**
 * Adds Value to a pSet object.
 *
 * Pre:
 *    *pSet satisfies the CSet contract
 *    Value has been initialized
 * Post:  
 *    If successful:
 *       Value is a member of *pSet
 *       pSet->Capacity has been increased, if necessary
 *       *pSet satisfies the CSet contract
 *    else:
 *       *pSet is unchanged
 * Returns:
 *    true if successful, false otherwise
 */
bool CSet_Insert(CSet* const pSet, int32_t Value);

/**
 * Makes a deep copy of a CSet object.
 *
 * Pre:
 *    *pCopy satisfies the CSet contract
 *    *pSet satisfies the CSet contract
 *    pSet != pCopy
 * Post:  
 *    *pSet is unchanged
 *    If successful:
 *       pCopy->Capacity == pSet->Capacity
 *       pCopy->Usage == pSet->Usage
 *       pCopy[0:pCopy->Capacity] ==  pSet[0:pSet->Capacity]
 *       pCopy->Data != pSet->Data
 *       *pCopy satisfies the PSet contract.
 *    else:
 *       *pCopy is unchanged
 * Returns:
 *    true if successful, false otherwise
 */
bool CSet_Copy(CSet* const pTarget, const CSet* const pSource);

/**
 * Determines if Value belongs to a pSet object.
 *
 * Pre:
 *    *pSet satisfies the CSet contract
 *    Value has been initialized
 * Post:  
 *    *pSet is unchanged
 * Returns:
 *    true if Value belongs to *pSet, false otherwise
 */
bool CSet_Contains(const CSet* const pSet, int32_t Value);

/**
 * Removes Value from a pSet object.
 *
 * Pre:
 *    *pSet satisfies the CSet contract
 *    Value has been initialized
 * Post:  
 *    If Value was a member of *pSet:
 *       Value is no longer a member of *pSet
 *       pSet->Capacity is unchanged
 *       pSet->Usage is decremented
 *       *pSet satisfies the CSet contract
 *    else:
 *       *pSet is unchanged
 * Returns:
 *    true if Value was removed, false otherwise
 */bool CSet_Remove(CSet* const pSet, int32_t Value);

/**
 * Determines if two CSet objects contain the same elements.
 *
 * Pre:
 *    *pA satisfies the CSet contract
 *    *pB satisfies the CSet contract
 * Post:  
 *    *pA is unchanged
 *    *pB is unchanged
 * Returns:
 *    true if sets contain same elements, false otherwise
 */
bool CSet_Equals(const CSet* const pA, const CSet* const pB);

/**
 * Determines if one CSet object is a subset of another.
 *
 * Pre:
 *    *pA satisfies the CSet contract
 *    *pB satisfies the CSet contract
 * Post:  
 *    *pA is unchanged
 *    *pB is unchanged
 * Returns:
 *    true if *pB contains every element of *pA, false otherwise
 */
bool CSet_isSubsetOf(const CSet* const pA, const CSet* const pB);

/**
 * Sets *pUnion to be the union of the sets *pA and *pB.
 *
 * Pre:
 *    *pUnion satisfies the CSet contract
 *    *pA     satisfies the CSet contract
 *    *pB     satisfies the CSet contract
 *
 * Post:
 *    *pA and *pB are unchanged
 *    For every integer x, x is contained in *pUnion iff x is contained in
 *    *pA or *pB (or both).
 *    pUnion->Capacity == pA->Capacity + pB->Capacity
 *    pUnion->Usage    == pA->Usage + pB->Usage - number of elements that
 *                        occur in both *pA and *pB
 *    *pUnion satisfies the CSet contract
 *
 * Returns:
 *    true if the union is successfully created; false otherwise
 */
bool CSet_Union(CSet* const pUnion, const CSet* const pA, const CSet* const pB);

/**
 * Sets *pIntersection to be the intersection of the sets *pA and *pB.
 *
 * Pre:
 *    *pIntersection satisfies the CSet contract
 *    *pA            satisfies the CSet contract
 *    *pB            satisfies the CSet contract
 *
 * Post:
 *    *pA and *pB are unchanged
 *    For every integer x, x is contained in *pIntersection iff x is contained in
 *    *pA and *pB.
 *    pIntersection->Capacity == max(pA->Capacity, pB->Capacity)
 *    pIntersection->Usage    == number of elements that occur in both *pA and *pB
 *    *pIntersection satisfies the CSet contract
 *
 * Returns:
 *    true if the intersection is successfully created; false otherwise
 */
bool CSet_Intersection(CSet* const pIntersection, const CSet* const pA, const CSet* const pB);

/**
 * Sets *pIntersection to be the intersection of the sets *pA and *pB.
 *
 * Pre:
 *    *pDifference satisfies the CSet contract
 *    *pA          satisfies the CSet contract
 *    *pB          satisfies the CSet contract
 *
 * Post:
 *    *pA and *pB are unchanged
 *    For every integer x, x is contained in *pDifference iff x is contained in
 *    *pA and not contained in *pB.
 *    pDifference->Capacity == pA->Capacity
 *    pDifference->Usage    == number of elements that occur in both *pA and not in *pB
 *    *pDifference satisfies the CSet contract
 *
 * Returns:
 *    true if the intersection is successfully created; false otherwise
 */
bool CSet_Difference(CSet* const pDifference, const CSet* const pA, const CSet* const pB);

/**
 *  Reports the number of elements in a pSet object.
 *
 *  Pre:
 *     *pSet satisfies the CSet contract
 *  Post:
 *     *pSet is unchanged
 *  Returns:
 *     pSet->Usage
 */
uint32_t CSet_Size(const CSet* const pSet);

/**
 *  Determines whether a CSet object is empty.
 *
 *  Pre:
 *     *pSet satisfies the CSet contract
 *  Post:
 *     *pSet is unchanged
 *  Returns:
 *     true if pSet->Usage == 0, false otherwise
 */
bool CSet_isEmpty(const CSet* const pSet);

/**
 *  Determines whether a CSet object is full.
 *
 *  Pre:
 *     *pSet satisfies the CSet contract
 *  Post:
 *     *pSet is unchanged
 *  Returns:
 *     true if pSet->Usage == pSet->Capacity, false otherwise
 */
bool CSet_isFull(const CSet* const pSet);

/**
 *  Removes all elements from a CSet object.
 *
 *  Pre:
 *     *pSet satisfies the CSet contract
 *  Post:
 *     *pSet contains no elements
 *     *pSet satisfies the CSet contract
 */
void CSet_makeEmpty(CSet* const pSet);

