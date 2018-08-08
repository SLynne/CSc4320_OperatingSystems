/* 
 * Stephanie Wyckoff
 * CSc 4320 - Operating Systems
 * Programming Assignment 2
 * Due: 13 Feb 18 

 * Assignment: "Design and implement a Sudoku Solution Validator using Pthreads library in Linux virtual machine."
 * 
 * Assignment description taken from Programming Project 1 from Chapter 4 from Operating Systems Concepts 9th Ed, Silberschatz:
 * "A Sudoku puzzle uses a 9 × 9 grid in which each column and row, as well as
each of the nine 3 × 3 subgrids, must contain all of the digits 1 · · · 9. Figure
4.19 presents an example of a valid Sudoku puzzle. This project consists of
designing a multithreaded application that determines whether the solution to
a Sudoku puzzle is valid.
There are several different ways of multithreading this application. One
suggested strategy is to create threads that check the following criteria:
• A thread to check that each column contains the digits 1 through 9
• A thread to check that each row contains the digits 1 through 9
• Nine threads to check that each of the 3 × 3 subgrids contains the digits 1
through 9
This would result in a total of eleven separate threads for validating a
Sudoku puzzle. However, you are welcome to create even more threads for
this project. For example, rather than creating one thread that checks columns, you could create nine separate threads and have each of them check
one column."
 * 
 * Program Description: This program reads in a sudoku board from a text file and incorporates it into a 9x9 matrix. The program 
 * then creates 11 variations of the struct for row and column information. Next the program creates the pthreads and calls the 
 * pthread functions. The functions check if each row, column and 3x3 subgrid (as determined in the struct) is valid (contains
 * all numbers 1-9). If it is valid, the function updates the global array valid to read 1. If any row, column or 3x3 subgrid is not
 * valid then the function updates the global array valid to read 0. Once all threads are finished and joined, a loop iterates through 
 * valid to check for 1s. If all elements are 1s, print out the "is Solved" statement and exit. If there is a 0, program prints out the 
 * "NOT Solved" statement and ends the program. 
 * 
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


int board[9][9]; //The sudoku file is imported into this board
/* You will need to declare an array of integer values that is visible to each thread. The value in the array (0 or 1) indicates whether the worker thread's number is valid (see "Returning Results to the Parent Thread" in the textbook's project description) */
int valid[11]; //When the threads are joined, if that thread is a valid part of a sudoku puzzle, it returns 1 to valid[], else it returns 0
int subgridCt = 2; //used to track through valid array sections for the 3x3 subgrids


/* You will need a structure to store the information to be passed to each thread (see "Passing Parameters to Each Thread" in the textbook's project description)*/
typedef struct{
    
    int row;
    int column;
    
}parameters;

//declare the threads
pthread_t col_thread, row_thread, first_thread, second_thread, third_thread, fourth_thread, fifth_thread, sixth_thread, seventh_thread, eighth_thread, ninth_thread;

/* Declare the thread that checks columns, rows and 3x3 subgrids */
void *column_worker(void *param); 	
void *row_worker(void *param);
void *square_worker(void *param);


