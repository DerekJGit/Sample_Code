// Write a program inC or C++ to display periodically the CPU time consumed by your program and 
// that by the kernel on behalf of your program. 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <sys/times.h>
#include <time.h>
#include <signal.h>


using namespace std;

#define MILLION 1000000L
void disableTimer();
void startTimer();
//volatile int sigcount = 0; // used to count how many SIGALRM signals have been raised
bool flag = false;

void catcher(int sig) { 
    //cout << "Signal catcher called for signal " << sig << "(SIGALRM)" << endl; // SIGALRM is caught
    flag = true;
    
}




int main() {
    struct itimerval cpuStartValue, cpuEndValue;
    long diftime;

    cpuStartValue.it_interval.tv_sec = 0; // timer is disabled after next expiration
    cpuStartValue.it_interval.tv_usec = 0;
    cpuStartValue.it_value.tv_sec = MILLION; // a large remaining time reset to 0 after decrement to 0
    cpuStartValue.it_value.tv_usec = 0;
    setitimer(ITIMER_PROF, &cpuStartValue, NULL);

    //Initialize signal handler
    struct sigaction sact;
    sigemptyset(&sact.sa_mask);
    sact.sa_flags = 0;
    sact.sa_handler = catcher;
    sigaction(SIGALRM, &sact, NULL); 
    //the signal is generated when 200 mseconds are elapsed


    startTimer();
  
    int counter, MAX, randNum, sum;
    for (counter = 0, MAX = 1000000; counter <= MAX; counter++) 
    {
        if (flag == true) //check before the computation to see if the flag is true
        {
            //disable the timer
            disableTimer();
            //cout << endl << "timer is now DISABLED" << endl;

            //LETS GET THE CPU TIME THAT HAS PASSED.
            getitimer(ITIMER_PROF, &cpuEndValue);
            diftime = MILLION*(cpuStartValue.it_value.tv_sec - cpuEndValue.it_value.tv_sec) + (cpuStartValue.it_value.tv_usec - cpuEndValue.it_value.tv_usec);
            cout << "Time since program started: " << diftime << " microseconds, " << diftime/(double)MILLION << " seconds" << endl;
            
            flag = false;
            //cout << "timer is now ENABLED" << endl << endl;
            startTimer();
        }
        sum = counter + 1;
        if (counter == MAX) 
        {
            counter = 0;
            randNum = (rand() % 3);
            //cout << "randNum = " << randNum << endl;
            switch (randNum) 
            {
                case 0: MAX = 1000000; break;
                case 1: MAX = 2000000; break;
                case 2: MAX = 3000000; break;
                default: cout << "Random number generator error" << endl; return 1;
            }
        }
    }
    return 1;
}

void disableTimer() {
        struct itimerval value;
        int which = ITIMER_REAL; // wall clock time elapsed
        // Disable the real time interval timer after the program is interrupted by SIGALRM twice
        getitimer(which, &value);
        value.it_value.tv_sec = 0; // set it_value to 0 to disable the timer
        value.it_value.tv_usec = 0;
        setitimer(which, &value, NULL);
        // Real time interval timer is now disabled
}

void startTimer() {
    int result = 0;
    struct itimerval value, ovalue, pvalue; //itimerval struct
    int which = ITIMER_REAL;   //will generate a SIGALRM after time expires
    
    getitimer(which, &pvalue);
    
    // Set a real-time interval timer to 500 milliseconds initialily
    // and repeat every 200 milliseconds after the initial time expires
    value.it_interval.tv_sec = 2;        // Zero second time interval
    value.it_interval.tv_usec = 0;       // Two hundred milliseconds time interval
    value.it_value = value.it_interval;
    result = setitimer(which, &value, &ovalue);
}



//////////STEPS DONE/////////

/* Write a program in C or C++ to display periodically the CPU time consumed by your program 
and that by the kernel on behalf of your program.

Your program should contain a for loop that adds numbers from 0 to max, where max is set 
initially to 1000000. 

Your program may use a variable called counter (initialized to 0) to compute 
the sum. 

After the for loop is iterated 1000000 times, the counter is reinitialized to 0 and max is set to 
either 1000000, 2000000, or 3000000, determined using the random number generator library function. 

For example, if the random number generated is 0, then max is set to 1000000, if the random 
number generated is 1, then max is set to 2000000, and so on. The for loop is then restarted using 
maxdetermined above.*/


/*In other words, your program should be interrupted by the SIGALRM signal 
every 2 seconds (i.e.the real time that has elapsed).*/



/*Your program should be able to display the CPU time consumed by the program (in seconds at 
microsecond level) and that by the kernel on behalf of the program (in seconds at microsecond level)
every 2 seconds.*/



/*When this happens, the signal will be trapped to a signal handler that stops the timer, displays to the 
screen the CPU time consumed by the program and that by the kernel on behalf of theprogram, and then 
sets the timer back to 2 seconds. Note that the alarm() and sleep() functions are NOT allowed for this 
program.*/