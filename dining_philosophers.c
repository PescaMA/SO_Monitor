#include <pthread.h>
#include <stdlib.h>
#include <stdio.h> 
#include "./monitor.h" /// custom header

const int nr = 5;


void test (Monitor *m, int i) {
	if (monitor_getSharedData(m, (i+nr-1) % nr) != 2 && // if left philosopher isn't eating
		monitor_getSharedData(m, i) == 1 && 			// and current one is hungry
		monitor_getSharedData(m, (i+1) % nr) != 2){		// and right one isn't eating
			
			monitor_setSharedData(m, i, 2); // mark the current one as eating
			monitor_csignal(m, i); // make them stop waiting 
		}
}

void pickup(Monitor* m, int i) {
	
	if (monitor_getSharedData(m, i) == 2) // if philosopher is already eating, return
		return;
		
	monitor_setSharedData(m, i, 1); // mark the philospher as hungry
	test(m, i);  // check if philosopher can eat
	if (monitor_getSharedData(m, i) != 2) // if the philospher isn't eating, wait
		monitor_cwait(m, i);
}

void putdown(Monitor* m, int i) {
	
	if (monitor_getSharedData(m, i) == 0) // if philosopher is already thinking, return
		return;
		
	monitor_setSharedData(m, i, 0); // mark them as thinking
	
	// check if left and right philosphers are hungry, and try to signal them
	
	test(m, (i+nr-1) % nr); 
	test(m, (i+1) % nr);
}

/*
	We'll consider: 
	
	0 - thinking 
	1 - hungry 
	2 - eating 
*/


/// testing:
int main(){
	Monitor m;
	monitor_init(&m, nr, nr);
	
	pthread_t philosophers[nr];
	int ids[nr];

	// Toti filozofii se gandesc initial
	for (int i = 0; i < nr; i++) {
		monitor_setSharedData(&m, i, 0);
	}

	void* philosopher(void* arg) {
		int id = *(int*)arg;

		while (1) {
		    // Gandire
		    int timp_0 = rand() % 5 + 3;
		    printf("Filozoful %d se gandeste la nemurirea sufletului timp de %d secunde.\n", id, timp_0);
		    sleep(timp_0); // Se gandeste intre 3 si 7 secunde

		    // Foamete
		    printf("Filozofulul %d ii e foame.\n", id);
		    pickup(&m, id);

		    // Mancare
		    int timp_2 = rand() % 3 + 1;
		    printf("Filozoful %d mananca timp de %d secunde.\n", id, timp_2);
		    sleep(timp_2); // Mananca intre 1 si 3 secunde

		    // Gata
		    printf("Filozoful %d a terminat de mancat si se gandeste iar la nemurirea sufletului.\n", id);
		    putdown(&m, id);
		}

		return NULL;
	}

	// Threaduri pentru fiecare filozof
	for (int i = 0; i < nr; i++) {
		ids[i] = i;
		pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
	}

	// Unim threadurile
	for (int i = 0; i < nr; i++) {
		pthread_join(philosophers[i], NULL);
	}


	monitor_destroy(&m);
	return 0;
}

