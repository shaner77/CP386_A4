/*
 -------------------------------------
 File:    banker.c
 Project: A04Q1S21
 GitHub:  https://github.com/shaner77/CP386_A4
 -------------------------------------
 Author:  Shane Riley
 ID:      170696320
 Email:   rile6320@mylaurier.ca
 Version: 2021-07-30
 -------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

int Resources, Customers;
int *available;
int **allocated;  //currently allocated
int **max;	//maximum demand
int **need;   //max - allocated
int *safe;
int completed = 0;

pthread_mutex_t lock;
pthread_cond_t cond;

// get safe sequence is there is one else return false
bool getSafeSeq();
// process function
void* processCode(void*);

int main(int argc, char** argv) {
	printf("How many Customers: ");
	scanf("%d", &Customers);
	//Resources = argc - 1;
	Resources = (int) argc - 1;
	available = (int *) malloc(Resources * sizeof(*available));

	printf("Currently Available Resources: ");
	for (int i = 0; i < Resources; i++) {
		available[i] = atoi(argv[i + 1]);
		printf("%d ", available[i]);
	}

	allocated = (int **) malloc(Customers * sizeof(*allocated));
	for (int i = 0; i < Customers; i++) {
		allocated[i] = (int *) malloc(Resources * sizeof(**allocated));
	}

	max = (int **) malloc(Customers * sizeof(*max));
	for (int i = 0; i < Customers; i++)
		max[i] = (int *) malloc(Resources * sizeof(**max));


	printf("\nMaximum Resources Required From file: \n");

	FILE* f = fopen("sample4_in.txt", "r");
	if (f == NULL) {
		printf("couldn't open");
		exit(1);
	}

	char line[Resources];
	int i = 0;
	int j = 0;
	int m = 0;
	while (i < Customers * Resources) {
		fgets(line, sizeof(line), f);
		printf("%s", line);
		i += 1;
		if (i == Resources) {
			m = m + 1;
			printf("\n");
			j = 0;
		}
		max[m][j] = *line;

	}

	char op[Resources * 2];
	char cmd;

	need = (int **) malloc(Customers * sizeof(*need));
	for (int i = 0; i < Customers; i++) {
		need[i] = (int *) malloc(Resources * sizeof(**need));
	}
	for (int i = 0; i < Customers; i++) {
		for (int j = 0; j < Resources; j++) {
			need[i][j] = max[i][j] - allocated[i][j];
		}
	}
	// safe sequence
	safe = (int *) malloc(Customers * sizeof(*safe));
	for (int i = 0; i < Customers; i++)
		safe[i] = -1;

	// allocated
	printf("\nEnter Command:");
	scanf("%s", &op);

	printf("Customer: %s\n", op[3]);
	if (op[0] == '*') {
		printf("Available:\nMaximum:\nAllocated:\nNeed:\n");
	} else if (op[0] == 'R' && op[1] == 'Q') {
		printf("User Request %s ", op);
	} else if (op[0] == 'R' && op[1] == 'L') {
		printf("User Release %s ", op);
	} else {
		printf("None: <%s>\n", op);
	}


	if (!getSafeSeq()) {
		printf(
				"\nThis process cannot execute. It leads the system to a unsafe state.\n\n");
		exit(-1);
	}

	printf("\n\nSafe Sequence Found : ");
	for (int i = 0; i < Customers; i++) {
		printf("%d", safe[i] + 1);
	}

	printf("\nExecuting Processes...\n\n");
	sleep(1);

	// run threads
	pthread_t processes[Customers];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	int pcount[Customers];
	for (int i = 0; i < Customers; i++)
		pcount[i] = i;

	for (int i = 0; i < Customers; i++)
		pthread_create(&processes[i], &attr, processCode,
				(void *) (&pcount[i]));

	for (int i = 0; i < Customers; i++)
		pthread_join(processes[i], NULL);

	printf("\nAll Processes Finished\n");

	// free resources
	free(available);
	for (int i = 0; i < Customers; i++) {
		free(allocated[i]);
		free(max[i]);
		free(need[i]);
	}
	free(allocated);
	free(max);
	free(need);
	free(safe);
}

bool getSafeSeq() {
	// get safe sequence
	int temp[Resources];
	for (int i = 0; i < Resources; i++)
		temp[i] = available[i];

	bool finished[Customers];
	for (int i = 0; i < Customers; i++)
		finished[i] = false;
	int fin = 0;
	while (fin < Customers) {
		bool state = false;

		for (int i = 0; i < Customers; i++) {
			if (!finished[i]) {
				bool possible = true;

				for (int j = 0; j < Resources; j++)
					if (need[i][j] > temp[j]) {
						possible = false;
						break;
					}

				if (possible) {
					for (int j = 0; j < Resources; j++)
						temp[j] += allocated[i][j];
					safe[fin] = i;
					finished[i] = true;
					fin += 1;
					state = true;
				}
			}
		}

		if (!state) {
			for (int k = 0; k < Customers; k++)
				safe[k] = -1;
			return false; // no safe sequence found
		}
	}
	return true; // safe sequence found
}

// process code
void* processCode(void *arg) {
	int p = *((int *) arg);

	// lock resources
	pthread_mutex_lock(&lock);

	// condition check
	while (p != safe[completed])
		pthread_cond_wait(&cond, &lock);

	// process
	printf("\n--> Process %d", p + 1);
	printf("\n\tAllocated : ");
	for (int i = 0; i < Resources; i++)
		printf("%3d", allocated[p][i]);

	printf("\n\tNeeded    : ");
	for (int i = 0; i < Resources; i++)
		printf("%3d", need[p][i]);

	printf("\n\tAvailable : ");
	for (int i = 0; i < Resources; i++)
		printf("%3d", available[i]);

	printf("\n");
	sleep(1);

	printf("\tResources Allocated");
	printf("\n");
	printf("\tProcess Code Running...");
	printf("\n");
	printf("\tProcess Code Completed...");
	printf("\n");
	printf("\tProcess Releasing Resource...");
	printf("\n");
	printf("\tResource Released!");

	for (int i = 0; i < Resources; i++)
		available[i] += allocated[p][i];

	printf("\n\tNow Available : ");
	for (int i = 0; i < Resources; i++)
		printf("%3d", available[i]);
	printf("\n\n");

	sleep(1);

	completed++;
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}
