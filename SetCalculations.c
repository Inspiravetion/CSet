#include "CSet.h"
#include <assert.h>


void Test_Init(){
	printf("Test_Init()----------------------------------------------\n");
	CSet set;
	CSet_Init(&set, 12);
	assert(set.Capacity == 12);
	assert(set.Usage == 0);
	assert(set.Data[0] == INT32_MAX);
	CSet_Init(&set, 0);
	assert(set.Capacity == 0);
	assert(set.Usage == 0);
	assert(set.Data == NULL);
	printf("%s\n", "passed Init Tests...\n");
}

void Test_Load(){
	printf("Test_Load()----------------------------------------------\n");
	CSet set;
	int32_t* Data = (int32_t*) malloc(sizeof(int32_t) * 10);
	Data[0] = 0;
	Data[9] = 9;
	CSet_Load(&set, 15, Data, 10);
	assert(set.Data[0] == 0);
	assert(set.Data[9] == 9);
	assert(set.Capacity == 15);
	printf("%s\n", "Passed Load Tests...\n");
}

void Test_Insert(){
	printf("Test_Insert()----------------------------------------------\n");
	CSet set;
	CSet nullSet;
	CSet_Init(&nullSet, 0);
	CSet_Init(&set, 10);

	CSet_Insert(&nullSet, 1);
	CSet_Insert(&set, 1);
	CSet_Insert(&nullSet, 66);
	CSet_Insert(&set, 66);
	CSet_Insert(&nullSet, 5);
	CSet_Insert(&set, 5);
	CSet_Insert(&nullSet, 6);
	CSet_Insert(&set, 6);
	CSet_Insert(&nullSet, 89);
	CSet_Insert(&set, 89);
	CSet_Insert(&nullSet, 7);
	CSet_Insert(&set, 7);
	CSet_Insert(&nullSet, 11);
	CSet_Insert(&set, 11);
	CSet_Insert(&nullSet, 14);
	CSet_Insert(&set, 14);
	CSet_Insert(&nullSet, 3);
	CSet_Insert(&set, 3);
	CSet_Insert(&nullSet, 87);
	CSet_Insert(&set, 87);
	CSet_Insert(&nullSet, 9);
	CSet_Insert(&set, 9);

	assert(set.Data[3] == 6);
	assert(set.Data[5] == 9);
	assert(set.Data[8] == 66);
	assert(set.Usage == 11);
	assert(set.Capacity == 20);
	assert(CSet_Insert(&set, 5) == false);

	assert(nullSet.Data[3] == 6);
	assert(nullSet.Data[5] == 9);
	assert(nullSet.Data[8] == 66);
	assert(nullSet.Usage == 11);
	assert(nullSet.Capacity == 20);
	printf("%s\n", "Passed Insert Tests...\n");
}

void Test_Copy(){
	printf("Test_Copy()----------------------------------------------\n");
	CSet nullSet;
	CSet set1;
	CSet_Init(&nullSet, 0);
	CSet_Init(&set1, 10);
	CSet_Insert(&set1, 0);
	CSet_Insert(&set1, 9);
	CSet_Copy(&nullSet, &set1);
	assert(nullSet.Data[0] == 0);
	assert(nullSet.Data[1] == 9);
	assert(nullSet.Capacity == 10);
	assert(nullSet.Usage == 2);

	CSet big;
	CSet small;
	CSet_Init(&small, 3);
	CSet_Init(&big, 10);
	CSet_Insert(&big, 0);
	CSet_Insert(&big, 9);
	CSet_Copy(&small, &big);
	assert(small.Data[0] == 0);
	assert(small.Data[1] == 9);
	assert(small.Capacity == 10);
	assert(small.Usage == 2);

	CSet big2;
	CSet small2;
	CSet_Init(&big2, 0);
	CSet_Init(&small2, 10);
	CSet_Insert(&small2, 0);
	CSet_Insert(&small2, 9);
	CSet_Copy(&big2, &small2);
	assert(big2.Data[0] == 0);
	assert(big2.Data[1] == 9);
	assert(big2.Capacity == 10);
	assert(big2.Usage == 2);
	
	printf("%s\n", "Passed Copy Tests...\n");
}

void Test_Contains(){
	printf("Test_Contains()----------------------------------------------\n");
	CSet set;
	CSet_Init(&set, 0);
	int32_t i = 0;
	int32_t count = 0;
	while(i < 30){
		CSet_Insert(&set, i);
		i += 2;
	}
	int32_t x = 1;
	while(x < 30){
		assert(CSet_Contains(&set, x) == false);
		assert(CSet_Contains(&set, x-1) == true);
		x += 2;
	}
	printf("%s\n", "Passed Contains Tests...\n");
}

void Test_Remove(){
	printf("Test_Remove()----------------------------------------------\n");
	CSet set;
	CSet_Init(&set, 0);
	int32_t i = 0;
	while(i < 30){
		CSet_Insert(&set, i);
		i += 2;
	}
	assert(CSet_Remove(&set, 10) == true);
	assert(CSet_Remove(&set, 20) == true);
	assert(CSet_Remove(&set, 11) == false);
	assert(set.Usage == 13);
	assert(set.Data[4] == 8);
	assert(set.Data[5] == 12);
	assert(set.Data[9] == 22);
	assert(set.Data[set.Usage - 1] == 28);
	printf("%s\n", "Passed Remove Tests...\n");	
}

