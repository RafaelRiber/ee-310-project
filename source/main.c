/*
 * Template Nintendo DS
 * May 2011
 */

#include <nds.h>
#include <stdio.h>
#define SIZE 10
int main(void) {
	
    consoleDemoInit();

    printf("\nHello World!! This is a test\n");

    int array[SIZE] = {0,1,2,3,4,5,6,7,8,9};

    int max = array[0];
    int i = 0;
    for (;i<=SIZE; i++){
    	if(array[i] > max){
    		max = array[i];
    	}
    }
    printf("\n%d", max);

    int min = array[0];
    int j;
    for (j = 0;j<=SIZE; j++){
        if(array[i] < min){
        	min = array[i];
        }
    }
    printf("\n%d", min);

    int sum = 0;
    int k;
    for(k = 0;k<=SIZE;k++){
    	sum = sum+array[k];
    }
    //printf("%d", sum);
    float avg = sum / SIZE;
    printf("\nAverage : %f", avg);

    while(1)
    	swiWaitForVBlank();
}
