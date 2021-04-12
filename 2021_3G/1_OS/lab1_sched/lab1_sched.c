/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32170533
*	    Student name : 김민준
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
*
*/
#include <stdbool.h>
#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>
#include <math.h>

#include "lab1_sched_types.h"

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler.
 *
 *
 */


que* create_queue(unsigned int _capacity) {
	que* s_queue = NULL;
	s_queue = (que*)malloc(sizeof(que));
	s_queue->data = (int*)malloc(_capacity* sizeof(int));
	s_queue->capacity = _capacity;
	s_queue->back = -1;
	s_queue->front = 0;
	s_queue->size = 0;
	return s_queue;
}



int queue_enqueue(que *q, int item) {
	int position = (q->back + 1) % q->capacity;
	if (q->size < q->capacity)
	{
		q->back = position;
		q->data[q->back] = item;
		q->size++;
		return 0;
	}
	else
	{
		return -1;
	}
}

int queue_dequeue(que *q) {
	int position = (q->front+ 1) % q->capacity;
	if (q->size>0)
	{
		int front_element = q->data[q->front];
		q->front = position;
		q->size--;
		return front_element;
	}
	else
	{
		return -1;
	}
}


unsigned int queue_size(que* q) {
	return q->size;
}


void free_queue(que* q) {
	
	
	free(q->data);
	free(q);
}

void FCFS(struct process **p,int process_num){
	printf("\n");
	printf("FCFS Algorithm\n");
	int locking=0;
	int sumtime=0;
	int j = 0;
	int gui_value=0;
	for(int i =0; i<process_num; i++){
		sumtime += p[i]->cpu_time;
	}
	que* q1 = create_queue(sumtime);
	        printf("가로가 process_num 이고 세로가 현재 시간입니다.\n");

	        printf("         1    2    3    4    5\n");
                printf("===========================================\n");


	for(int current_t=0;current_t<sumtime;current_t++){
		
		
		for(int j =0; j<process_num; j++){
			if(current_t == p[j]->arr_time) queue_enqueue(q1,j);	
		}	
		
		if(locking == 0){
			if(queue_size(q1)==0){
			       	gui_value = process_num+1;
				sumtime += 1;
			}	
			else{
				int sche_value = queue_dequeue(q1);
				
				gui_value = sche_value;
				locking= p[sche_value]->cpu_time;
				locking -= 1;
				}
		}
		else{
		locking--;
		}
		for(int ks = 0; ks <process_num;ks++){
			if(ks == gui_value){
				strcpy(p[ks]->gui,"I");
			}
			else if(ks != gui_value){
				strcpy(p[ks]->gui," ");
			}
		}
		
	

		printf("%2d      |%s|  |%s|  |%s|  |%s|  |%s|\n",current_t,p[0]->gui,p[1]->gui,p[2]->gui,p[3]->gui,p[4]->gui);
		
		
	}
	
	free_queue(q1);
	printf("\n");
}

