/* 
 * Stephanie Wyckoff
 * CSc 4320 - Operating Systems
 * Programing Assignment 1
 * Due: 4 Feb 18 

 * Assignment: "Write a C program that meets the requirement in Programming Problem 3.21 in the textbook.
 * In addition, your program should also display the process IDs of both the parent and child process. 
 * Follow the code in Figure 3.34 to print the process IDs."
 * 
 * Programing Problem 3.21 from Operating Systems Concepts 9th Ed, Silberschatz: 
 * "The collatz conjecture concerns what happens when we take any positive integer n and apply 
 * the following algorithm: (n=n/2, if n is even, n=3*n=1, if n is odd). The conjecture states that
 * when this algorithm is continually applied, all positive integers will eventually reach 1... 
 * Write a C program using the fork() system call that generates this sequence in the child 
 * process. The starting number will be provided from the command line... Because the parent and
 * child processes have their own copies of the data, it will be necessary for the child to 
 * output the sequence. Have the parent invoke the wait() call to wait for the child process
 * to complete before exiting the program. Perform necessary error checking to ensure that a 
 * positive integer is passed on the command line." 
 * 
 * Program Description: This program takes in a string and converts it to an integer value. If the value 
 * is not 1, then the value is checked to see if it is even. If it is even, program will divide 
 * value by 2. The value is updated and printed to the screen. If the value is not even, the else 
 * clause is implemented and the value is multiplied by 3 and added to 1. The value is updated and 
 * printed to the screen. The program will continue these calculations until the value = 1. The 
 * program will print the pid and pid1 values after the calculations are finished and printed. 
 * Finally the child process will terminate and the parent process will resume and the program will
 * print the pid and pid1 of the parent process. Note: child pid will be zero because this causes the
 * the fork()... child pid1 will be equal to parent pid because the child pid1 is a copy of the
 * parent... I am not sure why the parent pid1 is one less than parent pid and child pid1.
 * 
 * #, #, #, ...
 * child: pid = 0
 * child: pid1 = num  
 * parent: pid = num
 * parent: pid1 = num-1
 * 
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> //included to make wait() work
#include <stdlib.h> //included to make exit() work


//Begin code provided from assignment. 
int main(int argc, char *argv[])
{
    int n;
    pid_t pid, pid1;


    //My Comment: If 1 is entered before ./hw1, return error because ./hw1 is the first parameter
    if (argc == 1) {
            fprintf(stderr,"Usage: ./hw1 <starting value>\n");

            return -1;
    }
    
    //My Comment: The second parameter for this program is a positive integer, this is assigned to n
    n = atoi(argv[1]); //  n is the input starting value
    
//End code provided by assignment.
    
//Begin code and comments written by me 

    //If n is not a positive integer value, print this error message
    if(n < 0){
        fprintf(stderr,"Enter positive integer value.\n");
	exit(0);
    }


    /* fork a child process */
    pid = fork();//This line of code provided by example 3.34 from book

    
  
    //If in child process, run this while loop and print out n til n=1 
    if(pid == 0){   
        
	//Print n before any computations are done on n
        printf("%d, ", n);  
        
        //If n != 1, enter loop
        while(n != 1){

            //If n is even, do n/2, update n with calculated result and print result to screen
            if(n % 2 == 0){
                
                n = n/2;
                printf("%d, ", n);
            }
            //If n is odd, do 3*n+1, update n with calculated result and print result to screen
            else{

                n = 3*n+1;
                printf("%d, ", n);
            }
            //Repeat loop until n=1
        }
        //When loop completes, n = 1, all calculations of n are printed in a line
        //Insert line break after n = 1 and 1 is printed to screen
        printf("\n"); 
    }    
//End code and comments written by me    

//Begin code from example 3.34 from Operating Systems Concepts 9th Ed, Silberschatz

    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    //My Comment: prints at the end of child process. Line breaks added by me
    else if (pid == 0){
        /* child process */
        pid1 = getpid();
        printf("child: pid=%d\n",pid); /* A */
        printf("child: pid1=%d\n",pid1); /* B */
    }
    //My Comment: prints at the end of parent process process. Line breaks added by me
    else{
        /* parent process */
        /*My Comment: 'wait(NULL)' line was moved from the end of the else clause. This was due to 
         * a print formatting error. If 'wait(NULL)' is at the end of the clause, then the output
         * is " 'parent pid...' 'calculations of n' 'child pid...' 'child pid1...' 'parent pid1...' "
        */
        wait(NULL); 
        pid1 = getpid();
        printf("parent: pid=%d\n",pid); /* C */
        printf("parent: pid1=%d\n",pid1); /* D */
    }
	
return 0;

        

}
