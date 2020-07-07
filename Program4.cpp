// Write a program inC or C++ to implement an integer adder that can be interrupted by a SIGINT (i.e.Ctrl-C) signal. 

#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include <string.h>
#include <vector>
#include <sstream>
#include <fcntl.h>
#include <signal.h>

using namespace std;


static volatile sig_atomic_t doneflag = 0;

void IgnoreSIGINT() {
	struct sigaction act;
	if (sigaction(SIGINT, NULL, &act) == -1)  /* Find current SIGINT handler */
   		cout << "Failed to get old handler for SIGINT" << endl;
	//else if (act.sa_handler == SIG_DFL) {    /* check if SIGINT handler is default */
   		act.sa_handler = SIG_IGN;         /* set new SIGINT handler to ignore */
   		if (/*(sigemptyset(&act.sa_mask) == -1) || */(sigaction(SIGINT, &act, NULL) == -1))
     			cout << "Failed to ignore SIGINT" << endl;
	//}
}
void DefaultSIGINT(struct sigaction act) {
	if (sigaction(SIGINT, NULL, &act) == -1)  /* Find current SIGINT handler */
   		cout << "Failed to get old handler for SIGINT" << endl;
		act.sa_handler = SIG_DFL;	  /* set new SIGINT handler back to ignore*/
		if (/*(sigemptyset(&act.sa_mask) == -1) || */(sigaction(SIGINT, &act, NULL) == -1)) 
     			cout << "Failed to ignore SIGINT" << endl;
}


static void setdoneflag(int signo) {
    doneflag = 1;
}

void CustomHandler(struct sigaction act) {
	act.sa_handler = setdoneflag;
    	act.sa_flags = 0;
   	if (/*(sigemptyset(&act.sa_mask) == -1) || */(sigaction(SIGINT, &act, NULL) == -1)) {
      	 	perror("Failed to set SIGINT handler");
      		//return 1;
	}
}

int main() {

int n, count;
struct sigaction act;

while (1) {
	
	n = 0, count = 0;
	IgnoreSIGINT(); //set program to ignore SIGINT
	cout << endl << "Enter a large integer: ";
	cin.clear();	
	cin >> n;
	DefaultSIGINT(act); //set program back to default for SIGINT
	

	

	CustomHandler(act);
	cout << "adding..." << endl;
	while (!doneflag) {
		count += 1;
		if (count == n) {
			cout << "done adding!!" << endl;
			cout << "summation completed succesfully" << endl;
			break;
		}
	}
	
	if (doneflag != 0) {
		cout << endl << "Summation interupted by SIGINT" << endl;
		cout << "Completed additions = " << count << endl;
		return 1;
	}
	//else the program loops again
}











return 1;
}
