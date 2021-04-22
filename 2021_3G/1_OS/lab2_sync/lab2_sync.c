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
int thread_n;
bool checking[10];
bool e_checking = true;

pthread_cond_t empty, fill;
pthread_mutex_t mutex,cus_lock,tailLock,headLock;
queue_t *queue;




void queue_init(queue_t *q){
        node_t *tmp = malloc(sizeof(node_t *));
        tmp->next = NULL;
        q->head = q->tail = tmp;
        q->balance = 0;
	pthread_mutex_init(&headLock,NULL);
	pthread_mutex_init(&tailLock,NULL);
	
}
void queue_enqueue(queue_t *q, int car_num){
	
        node_t *tmp = malloc(sizeof(node_t));
        assert(tmp != NULL);
        tmp->car_num = car_num;
        tmp->next = NULL;

	pthread_mutex_lock(&tailLock);        
        q->tail->next = tmp;
        q->tail = tmp;
	pthread_mutex_unlock(&tailLock);
	
        
	q->balance++;
}
int queue_dequeue(queue_t *q){

        pthread_mutex_lock(&headLock);
        
        node_t *tmp = q->head;
        node_t *newHead = tmp->next;
	if(newHead ==NULL){
		pthread_mutex_unlock(&headLock);
		return -1;
	}
        
        q->head = newHead;
	pthread_mutex_unlock(&headLock);
        free(tmp);
	q->balance--;
	return 0;
        
}

void clean_queue(queue_t *q){
	while(q->head->next != NULL){
		queue_dequeue(q);
	}
}
int RR()
{
pthread_mutex_lock(&cus_lock);
if(turn +1 != thread_n){
       turn =(turn+1) % thread_n;
}
else if(turn+1 ==thread_n){
	turn = thread_n;
}
pthread_mutex_unlock(&cus_lock);
return turn;

}
void *producer_c(void *arg){
	pro *data = (pro *) arg;
	while(data->burst > 0){
		
			pthread_mutex_lock(&mutex);
                if(turn == data->car_num){ 
		
			for(int i =0; i < time_quantum; i++){	
                    	while(queue->balance >= 10) pthread_cond_wait(&empty,&mutex);
			
				queue_enqueue(queue,data->car_num);
	
				data->burst--;
				if(data->burst ==0){
				       	checking[data->car_num-1] = false;
				}

				current_total_car++;
				
				pthread_cond_signal(&fill);
				if(data->burst ==0) break;
                		}

				turn = RR();
			        while(checking[turn-1] == false) {
                                        turn = RR();
					
                                 	if(thread_n ==5 && !checking[0] && !checking[1] && !checking[2] && !checking[3] && !checking[4]){
                                                e_checking=false;
                                                break;
                                        }
					else if(thread_n ==2 && !checking[0] && !checking[1]){
						e_checking = false;
						break;
					}
					else if(thread_n ==10 && !checking[0] &&!checking[1] &&!checking[2]&&!checking[3]&&!checking[4]&&!checking[5]&&!checking[6]&&!checking[7]&&!checking[8]&&!checking[9]){
						e_checking = false;
						break;
					}
                                }
		}
		
	pthread_mutex_unlock(&mutex);
	}
	if(e_checking ==false) pthread_cond_broadcast(&fill);
}
void *producer_nl(void *arg){
        pro *data = (pro *) arg;
        while(data->burst > 0){

                if(turn == data->car_num){

                        for(int i =0; i < time_quantum; i++){
                        while(queue->balance >= 10) pthread_cond_wait(&empty,&mutex);

                                queue_enqueue(queue,data->car_num);

                                data->burst--;
                                if(data->burst ==0){
                                        checking[data->car_num-1] = false;
                                }

                                current_total_car++;

                                pthread_cond_signal(&fill);
                                if(data->burst ==0) break;
                                }

                                turn = RR();
                                while(checking[turn-1] == false) {
                                        turn = RR();
					if(thread_n ==5 && !checking[0] && !checking[1] && !checking[2] && !checking[3] && !checking[4]){
                                                e_checking=false;
                                                break;
                                        }
                                        else if(thread_n ==2 && !checking[0] && !checking[1]){
                                                e_checking = false;
                                                break;
                                        }
                                        else if(thread_n ==10 && !checking[0] &&!checking[1] &&!checking[2]&&!checking[3]&&!checking[4]&&!checking[5]&&!checking[6]&&!checking[7]&&!checking[8]&&!checking[9]){
                                                e_checking = false;
                                                break;
                                        }

                                }
                }

        }
        if(e_checking ==false) pthread_cond_broadcast(&fill);
}

