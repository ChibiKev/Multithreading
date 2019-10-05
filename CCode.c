#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define TOTAL 10

void LongestWord(char* arr[]);
void ShortestWord(char* arr[]);
void MostCommonWord(char* arr[]);

char *test[TOTAL];
char *longest;
int longestchar;
char *shortest;
int shortestchar;
char *most;
int mostchar;

void *reading(void *arg){
	char *val_p = (char *)arg;
	FILE *pointer_file;
	pointer_file = fopen(val_p, "r");
	int BUFSIZE = 100;
	int i = 0;
	test[i] = malloc(BUFSIZE);
	while(fgets(test[i], BUFSIZE,pointer_file) != NULL){
		strtok(test[i],"\n");
		i++;
		test[i] = malloc(BUFSIZE);
		if (i == TOTAL){
			break;
		}
	}
	// for(int i = 0; i < 10; i++){
	// 	printf("%d: %s\n", i, test[i]);
	// }
	fclose(pointer_file);
	LongestWord(test);
	ShortestWord(test);
	MostCommonWord(test);
	pthread_exit(0);
}

void LongestWord(char* arr[]){
	int maxlength = strlen(arr[0]);
	int maxindex = 0;
	for(int i = 1; i < TOTAL; i++){
		int length = strlen(arr[i]);
		if(length > maxlength){
			maxlength = length;
			maxindex = i;
		}
	}
	longest = arr[maxindex];
	longestchar = maxlength;
}

void ShortestWord(char* arr[]){
	int minlength = strlen(arr[0]);
	int minindex = 0;
	for(int i = 1; i < TOTAL; i++){
		int length = strlen(arr[i]);
		if(length < minlength){
			minlength = length;
			minindex = i;
		}
	}
	shortest = arr[minindex];
	shortestchar = minlength;
}

void MostCommonWord(char* arr[]){
	int storage = 0;
	char *arr2[100];
	int count[100] = {};
	for(int i = 0; i < TOTAL; i++){
		int found = 0;
		//printf("testing: %s.... \n", val_p[i]);
		for(int j = 0; j < storage; j++){
			// char first = *arr[j];
			// char second = *val_p[i];
			// printf("i = %d, j = %d \nArray: %s\nValue: %s\n",i,j,arr[j],val_p[i]);
			// if(first == second){
			// 	printf("true");
			// }
			// else{
			// 	printf("False: %s != %s\n", arr[j],val_p[i]);
			// }
			if(*arr2[j] == *arr[i]){
				//printf("Count ++ %s\n", val_p[i]);
				count[j]++;
				//printf("storing %s, count %d \n", val_p[i],count[j]);
				found = 1;
				break;
			}
		}
		if (found == 0){
			//printf("Stored: %sTotal Stored: %d\n", val_p[i], storage);
			arr2[storage]=arr[i];
			count[storage]++;
			storage++;
		}
	}
	//printf("storage: %d\n", storage);
	int max = 0;
	int maxindex = 0;
	for(int i = 0; i < storage; i++){
		if(count[i] > max){
			max = count[i];
			maxindex = i;
			//printf("Index Max = %d, count = %d\n", i, count[i]);
		}
	}
	//printf("Most: %s", arr[maxindex]);
	most = arr2[maxindex];
	mostchar = count[maxindex];
}

int main(){

	pthread_t tid[4];
	//char *test[10] = {"testing","testing","testest","derpy","blgawgaw","aefeawfewa", "fewafa","feafa","feawfaw","efwa"};
	for(int i = 0; i < 4; i++){
		pthread_create(&tid[i], NULL, reading, "file.txt");
	}
	for(int i = 0; i < 4; i++){
		pthread_join(tid[i],NULL);
	}
	for(int i = 0; i < TOTAL; i++){
		printf("listing: %s \n", test[i]);
	}
	printf("Longest Word: %s With %d Characters\n", longest, longestchar);
	printf("Shortest Word: %s With %d Characters\n", shortest, shortestchar);
	printf("Most Word: %s Appeared %d Times\n", most, mostchar);
	return 0;
}