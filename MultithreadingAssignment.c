/*
 * MultithreadingAssignment.c
 *
 *  Created on: Feb 25, 2016
 *      Author: bowenchan
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(){
	time_t begin, end;
	begin = time(NULL);
	printf("%f\n\n", (float) begin);
	int counter;
	end = time(NULL);
	while(difftime(end, begin) != 60)
	{


	}
	printf("The time is %f", (float)(end-begin)/CLOCKS_PER_SEC );
	return 0;
}
