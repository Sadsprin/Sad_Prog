/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32170533 
*	    Student name : 김민준
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H


/*
 * You need to Declare functions in  here
 */
struct process{
        int arr_time;
        int cpu_time;
        int wait_time;
	char gui[3];
};

void FIFO(struct process **p,int process_num);
void SPN(struct process **p, int process_num);
void RR(struct process **p, int process_num,int time_slice);
void HRRN(struct process **p, int process_num);
void MLFQ(struct process **p, int process_num,int time_q);

struct queue {
        unsigned int back;

        unsigned int front;

        unsigned int size;
        unsigned int capacity;
        int* data;
};
typedef struct queue que;

que* create_queue(unsigned int _capacity);

int queue_enqueue(que* q, int item);
int queue_dequeue(que* q);
unsigned int queue_size(que* q);
void free_queue(que* q);
#endif /* LAB1_HEADER_H*/