int main(int argc, char *argv[])
{
   	int j = 0; //used in for loop to read in file elements to board array
	int i = 0; //used in for loop to read in file elements to board array
        int k = 0; //used in while loop to check valid array
	
        
        //Error checking: if user only enters ./hw2 then gets this error
	if (argc == 1) {
            fprintf(stderr,"Usage: ./hw2 <sudoku grid>\n");

            return -1;
    	}
        
        
        //reading file from agrv[] and portioning the numbers to the sudoku board
	FILE *sudoku; 
	sudoku = fopen(argv[1], "r");

	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			fscanf(sudoku, "%d ", &board[i][j]);
		}
	}		
		    
   
        /*You need to assign values to the structure variable. Then you can create multiple worker threads by passing the information using the structure variable*/
        parameters *checkRows = (parameters *) malloc(sizeof(parameters));
        checkRows->row = 0;
        checkRows->column = 0;
       
        parameters *checkCols = (parameters *) malloc(sizeof(parameters));
        checkCols->row = 0;
        checkCols->column = 0;
       
        parameters *first3by3 = (parameters *) malloc(sizeof(parameters));
        first3by3->row = 0;
        first3by3->column = 0;
        
        parameters *second3by3 = (parameters *) malloc(sizeof(parameters));
        second3by3->row = 0;
        second3by3->column = 3;
     
        parameters *third3by3 = (parameters *) malloc(sizeof(parameters));
        third3by3->row = 0;
        third3by3->column = 6;
      
        parameters *fourth3by3 = (parameters *) malloc(sizeof(parameters));
        fourth3by3->row = 3;
        fourth3by3->column = 0;
       
        parameters *fifth3by3 = (parameters *) malloc(sizeof(parameters));
        fifth3by3->row = 3;
        fifth3by3->column = 3;
       
        parameters *sixth3by3 = (parameters *) malloc(sizeof(parameters));
        sixth3by3->row = 3;
        sixth3by3->column = 6;
       
        parameters *seventh3by3 = (parameters *) malloc(sizeof(parameters));
        seventh3by3->row = 6;
        seventh3by3->column = 0;
    
        parameters *eighth3by3 = (parameters *) malloc(sizeof(parameters));
        eighth3by3->row = 6;
        eighth3by3->column = 3;
      
        parameters *ninth3by3 = (parameters *) malloc(sizeof(parameters));
        ninth3by3->row = 6;
        ninth3by3->column = 6;
        
        
        //return values? this is used for pthread join
        void * rows;
        void * cols;
        void * first_square;
        void * second_square;
        void * third_square;
        void * fourth_square;
        void * fifth_square;
        void * sixth_square;
        void * seventh_square;
        void * eighth_square;
        void * ninth_square;
        
        
        //create the pthreads(pthread_t, pthread_attr (NULL), pthread function, copy of struct for pthread)
        pthread_create(&col_thread, NULL, column_worker, (void *) checkCols);
        pthread_create(&row_thread, NULL, row_worker, (void *) checkRows);
        pthread_create(&first_thread, NULL, square_worker, (void *) first3by3);
        pthread_create(&second_thread, NULL, square_worker, (void *) second3by3);
        pthread_create(&third_thread, NULL, square_worker, (void *) third3by3);
        pthread_create(&fourth_thread, NULL, square_worker, (void *) fourth3by3);
        pthread_create(&fifth_thread, NULL, square_worker, (void *) fifth3by3);
        pthread_create(&sixth_thread, NULL, square_worker, (void *) sixth3by3);
        pthread_create(&seventh_thread, NULL, square_worker, (void *) seventh3by3);
        pthread_create(&eighth_thread, NULL, square_worker, (void *) eighth3by3);
        pthread_create(&ninth_thread, NULL, square_worker, (void *) ninth3by3);
     
        

	/*You need to call pthread_join() for each children thread so that the parent will wait*/
	pthread_join(col_thread, &cols);
        pthread_join(row_thread, &rows);
        pthread_join(first_thread, &first_square);
        pthread_join(second_thread, &second_square);
        pthread_join(third_thread, &third_square);
        pthread_join(fourth_thread, &fourth_square);
        pthread_join(fifth_thread, &fifth_square);
        pthread_join(sixth_thread, &sixth_square);
        pthread_join(seventh_thread, &seventh_square);
        pthread_join(eighth_thread, &eighth_square);
        pthread_join(ninth_thread, &ninth_square);
        
	
        /* Finally, after all children returns, you can check the status array that is visible to everyone and see if it is valid. You then print out the final checking result*/
	while(k < 11){
	
            /*if the thread is valid (contains all numbers 1-9) then the value of valid[k] will be 1, continue to check all elements of valid until 
             either the array ends (k = 11) or the value of valid[k] is 0. If the loop terminates on its own (k = 11), then print the "is Solved" statement and 
             * exit the program. If the value is 0 then there is a thread that did was not a valid part of a sudoku puzzle and the whole puzzle is invalid. Print 
             * the "NOT Solved" statement and end the program.
             */
            
            if(valid[k] == 1){               
		k++;
            }
            else{
                printf("The Sudoku Puzzle is NOT solved. \n");
                exit(0);
            }
            
        }
        printf("The Sudoku Puzzle is solved. \n");
	

	return 0;
}


