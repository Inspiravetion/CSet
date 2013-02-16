#include "CSet.h"

int main(int argc, char* argv[]){
	printf("Started to do set calculations...\n");
	CSet set;
	CSet_Init(&set, 12);
	printf("Capacity: %d", set.Capacity);
	printf("Usage: %d\n", set.Usage);
	printf("Data: %d\n", (set.Data));
}