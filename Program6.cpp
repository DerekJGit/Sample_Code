// Write a program inC or C++ to merge two files using three POSIX threads. 

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <string.h>
#include <errno.h>
using namespace std;

#define BLKSIZE 100
#define BUFSIZE 1024
#define READ_FLAGS O_RDONLY
#define WRITE_FLAGS_COPY (O_WRONLY | O_CREAT)
#define WRITE_FLAGS_APPEND (O_WRONLY | O_CREAT | O_APPEND | O_RDONLY)
#define READ_WRITE_FLAGS (O_RDWR)

void *OpenFile(void *params);  
void *ConcatenateFile(void *params); 
void *DisplayContents(void *params);

typedef struct
{
    string first;
    string last;
} parameters;
typedef struct
{
    int first;
    int last;
} intParameters;
int fd1, fd2;
int fileDescriptors[2];

int main() {
    pthread_t threads[3];                                            //Declare the three threads
    string fileNames[2] = {"inFile1.txt", "inFile2.txt"};            //Array of fileNames
    
    
    cout << endl << "Thread 1 opening the two files" << endl;
    pthread_create(&threads[0], NULL, OpenFile, fileNames);          //Create FIRST thread to open two files
    pthread_join(threads[0], NULL);                                  //Wait for the FIRST thread to finnish

    cout << endl << "Thread 2 concatenating inFile2.txt to inFile1.txt" << endl;
    pthread_create(&threads[1], NULL, ConcatenateFile, NULL);        //Create SECOND merge the two files
    pthread_join(threads[1], NULL);                                  //Wait for the SECOND thread to finnish
    
    cout << endl << "Thread 3 displaying conents of inFile1.txt" << endl;
    pthread_create(&threads[2], NULL, DisplayContents, fileDescriptors);        //Create THIRD thread to display files
    pthread_join(threads[2], NULL);                                  //Wait for the THIRD thread to finnish

    cout << endl;
    return 0;
}

void *OpenFile(void *params) {
    //cout << endl << "OpenFile function running..." << endl;
    parameters* p = (parameters *)params;
    string firstFile = p->first;
    string secondFile = p->last;
    const char *firstFileChar = firstFile.c_str();                              //Convert strings to char
    const char *secondFileChar = secondFile.c_str();                            //Convert strings to char
    mode_t fmode = (S_IRWXO);                        
    //cout << firstFileChar << ", " << secondFileChar << endl;
    
    fd1 = open(firstFileChar, WRITE_FLAGS_APPEND, fmode);                              //Open inFile1
    fd2 = open(secondFileChar, READ_WRITE_FLAGS);                                     //Open inFile2
    fileDescriptors[0] = fd1;
    fileDescriptors[1] = fd2;
    if ((fd1 != -1) && (fd2 != -1))
    {
        cout << "First thread has opened both files successfully" << endl;      //Check to make sure both inFile1 and inFile2 have opened
    }
}

void *ConcatenateFile(void *params) {
    //cout << endl << "ConcatenateFile function running..." << endl;
    
   char *bp;
   char buf[BLKSIZE];
   char *bp2;
   char buf2[BLKSIZE];
   int bytesread, byteswritten;
   int totalbytes = 0;
   int done = 0;

   while (!done) {
      while (((bytesread = read(fd2/*fromFile*/, buf, BLKSIZE)) == -1) &&
            (errno == EINTR)); // handle interruption by signal
            
      if (bytesread <= 0) // real error or end-of-file on fromfd
      {  
         //cout << "inFile2.txt is empty, no contents have been written" << endl;
         break;
      }
      bp = buf;
      while (bytesread > 0) {
         while (((byteswritten = write(fd1/*toFile*/, bp, bytesread)) == -1 ) &&
               (errno == EINTR)); // handle interruption by signal

         if (byteswritten <= 0) //  real error on tofd
            break;
         totalbytes += byteswritten;
         bytesread -= byteswritten;

         if (bytesread == 0)
            done = 1;
      }

      if (byteswritten == -1) // real error on tofd
         break;
   }
   if (byteswritten > 0)
        cout << "Contents of inFile2.txt written to inFile1.txt" << endl;
 
}
void *DisplayContents(void *params) {
    //cout << endl << "DisplayContents function running..." << endl;
  
    char bufFile1[BUFSIZE], bufFile2[BUFSIZE]; 
    //int bufSize1, bufSize2;
    char *bp, *bp2;
    int inFile1 = open("inFile1.txt", READ_FLAGS);
    int bytesread = read(inFile1, bufFile1, BUFSIZE);
    
    if (bytesread > 0)
    {
        for (int i = 0; i < bytesread; i++)
            cout << bufFile1[i];
    }
    else 
    {
        cout << "inFile1.txt is empty" << endl;
    }
  

}




///////////////DONE/////////////////////
//Your program should first create a thread to open two files (named inFile1.txtand inFile2.txt). 


///////////////STILL WORKING////////////
//It then creates another thread to concatenate the second file (i.e.inFile2.txt) to the first file (i.e.inFile1.txt). 

//Your program then creates the last thread to display the contents of the merged file to the screen.

//You may assume both files exist when they are open.However, one or both input files can be empty.

//Also, the main thread has to use the pthread_join()function to wait until the corresponding thread terminates.