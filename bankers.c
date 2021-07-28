/*
-------------------------------------
File:    bankers.c
Project: CP386_A4
file description
-------------------------------------
Author:  Shane Riley
ID:      170696320
Email:   rile6320@mylaurier.ca
Version  2021-07-26
-------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

int res = 0;
int Resources = 0;
int Customers;
int *resources;
int **allocated;
//int **max;
int **need;
int *safe;

pthread_mutex_t lock;
pthread_cond_t cond;

bool getSafeSeq(){
	return True;
}

int main(int argc, char *argv[]) {

	printf("How many Customers: ");
	scanf("%d", &Customers);
	//Resources = argc - 1;
	res = (int) argc - 1;
	printf("Currently Available Resources: ");

	resources = (int *) malloc(res * sizeof(*resources));

	for (int i = 0; i < res; i++) {
		resources[i] = atoi(argv[i + 1]);
		printf("%d ", resources[i]);
	}
	printf("(%d)\n", res);
	//
	/*
	allocated = (int **) malloc(Customers * sizeof(*allocated));
	for (int i = 0; i < Customers; i++) {
		allocated[i] = (int *) malloc(Resources * sizeof(**allocated));
	}
	max = (int **) malloc(Customers * sizeof(*max));
	for (int i = 0; i < Customers; i++) {
		max[i] = (int *) malloc(Resources * sizeof(**max));
	 } */
	printf("\nMaximum Resources Required From file: \n");
	//for (int i = 0; i < Customers; i++) {
	//	printf("\nResources Required (P%d)\n (Space Separated): ", i + 1);
	//	for (int j = 0; j < Resources; j++)
	//		scanf("%d", &allocated[i][j]);
	//}
	char *pt = NULL;
	//char* pt = (char*) malloc(res * sizeof(char));
	size_t len = 0;
	ssize_t size;
	int count = 0;
	FILE* f = fopen("sample4_in.txt", "r");
	if (f == NULL) {
		printf("couldn't open");
		exit(1);
	}
	char lin[sizeof(int) * res * 2];
	char max[Customers][res];
	//size = getline(&pt, &len, f);
	//printf("size %d\n", Customers);
	for (int i = 0; i < Customers; i++) {
		if (fgets(lin, sizeof(lin), f) != NULL) {
			//fputs(("%d\n", lin), stdout);
			int j = 0;
			int m = 0;
			int f = 1;
			int ind = 0;

			//for (int i = 0; i < Customers; i++) {
			while (j < sizeof(lin)) {

				if (lin[j] != ',') {
					//max[i][m] = atoi(&lin[j]);
					//printf("max[%d][%d]%d\n", i, m, max[i][m]);
					m += 1;
				}
				else {
					lin[j] = ' ';
				}

				j += 1;
			}
			printf("%s", lin);
		}
		//printf("\n");
	}
	fclose(f);
	need = (int **) malloc(Customers * sizeof(*need));
	for (int i = 0; i < Customers; i++) {
		need[i] = (int *) malloc(Resources * sizeof(**need));
	}
	for (int i = 0; i < Customers; i++) {
		for (int j = 0; j < Resources; j++) {
			need[i][j] = max[i][j] - allocated[i][j];
		}
	}
	safe = (int *) malloc(Customers * sizeof(*safe));
	for (int i = 0; i < Customers; i++)
		safe[i] = -1;

	if (!getSafeSeq()) {
		printf("\nUnsafe State!\n");
		exit(-1);
	}

	printf("\n\nSafe Sequence Found : ");
	for (int i = 0; i < Customers; i++) {
		printf("%-3d", safe[i] + 1);
	}
}