void *producer_f(void *arg){
        pro *data = (pro *) arg;
        while(data->burst > 0){
		
		
                if(turn == data->car_num){
                        for(int i =0; i < time_quantum; i++){

				pthread_mutex_lock(&mutex);
                        	while(queue->balance >= 10) pthread_cond_wait(&empty,&mutex);
                                queue_enqueue(queue,data->car_num);

                                data->burst--;
				pthread_cond_signal(&fill);
				pthread_mutex_unlock(&mutex);
                                if(data->burst ==0){
                                        checking[data->car_num-1] = false;
                                }
				
                                current_total_car++;

                                
                                if(data->burst ==0) break;
                                }
		
				
				pthread_mutex_lock(&mutex);
                                turn = RR();
                                while(checking[turn-1] == false) {
                                        turn = RR();
                                        if(thread_n ==5 && !checking[0] && !checking[1] && !checking[2] && !checking[3] && !checking[4]){
                                                e_checking=false;
                                                break;
                                        }
                                        else if(thread_n ==2 && !checking[0] && !checking[1]){
                                                e_checking = false;
                                                break;
                                        }
                                        else if(thread_n ==10 && !checking[0] &&!checking[1] &&!checking[2]&&!checking[3]&&!checking[4]&&!checking[5]&&!checking[6]&&!checking[7]&&!checking[8]&&!checking[9]){
                                                e_checking = false;
                                                break;
                                        }

                                }
				pthread_mutex_unlock(&mutex);
                }

        }
        if(e_checking ==false) pthread_cond_broadcast(&fill);
}
void *customer_f(void *arg){
	cus *data = (cus *)arg;
	while(e_checking){
	
		
		pthread_mutex_lock(&mutex);
        	while(queue->head->next == NULL){	
			
	       		pthread_cond_wait(&fill,&mutex);
			if(e_checking == false){
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}
		}
		
		if(data->cus_num == queue->head->next->car_num){
			
			queue_dequeue(queue);
		
			pthread_cond_signal(&empty);
		
		}
		
		pthread_mutex_unlock(&mutex);
	}
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
void setting(pro **p){
	
	for(int i =0; i < 10; i++){
		checking[i] = true;
	}
	e_checking = true;
	turn = 1;
	current_total_car = 0;
	if(total_car ==100 && thread_n==2){ p[0]->burst = 40; p[1]->burst = 60;}
	else if(total_car == 100 && thread_n ==5){p[0]->burst=14;p[1]->burst = 18;p[2]->burst = 26; p[3]->burst = 30; p[4]->burst=12;}
	else if(total_car == 100 && thread_n ==10){p[0]->burst = 4; p[1]->burst = 8; p[2]->burst = 12; p[3]->burst = 16; p[4]->burst = 10; p[5]->burst= 8; p[6]->burst = 12; p[7]->burst = 8; p[8]->burst = 10; p[9]->burst = 12;}
	else if(total_car ==1000 && thread_n==2){ p[0]->burst = 400; p[1]->burst = 600;}
        else if(total_car == 1000 && thread_n ==5){p[0]->burst=140;p[1]->burst = 180;p[2]->burst = 260; p[3]->burst = 300; p[4]->burst=120;}
        else if(total_car == 1000 && thread_n ==10){p[0]->burst = 40; p[1]->burst = 80; p[2]->burst = 120; p[3]->burst = 160; p[4]->burst = 100; p[5]->burst= 80; p[6]->burst = 120; p[7]->burst = 80; p[8]->burst = 100; p[9]->burst = 120;}
	else if(total_car ==10000 && thread_n==2){ p[0]->burst = 4000; p[1]->burst = 6000;}
        else if(total_car ==10000 && thread_n ==5){p[0]->burst=1400;p[1]->burst = 1800;p[2]->burst = 2600; p[3]->burst = 3000; p[4]->burst=1200;}
        else if(total_car ==10000 && thread_n ==10){p[0]->burst = 400; p[1]->burst = 800; p[2]->burst = 1200; p[3]->burst = 1600; p[4]->burst = 1000; p[5]->burst= 800; p[6]->burst = 1200; p[7]->burst = 800; p[8]->burst = 1000; p[9]->burst = 1200;}
	else{
		printf("스레드의 수는 2, 5, 10만 입력가능합니다.\n");
		exit(0);
	}



}

int main(int argc, char* argv[]) {
	char op;
	int n; 
	char junk;
	struct timeval start, end;
	double result_T;
	

	printf("차량 종류의 수를 입력해 주세요.(문제는 5종류이며 2, 10종류도 넣어봤습니다.\n");
	scanf("%d",&thread_n);
	queue = malloc(sizeof(queue_t *));
	pro *producer[thread_n];
	cus *customer[thread_n];

	for(int i = 0; i < thread_n; i++){
		producer[i] = malloc(sizeof(pro *));
	}

	for(int i = 0; i < thread_n; i++){
              	customer[i] = malloc(sizeof(cus *));
        }

	queue_init(queue);

	for(int i = 0; i < thread_n; i++){
		producer[i]->car_num = i+1;
		customer[i]->cus_num = i+1;	
	}

	pthread_mutex_init(&mutex,NULL);
	pthread_mutex_init(&cus_lock,NULL);
	pthread_cond_init(&fill,NULL);
	pthread_cond_init(&empty,NULL);

	if (argc <= 1) {
		lab2_sync_usage(argv[0]);
		lab2_sync_example(argv[0]);
		exit(0);
	}
	
	printf("*ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ*\n");
	printf("|차량 종류는 2,5,10개 선택 가능합니다.                                                         |\n");
	printf("|생산라인의 생산수                                                                             |\n");
	printf("|total_car는 100, 1000, 10000대의 경우가 있습니다.                                             |\n");
	printf("|생산라인의 개수는 소비자의 수와 같습니다.                                                     |\n");
	printf("|생산라인의 수는 2, 5, 10의 경우가 있습니다.                                                   |\n");
	printf("|각 생산라인의 생산 수(100개 기준) 1000개(100개 기준 * 10) 10000개(100개 기준 * 100)           |\n");
	printf("|차종이   2개일 때 => 1: 4 , 2: 6.                                                             |\n");
	printf("|차종이   5개일 때 => 1: 14, 2: 18, 3: 26, 4: 30, 5: 12                                        |\n");
	printf("|차종이  10개일 때 => 1: 4, 2: 8, 3: 12, 4: 16, 5: 10, 6: 8, 7: 12, 8: 8, 9: 10, 10: 12        |\n"); 
	printf("|arrival time은 모두 0초로 같습니다.                                                           |\n");
	printf("*ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ*\n");

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
	
	setting(producer);
	/*
	   * You need to implement Vehicle production Problem 
	   * If you want to use other header file, it's ok!
	   * If you have any question for this task, please contact me (ghwls03s@gmail.com)
	   * You may be need to use pthread_create function
	*/

	gettimeofday(&start,NULL);
	for(int i =0 ; i < thread_n ; i++){
		pthread_create(&producer[i]->pa,NULL,producer_c,(void *)producer[i]);

	}
	for(int i =0 ; i < thread_n ; i++){
                pthread_create(&customer[i]->pa,NULL,customer_f,(void *)customer[i]);
	
        }
	
	for(int i =0 ; i < thread_n ; i++){
                pthread_join(producer[i]->pa,NULL);
        }
        for(int i =0 ; i < thread_n ; i++){
                pthread_join(customer[i]->pa,NULL);
        }
	
	gettimeofday(&end,NULL);
	result_T = ((end.tv_sec - start.tv_sec)*1000) + ((end.tv_usec - start.tv_usec)/1000);

	printf("==== Vehicle production problem ====\n");
	printf("(1) Coarse-grained Lock Experiment\n");
	printf("	Total produce Number = %d\n",current_total_car);
	printf("	Final Balance Value = %d\n",queue->balance);
	printf("	Execution time = %.lfms\n",result_T);
	clean_queue(queue);
	setting(producer);
	gettimeofday(&start,NULL);
        for(int i =0 ; i < thread_n ; i++){
                pthread_create(&producer[i]->pa,NULL,producer_f,(void *)producer[i]);

        }
        for(int i =0 ; i < thread_n ; i++){
                pthread_create(&customer[i]->pa,NULL,customer_f,(void *)customer[i]);

        }
	
        for(int i =0 ; i < thread_n ; i++){
                pthread_join(producer[i]->pa,NULL);
        }
        for(int i =0 ; i < thread_n ; i++){
                pthread_join(customer[i]->pa,NULL);
        }
	
        gettimeofday(&end,NULL);
        result_T = ((end.tv_sec - start.tv_sec)*1000) + ((end.tv_usec - start.tv_usec)/1000);



	printf("==== Vehicle production problem ====\n");
        printf("(2) fine-grained Lock Experiment \n");
        printf("        Total produce Number = %d\n",current_total_car);
        printf("        Final Balance Value = %d\n",queue->balance);
 	printf("        Execution time = %.lfms\n",result_T);
        printf("==== Vehicle production problem ====\n");
        printf("(3) No Lock Experiment (생산자에만 No Lock을 적용했습니다./ 락이 없으면 대부분 오류가 발생하기 때문에 주석처리 했습니다.)\n");
/* No Lock experiment code
	clean_queue(queue);
	setting(producer);
        
	gettimeofday(&start,NULL);
	for(int i =0 ; i < thread_n ; i++){
               pthread_create(&producer[i]->pa,NULL,producer_nl,(void *)producer[i]);

        }
        for(int i =0 ; i < thread_n ; i++){
                pthread_create(&customer[i]->pa,NULL,customer_f,(void *)customer[i]);

        }
	
        for(int i =0 ; i < thread_n ; i++){
                pthread_join(producer[i]->pa,NULL);
        }
	
        for(int i =0 ; i < thread_n ; i++){
                pthread_join(customer[i]->pa,NULL);
        }
	
        gettimeofday(&end,NULL);
        result_T = ((end.tv_sec - start.tv_sec)*1000) + ((end.tv_usec - start.tv_usec)/1000);


        printf("        Total produce Number = %d\n",current_total_car);
        printf("        Final Balance Value = %d\n",queue->balance);
        printf("        Execution time = %.lfms\n",result_T);
	*/
	for(int i = 0 ; i< thread_n; i++){
		free(producer[i]);
		free(customer[i]);
	}
	free(queue);
	return 0;
}
