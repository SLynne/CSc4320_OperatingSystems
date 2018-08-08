/* 
 * Stephanie Wyckoff
 * CSc 4320 - Operating Systems
 * Programming Assignment 3
 * Due: 10 Mar 18 

 * Assignment: "Design and implement a solution for Producer–Consumer Problem using Pthreads library in Linux virtual machine."
 * 
 * Assignment description taken from Programming Project 3, from Chapter 5, from Operating Systems Concepts 9th Ed, Silberschatz:
 * "In Section 5.7.1, we presented a semaphore-based solution to the producer–
 * consumer problem using a bounded buffer. In this project, you will design a
 * programming solution to the bounded-buffer problem using the producer and
 * consumer processes shown in Figures 5.9 and 5.10. The solution presented in
 * Section 5.7.1 uses three semaphores: empty and full, which count the number
 * of empty and full slots in the buffer, and mutex, which is a binary (or mutual exclusion)
 * semaphore that protects the actual insertion or removal of items
 * in the buffer. For this project, you will use standard counting semaphores for
 * empty and full and a mutex lock, rather than a binary semaphore, to represent
 * mutex. The producer and consumer—running as separate threads—will move
 * items to and from a buffer that is synchronized with the empty, full, and mutex
 * structures. You can solve this problem using either Pthreads or the Windows API."
 * 
 * Program Description: this program takes in user input defining the number of seconds the program should sleep, the
 * number of producer threads and the number of consumer threads to be created. After error checking for 4 valid inputs, 
 * the file name and 3 integers all greater than 1, the program begins in earnest. First the program will create the 
 * producer threads. Each producer thread will call the producer function which will create a random number to be fed
 * to the insert_item function to be inserted into the buffer. If the item is inserted successfully, then successful will
 * be set equal to zero and return this value to the producer function, count will incriment. When a zero is returned to the
 * producer function from the insert_item function, the program will print a message indicating the item produced. This will 
 * continue until buffer is full, as indicated by count == buffer size. Then the program will generate the consumer threads 
 * and each consumer thread will call the consumer function. The consumer function then calls the remove_item fucntion providing 
 * the full buffer and removing one item from the buffer. If the removal is successful, then successful is set to 0
 * and the value is returned to the consumer, count will decriment. When a zero is returned to the consumer function from the 
 * remove_item function, the program will print a message indicating the item consumed. This will continue until buffer is empty,
 * as indicated by count == 0.
 * 
*/

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex; //declares Pthread mutex lock
sem_t empty; //declares counting semaphore
sem_t full; //declares counting semaphore
int insert; //tracks the element number for item insertion to buffer
int removal; //tracks the element number for item removal from buffer
int count; //counter from 0 to buffer size

/* values provided by user */
    int sleep_time;
    int produced_number;
    int consumed_number;

void *producer(void *param); 
void *consumer(void *param);
int insert_item(buffer_item item);
int remove_item(buffer_item *item);



int main(int argc, char *argv[])
{
    
    
    /* Get command line arguments argv[1],argv[2],argv[3], if 4 arguments are not provided, display error and quit*/
    if (argc != 4) {
            
        fprintf(stderr,"Usage: ./hw3 <sleep time> <# of producer threads> <# of consumer threads>\n");

            return -1;
    }
    else{
       /* if the arguments provided are not file name, int, int, int, display error and quit */
        if(atoi(argv[1])<2 || atoi(argv[2])<2 || atoi(argv[3])<2){
            
            fprintf(stderr,"<sleep time>, <# of producer threads>, and <# of consumer threads> values should be greater than 1\n");

            return -1;
        }
        else{
            
            /* set user defined values */
            sleep_time = atoi(argv[1]);
            produced_number = atoi(argv[2]);
	    consumed_number = atoi(argv[3]);
            
            /* Initialize buffer related synchronization tools */

            int i=0; //for for loop
            int j=0; //for for loop
            srand(time(NULL));
            
            /* Silberschatz Operating System Concepts pg 253*/
            pthread_mutex_init(&mutex, NULL);
            
            /* Create the semaphores and initialize to buffer size and 0, respectively ... Silberschatz Operating System Concepts pg 262*/
            sem_init(&empty, 0, BUFFER_SIZE);
            sem_init(&full, 0, 0);
            
            /* initiallize counters to zero */
            count=0;
            insert=0;
            removal=0;

            /* Create producer threads based on the command line input using for loop to ensure that the 
             * correct number of threads is produced
             */
            pthread_t producer_thread[produced_number];
            
            for(i = 0; i < produced_number; i++){
                pthread_create(&producer_thread[i], NULL, producer, NULL);
            }
            
            /* Create consumer threads based on the command line input using for loop to ensure that the 
             * correct number of threads is produced
             */
            pthread_t consumer_thread[consumed_number];
            
            for(j = 0; j < consumed_number; j++){
                pthread_create(&consumer_thread[j], NULL, consumer, NULL);
            }

            /* Sleep for user specified time based on the command line input */
            sleep(sleep_time);

            return 0;
        }
    } 
}

