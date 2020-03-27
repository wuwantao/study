#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t count_lock = PTHREAD_MUTEX_INITIALIZER;

void one() {
	pthread_mutex_lock (&count_lock);
	printf("one time begin\n");
	printf("one time end\n");
}

void two(){
	pthread_mutex_unlock(&count_lock);
	printf("two exit\n");
}

void three(){
	pthread_mutex_lock (&count_lock);
	printf("three begin\n");
	pthread_mutex_unlock(&count_lock);
	printf("three exit\n");
}

int main()
{


	pthread_t idone,idtwo,idthree;
	int ret;
	ret=pthread_create(&idone,NULL,(void *)one,NULL);
	if(ret!=0){
		printf ("Create pthread 1 error!/n");
		exit (1);
	}
	sleep(2);
	ret=pthread_create(&idtwo,NULL,(void *)two,NULL);
	if(ret!=0){
		printf ("Create pthread 2 error!/n");
		exit (1);
	}
	sleep(2);
	ret=pthread_create(&idthree,NULL,(void *)three,NULL);
	if(ret!=0){
		printf ("Create pthread 3 error!/n");
		exit (1);
	}
	sleep(1);
	printf(" exec: pthread_join(idone,NULL); \n ");
	pthread_join(idone,NULL);
	pthread_join(idtwo,NULL);
	pthread_join(idthree,NULL);

	printf("done\n");
	return 0;
}
