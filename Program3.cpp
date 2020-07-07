// Write a program inC or C++ to implement Linux pipes using the pipe() and dup2() system calls. 
// Your program should be able to handle up to 5 pipes specified in the user command.

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

using namespace std;

void execCommand(int n, vector<string> tokens);
void OnePipe(vector<vector<string> > Args);
void TwoPipes(vector<vector<string> > Args);
void ThreePipes(vector<vector<string> > Args);
void FourPipes(vector<vector<string> > Args);
void FivePipes(vector<vector<string> > Args);

int main() {
cout << "Program #3" << endl;
bool cont = true;
while ( cont == true ) {

	cout << "Enter a command: ";
	string insertedToken = "";
	getline(cin, insertedToken);
	int n = insertedToken.length();  
	char char_array[n+1];  
	strcpy(char_array, insertedToken.c_str());  
        vector<string> tokenVector;
	char *token;
	token = strtok(char_array, " ");
	cout << endl;
   			do
   			{
				string s(token);
				stringstream sstream(s); 
    				int x = 0; 
    				sstream >> x;
				tokenVector.push_back(s);
   			}
   			while (token = strtok(NULL, " "));
	vector<vector<string> > newTokenVector;
	newTokenVector.resize(6);
	vector<string> buffer;
	int pipeCount = 0;
	for (int i = 0; i < tokenVector.size(); i++) {
		//cout << "Token[" << i << "] = '" << tokenVector[i] << "'" << endl;
		if (tokenVector[i] != "|") {
			buffer.push_back(tokenVector[i]);
		}
		else {	
			newTokenVector[pipeCount] = buffer;
			buffer.clear();	
			pipeCount+= 1;
		}
		if (i == tokenVector.size() - 1) {
			newTokenVector[pipeCount] = buffer;
		}
		
	}
	
	
	pipeCount = -1;
	for (int i = 0; i < newTokenVector.size(); i++) {
		if (newTokenVector[i].size() > 0) {
			pipeCount+= 1;
		}
	}
	//cout << "pipeCount = " << pipeCount << endl;
	switch (pipeCount) {
		case 1:
			OnePipe(newTokenVector); break;
		case 2:
			TwoPipes(newTokenVector); break;	
		case 3:
			ThreePipes(newTokenVector); break;
		case 4:
			FourPipes(newTokenVector); break;	
		case 5:
			FivePipes(newTokenVector); break;
		default: 
			cout << "No command with pipe entered" << endl;	
	}

	








cont = false;
}// end while loop
return 0;
}

void execCommand(int n, vector<string> tokens) {
	if (n == 1) {
		execlp(tokens[0].c_str(), tokens[0].c_str(), NULL);
	}
	else if (n == 2) {
		execlp(tokens[0].c_str(), tokens[0].c_str(), tokens[1].c_str(), NULL);
	}
	else if (n == 3) {
		execlp(tokens[0].c_str(), tokens[0].c_str(), tokens[1].c_str(), tokens[2].c_str(), NULL);
	}
}



void OnePipe(vector<vector<string> > Args) {
  int status;
  int i;
  int fd[2];
  pipe(fd);
  //pipes[0] = read end of ls -> sort pipe (read by sort)
  //pipes[1] = write end of ls -> sort pipe (written by ls)
   if (fork() == 0)
    {
      dup2(fd[1], 1);
      close(fd[0]);
      close(fd[1]);
      execCommand(Args[0].size(), Args[0]);
    }
  else
    {
      // fork a second child (to execute sort)

      if (fork() == 0)
	{
	  dup2(fd[0], 0);
	  close(fd[0]);
	  close(fd[1]);
	  execCommand(Args[1].size(), Args[1]);
	}
    }
  close(fd[0]);
  close(fd[1]);
  for (i = 0; i < 2; i++)
    wait(&status);
}