void RR(struct process **p, int process_num,int time_slice){
	printf("\nRR Algorithm q = %d \n",time_slice);
	int locking =0;	
	int sumtime = 0;
	bool abs;
	int gui_value =0;
	for(int i =0; i<process_num; i++){
                sumtime += p[i]->cpu_time;
        }
        que* q1 = create_queue(sumtime);
	        printf("         1    2    3    4    5\n");

                printf("===========================================\n");

				
	for(int current_t =0; current_t < sumtime;current_t++){

		for(int j = 0; j< process_num; j++){
			if(current_t == p[j]->arr_time){
			       	int a[10] = {0,};
				queue_enqueue(q1,j);

				if(queue_size(q1) > 1 && abs){
					int qs = queue_size(q1);
					for(int k = 0; k<qs; k++){
						a[k] = queue_dequeue(q1);				
					}
					int temp = 0;
					temp = a[qs-1];
			       		a[qs-1] = a[qs-2];
					a[qs-2] = temp;
				
					for(int s = 0; s<qs; s++){
						queue_enqueue(q1,a[s]);	
				}
			}
		}
	}

	

		if(locking ==0){
			if(queue_size(q1) ==0){
				gui_value = process_num+1;
				sumtime++;
			}
			else if(queue_size(q1)>0){
				int sche_value = queue_dequeue(q1);
				if(p[sche_value]->cpu_time > time_slice){
				
					locking = time_slice;
					locking--;
					p[sche_value]->cpu_time -= time_slice;
					queue_enqueue(q1,sche_value);
					abs =true;
					gui_value=sche_value;
				}
				else if(p[sche_value]->cpu_time <=  time_slice){
					locking = p[sche_value]->cpu_time;
					locking--;
					p[sche_value]->cpu_time = 0;
					abs = false;
					gui_value=sche_value;
				
				}
			}	
		}
		else{
		locking--;
		}
		        for(int ks = 0; ks <process_num;ks++){
                        if(ks == gui_value){
                                strcpy(p[ks]->gui,"I");
                        }
                        else if(ks != gui_value){
                                strcpy(p[ks]->gui," ");
                        }
                }



                printf("%2d      |%s|  |%s|  |%s|  |%s|  |%s|\n",current_t,p[0]->gui,p[1]->gui,p[2]->gui,p[3]->gui,p[4]->gui);

	}
	free_queue(q1);
	printf("\n");
}
void SPN(struct process **p, int process_num){
	printf("\nSPN Algorithm\n");
	int locking =0;
	int sumtime =0;	
	int gui_value = 0; 
        for(int i =0; i<process_num; i++){
                sumtime += p[i]->cpu_time;
        }
	que* q1 = create_queue(sumtime);

                printf("         1    2    3    4    5\n");

                printf("===========================================\n");

	for(int current_t =0; current_t<sumtime; current_t++){
		
		for(int j =0; j<process_num;j++){
			if( current_t == p[j]->arr_time) queue_enqueue(q1,j);
		}
	
	
	if(locking ==0){

		if(queue_size(q1) ==0){
			gui_value = process_num+1;
			sumtime++;
		}
		else if(queue_size(q1) ==1){
			int sche_value = queue_dequeue(q1);
			locking = p[sche_value]->cpu_time;
			locking--;
			gui_value = sche_value;
		}
		else if(queue_size(q1) > 1){
			int min =50000;
			int min_idx =0;
			int qs = queue_size(q1);
			int a[50] ={0,};
			for(int z = 0; z<qs; z++){
				a[z] = queue_dequeue(q1);

				if(p[a[z]]->cpu_time < min){
				       min = p[a[z]]->cpu_time;
				       min_idx = a[z];	
				}
			}


			locking = p[min_idx]->cpu_time;
			locking--;
			gui_value = min_idx;

			for(int k = 0; k < qs; k++){
				if(a[k] != min_idx) queue_enqueue(q1,a[k]);
			}
		}
	}
	
	else{
	locking--;
	}
	                for(int ks = 0; ks <process_num;ks++){
                        if(ks == gui_value){
                                strcpy(p[ks]->gui,"I");
                        }
                        else if(ks != gui_value){
                                strcpy(p[ks]->gui," ");
                        }
                }



                printf("%2d      |%s|  |%s|  |%s|  |%s|  |%s|\n",current_t,p[0]->gui,p[1]->gui,p[2]->gui,p[3]->gui,p[4]->gui);

	}

	free_queue(q1);
	printf("\n");
}


void HRRN(struct process **p, int process_num){
        printf("\nHRRN Algorithm\n");
	int locking =0;
        int sumtime =0;
	int gui_value = 0;
        for(int i =0; i<process_num; i++){
                sumtime += p[i]->cpu_time;
        }
        que* q1 = create_queue(sumtime);
                printf("         1    2    3    4    5\n");

                printf("===========================================\n");

        for(int current_t =0; current_t<sumtime; current_t++){

                for(int j =0; j<process_num;j++){
                        if( current_t == p[j]->arr_time) queue_enqueue(q1,j);
                }

	
        if(locking ==0){

                if(queue_size(q1) ==0){
			gui_value = process_num+1;
                        sumtime++;
                }
                else if(queue_size(q1) ==1){
                        int sche_value = queue_dequeue(q1);
                        locking = p[sche_value]->cpu_time;
                        locking--;
			gui_value = sche_value;
                }
                else if(queue_size(q1) > 1){
                        float response_ratio = 0;
                        int res_idx =0;
                        int qs = queue_size(q1);
			for(int s = 0; s<process_num;s++){
				if(current_t <  p[s]->arr_time){
					p[s]->wait_time = 0; 
				}
				else if(current_t >= p[s]->arr_time){
	
					p[s]->wait_time = current_t - p[s]->arr_time;

				}	
			}
                        int a[50] ={0,};
                        for(int z = 0; z<qs; z++){
                                a[z] = queue_dequeue(q1);
				float res_ratio = (p[a[z]]->wait_time + p[a[z]]->cpu_time)/ p[a[z]]->cpu_time;
                                if( res_ratio  > response_ratio){
                                       response_ratio = res_ratio;
                                       res_idx = a[z];
                                }
                        }

                        locking = p[res_idx]->cpu_time;
                        locking--;
			gui_value = res_idx;

                        for(int k = 0; k < qs; k++){
                                if(a[k] != res_idx) queue_enqueue(q1,a[k]);
                        }
                }
        }

        else{
        locking--;
        }
	       for(int ks = 0; ks <process_num;ks++){
                        if(ks == gui_value){
                                strcpy(p[ks]->gui,"I");
                        }
                        else if(ks != gui_value){
                                strcpy(p[ks]->gui," ");
                        }
                }



                printf("%2d      |%s|  |%s|  |%s|  |%s|  |%s|\n",current_t,p[0]->gui,p[1]->gui,p[2]->gui,p[3]->gui,p[4]->gui);

        }
        free_queue(q1);
	printf("\n");
}

