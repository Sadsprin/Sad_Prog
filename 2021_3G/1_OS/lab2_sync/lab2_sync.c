/*
*   DKU Operating System Lab
*           Lab2 (Vehicle production Problem)
*           Student id : 32170533 
*           Student name : 김민준
*
*   lab2_sync.c :
*       - lab2 main file.
*       - must contains Vehicle production Problem function's declations.
*
*/

#include <aio.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>
#include <stdbool.h>

#include "lab2_sync_types.h"

/*
 * you need to implement Vehicle production Problem. 
 */
int turn = 1;
int time_quantum =0;
int current_total_car = 0;
int total_car = 0;
int count =0;

pthread_cond_t empty, fill;
pthread_mutex_t mutex;
queue_t *queue;




void queue_init(queue_t *q){
        node_t *tmp = malloc(sizeof(node_t *));
        tmp->next = NULL;
        q->head = q->tail = tmp;
        q->balance = 0;
        pthread_mutex_init(&q->headLock,NULL);
        pthread_mutex_init(&q->tailLock,NULL);
}
void queue_enqueue(queue_t *q, int car_num){
	
        node_t *tmp = malloc(sizeof(node_t));
        assert(tmp != NULL);
        tmp->car_num = car_num;
        tmp->next = NULL;

        pthread_mutex_lock(&q->tailLock);
        q->tail->next = tmp;
        q->tail = tmp;
        pthread_mutex_unlock(&q->tailLock);
	q->balance++;
}
int queue_dequeue(queue_t *q){

        int car_num;
        pthread_mutex_lock(&q->headLock);
        node_t *tmp = q->head;
        node_t *newHead = tmp->next;
        if(newHead == NULL){
                pthread_mutex_unlock(&q->headLock);
                exit(-1);
        }
        car_num = newHead->car_num;
        q->head = newHead;
        pthread_mutex_unlock(&q->headLock);
        free(tmp);
	q->balance--;
        return car_num;
}

void next_turn_RR(int next){
	next = (next+1) % 5;
	if(next ==0) next = 1;

	turn = next;
}

void *producer_f(void *arg){
	pro *data = (pro *) arg;
	while(data->burst > 0){
		
		if(count == 5){
			next_turn_RR(data->car_num);
			count = 0;
		}
		count++;
		
		while(turn == data->car_num){
			printf("checking\n");	  
			pthread_mutex_lock(&mutex);
			
				if(data->burst > time_quantum){
					for(int i =0; i<time_quantum;i++){
						while(false) pthread_cond_wait(&empty,&mutex);	
						queue_enqueue(queue,data->car_num);
						pthread_cond_signal(&fill);
					
					
					}
					current_total_car += time_quantum;
					data->burst -= time_quantum;
				}
				else{
					for(int i =0; i<data->burst;i++){
						while(false) pthread_cond_wait(&empty,&mutex);
						queue_enqueue(queue,data->car_num);
						pthread_cond_signal(&fill);
					

					}
					current_total_car += data->burst;
					data->burst = 0;
				}
				next_turn_RR(data->car_num);
				count = 0;	
			pthread_mutex_unlock(&mutex);
				
			}
		}
	}
	


void *customer_f(void *arg){
	
	cus *data = (cus *)arg;
	printf("cus_num : %d\n",data->cus_num);
	while(current_total_car < total_car ){
		
		if(queue->head->next != NULL){
		while(data->cus_num == queue->head->next->car_num){
				
			pthread_mutex_lock(&mutex);
			while(queue->balance ==0)
				pthread_cond_wait(&fill,&mutex);
			
			queue_dequeue(queue);
			

			pthread_cond_signal(&empty);
			pthread_mutex_unlock(&mutex);
		}
		
	}
	}
	if(current_total_car >= total_car) pthread_exit(0);


}
void lab2_sync_usage(char *cmd) {
	printf("\n Usage for %s : \n",cmd);
    printf("    -c: Total number of vehicles produced, must be bigger than 0 ( e.g. 100 )\n");
    printf("    -q: RoundRobin Time Quantum, must be bigger than 0 ( e.g. 1, 4 ) \n");
}

