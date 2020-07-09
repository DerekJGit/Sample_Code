// Using sem_init(),sem_wait(), sem_post(), sem_destroy(), pthread_mutex_init(), pthread_mutex_lock(), 
// pthread_mutex_unlock(), and pthread_mutex_destroy() functions by writing a program in C++ for 
// the producer-consumer (i.e.bounded-buffer) problem.

#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
using namespace std;


sem_t full,empty;
pthread_mutex_t prod_mutex, cons_mutex;
int buffer_pool[10];
//empty.count = n; 
//full.count = 0;
int in = 0, out = 0;
int in_item, out_item;
vector<int> IdVector;
int insert_item(int in_item) 
{
	sem_wait(&empty);//P(empty); // make sure buffer pool is not full
	pthread_mutex_lock(&prod_mutex);//bP(prod_mutex); // synchronize multiple producer threads
	buffer_pool[in]= in_item;
	in = (in + 1) % 10;// updatethe inpointer
	pthread_mutex_unlock(&prod_mutex);//bV(prod_mutex); // synchronize multiple producer threads
	sem_post(&full);//V(full);// notify the consumerthreads
	/* return 0 if successful*/
	return 0;
	/* otherwise, return -1 indicating an error condition */
}

int remove_item(int *out_item) 
{
	sem_wait(&full);//P(full); // make sure buffer pool is not empty
	pthread_mutex_lock(&cons_mutex);//bP(cons_mutex); // synchronize multiple consumer threads
	*out_item = buffer_pool[out];
	out = (out + 1) % 10;// update the our pointer
	pthread_mutex_unlock(&cons_mutex);//bV(cons_mutex); // synchronize multiple consumer threads
	sem_post(&empty);//V(empty);// notify the producer threads
	/* return 0 if successful */
	return 0;
	/* otherwise, return -1 indicating an error condition */
}

void *producer(void *param)
{
int item;
while (true) {
	/* generate a random number for the producer thread to sleep */
	int randNum;
	randNum = rand() % 3;
	sleep(randNum);
	/* generate a random number representing an item to be deposited */
	item = rand() % 20;
	if (insert_item(item))
	{
		// semaphores and mutexes are implemented in the function
		cout << "report error condition" << endl;
	}
	else {
	bool flag = false;
	for (int i = 0; i < IdVector.size(); i++)
	{
		if (IdVector[i] == pthread_self()) flag = true;
	}
	if (flag == true) IdVector.push_back(pthread_self());
	cout << "producer " << pthread_self() << " produced " << item << " in buffer " << buffer_pool[item] << endl << endl;
	}
}
}

void *consumer(void *param) 
{
	int item;
	while (true) 
	{
	
	/* generate a random numberfor the consumer thread to sleep */
	int randNum;
	randNum = rand() % 3;
	sleep(randNum);
	if (remove_item(&item)) 
	{
	// semaphores and mutexes are implemented in the function
	cout << "report error condition" << endl;
	}
	else {
	bool flag = false;
	for (int i = 0; i < IdVector.size(); i++)
	{
		if (IdVector[i] == pthread_self()) flag = true;
	}
	if (flag == true) IdVector.push_back(pthread_self());
	cout << "consumer " << pthread_self() << " consumed " << item << " from buffer " << buffer_pool[item] << endl << endl;
	}
}
}


int main() {
	//int buffer_pool[10];
	//sem_t empty, full; /* two general semaphores */
	//binary_semaphore prod_mutex, cons_mutex;/* two binary semaphores */
	//item_type in_item, out_item;
	




	pthread_t producerThread,consumerThread;
	sem_init(&empty,0,10);
	sem_init(&full,0,0);
	pthread_mutex_init(&prod_mutex,NULL);
	pthread_mutex_init(&cons_mutex,NULL);
	/* get the three pieces of information from the user */
	int num1, num2, num3;
	cout << "Enter the number of seconds for the main() function to sleep: ";
	cin >> num1;
	cout << "Enter the number of producer threads: ";
	cin >> num2;
	cout << "Enter the number of consumer threads: ";
	cin >> num3;
	
	/* create producer thread(s) */
	/* create consumer thread(s) */
	for (int i = 0; i < num2; i++) {
		pthread_create(&producerThread,NULL,producer,NULL);
	}
	for (int i = 0; i < num3; i++) {
		pthread_create(&consumerThread,NULL,consumer,NULL);
	}
	//pthread_create(&producerThread,NULL,producer,NULL);
	//pthread_create(&consumerThread,NULL,consumer,NULL);
	sleep(num1);
	/* sleep */
	/* exit */
	//pthread_exit(NULL);
	for (int i = 0; i < IdVector.size(); i++) 
	{
		pthread_exit(&IdVector[i]);
	}
	return 0;
}


