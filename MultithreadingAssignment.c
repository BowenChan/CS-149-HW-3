/*
 * MultithreadingAssignment.c
 *
 *  Created on: Feb 25, 2016
 *      Author: bowenchan
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "MultithreadingAssignment.h"

int randSeller();
bool compare(float a, float b);
//extern seller;
/*
 *
 */
int main(){
	time_t begin, end;
	begin = time(NULL);

	//Set the current seller
	int sell = 0;
	seller currentSeller = H;

	printf("%i\n", currentSeller);

	end = time(NULL);
	//Run the program for 60 real time seconds
	while(difftime(end, begin) != 2)
	{
		sell = randSeller();
		currentSeller = (seller) sell;
		//printf("%s\n", currentSeller.ToString());
		switch(currentSeller){
			H: printf("H"); break;
			M1: printf("M1"); break;
			M2: printf("M2"); break;
			M3: printf("M3"); break;
			L1: printf("L1"); break;
			L2: printf("L2"); break;
			L3: printf("L3"); break;
			L4: printf("L4"); break;
			L5: printf("L5"); break;
			L6: printf("L6"); break;
			default: printf("Nope");
		}
		printf("\n");
		end = time(NULL);
	}


	printf("The time is %f", (float)(end-begin)/CLOCKS_PER_SEC );
	return 0;
}

int randSeller(){

	return rand() % 10;
}

bool compare(float a, float b)
{
	if(a-b != 0) return false;
	else return true;
}
