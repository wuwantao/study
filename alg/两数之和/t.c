#include <stdio.h>
#include <stdlib.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize){
    int i = 0;
    int j = 0;
    int *p = (int *)malloc(sizeof(int) * 2);
    for (i = 0; i < numsSize -1; i++) {
        for (j = i + 1; j < numsSize; j++) {
            if (nums[i] == (target - nums[j])) {
                p[0] = i;
                p[1] = j;
                return p;
            }
        }
    }
    return p;
}

int main()
{
	int i = 4;
	int array[4] = {2,7,11,15};	
	int *p = NULL;	

	p = twoSum(array, 4, 9, p);

	printf("%d %d\n", p[0], p[1]);


	return 0;
}
