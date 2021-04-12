/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 김민준
*	    Student name : 32170533
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
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

#include "lab1_sched_types.h"

/*
 * you need to implement scheduler simlator test code.
 *
 */
void test_1(struct process **p){
                p[0]->arr_time =0;
                p[0]->cpu_time =3;
		p[1]->arr_time =2;
                p[1]->cpu_time =6;
		p[2]->arr_time =4;
                p[2]->cpu_time =4;
		p[3]->arr_time =6;
                p[3]->cpu_time =5;
		p[4]->arr_time =8;
                p[4]->cpu_time =2;
}
void test_2(struct process **p){
	        p[0]->arr_time =0;
                p[0]->cpu_time =4;
                p[1]->arr_time =3;
                p[1]->cpu_time =7;
                p[2]->arr_time =5;
                p[2]->cpu_time =6;
                p[3]->arr_time =7;
                p[3]->cpu_time =4;
                p[4]->arr_time =9;
                p[4]->cpu_time =2;
}

int main(int argc, char *argv[]){



	int select;
	struct process *p[5];

	for(int i=0; i < 5;i++){
		p[i] = malloc(sizeof(struct process));
	}
	printf("please select 1 or 2\n");
	printf("workload is ordered by arrival time and operation time.\n"); 
	printf("[1] p.24 workload\n");
	printf("1th. 0 3 | 2th. 2 6 | 3th. 4 4 | 4th. 6 5 | 5th. 8 2\n");
	printf("[2] custom workload\n");
	printf("1th. 0 4 | 2th. 3 7 | 3th. 5 6 | 4th. 7 4 | 5th. 9 2\n");
	printf("click 1 or 2\n");
	scanf("%d",&select);






	
	if(select ==1){
		test_1(p);
		FCFS(p,5);
		test_1(p);
		RR(p,5,1);
		test_1(p);
		RR(p,5,4);
		test_1(p);
		SPN(p,5);
		test_1(p);
		HRRN(p,5);
		test_1(p);
		printf("p.24 방식의 MLFQ 구현은 실패했습니다. 무조건 선점형 MLFQ입니다.\n");
		MLFQ(p,5,1);
		test_1(p);
		MLFQ(p,5,2);
	}
	else if(select ==2){
		test_2(p);
                FCFS(p,5);
                test_2(p);
                RR(p,5,1);
		test_2(p);
		RR(p,5,4);
                test_2(p);
                SPN(p,5);
                test_2(p);
                HRRN(p,5);
                test_2(p);
                MLFQ(p,5,1);
                test_2(p);
                MLFQ(p,5,2);

	}

	
	for(int i=0;i<5;i++){
		free(p[i]);
	}

	return 0;

	
}

