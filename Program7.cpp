// Multithreaded Sorting Program

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void *Sort(void *params);  
void *Merge(void *params);    
vector<int> list;
vector<int> result;
int SIZE;

typedef struct
{
    int first;
    int last;
} parameters;

int main (int argc, const char * argv[])
{
    string line;
    int num;
    ifstream myfile("p2data.txt");
    if(myfile.is_open())
    {
            while(!myfile.eof()){
            if (getline(myfile,line)) {
                num = atoi(line.c_str());
                list.push_back(num);
            }
            }
            myfile.close();
    }
    else cout << endl << "Unable to open file" << endl;; 
    SIZE = list.size();
    pthread_t threads[3];
    parameters *data = (parameters *) malloc (sizeof(parameters));
    data->first = 0;
    data->last = (SIZE/2) - 1;
    pthread_create(&threads[0], 0, Sort, data); //create first thread

    data = (parameters *) malloc (sizeof(parameters));
    data->first = (SIZE/2);
    data->last = SIZE - 1;
    pthread_create(&threads[1], 0, Sort, data); //create second thread

    for (int i = 0; i < 2; i++) 
    {
        pthread_join(threads[i], NULL); //wait to finnish
    }

    data = (parameters *) malloc(sizeof(parameters));
    data->first = 0;
    data->last = (SIZE/2);
    pthread_create(&threads[2], 0, Merge, data); // create third thread

    pthread_join(threads[2], NULL); // wait to finish
    return 0;
}

void *Sort(void *params)
{
    parameters* p = (parameters *)params;
    int begin = p->first;
    int end = p->last+1;
    cout << endl;
    for(int i = begin; i < end; i++)
    {
        cout << "Array read in: " << list[i] << endl;
    }
    cout << endl;
    int temp;
    for(int i = begin; i < end; i++)
    {
        for(int j = begin; j < end-1; j++)
        {
            if(list[j] > list[j+1])
            {
                temp = list[j];
                list[j] = list[j+1];
                list[j+1] = temp;
            }
        }
    }

    for(int i = begin; i < end; i++)
    {
        cout << "Sorted Array: " << list[i] << endl;
    }

    for(int i = begin; i < end; i++)
    {
        result.push_back(list[i]);
    }
    pthread_exit(NULL);
}

void *Merge(void *params)
{
    parameters* p = (parameters *)params;
    int begin = p->first;
    int end = p->last+1;
    for(int i = begin; i < SIZE; i++)
    {
    result.push_back(list[i]);
    }
    int temp;
    for(int i = begin; i < SIZE - 1; i++)
    {
            for(int j = begin; j < SIZE - 1; j++)
            {
                if(result[j] > result[j+1])
                {
                    temp = result[j];
                    result[j] = result[j+1];
                    result[j+1] = temp;
                }
            }
    }
    cout << endl;
    cout << "Final Array Merged and Sorted: ";
    for(int i = begin; i < SIZE; i++)
    {
        cout << result[i] << ", ";
    }
    cout << endl;
    pthread_exit(NULL);
}
