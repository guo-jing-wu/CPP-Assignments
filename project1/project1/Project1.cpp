//Create a type that is equivalent to a record, then open the file to read the data and store it into memory.
//To make sure the program can repeat this anywhere in the file, a variable is used to randomize the location
//of where it starts and another variable that randomize how much can be stored in memory
#include "stdafx.h"
#include <stdio.h> //fclose, fopen, fread, fseek, printf, rewind
#include <stdlib.h> //free, malloc, rand, srand
#include <time.h> //time
#include <Windows.h> //Sleep

typedef struct
{
	char result_time[17];
	char result_blocks[64][19];
} Element; //one record of the file

int main(int argc, char** argv)
{
	FILE *fp = fopen ("2004_21APR_L001178021.txt", "rb"); //open the "2004_21APR_L001178021.txt" as a binary file"
	Element *element; //pointer to struct Element
	srand(time(NULL)); //initialize random generator
	int i = 1; //1st while loop
	int j = 1; //2nd while loop
	unsigned long blkhdr_ticks; //used for debugging; first line of a record's characters
	float relative_time; //used for debugging; first line of a record's time
	//repeat the process of allocating and deallocating the malloc ten times
	while (i<=10)
	{
		int r = rand()%1; //random size of the file's records
		int NRecords = rand()%5000+1; //generate a number from 1 to 5000
		fseek (fp, 1298*(r-1), SEEK_SET); //random starting point
		if((element = (Element *)malloc(NRecords*sizeof(Element))) == NULL) //allocate memory block for elements
		{
			return 1;//end the program if there is not enough memory for allocating
		}
		Element *p = element; //pointer to malloc
		//read from the file each record from 1 to NRecord, then allocate it into memory
		while(j<=NRecords)
		{
			fread(&p->result_time,18,1,fp); //insert result_time into malloc
			fread(&p->result_blocks,20,64,fp); //insert result_blocks into malloc
			sscanf(p->result_time,"%d%f",&blkhdr_ticks,&relative_time); //scan the first line of a record
			printf("%d: %d\n",j,blkhdr_ticks); //print the first line of a record
			element++; //increment to the next block
			j++;
		}
		rewind (fp); //rewind back to the first line of the file
		Sleep(10000); //delay for 10000 milliseconds
		element = p; //know where to free
		free (element); //free memory block
		i++;
	}
	fclose(fp); //close file
	return 0; //end program
}