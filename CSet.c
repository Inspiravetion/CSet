#include "CSet.h"
#include <stdlib.h>
#include <math.h>

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

//Global Declaration
#define DEFAULT_CAPACITY 10
#define DUPLICATE_FLAG -1

//Internal Helper Declarations
void CSet_Init_Empty(CSet* const pSet);
void Copy_Elements(uint32_t* source, uint32_t* target, uint32_t Sz);
bool CSet_Insert_(CSet* pSet, int32_t val);
bool CSet_Init_(CSet* const pSet, uint32_t Sz);
bool Make_Initialized_Array(int32_t** arr, uint32_t Sz);
bool Extend_CSet_Data_Array(CSet* pSet, int32_t size);
int Get_Insertion_Index_Of(CSet* pSet, int32_t val);
int Find_Index_Helper(CSet* pSet, int32_t val);

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
bool CSet_Init(CSet* const pSet, uint32_t Sz){
	if(Sz == 0){
		CSet_Init_Empty(pSet);
		return true;
	}
	else{
		return CSet_Init_(pSet, Sz);
	}
}; 

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
bool CSet_Load(CSet* const pSet, uint32_t Sz, const int32_t* const Data, uint32_t DSz){
	/**
	 * MAKE SURE TO CHECK THAT THE VALUE IS NOT ALREADY IN THE SET!!!
	 * might have to sort data TODO:
	 */
	int32_t* temp;
	int i = 0;
	bool success = Make_Initialized_Array(&temp, Sz);
	if(!success){
		return false;
	}
	if(pSet->Data != NULL){
		free(pSet->Data);
	}
	pSet->Capacity = Sz;
	pSet->Usage    = DSz;
	pSet->Data     = temp;
	Copy_Elements(Data, pSet->Data, DSz);
	return true;
};

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
bool CSet_Insert(CSet* const pSet, int32_t Value){
	bool success;
	if(!(pSet->Data)){
		if((success = CSet_Init(pSet, DEFAULT_CAPACITY))){
			return CSet_Insert_(pSet, Value);
		}
		else{
			return false;
		}
	}
	else if((pSet->Usage) == (pSet->Capacity - 1)){
		success = Extend_CSet_Data_Array(pSet, (pSet->Capacity * 2));
		if(success){
			return CSet_Insert_(pSet, Value);
		}
		return false;
	}
	return CSet_Insert_(pSet, Value);
};

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
bool CSet_Copy(CSet* const pTarget, const CSet* const pSource){
	if(pTarget->Data == NULL){
		if(!CSet_Init(pTarget, pSource->Capacity)){
			return false;
		}
	}
	if(pTarget->Capacity < pSource->Capacity){
		bool success = Extend_CSet_Data_Array(pTarget, pSource->Capacity);
		if(!success){
			return false;
		}
	}
	else if(pTarget->Capacity > pSource->Capacity){
		free(pTarget->Data);
		bool success = Make_Initialized_Array(&(pTarget->Data), pSource->Capacity);
		if(!success){
			return false;
		}
	}
	Copy_Elements(pSource->Data, pTarget->Data, pSource->Usage);
	pTarget->Usage = pSource->Usage;
	pTarget->Capacity = pSource->Capacity;
	return true;
};

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
bool CSet_Contains(const CSet* const pSet, int32_t Value){
	if(pSet->Data == NULL){
		return false;
	}
	int index = Find_Index_Helper(pSet, Value);
	return (pSet->Data[index] == Value);
};

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
 */ 