void lab2_sync_example(char *cmd) {
	printf("\n Example : \n");
    printf("    #sudo %s -c=100 -q=1 \n", cmd);
    printf("    #sudo %s -c=10000 -q=4 \n", cmd);
}

int main(int argc, char* argv[]) {
	char op;
	int n; char junk;
	struct timeval start, end;
	double result_T;

	queue = malloc(sizeof(queue_t *));
	pro *producer[5];
	cus *customer[5];
	for(int i = 0; i < 5; i++){
		producer[i] = malloc(sizeof(pro));
	}

	for(int i = 0; i < 5; i++){
               	customer[i] = malloc(sizeof(cus));
        }
	printf("생산라인을 구성합니다. 각 라인의 생산 수의 합은 argv[1]에서 입력했던 총 생산 수랑 같아야합니다.\n");
	for(int i = 0; i < 5; i++){
		printf("%d번 생산라인의 생산 수를 입력해주세요.\n",i+1);
		producer[i]->car_num = i+1;
		customer[i]->cus_num = i+1;
		scanf(" %d",&producer[i]->burst);
	}
	queue_init(queue);

	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&fill,NULL);
	pthread_cond_init(&empty,NULL);

	if (argc <= 1) {
		lab2_sync_usage(argv[0]);
		lab2_sync_example(argv[0]);
		exit(0);
	}
	
	printf("===========================================================\n");
	printf("한 생산 라인에서 차는 1초에 1대씩 만들어진다고 가정합니다.\n");
	printf("또한 생산라인을 각각 A,B,C,D,E라 칭하고, 소비자는 각각 5명입니다.\n");
	printf("arrival time은 모든 0초로 같습니다.\n");
	printf("===========================================================\n");
	for (int i = 1; i < argc; i++) {
		if (sscanf(argv[i], "-c=%d%c", &n, &junk) == 1) {
			total_car = n;
		}
		else if (sscanf(argv[i], "-q=%d%c", &n, &junk) == 1) {
			time_quantum = n;
		}
		else {
			lab2_sync_usage(argv[0]);
			lab2_sync_example(argv[0]);
			exit(0);
		}
	}


	/*
	   * You need to implement Vehicle production Problem 
	   * If you want to use other header file, it's ok!
	   * If you have any question for this task, please contact me (ghwls03s@gmail.com)
	   * You may be need to use pthread_create function
	*/
	
	gettimeofday(&start,NULL);
	for(int i =0 ; i < 5 ; i++){
		pthread_create(&producer[i]->pa,NULL,producer_f,(void *)producer[i]);
		usleep(100);
	}
	for(int i =0 ; i < 5 ; i++){
                pthread_create(&customer[i]->pa,NULL,customer_f,(void *)customer[i]);
		usleep(100);
        }

	printf("HI\n");
	for(int i =0 ; i < 5 ; i++){
                pthread_join(producer[i]->pa,NULL);
        }
        for(int i =0 ; i < 5 ; i++){
                pthread_join(customer[i]->pa,NULL);
        }
	gettimeofday(&end,NULL);
	printf("HI\n");
	result_T = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)/1000000);

	printf("==== Vehicle production problem ====\n");
	printf("(1) No Lock Experiment\n");
	printf("	Total produce Number =\n");
	printf("	Final Balance Value =\n");
	printf("	Execution time =\n");
	printf("==== Vehicle production problem ====\n");
        printf("(2) Coarse-grained Lock Experiment \n");
        printf("        Total produce Number = %d\n",current_total_car);
        printf("        Final Balance Value = %d\n",queue->balance);
        printf("        Execution time =%lf\n",result_T);
        printf("==== Vehicle production problem ====\n");
        printf("(3) fine-grained Lock Experiment\n");
        printf("        Total produce Number =\n");
        printf("        Final Balance Value =\n");
        printf("        Execution time =\n");

	for(int i = 0 ; i< 5; i++){
		free(producer[i]);
		free(customer[i]);
	}
	return 0;
}
