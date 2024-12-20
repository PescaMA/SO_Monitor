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
	
	
	// do stuff
	
	
	monitor_destroy(&m);
	return 0;
}


