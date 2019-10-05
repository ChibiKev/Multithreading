#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define TOTAL 20000000

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
	pthread_exit(0);
}

void *LongestWord(void *arg){
	char **val_p = (char **)arg;
	int maxlength = strlen(val_p[0]);
	int maxindex = 0;
	for(int i = 1; i < TOTAL; i++){
		int length = strlen(val_p[i]);
		if(length > maxlength){
			maxlength = length;
			maxindex = i;
		}
	}
	longest = val_p[maxindex];
	longestchar = maxlength;
	pthread_exit(0);
}

void *ShortestWord(void *arg){
	char **val_p = (char **)arg;
	int minlength = strlen(val_p[0]);
	int minindex = 0;
	for(int i = 1; i < TOTAL; i++){
		int length = strlen(val_p[i]);
		if(length < minlength){
			minlength = length;
			minindex = i;
		}
	}
	shortest = val_p[minindex];
	shortestchar = minlength;
	pthread_exit(0);
}

void *MostCommonWord(void *arg){
	char **val_p = (char **)arg;
	int storage = 0;
	char *arr[100];
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
			if(*arr[j] == *val_p[i]){
				//printf("Count ++ %s\n", val_p[i]);
				count[j]++;
				//printf("storing %s, count %d \n", val_p[i],count[j]);
				found = 1;
				break;
			}
		}
		if (found == 0){
			//printf("Stored: %sTotal Stored: %d\n", val_p[i], storage);
			arr[storage]=val_p[i];
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
	most = arr[maxindex];
	mostchar = count[maxindex];
	pthread_exit(0);
}

int main(){

	pthread_t tid[4];
	//char *test[10] = {"testing","testing","testest","derpy","blgawgaw","aefeawfewa", "fewafa","feafa","feawfaw","efwa"};
	pthread_create(&tid[0], NULL, reading, "file.txt");
	pthread_join(tid[0],NULL);
	pthread_create(&tid[1], NULL, LongestWord, (void*)test);
	pthread_create(&tid[2], NULL, ShortestWord, (void*)test);
	pthread_create(&tid[3], NULL, MostCommonWord, (void*)test);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	pthread_join(tid[3],NULL);
	//for(int i = 0; i < 10; i++){
		//printf("listing: %s \n", test[i]);
	//}
	printf("Longest Word: %s With %d Characters\n", longest, longestchar);
	printf("Shortest Word: %s With %d Characters\n", shortest, shortestchar);
	printf("Most Word: %s Appeared %d Times\n", most, mostchar);
	return 0;
}