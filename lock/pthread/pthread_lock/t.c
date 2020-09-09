#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t count_lock = PTHREAD_MUTEX_INITIALIZER;

void one() {
	pthread_mutex_lock (&count_lock);
	printf("one time begin\n");
	sleep(6000);
	printf("one time end\n");
	pthread_mutex_unlock (&count_lock);
}

void two(){
	printf("two\n");
	pthread_mutex_lock(&count_lock);
	pthread_mutex_unlock(&count_lock);
	printf("two exit\n");
}

typedef enum {
    NOT_RECV = 0,
    ALREADY_RECV    
} udp_recv_flag_e;

    pthread_t udp_loop_id;
udp_recv_flag_e udp_recv_flag = NOT_RECV;

void udp_loop(void *args)
{
//    GateWay *p = (GateWay *)args;
    while(udp_recv_flag != ALREADY_RECV) {
 //       p->on_scanBtn_clicked();
	printf("xxx\n");
        sleep(1);
    }
}

int main()
{


	pthread_t idone,idtwo;
	int ret;
//	ret=pthread_create(&idone,NULL,(void *)one,NULL);
	if(ret!=0){
		printf ("Create pthread 1 error!/n");
		exit (1);
	}
	sleep(2);
//	ret=pthread_create(&idtwo,NULL,(void *)two,NULL);
	if(ret!=0){
		printf ("Create pthread 2 error!/n");
		exit (1);
	}


	printf(" exec: pthread_join(idone,NULL); \n ");
//	pthread_join(idone,NULL);
//	pthread_join(idtwo,NULL);

	printf("done\n");
char *this = NULL;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	while(1) {
	if (udp_loop_id == 0) {
	    pthread_create(&udp_loop_id, &attr, (void *)udp_loop, this); 
	}
	printf("%ld\n", udp_loop_id);
	}
sleep (10);
	return 0;
}