void TwoPipes(vector<vector<string> > Args) {

  int status;
  int i;

  int fd[4];
  pipe(fd); 
  pipe(fd + 2); 

  
  if (fork() == 0)
    {


      dup2(fd[1], 1);



      close(fd[0]);
      close(fd[1]);
      close(fd[2]);
      close(fd[3]);


      execCommand(Args[0].size(), Args[0]);
    }
  else
    {


      if (fork() == 0)
	{

	  
	  dup2(fd[0], 0);



	  dup2(fd[3], 1);



	  close(fd[0]);
	  close(fd[1]);
	  close(fd[2]);
	  close(fd[3]);


	 execCommand(Args[1].size(), Args[1]);
	}
      else
	{


	  if (fork() == 0)
	    {


	      dup2(fd[2], 0);



	      close(fd[0]);
	      close(fd[1]);
	      close(fd[2]);
	      close(fd[3]);


		execCommand(Args[2].size(), Args[2]);
	    }
	}
    }
      

  close(fd[0]);
  close(fd[1]);
  close(fd[2]);
  close(fd[3]);

  for (i = 0; i < 3; i++)
    wait(&status);

}//Two Pipes Done Here



void ThreePipes(vector<vector<string> > Args) {

  int status;
  int i;



  int fd[6];
  pipe(fd); 
  pipe(fd + 2); 
  pipe(fd + 4);
  
  
  if (fork() == 0)
    {


      dup2(fd[1], 1);



      close(fd[0]);
      close(fd[1]);
      close(fd[2]);
      close(fd[3]);
      close(fd[4]);
      close(fd[5]);

      execCommand(Args[0].size(), Args[0]);
    }
  else
    {


      if (fork() == 0)
	{

	  
	  dup2(fd[0], 0);



	  dup2(fd[3], 1);




	  close(fd[0]);
	  close(fd[1]);
	  close(fd[2]);
	  close(fd[3]);
	  close(fd[4]);
	  close(fd[5]);

	 execCommand(Args[1].size(), Args[1]);
	}
      else
	{


	  if (fork() == 0)
	    {


	      dup2(fd[2], 0);

	      dup2(fd[5], 1);



	      close(fd[0]);
	      close(fd[1]);
	      close(fd[2]);
	      close(fd[3]);
	      close(fd[4]);
	      close(fd[5]);

		execCommand(Args[2].size(), Args[2]);
	    }
	   else {


	  	if (fork() == 0)
	   	 {


	    	  dup2(fd[4], 0);



	     	 close(fd[0]);
	     	 close(fd[1]);
	      	 close(fd[2]);
	      	 close(fd[3]);
		 close(fd[4]);
	      	 close(fd[5]);

		 execCommand(Args[3].size(), Args[3]);
	    	}
	   }
	}
    }
      

  
  close(fd[0]);
  close(fd[1]);
  close(fd[2]);
  close(fd[3]); 
  close(fd[4]);
  close(fd[5]);

  for (i = 0; i < 4; i++)
    wait(&status);
}//Three Pipes Done Here

























void FourPipes(vector<vector<string> > Args) {

  int status;
  int i;



  int fd[8];
  pipe(fd); // sets up 1st pipe
  pipe(fd + 2);
  pipe(fd + 4);
  pipe(fd + 6);

  
  if (fork() == 0)
    {


      dup2(fd[1], 1);



      close(fd[0]);
      close(fd[1]);
      close(fd[2]);
      close(fd[3]);
      close(fd[4]);
      close(fd[5]);
      close(fd[6]);
      close(fd[7]);

      execCommand(Args[0].size(), Args[0]);
    }
  else
    {


      if (fork() == 0)
	{

	  
	  dup2(fd[0], 0);



	  dup2(fd[3], 1);




	  close(fd[0]);
          close(fd[1]);
     	  close(fd[2]);
      	  close(fd[3]);
     	  close(fd[4]);
      	  close(fd[5]);
      	  close(fd[6]);
     	  close(fd[7]);

	 execCommand(Args[1].size(), Args[1]);
	}
      else
	{


	  if (fork() == 0)
	    {


	      dup2(fd[2], 0);

	      dup2(fd[5], 1);



	     close(fd[0]);
      	     close(fd[1]);
     	     close(fd[2]);
      	     close(fd[3]);
     	     close(fd[4]);
     	     close(fd[5]);
     	     close(fd[6]);
     	     close(fd[7]);

		execCommand(Args[2].size(), Args[2]);
	    }
	   else {


	  	if (fork() == 0)
	   	 {


	    	  dup2(fd[4], 0);

		  dup2(fd[7], 0);



	     	 close(fd[0]);
	     	 close(fd[1]);
	      	 close(fd[2]);
	      	 close(fd[3]);
		 close(fd[4]);
	      	 close(fd[5]);
		 close(fd[6]);
		 close(fd[7]);

		 execCommand(Args[3].size(), Args[3]);
	    	}
		 else {


	  		if (fork() == 0)
	   		 {


	    		  dup2(fd[6], 0);



	     	 	close(fd[0]);
	     		close(fd[1]);
	      		close(fd[2]);
	      	 	close(fd[3]);
		 	close(fd[4]);
	      	 	close(fd[5]);
		 	close(fd[6]);
			close(fd[7]);

		 	execCommand(Args[4].size(), Args[4]);
	    		}
	   	}
	   }
	}
    }
      

  
  close(fd[0]);
  close(fd[1]);
  close(fd[2]);
  close(fd[3]); 
  close(fd[4]);
  close(fd[5]);
  close(fd[6]);
  close(fd[7]);

  for (i = 0; i < 5; i++)
    wait(&status);
}//four Pipes Done Here












































