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
#include <time.h>

int Resources;
int Processes;
int *resources;
int **allocated;
int **max;

pthread_mutex_t lock;
pthread_cond_t cond;

int main(int argc, char** argv) {
	srand(time(NULL));

	printf("How many processes: ");
	scanf("%d", &Processes);

	printf("How many resources: ");
	scanf("%d", &Resources);

	resources = (int *) malloc(Resources * sizeof(*resources));
	printf("Available Resources\n (space separated): ");
	for (int i = 0; i < Resources; i++) {
		scanf("%d", &resources[i]);
	}

	allocated = (int **) malloc(Processes * sizeof(*allocated));
	for (int i = 0; i < Processes; i++) {
		allocated[i] = (int *) malloc(Resources * sizeof(**allocated));
	}
	max = (int **) malloc(Processes * sizeof(*max));
	for (int i = 0; i < Processes; i++) {
		max[i] = (int *) malloc(Resources * sizeof(**max));
	}
}
