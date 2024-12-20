#include <pthread.h>
#include <stdlib.h> /// for free
#include <stdio.h> /// printf
#include <stdbool.h> /// bool

typedef int monitor_data;

/// THE MONITOR STRUCT
typedef struct Monitor {
	pthread_mutex_t mutex;
	int condition_count;
	pthread_mutex_t* condition;
	int shared_data_count;
	monitor_data* shared_data;
} Monitor;

void monitor_init(Monitor* monitor, int shared_data_count,int condition_count) {
	pthread_mutex_init(&monitor->mutex, NULL);

	if(shared_data_count < 0){
		printf("Cannot have less than zero shared data.\n");
		exit(1);
	}
	monitor->shared_data_count = shared_data_count;
	monitor->shared_data = (int*)malloc(shared_data_count * sizeof(monitor_data));

	if(condition_count < 0){
		printf("Cannot have less than zero condition variables.\n");
		exit(1);
	}
	monitor->condition_count = condition_count;
	monitor->condition = (pthread_mutex_t*)malloc(condition_count * sizeof(pthread_mutex_t));

	for(int i=0; i < condition_count; i++)
		pthread_mutex_init(&monitor->condition[i], NULL);
	
}

void monitor_destroy(Monitor* monitor) {
	pthread_mutex_destroy(&monitor->mutex);
	free(monitor->shared_data);
	
	for (int i = 0; i < monitor->condition_count; i++)
		pthread_mutex_destroy(&monitor->condition[i]);
		
	free(monitor->condition);
}

int monitor_setSharedData(Monitor* monitor, int i, monitor_data value){

    pthread_mutex_lock(&monitor->mutex);
	bool invalid = i<0 || i > monitor->shared_data_count;
    pthread_mutex_unlock(&monitor->mutex);

	if(invalid){
		printf("Shared data index outside of bounds.\n");
		exit(1);
	}
	
	pthread_mutex_lock(&(monitor->mutex));
	monitor->shared_data[i] = value;
	pthread_mutex_unlock(&(monitor->mutex));
}

int monitor_getSharedData(Monitor* monitor, int i){


    pthread_mutex_lock(&monitor->mutex);
	bool invalid = i<0 || i > monitor->shared_data_count;
    pthread_mutex_unlock(&monitor->mutex);

	if(invalid){
		printf("Shared data index outside of bounds.\n");
		exit(1);
	}
	
	pthread_mutex_lock(&(monitor->mutex));
	monitor_data d = monitor->shared_data[i];
	pthread_mutex_unlock(&(monitor->mutex));
	
	return d;
}


// useless?
void* monitor_runFunction(Monitor* monitor, void* f(void*), void* a){
	pthread_mutex_lock(&(monitor->mutex));
	void*  result = f(a);
	pthread_mutex_unlock(&(monitor->mutex));
	return result;
}

void monitor_cwait(Monitor* monitor, int condition_index) {

    pthread_mutex_lock(&monitor->mutex);
	bool invalid = condition_index < 0 || condition_index >= monitor->condition_count;
    pthread_mutex_unlock(&monitor->mutex);
	

    if (invalid) {
        printf("Index de conditie invalid.\n");
        exit(1);
    }

    pthread_mutex_lock(&monitor->condition[condition_index]);
}

/// signal condition finished
void monitor_csignal(Monitor* monitor, int condition_index) {

    pthread_mutex_lock(&monitor->mutex);
	bool invalid = condition_index < 0 || condition_index >= monitor->condition_count;
    pthread_mutex_unlock(&monitor->mutex);
	

    if (invalid) {
		printf("Index de conditie invalid.\n");
        exit(1);
    }

    pthread_mutex_unlock(&monitor->condition[condition_index]);
}


