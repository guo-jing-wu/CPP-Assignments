// Semaphore.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stdio.h> //FILE,fopen,fprintf
#include <stdlib.h> //rand,srand
#include <windows.h> //Sleep,HANDLE,WaitForSingleObject,ReleaseSemaphore,CreateSemaphore,CreateThread
#include <time.h> //time
#define SIZE 20 //buffer size

FILE *fp;
HANDLE empty;
HANDLE full;
HANDLE mutexD;
HANDLE mutexF;
int front = 0; //front of buffer
int rear = 0; //back of buffer
int prod = 0; //producer thread number
int con = 0; //consumer thread number
char buf[SIZE]; //buffer

DWORD WINAPI producer (LPVOID param){
  srand(time(NULL)); //initialize randomization
  prod++; //increment producer thread number
  int pN = prod; //producer thread #
  while (1){ //infinite loop
    int i = rand()%2; //create data to add
    WaitForSingleObject(full,INFINITE); //wait for consumer
    WaitForSingleObject(mutexD,INFINITE); //enter critical section
    buf[rear] = i; //end of buffer
    rear = (rear+1)%SIZE; //insert data
    fprintf(fp,"Producer %d: %d\n",pN, i);
    ReleaseSemaphore(mutexD,1,NULL); //leave critical section
    ReleaseSemaphore(empty,1,NULL); //signal consumer
    if ((i%2) == 1){
      Sleep(1); //vary producer thread speed
    }
  }
}

DWORD WINAPI consumer (LPVOID param){
  con++; //increment consumer thread number
  int cN = con; ///increment consumer thread number
  while (1){ //infinite loop
    WaitForSingleObject(empty,INFINITE); //wait for producer
    WaitForSingleObject(mutexF,INFINITE); //enter critical section
    int j = rand()%2; //data to be removed
    j = buf[front]; //front of buffer
    front = (front+1)%SIZE;//remove data
    fprintf(fp,"Consumer %d: %d\n",con, j);
    ReleaseSemaphore(mutexF,1,NULL); //leave critical section
    ReleaseSemaphore(full,1,NULL); //signal producer
    if ((j%2) == 1){
      Sleep(1); //vary consumer thread speed
    }
  }
}

int _tmain(int argc, _TCHAR* argv[]){
	fp = fopen("log.txt","w+");
	int index = 0; //initialize number to be created
	HANDLE p[4]; //initialize producer thread id
	HANDLE c[3]; //initialize consumer thread id
	full = CreateSemaphore(NULL,SIZE-1,SIZE-1,NULL); //initialize semaphore full
	empty = CreateSemaphore(NULL,0,SIZE-1,NULL); //initialize semaphore empty
	mutexD = CreateSemaphore(NULL,1,1,NULL); //initialize semaphore mutexD
	mutexF = CreateSemaphore(NULL,1,1,NULL); //initialize semaphore mutexF
	for(index=0; index<4; index++){ //0 to P
		p[index] = CreateThread(0,0,producer,&index,0,NULL); //create producer thread
	}
	for(index=0; index<3; index++){ //0 to C
	    c[index] = CreateThread(0,0,consumer,&index,0,NULL); //create consumer thread
	}
	for(index=0; index<3; index++){ //0 to C
		WaitForSingleObject(p[index],INFINITE); //wait for consumer thread to join
	}
	for(index=0; index<4; index++){ //0 to P
		WaitForSingleObject(c[index],INFINITE); //wait for producer thread to join
	}
	return 0;
}