bool CSet_Remove(CSet* const pSet, int32_t Value){
	int index = Find_Index_Helper(pSet, Value);
	if(pSet->Data[index] == Value){
		while(index < (pSet->Usage -1)){
			pSet->Data[index] = pSet->Data[index + 1];
			index++;
		}
		pSet->Data[pSet->Usage - 1] = INT32_MAX;
		pSet->Usage--;
		return true;
	}
	return false;
};

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
bool CSet_Equals(const CSet* const pA, const CSet* const pB){
	if(!pA->Data && !pB->Data){
		return true;
	}
	else if(!pA->Data || !pB->Data || (pA->Usage != pB->Usage)){
		return false;
	}
	int i = 0;
	while(i < pA->Usage){
		if(pA->Data[i] != pB->Data[i]){
			return false;
		}
		i++;
	}
	return true;
};

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
bool CSet_isSubsetOf(const CSet* const pA, const CSet* const pB){
	if(pA->Data == NULL || pB->Data == NULL || pA->Usage > pB->Usage){
		return false;
	}
	int i = 0, smallInd = 0;
	while(i < pB->Usage){
		if(pB->Data[i] == pA->Data[smallInd]){
			smallInd++;
		}
		i++;
	}
	return (smallInd == pA->Usage);
};

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
bool CSet_Union(CSet* const pUnion, const CSet* const pA, const CSet* const pB){
	if (!CSet_Init(pUnion, (pA->Capacity + pB->Capacity))){
		return false;
	}
	int i = 0, a = 0, b = 0;
	int maxUse = pA->Usage > pB->Usage ? pA->Usage : pB->Usage;
	bool success;
	while(i < maxUse){
		if((pA->Data[a] > pB->Data[b])){
			if(b == (pB->Usage - 1)){
				success = CSet_Insert(pUnion, pB->Data[b]);
				if(!success){
					return false;
				}
				while(a < pA->Usage){
					success = CSet_Insert(pUnion, pA->Data[a]);
					if(!success){
						return false;
					}
					a++;
				}
				break;
			}
			else{
				success = CSet_Insert(pUnion, pB->Data[b]);
				if(!success){
					return false;
				}
				b++;
			}
		}
		else if((pA->Data[a] < pB->Data[b])){
			if(a == (pA->Usage - 1)){
				success = CSet_Insert(pUnion, pA->Data[a]);
				if(!success){
					return false;
				}
				while(b < pB->Usage){
					success = CSet_Insert(pUnion, pB->Data[b]);
					if(!success){
					return false;
				}
					b++;
				}
				break;
			}
			else{
				success = CSet_Insert(pUnion, pA->Data[a]);
				if(!success){
					return false;
				}
				a++;
			}
		}
		else{
			success = CSet_Insert(pUnion, pA->Data[a]);
			if(!success){
					return false;
				}
			a++;
			b++;
		}
		i++;
	} 
	return true;
};

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
bool CSet_Intersection(CSet* const pIntersection, const CSet* const pA, const CSet* const pB){
	if (!CSet_Init(pIntersection, pA->Capacity > pB->Capacity ? pA->Capacity : pB->Capacity)){
		return false;
	}
	bool done = false, success;
	int a = 0, b = 0;
	while(!done){
		if((pA->Data[a] > pB->Data[b])){
			if(b == (pB->Usage - 1)){
				done = true;
			}
			b++;
		}
		else if((pA->Data[a] < pB->Data[b])){
			if(a == (pA->Usage - 1)){
				done = true;
			}
			a++;
		}
		else{
			success = CSet_Insert(pIntersection, pA->Data[a]);
			if(!success){
				return false;
			}
			a++;
			b++;
		}
	}
	return true;
};

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
bool CSet_Difference(CSet* const pDifference, const CSet* const pA, const CSet* const pB){
	if (!CSet_Init(pDifference, pA->Capacity)){
		return false;
	}
	bool done = false, success;
	int a = 0, b = 0;
	while(!done){
		if((pA->Data[a] > pB->Data[b])){
			if(!(b == (pB->Usage - 1))){
				b++;
			}
			else{
				while(a < pA->Usage){
					success = CSet_Insert(pDifference, pA->Data[a]);
					if(!success){
						return false;
					}
					a++;
				}
				done = true;
			}
		}
		else if((pA->Data[a] < pB->Data[b])){
			if(a == (pA->Usage - 1)){
				done = true;
			}
			success = CSet_Insert(pDifference, pA->Data[a]);
			if(!success){
				return false;
			}
			a++;
		}
		else{
			if(a == (pA->Usage - 1)){
				done = true;
			}
			else{
				a++;
				b++;
			}
		}
	}
	return true;
}

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
uint32_t CSet_Size(const CSet* const pSet){
	if(pSet->Data == NULL){
		return 0;
	}
	return pSet->Usage;
}

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
bool CSet_isEmpty(const CSet* const pSet){
	return (pSet->Data == NULL || pSet->Usage == 0);
}

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
bool CSet_isFull(const CSet* const pSet){
	return pSet->Data != NULL && pSet->Usage == pSet->Capacity;
};


