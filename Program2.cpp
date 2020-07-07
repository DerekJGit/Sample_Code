// Write a program inC or C++ to read inthreefile names via command-line arguments, 
// among which the first two files are input files and the third one an output file. 
// Your program shouldfirst determine the input file(out of the 2 input files)that has 
// more words in it and then dump the contents of it into the output file.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;


#define BLKSIZE 100
#define READ_FLAGS O_RDONLY
// #define WRITE_FLAGS (O_WRONLY | O_CREAT | O_EXCL) // This will result in open failure
#define WRITE_FLAGS_COPY (O_WRONLY | O_CREAT)
#define WRITE_FLAGS_APPEND (O_WRONLY | O_CREAT | O_APPEND) // O_CREAT has no effect
#define WRITE_PERMS (S_IRUSR | S_IWUSR)

int copyfile(int fromFile, int toFile);
int appendToFile(int fromFile, int toFile);
int main(int argc, char *argv[]) {
	
	int bytes, inputFile1, inputFile2, outputFile3;
	char buf[BLKSIZE];
/*(1) The number of arguments passed to your program is notequal to 4. 
In this case, your program should first print a message to the screen indicating the correct way to enter
thecommand andthenterminate.*/
	if (argc != 4) {
	//fprintf(stderr, "Usage: %s from_file to_file\n", argv[0]);
	cout << endl << "Case (1): The number of arguments passed to your program is notequal to 4. " << endl;
	cout << "Correct Arguments -> 'inputFile1' 'inputFile2' 'outputFile3' as the 3 arguments" << endl << endl;
	return 1;
	}

	cout << endl << endl;
	



/*(2) Both input files do not exist. In this case, your program should first print a message to thescreen indicating the 		case and then terminate.*/
	 if (((inputFile1 = open(argv[1], READ_FLAGS)) == -1) && ((inputFile2 = open(argv[2], READ_FLAGS)) == -1)) {
      		//perror("Failed to open input file");
		cout << "Case (2): Both input files do not exist." << endl << endl;
      		return 1;
   	}




/*(3) One of the two input files does not exist. 
- In this case, your program shouldfirst print avmessage to the screen indicating the case and 
- then create an output file (if it does not exist)and dump the contents of the existing input file(may be empty)into the output file. 
- If the outputfile already exists, your program should append the contents of the existing input file to the output file.*/

	if ((inputFile1 = open(argv[1], READ_FLAGS)) == -1) { //check if inputFile1 exists
      		//perror("Failed to open input file");
		cout << "Case (3): One of the two input files does not exist." << endl;		
		cout << "inputFile1 does not exist" << endl;
		cout << "Dumping contents of inputFile2 into outputFile3..." << endl << endl;
		if ((outputFile3 = open(argv[3], WRITE_FLAGS_APPEND, WRITE_PERMS)) == -1) { 
      			perror("Failed to create output file");
   			cout << "Failed to create output file" << endl << endl;
   		}
		else {		//output file already exists
			bytes = appendToFile(inputFile2, outputFile3);
			return 1;
		}
				
   	}
	
	if ((inputFile2 = open(argv[2], READ_FLAGS)) == -1) { //check if inputFile2 exists
      		//perror("Failed to open input file");
		cout << "Case (3): One of the two input files does not exist." << endl;				
		cout << "inputFile2 does not exist" << endl;
		cout << "Dumping contents of inputFile1 into outputFile3..." << endl << endl;
		//try to create output file if not exists
		if ((outputFile3 = open(argv[3], WRITE_FLAGS_APPEND, WRITE_PERMS)) == -1) { // open output file
      			perror("Failed to create output file");
   			cout << "Failed to create output file" << endl << endl;
   		}
		else {		//output file already exists	
			bytes = appendToFile(inputFile1, outputFile3);
			return 1;
		}	
   	}




/*(4) Both input files already exist but both are empty. 
- In this case, your program shouldfirst printa message indicating the case and then create an empty output file if it does not exist. - If theoutput file already exists, your program simply terminates.*/
	
	if ((read(inputFile1, buf, BLKSIZE) == 0) && (read(inputFile2, buf, BLKSIZE) == 0)) {
		cout << "Case (4): Both input files exist but both are empty" << endl << endl;
		return 1;
	}

	


/*(5) Both input files already exist but one of them is empty. 
- In this case, your program should first print a message to the screen indicating the case, 
- create an output file(if it does not exist),and 
- then dump the contents of the non-empty input file to the output file. 
- If the output file already exists, your program should append the contents of the non-empty input file to theoutput file.*/

	//int bytesread;	
	//char buf[BLKSIZE];
	//cout << "Checking case 5..." << endl;
	if ((inputFile1 = open(argv[1], READ_FLAGS)) != -1) { //check if inputFile1 exists
		if (read(inputFile1, buf, BLKSIZE) == 0) {
			cout << "Case (5): Both input files already exist but one of them is empty." << endl;			
			cout << "inputFile1 exists but is empty" << endl; //so then write what is on inputFile2	
			//this block writes to the file			
			if ((outputFile3 = open(argv[3], WRITE_FLAGS_APPEND, WRITE_PERMS)) == -1) { // open output file
      				perror("Failed to create output file");
   				cout << "Failed to create output file" << endl;
   			}
			else {		//output file already exists
				inputFile2 = open(argv[2], READ_FLAGS);
				bytes = appendToFile(inputFile2, outputFile3);
				cout << "Writing contents of inputFile2 to output file." << endl << endl;
				return 1;
			}
			//return 1;
		}
   	}
	if ((inputFile2 = open(argv[2], READ_FLAGS)) != -1) { //check if inputFile2 exists
		if (read(inputFile2, buf, BLKSIZE) == 0) {
			cout << "Case (5): Both input files already exist but one of them is empty." << endl;	
			cout << "inputFile2 exists but is empty" << endl; //so then write what is on inputFile1
			//this block writes to the file			
			if ((outputFile3 = open(argv[3], WRITE_FLAGS_APPEND, WRITE_PERMS)) == -1) { // open output file
      				perror("Failed to create output file");
   				cout << "Failed to create output file" << endl;
   			}
			else {		//output file already exists
				inputFile1 = open(argv[1], READ_FLAGS);	
				bytes = appendToFile(inputFile1, outputFile3);
				cout << "Writing contents of inputFile1 to output file." << endl << endl;
				return 1;
			}
			//return 1;
		}
   	}
	//cout << "Case 5 Success!" << endl << endl;
	






/*(6) Both of the input files exist and are non-empty. 
- In this case, your program shouldfirst print a message to the screen indicating the case, 
- create an output file (if it does exist), 
- and then dump the contents of the input file that has more words into the output file. 
- If the two input files contain the same number of words, your program should always dump the contents of the first input file into the output file. 
- If the output file already exists, your program should append the contents of the input file determined to the output file.*/
	char buf1[BLKSIZE];
	char buf2[BLKSIZE];
	//cout << "Checking case 6..." << endl;
	if (((inputFile1 = open(argv[1], READ_FLAGS)) != -1) && ((inputFile2 = open(argv[2], READ_FLAGS)) != -1)) {
		int bytes1 = read(inputFile1, buf1, BLKSIZE);
		int bytes2 = read(inputFile2, buf2, BLKSIZE);
		//cout << "bytes1 = " << bytes1 << " bytes2 = " << bytes2 << endl;
		if ((bytes1 > 0) && (bytes2 > 0)) {
			cout << "Case (6): Both input files exists but both are non-empty" << endl;
			if (bytes1 > bytes2) {
				if ((inputFile1 = open(argv[1], READ_FLAGS)) != -1) {
					bytes = appendToFile(inputFile1, outputFile3);
					cout << "*****inputFile1 contains more words, contents of inputFile1 written to output file.*****" << endl << endl;
				}
			}
			else if (bytes1 == bytes2) {
				if ((inputFile1 = open(argv[1], READ_FLAGS)) != -1) {
					bytes = appendToFile(inputFile1, outputFile3);
					cout << "*****Both input files contain same number of words, contents of inputFile1 written to output file.*****" << endl << endl;
				}
			}
			else {
				if ((inputFile2 = open(argv[2], READ_FLAGS)) != -1) {
					bytes = appendToFile(inputFile2, outputFile3);
					cout << "*****inputFile2 contains more words, contents of inputFile2 written to output file.*****" << endl << endl;
				}
			}	
			return 1;
		}	
	}	
	//cout << "Case 6 Success" << endl;

	



return 0;
}






int copyfile(int fromFile, int toFile) {
   char *bp;
   char buf[BLKSIZE];
   int bytesread, byteswritten;
   int totalbytes = 0;
   int done = 0;

   while (!done) {
      while (((bytesread = read(fromFile, buf, BLKSIZE)) == -1) &&
            (errno == EINTR)); // handle interruption by signal

      if (bytesread <= 0) // real error or end-of-file on fromfd
         break;

      bp = buf;

      while (bytesread > 0) {
         while (((byteswritten = write(toFile, bp, bytesread)) == -1 ) &&
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

   return totalbytes;
}




int appendToFile(int fromFile, int toFile) {
   char *bp;
   char buf[BLKSIZE];
   char *bp2;
   char buf2[BLKSIZE];
   int bytesread, byteswritten;
   int totalbytes = 0;
   int done = 0;

   while (!done) {

 	
      while (((bytesread = read(fromFile, buf, BLKSIZE)) == -1) &&
            (errno == EINTR)); // handle interruption by signal

      if (bytesread <= 0) // real error or end-of-file on fromfd
         break;

      bp = buf;


      while (bytesread > 0) {



         while (((byteswritten = write(toFile, bp, bytesread)) == -1 ) &&
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

   return totalbytes;
}

