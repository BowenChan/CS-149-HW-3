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
#include <unistd.h>
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

	//set up timestamp
	int hour = 0;
	int tenth = 0;
	int minute = 0;
	seller currentSeller = H;

	printf("%i\n", currentSeller);

	end = time(NULL);

	int counter = 0;
	//Run the program for 60 real time seconds
	while((end - begin) <= 60)
	{
		printf("%i ", counter++);
		sell = randSeller();
		currentSeller = (seller) sell;
		//printf("%s\n", currentSeller.ToString());
		switch(currentSeller){
			case H: printf("Ticket Seller: H "); break;
			case M1: printf("Ticket Seller: M1"); break;
			case M2: printf("Ticket Seller: M2"); break;
			case M3: printf("Ticket Seller: M3"); break;
			case L1: printf("Ticker Seller: L1"); break;
			case L2: printf("Ticket Seller: L2"); break;
			case L3: printf("Ticket Seller: L3"); break;
			case L4: printf("Ticket Seller: L4"); break;
			case L5: printf("Ticket Seller: L5"); break;
			case L6: printf("Ticket Seller: L6"); break;
			default: printf("");
		}

		if((end - begin) > 10)
		{
			tenth = (end - begin)/2;
			minute = (end - begin) % 10;
		}
		else
			minute = (end - begin);
		printf(" time stamp %i:%i%i", hour, tenth, minute);
		printf("\n");
		sleep(1);
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
