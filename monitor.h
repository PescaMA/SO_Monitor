#include <pthread.h>
#include <stdlib.h> /// for free
#include <stdio.h> /// printf

/// simply linked list implementation:
typedef struct Node{
	void* data;
	struct Node* next;
} Node;
Node *createNode(void* data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}
Node* deleteNode(Node* node){
	if(node == NULL)
		return NULL;
	Node* nextNode = node->next;
	free(node);
	return node = nextNode;
}
void addNodeAtEnd(Node* node, void* data){
	Node* new_node = createNode(data);
	if (node == NULL) {
		node = new_node;
		return;
	}
	Node* temp = node;
	while (temp->next != NULL) temp = temp->next;
	temp->next = new_node;
}

typedef int monitor_condition;
typedef int monitor_data;

/// THE MONITOR STRUCT
typedef struct Monitor {
	Node* entryQueue; /// nici nu am folosit lol
	pthread_mutex_t mutex;
	int condition_count;
	monitor_condition* condition;
	int shared_data_count;
	monitor_data* shared_data;
} Monitor;

void monitor_init(Monitor* monitor, int shared_data_count,int condition_count) {
	monitor->entryQueue = NULL;
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
	monitor->condition = (int*)malloc(condition_count * sizeof(monitor_condition));

	for(int i=0; i < condition_count; i++)
		monitor->condition[i] = 1; /// all conditions are initially available.

}

void monitor_destroy(Monitor* monitor) {
	pthread_mutex_destroy(&monitor->mutex);
	free(monitor->shared_data);
	free(monitor->condition);
}

int monitor_setSharedData(Monitor* monitor, int i, monitor_data value){
	if(i<0 || i > monitor->shared_data_count){
		printf("Shared data index outside of bounds.\n");
		exit(1);
	}
	pthread_mutex_lock(&(monitor->mutex));
	monitor->shared_data[i] = value;
	pthread_mutex_unlock(&(monitor->mutex));
}

int monitor_getSharedData(Monitor* monitor, int i){
	if(i<0 || i > monitor->shared_data_count){
		printf("Shared data index outside of bounds.\n");
		exit(1);
	}
	pthread_mutex_lock(&(monitor->mutex));
	return monitor->shared_data[i];
	pthread_mutex_unlock(&(monitor->mutex));

}

void* monitor_runFunction(Monitor* monitor, void* f(void*), void* a){
	pthread_mutex_lock(&(monitor->mutex));
	void*  result = f(a);
	pthread_mutex_unlock(&(monitor->mutex));
	return result;
}

/// wait on condition
void monitor_cwait(Monitor* monitor, int condition_index) {
    if (condition_index < 0 || condition_index >= monitor->condition_count) {
        printf("Index de conditie invalid.\n");
        exit(1);
    }

		//  pthread_mutex_lock(&monitor->mutex);
		/// trebuie ceva facut cu entryQueue?
		/// addNodeAtEnd(monitor->entryQueue,(void*)pthread_self()); // Store thread ID
    // pthread_mutex_unlock(&monitor->mutex);

    while (monitor->condition[condition_index] == 0) {
        sched_yield(); // Let other threads run (puts itself on the back of cpu thread queue).
    }

}

/// signal condition finished
void monitor_csignal(Monitor* monitor, int condition_index) {
    if (condition_index < 0 || condition_index >= monitor->condition_count) {
				printf("Index de conditie invalid.\n");
        exit(1);
    }

    pthread_mutex_lock(&monitor->mutex);

		monitor->condition[condition_index] = 1;

		/// trebuie ceva facut cu entryQueue?
		/// monitor->entryQueue = deleteNode(monitor->entryQueue); // Remove one thread from the entryQueue

    pthread_mutex_unlock(&monitor->mutex);
}