void FivePipes(vector<vector<string> > Args) {

  int status;
  int i;



  int fd[10];
  pipe(fd); 
  pipe(fd + 2); 
  pipe(fd + 4);
  pipe(fd + 6);
  pipe(fd + 8); 

  
  
  if (fork() == 0)
    {


      dup2(fd[1], 1);



      close(fd[0]);
      close(fd[1]);
      close(fd[2]);
      close(fd[3]);
      close(fd[4]);
      close(fd[5]);
      close(fd[6]);
      close(fd[7]);
      close(fd[8]);
      close(fd[9]);

      execCommand(Args[0].size(), Args[0]);
    }
  else
    {


      if (fork() == 0)
	{

	  
	  dup2(fd[0], 0);



	  dup2(fd[3], 1);




	  close(fd[0]);
          close(fd[1]);
     	  close(fd[2]);
      	  close(fd[3]);
     	  close(fd[4]);
      	  close(fd[5]);
      	  close(fd[6]);
     	  close(fd[7]);
 close(fd[8]);
      close(fd[9]);

	 execCommand(Args[1].size(), Args[1]);
	}
      else
	{


	  if (fork() == 0)
	    {


	      dup2(fd[2], 0);

	      dup2(fd[5], 1);



	     close(fd[0]);
      	     close(fd[1]);
     	     close(fd[2]);
      	     close(fd[3]);
     	     close(fd[4]);
     	     close(fd[5]);
     	     close(fd[6]);
     	     close(fd[7]);
 	     close(fd[8]);
             close(fd[9]);

		execCommand(Args[2].size(), Args[2]);
	    }
	   else {


	  	if (fork() == 0)
	   	 {


	    	  dup2(fd[4], 0);

		  dup2(fd[7], 0);



	     	 close(fd[0]);
	     	 close(fd[1]);
	      	 close(fd[2]);
	      	 close(fd[3]);
		 close(fd[4]);
	      	 close(fd[5]);
		 close(fd[6]);
		 close(fd[7]);
 		 close(fd[8]);
     		 close(fd[9]);

		 execCommand(Args[3].size(), Args[3]);
	    	}
		 else {


	  		if (fork() == 0)
	   		 {


	    		  dup2(fd[6], 0);
			  dup2(fd[9], 1);


	     	 	close(fd[0]);
	     		close(fd[1]);
	      		close(fd[2]);
	      	 	close(fd[3]);
		 	close(fd[4]);
	      	 	close(fd[5]);
		 	close(fd[6]);
			close(fd[7]);
             		close(fd[8]);
      			close(fd[9]);

		 	execCommand(Args[4].size(), Args[4]);
	    		}
			 else {


	  			if (fork() == 0)
	   			 {


	    			  dup2(fd[8], 0);



	     	 		close(fd[0]);
	     			close(fd[1]);
	      			close(fd[2]);
	      	 		close(fd[3]);
		 		close(fd[4]);
	      	 		close(fd[5]);
		 		close(fd[6]);
				close(fd[7]);
             			close(fd[8]);
      				close(fd[9]);

		 		execCommand(Args[5].size(), Args[5]);
	    			}
	   		}
	   	}
	   }
	}
    }
      

  
  close(fd[0]);
  close(fd[1]);
  close(fd[2]);
  close(fd[3]); 
  close(fd[4]);
  close(fd[5]);
  close(fd[6]);
  close(fd[7]);
  close(fd[8]);
  close(fd[9]);

  for (i = 0; i < 6; i++)
    wait(&status);
}//five Pipes Done Here