/**
 *  Removes all elements from a CSet object.
 *
 *  Pre:
 *     *pSet satisfies the CSet contract
 *  Post:
 *     *pSet contains no elements
 *     *pSet satisfies the CSet contract
 */
void CSet_makeEmpty(CSet* const pSet){
	free(pSet->Data);
	pSet->Usage = 0;
	pSet->Capacity = 0;
	pSet->Data = NULL;
}


//Internal(Private) helpers====================================================

/**
 * Initializes a CSet to be empty 
 * @param pSet the passed in CSet to be altered
 */
void CSet_Init_Empty(CSet* const pSet){
	pSet->Capacity = 0;
	pSet->Usage    = 0;
	pSet->Data     = NULL;
};

/**
 * Initializes a CSet to have the given capacity of
 * numbers in its Data array
 * @param  pSet the passed in CSet
 * @param  Sz   the size of the capacity
 * @return bool whether or not the array allocation was successful
 */
bool CSet_Init_(CSet* const pSet, uint32_t Sz){
	bool success = Make_Initialized_Array(&(pSet->Data), Sz);
	if(!success){
		return false;
	}
	pSet->Capacity = Sz;
	pSet->Usage    = 0;
	return true;
};

/**
 * Allocates and initializes an int32_t array of the given size and returns whether or not the
 * creation was successful
 * @param  arr the int32_t** of the array being passed in to be made
 * @param  Sz  the size of the array
 * @return bool if the allocation was succesful
 */
bool Make_Initialized_Array(int32_t** arr, uint32_t Sz){
	int32_t* temp;
	temp = (int32_t*) malloc( sizeof(int32_t) * Sz);
	if(temp){
		int i = 0;
		while( i < Sz ){
			temp[i] = INT32_MAX;
			i++;
		}
		*arr = temp;
		return true;
	}
	return false;
};

bool CSet_Insert_(CSet* pSet, int32_t val){
	int insertInd = Get_Insertion_Index_Of(pSet, val);
	if (insertInd == DUPLICATE_FLAG){
		return false;
	}
	int32_t currVal = val;
	int32_t temp;
	while(insertInd < (pSet->Usage + 1)){
		temp = pSet->Data[insertInd];
		pSet->Data[insertInd] = currVal;
		currVal = temp;
		insertInd++;
	}
	(pSet->Usage)++;
	return true;
};

/**
 * Uses a Binary search to either find the offset of where the element should be or signify that
 * it should not be added because it is already in the set
 * @param  pSet the passed in set to add the element to
 * @param  val the passed in value to add 
 * @return int the index of where the value should be placed in the set or DUPLICATE_FLAG if 
 * it already exists.
 */
int Get_Insertion_Index_Of(CSet* pSet, int32_t val){
	int index = Find_Index_Helper(pSet, val);
	if(pSet->Data[index] == val){
		return DUPLICATE_FLAG;
	}
	return pSet->Data[index] < val ? (index + 1) : index;
};

/**
 * Calculates the index of a value in a CSet returning either the 
 * index at which the value is in the CSet->Data array or where it 
 * should go in that array.
 * @param  pSet a pointer to a CSet
 * @param  val  a int32_t value to search for
 * @return int the index of where the val is or should be
 */
int Find_Index_Helper(CSet* pSet, int32_t val){
	int bottom = 0;
	int top = pSet->Usage;
	int currInd;
	while(top >= bottom){
		currInd = (bottom + ((top - bottom) / 2));
		if(pSet->Data[currInd] == val){
			return currInd;
		}
		else if(pSet->Data[currInd] > val){
			top = currInd - 1;
		}
		else{
			bottom = currInd + 1;
		}
	}
	return currInd;
};

bool Extend_CSet_Data_Array(CSet* pSet, int32_t size){
	int32_t* newArr = realloc(pSet->Data, (sizeof(int32_t) * size));
	if(!newArr){
		return false;
	}
	int i = pSet->Usage;
	while(i < size){
		newArr[i] = INT32_MAX;
		i++;
	}
	pSet->Data = newArr;
	pSet->Capacity = (pSet->Capacity * 2);
	return true;
};

void Copy_Elements(uint32_t* source, uint32_t* target, uint32_t Sz){
		int i = 0;
		while(i < Sz){
			target[i] = source[i];
			i++;
		}
	}