void MLFQ(struct process **p, int process_num, int time_q){
        
	printf("\nMLFQ Algorithm q = %d^i\n",time_q);
	int locking =0;
        int sumtime =0;
	int sche_value =0;
	int time_slice =0;
	int gui_value =0;
        for(int i =0; i<process_num; i++){
                sumtime += p[i]->cpu_time;
        }
        que* q1 = create_queue(sumtime);
	que* q2 = create_queue(sumtime);
	que* q3 = create_queue(sumtime);
                printf("         1    2    3    4    5\n");
                printf("===========================================\n");

	for(int current_t =0; current_t < sumtime; current_t++){

		for(int s = 0; s < process_num ; s++){
			if(p[s]->arr_time == current_t){
			       	queue_enqueue(q1,s);
			}
			
		}
			
		if(locking == 0){
			
			if(queue_size(q1)!=0){
				sche_value = queue_dequeue(q1);
				if(p[sche_value]->cpu_time >0){
					locking = 1;
					locking --;
					p[sche_value]->cpu_time -= 1;
					gui_value = sche_value;
					if(p[sche_value]->cpu_time != 0)
					{
							queue_enqueue(q2,sche_value);
					}

				}
			}
			else if(queue_size(q2)!=0){
				sche_value =queue_dequeue(q2);
				time_slice = time_q;	
		 	        if(p[sche_value]->cpu_time >time_slice){
                                        locking = time_slice;
                                        locking --;
					gui_value = sche_value;
                                        p[sche_value]->cpu_time -= time_slice;
					queue_enqueue(q3,sche_value);
			


                                }
				else if(p[sche_value]->cpu_time <= time_slice){
                                        locking = p[sche_value]->cpu_time;
                                        locking --;
					gui_value = sche_value;
                                        p[sche_value]->cpu_time = 0;

				}

			}
			else if(queue_size(q3)!=0){
                                sche_value =queue_dequeue(q3);
                                time_slice = time_q * time_q;        
                                if(p[sche_value]->cpu_time >time_slice){
                                        locking = time_slice;
                                        locking--;
                                        p[sche_value]->cpu_time -= time_slice;
                                        queue_enqueue(q3,sche_value);
					gui_value = sche_value;

                                }
                                else if(p[sche_value]->cpu_time <= time_slice){
                                        locking = p[sche_value]->cpu_time;
                                        locking --;
                                        p[sche_value]->cpu_time = 0;
					gui_value = sche_value;
			
                                }

			}
			else if(queue_size(q3)==0){
				gui_value = process_num+1;
				sumtime++;
			}


		}
		else{
			locking--;
		}

				
                        for(int ks = 0; ks <process_num;ks++){
                        if(ks == gui_value){
                                strcpy(p[ks]->gui,"I");
                        }
                        else if(ks != gui_value){
                                strcpy(p[ks]->gui," ");
                        }
                	}

			

              printf("%2d      |%s|  |%s|  |%s|  |%s|  |%s|\n",current_t,p[0]->gui,p[1]->gui,p[2]->gui,p[3]->gui,p[4]->gui);

	}


	free_queue(q1);
	free_queue(q2);
	free_queue(q3);
	printf("\n");
}