void Test_Equals(){
	printf("Test_Equals()----------------------------------------------\n");
	CSet setA;
	CSet setB;
	CSet setC;
	CSet nullSet;
	CSet_Init(&setA, 0);
	CSet_Init(&setB, 0);
	CSet_Init(&setC, 0);
	CSet_Init(&nullSet, 0);

	assert(CSet_Equals(&setA, &nullSet) == true);

	CSet_Insert(&setA, 2);
	CSet_Insert(&setA, 3);
	CSet_Insert(&setA, 4);

	CSet_Insert(&setB, 2);
	CSet_Insert(&setB, 3);
	CSet_Insert(&setB, 4);

	CSet_Insert(&setC, 1);
	CSet_Insert(&setC, 3);
	CSet_Insert(&setC, 4);
	
	assert(CSet_Equals(&setA, &setB) == true);
	assert(CSet_Equals(&setA, &setC) == false);
	assert(CSet_Equals(&setA, &nullSet) == false);

	printf("%s\n", "Passed Equals Tests...\n");	
}

void Test_Is_Subset_Of(){
	printf("Test_Is_Subset_Of()----------------------------------------------\n");
	CSet setA;
	CSet setB;
	CSet setC;
	CSet nullSet;
	CSet_Init(&nullSet, 0);
	CSet_Init(&setA, 0);
	CSet_Init(&setB, 0);
	CSet_Init(&setC, 0);

	CSet_Insert(&setA, 2);
	CSet_Insert(&setA, 3);
	CSet_Insert(&setA, 4);
	CSet_Insert(&setA, 5);
	CSet_Insert(&setA, 6);

	CSet_Insert(&setB, 2);
	CSet_Insert(&setB, 3);
	CSet_Insert(&setB, 4);

	CSet_Insert(&setC, 1);
	CSet_Insert(&setC, 3);
	CSet_Insert(&setC, 4);

	assert(CSet_isSubsetOf(&setB, &setA) == true);
	assert(CSet_isSubsetOf(&setC, &setA) == false);
	assert(CSet_isSubsetOf(&setA, &setB) == false);
	assert(CSet_isSubsetOf(&setB, &nullSet) == false);
	
	printf("%s\n", "Passed isSubsetOf Tests...\n");	
}

void Test_Union(){
	printf("Test_Union()----------------------------------------------\n");
	CSet setA;
	CSet setB;
	CSet unionSet;

	CSet_Init(&unionSet, 0);
	CSet_Init(&setA, 0);
	CSet_Init(&setB, 0);

	CSet_Insert(&setA, 2);
	CSet_Insert(&setA, 3);
	CSet_Insert(&setA, 4);
	CSet_Insert(&setA, 5);
	CSet_Insert(&setA, 6);

	CSet_Insert(&setB, 2);
	CSet_Insert(&setB, 3);
	CSet_Insert(&setB, 4);
	CSet_Insert(&setB, 7);

	CSet_Union(&unionSet, &setA, &setB);

	assert(unionSet.Capacity == 20);
	assert(unionSet.Usage == 6);
	assert(CSet_Contains(&unionSet, 2));
	assert(CSet_Contains(&unionSet, 3));
	assert(CSet_Contains(&unionSet, 4));
	assert(CSet_Contains(&unionSet, 5));
	assert(CSet_Contains(&unionSet, 6));
	assert(CSet_Contains(&unionSet, 7));
	printf("%s\n", "Passed Union Tests...\n");	
}

void Test_Intersection(){
	printf("Test_Intersection()----------------------------------------------\n");
	CSet setA;
	CSet setB;
	CSet intersection;

	CSet_Init(&intersection, 0);
	CSet_Init(&setA, 10);
	CSet_Init(&setB, 10);

	CSet_Insert(&setA, 2);
	CSet_Insert(&setA, 3);
	CSet_Insert(&setA, 4);
	CSet_Insert(&setA, 5);
	CSet_Insert(&setA, 6);
	CSet_Insert(&setA, 12);

	CSet_Insert(&setB, 2);
	CSet_Insert(&setB, 1);
	CSet_Insert(&setB, 12);
	CSet_Insert(&setB, 3);
	CSet_Insert(&setB, 4);
	CSet_Insert(&setB, 7);

	CSet_Intersection(&intersection, &setA, &setB);

	assert(intersection.Usage == 4);
	assert(CSet_Contains(&intersection, 2));
	assert(CSet_Contains(&intersection, 3));
	assert(CSet_Contains(&intersection, 4));
	assert(CSet_Contains(&intersection, 12));
	printf("%s\n", "Passed Intersection Tests...\n");	
}

void Test_Difference(){
	printf("Test_Difference()----------------------------------------------\n");
	CSet setA;
	CSet setB;
	CSet diff;

	CSet_Init(&diff, 0);
	CSet_Init(&setA, 10);
	CSet_Init(&setB, 10);

	CSet_Insert(&setA, 2);
	CSet_Insert(&setA, 3);
	CSet_Insert(&setA, 4);
	CSet_Insert(&setA, 5);
	CSet_Insert(&setA, 6);
	CSet_Insert(&setA, 12);

	CSet_Insert(&setB, 2);
	CSet_Insert(&setB, 1);
	CSet_Insert(&setB, 12);
	CSet_Insert(&setB, 3);
	CSet_Insert(&setB, 4);
	CSet_Insert(&setB, 7);

	CSet_Difference(&diff, &setA, &setB);

	assert(diff.Usage == 2);
	assert(CSet_Contains(&diff, 5));
	assert(CSet_Contains(&diff, 6));
	assert(!CSet_Contains(&diff, 4));
	assert(!CSet_Contains(&diff, 7));
	printf("%s\n", "Passed Difference Tests...\n");
}

int main(int argc, char* argv[]){
	printf("Started to do set calculations...\n");
	Test_Init();
	Test_Load();
	Test_Insert(); 
	Test_Copy();
	Test_Contains();
	Test_Remove();
	Test_Equals();
	Test_Is_Subset_Of();
	Test_Union();
	Test_Intersection();
	Test_Difference();
}	