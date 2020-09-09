#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t map[20][2];

int* twoSum(int* nums, int numsSize, int target, int *returnSize){
    int i = 0;
    int j = 0;
    int *p = (int *)malloc(sizeof(int) * 2);
    for (i = 0; i < numsSize; i++) {
        map[nums[i]][0] = 1;
        map[nums[i]][1] = i;
    }

    for (i = 0; i < numsSize; i++) {
        j = target - nums[i];
        if (j < 0) {
            continue;
        }
        if (map[j][0] == 1 && map[j][1] != i) {
            *returnSize = 2;
            p[0] = i;
            p[1] = map[j][1];
            return p;
        }
    }
    
    *returnSize = 2;
    return p;
}

int main()
{
//	int i = 4;
//	int array[4] = {2,7,11,15};	
	int i = 3;
	int array[3] = {3,2,4};	
	int *p = NULL;	
	int returnSize = 0;

	p = twoSum(array, 3, 6, &returnSize);

	printf("%d %d\n", p[0], p[1]);


	return 0;
}
