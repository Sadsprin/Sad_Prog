/*
*   DKU Operating System Lab
*           Lab2 (Vehicle Production Problem)
*           Student id : 
*           Student name : 
*
*   lab1_sync_types.h :
*       - lab2 header file.
*       - must contains Vehicle Production Problem's declations.
*
*/

#ifndef _LAB2_HEADER_H
#define _LAB2_HEADER_H

#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#define MAX_SIZE 100

/*
 * You need to Declare functions in  here
 */

	

typedef struct __node_t {
        int car_num;
        struct __node_t *next;
} node_t;

typedef struct __queue_t{
        int balance;
        node_t *head;
        node_t *tail;
        pthread_mutex_t headLock;
        pthread_mutex_t tailLock;
}queue_t;


typedef struct __pro{
       
        int burst;
        int car_num;
        pthread_t pa;
}pro;

typedef struct __cus{
	pthread_t pa;
	int cus_num;
}cus;



#endif /* LAB2_HEADER_H*/