/*thread code for child checking all columns*/
void *column_worker(void *params)
{
    int i, j; 
    
    parameters * c_worker = (parameters *) params;
    int beginCol = c_worker->column;
    int beginRow = c_worker->row;

    /*sorting sudoku column; this will put the column into a sorted order and check for duplicates
      if there are duplicates, this section is not valid and returns 0 for the column_worker;*/
    
    for(i = beginCol; i < 9; i++){
     
	int col[9]= {0}; //array to hold sorted column
        
        for(j = beginRow; j < 9; j++){
            
            int val = board[i][j]; //this specific value in the column
            
            /*if the coresponding array element for the value is 0, it has not been seen and 
		we will input this value to the sorted column array. */
            
	    if(col[val-1] == 0 && val > 0){
               col[val-1] = val;
		
            }
		/*if the value is not 0, then the value is a duplicate and the sudoku puzzle
		  is not solved or valid so the value of column_worker in valid is 0; */
            else{
		valid[0] = 0;
                pthread_exit(0);
            }
        }
    }   
    valid[0] = 1;
    pthread_exit(0);	       
}


/*thread code for child checking all rows*/
void *row_worker(void *params){

    int i, j; 
    
    parameters * r_worker = (parameters *) params;
    int beginCol = r_worker->column;
    int beginRow = r_worker->row;
    
    /*sorting sudoku row; this will put the row into a sorted order and check for duplicates
      if there are duplicates, this section is not valid and returns 0 for the row_worker;*/
    
    for(i = beginCol; i < 9; i++){
        
        int row[9]= {0};//array to hold sorted row
        
        for(j = beginRow; j < 9; j++){
            
            int val = board[i][j];
            
            /*if the corresponding array element for the value is 0, it has not been seen and 
		we will input this value to the sorted column array. */
            
            if(row[val-1] == 0 && val > 0){
                row[val-1] = val;
            }
            
            /*if the value is not 0, then the value is a duplicate and the sudoku puzzle
		  is not solved or valid so the value of column_worker in valid is 0. */
            
            else{
                valid[1] = 0;
		pthread_exit(0);
            }
        }
    }        
    valid[1] = 1;
    pthread_exit(0);
}


/*thread code for child checking all 3x3 subgrids*/
void *square_worker(void *params){
	
    int i, j;

    parameters * worker = (parameters *) params;
    int beginRow = worker->row;
    int beginCol = worker->column;
    
    /*sorting sudoku subgrids; this will put the 3x3 subgrid, as determined by the beginRow and beginCol parameters
      into a sorted order and check for duplicates. if there are duplicates, this section is not valid and returns 0 for the square_worker;*/
	
    int square[9] = {0}; //1d array to hold sorted 3x3 square

    for (i = beginRow; i < beginRow + 3; ++i) {
        for (j = beginCol; j < beginCol + 3; ++j) {
            
            int val = board[i][j];
            
            /*if the corresponding array element for the value is 0, it has not been seen and 
		we will input this value to the sorted column array. */
	   
            if (square[val-1] == 0 && val > 0) {
                square[val-1] = val;
            }
            
            /*if the value is not 0, then the value is a duplicate and the sudoku puzzle
		  is not solved or valid so the value of square_worker in valid is 0. */
            
            else{
                valid[subgridCt] = 0;
		subgridCt++;
		pthread_exit(0);
            }
        }
    }	
    valid[subgridCt] = 1;
    subgridCt++;
    pthread_exit(0);
}

