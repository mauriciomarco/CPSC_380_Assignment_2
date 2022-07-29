/*
 * A multithreaded sorting program that 
 * takes a list of integers and divides the
 * list into two equal sub-lists. Each
 * sub-list is sorted in separate threads
 * and merged in a third thread
 * Author: Marco Mauricio
 * Email: mauricio@chapman.edu
 * Course: CPSC 380
 * Assignment: 2
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define LIST_SIZE 24 /* Maximum size of list */

int list[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8}; /* global unsorted array */
//int list[] = {12, 19, 3, 18, 4, 2, 6, 15, 8, 1, 27, 34, 54, 65, 90, 101, 0, 17, 21, 23}; /* global unsorted array - bigger */
int sorted_list[LIST_SIZE]; /* global sorted array */
int list_len = sizeof(list)/ sizeof(list[0]); /* global length of input list */

struct param { /* parameters for threads */
	int begin;
	int end;
	int len;
};

void *thread_sort(void *arg) { /* sorting function */
	
	struct param *args = (struct param *)arg; /* declare structure parameters */
	pthread_t sort_thread = pthread_self(); /* used to ID thread - multiple threads */
	int tmp_sort;
	
//	printf("Enter sort thread: %ld\n", sort_thread);
	
	for (int i = args->begin; i <= args->end; ++i) { /* sorting algorithm */
		for (int j = i + 1; j <= args->end; ++j) {
			if (list[i] > list[j]) {
				tmp_sort = list[i];
				list[i] = list[j];
				list[j] = tmp_sort;
			}
		}
	}
	
//	printf("Half sorted: "); /* check to see sorting progress - should return different values from 1st & 2nd threads */
//	for (int i = 0; i < list_len; i++){
//		printf("%d",list[i]);
//	}
//	printf("\n");
//	printf("Exit sort thread: %ld\n", sort_thread);
}

void *thread_merge(void *arg) { /* merging function */
	
	pthread_t merge_thread = pthread_self(); /* used to ID thread - multiple threads */
	
//	printf("Enter merge thread: %ld\n", merge_thread);
	
	int tmp_merge;

	for (int i = 0; i < list_len; i++) { /* sort two sorted sub lists */
		for (int j = i + 1; j < list_len; j++) {
			if (list[i] > list[j]) {
				tmp_merge = list[i];
				list[i] = list[j];
				list[j] = tmp_merge;
			}
		}
		sorted_list[i] = list[i]; /* copy final list to new sorted_list array */
	}

//	printf("Exit merge thread: %ld\n", merge_thread);
	
}

int main() {
	
	pthread_t thread_1, thread_2, thread_3; /* declare thread object */ 
	
	struct param *sort_thread_1 = malloc(sizeof(struct param)); /* instantiate looping parameters for first half of sorting */
	sort_thread_1->begin = 0;
	sort_thread_1->end = list_len/2 - 1;
	sort_thread_1->len = list_len;
	
	struct param *sort_thread_2 = malloc(sizeof(struct param)); /* instantiate looping parameters for second half of sorting */ 
	sort_thread_2->begin = list_len/2;
	sort_thread_2->end = list_len - 1;
	sort_thread_2->len = list_len;
	
//	printf("Unsorted List: ");
//	for(int i = 0; i < list_len; i++) {
//		printf("%d ", list[i]);
//	}
//	printf("\n");
	
	pthread_create(&thread_1, NULL, thread_sort, sort_thread_1); /* thread that sorts first half */
	pthread_create(&thread_2, NULL, thread_sort, sort_thread_2); /* thread that sorts second half */
	pthread_create(&thread_3, NULL, thread_merge, sort_thread_2); /* thread that merges two halves */
	
	/* waiting for specified thread to terminate */ 
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	pthread_join(thread_3, NULL);
	
	printf("Sorted List: ");
	for(int i = 0; i < list_len; i++) { /* output printing */
		printf("%d", sorted_list[i]);
		if (i < list_len - 1) { /* output formatting */
			printf(", ");
		}
	}
	printf("\n");
	
	pthread_exit(NULL);
	free(sort_thread_1); /* for every malloc have a free */
	free(sort_thread_2);
	return 0;
}