void *producer(void *param)
{
    /*  Silberschatz Operating System Concepts pg 279 "The Producer and Consumer Threads":
     * "The producer thread will alternate between sleeping for a random period of
     * time and inserting a random integer into the buffer. Random numbers will be produced using 
     * the rand() function, which produces random integers between 0 and RAND MAX."
     */
    
    buffer_item item;
    
    while(1){
        sleep(rand() % 8 + 1); // Sleep randomly between 1 and 5 seconds

        item = rand(); 
        
        if(insert_item(item)){
            
            printf("Error........\n");
        }
        else{
            
            printf("Producer %u produced %d \n",(unsigned int)pthread_self(), produced_number);
        }
    }

}

void *consumer(void *param)
{
    /*Silberschatz Operating System Concepts pg 279 "The Producer and Consumer Threads":
     *"The consumer will also sleep for a random period of time and, upon 
     *awakening, will attempt to remove an item from the buffer."
     */
    
    buffer_item item;
    
    while(1){
        sleep(rand() % 8 + 1);  // Sleep randomly between 1 and 5 seconds
   
        if(remove_item(&item)){
            
            printf("Error.......\n");
        }
        else{
            
            printf("Consumer %u consumed %d \n",(unsigned int)pthread_self(), consumed_number);
        }    
    }
}

int insert_item(buffer_item item)
{
    /* insert item into buffer:
     * return 0 if successful, otherwise 
     * return -1 indicating an error condition 
     */
    
    int successful=0;
    
    /* Acquire Empty Semaphore ... Silberschatz Operating System Concepts pg 262 */
    sem_wait(&empty);

    /* Acquire mutex lock to protect buffer ... Silberschatz Operating System Concepts pg 277 */
    pthread_mutex_lock(&mutex);

    /* Insert item into buffer */
    if(count!=BUFFER_SIZE)
    {
        buffer[insert]=item;
        insert++;
        count++;
        successful=0;
    }
    else
    {
        successful=-1;
    }
    
    /* Release mutex lock and full semaphore ... Silberschatz Operating System Concepts pg 277 & 262 */
    pthread_mutex_unlock(&mutex);
    sem_post(&full);

    return successful;
}

int remove_item(buffer_item *item)
{
    /* remove an object from buffer placing it in item:
     * return 0 if successful, otherwise
     * return -1 indicating an error condition 
     */
    
    int successful;
    
    /* Acquire Full Semaphore ... Silberschatz Operating System Concepts pg 262 */
     sem_wait(&full);

    /* Acquire mutex lock to protect buffer ... Silberschatz Operating System Concepts pg 277*/
    pthread_mutex_lock(&mutex);

    /* remove an object from buffer placing it in item */
    if(count!=0)
    {
        *item=buffer[removal];
        removal++;
        count--;
        successful=0;
    }
    else
    {
        successful=-1;
    }

    /* Release mutex lock and empty semaphore ... Silberschatz Operating System Concepts pg 277 & 262 */
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    return successful;
}
