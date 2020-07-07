// Write a program in C or C++ to process a list of Linux shell commands stored in a 
// batch file using the fan-of-processesapproach

//#include <pthread.h>
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
using namespace std;


int main() {
	//#1 Your program should first open a batchfile containing a list of shell commands. Thebatchfile should be named batch.datand hard coded in your program. 
	string line;
	pid_t childpid;
	int i, counter = 0;
	ifstream myfile("batch.dat");
	vector<string> Commands;
	if(myfile.is_open())
	{
        	while(!myfile.eof()){
			if (getline(myfile,line)) {
				//#2	Each shell command is stored on one line in the file. Once thefile is open, your program should firstdeterminethe number of commands contained in thefile. 
				counter += 1;	
				Commands.push_back(line.c_str());			
				//cout << line.c_str(); 
				//cout << "  " << counter << endl;
			}
       	 	}
       	 myfile.close();
	//cout << endl << counter << " commands in file" << endl;
	}
	else { 
		cout << endl << "Unable to open file" << endl;
	}

	//#3	It then creates a child process to execute each command. 
	//		For example, suppose the batch file has been determined to contain 3 shell commands. 
	//		Your program (i.e.the parent process) first takes the first command from the batch file and 
	//		creates a child process to execute it. 
	for (i = 1; i < counter; i++) {
		if ((childpid = fork()) == 0) {
			break;
		}
	}
	
	while (wait(NULL) > 0);
	
	string commandi = Commands[i-1];
	int strLength = Commands[i-1].length();
	char str[strLength] = {};
	//cout << "i: " << i << " command: " << Commands[i-1] << endl;
	vector<string> Tokens;
	for(int x = 0; x < commandi.length(); ++x){
    	str[x] = commandi[x];
	}
	char delim[2] = " ";
	char *token = strtok(str, delim);
	int count = 0;
	while (token)
    {
		Tokens.push_back(token);
        //cout << "Token " << count + 1 << ": " << token << endl;
		//count += 1;
        token = strtok(NULL,delim);
    }
	//cout << "token count = " << Tokens.size() << endl;
	//fprintf(stderr, "i:%d process ID:%d parent ID:%d child ID:%d\n", i, getpid(), getppid(), childpid);
	cout << "Command: " << commandi << endl;
	//#4	The parent process then waits until the child process finishes executing the command. 
	if (Tokens.size() == 1) {
		execlp(Tokens[0].c_str(), Tokens[0].c_str(), NULL);
	}
	else if (Tokens.size() == 2) {
		execlp(Tokens[0].c_str(), Tokens[0].c_str(), Tokens[1].c_str(), NULL);
	}
	else if (Tokens.size() == 3) {
		execlp(Tokens[0].c_str(), Tokens[0].c_str(), Tokens[1].c_str(), Tokens[2].c_str(), NULL);
	}
	Tokens.clear();
	cout << "Command has Executed." << endl << endl;
	//#5	The parent process then takes the second command and creates a child process to execute it. 

	//#6	This process is repeated until all of the commands stored in the batch file are executed. 

	//#7	The parent process then exits and terminates.



return 0;
}
