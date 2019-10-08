#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define TOTAL 50000000
#define THREADS 1
#define MAXSTORE 20
#define BUFSIZE 15
#define DIVIDED TOTAL/THREADS

bool check(char* first, char* second);
void Mapping(char* arr[]);
void LongestWord(char* arr[]);
void ShortestWord(char* arr[]);
void MostCommonWord(char* arr[]);

char *test[TOTAL]; 
char *longest;
int longestchar;
char *shortest;
int shortestchar;
char *storage[MAXSTORE];
int storagecount[MAXSTORE] = {};
char *most;
int mostchar;

void *reading(void *arg){
   char *val_p = (char *)arg;
   FILE *pointer_file;
   pointer_file = fopen(val_p, "r");
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
   //    printf("%d: %s\n", i, test[i]);
   // }
   fclose(pointer_file);
   // Mapping(test);
   // free(*test);
   // LongestWord(storage);
   // ShortestWord(storage);
   // MostCommonWord(storage);
   pthread_exit(0);
}

bool check(char* first, char* second){
   int firstlen = strlen(first);
   int secondlen = strlen(second);
   int found = 1;
   if (firstlen != secondlen){
      found = 0;
   }
   else{
      for(int i = 0; i < firstlen; i++){
         if(first[i] != second[i]){
            found = 0;
            break;
         }
      }
   }
   return found;
}

void Mapping(char* arr[]){
   int store = 0;
   for(int i = 0; i < TOTAL; i++){
      int found = 0;
      for(int j = 0; j < store; j++){
         //printf("storage = %s , arr = %s \n", storage[j],arr[i]);
         bool value = check(storage[j],arr[i]);
         if(value){
            //printf("Currently in %s with %d stored.\n", arr[i], storagecount[j]);
            storagecount[j]++;
            //printf("storing %s, count %d \n", arr[i],storagecount[j]);
            found = 1;
            free(arr[i]);
            break;
         }
      }
      if (found == 0){
         //printf("Stored: %s Total Stored: %d\n", arr[i], store);
         storage[store]=arr[i];
         storagecount[store]++;
         store++;
      }
   }
   //printf("storage: %d\n", storage);
}

void LongestWord(char* arr[]){
   int maxlength = strlen(arr[0]);
   int maxindex = 0;
   for(int i = 1; i < MAXSTORE; i++){
      if(arr[i] == NULL) break;
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
   for(int i = 1; i < MAXSTORE; i++){
      if(arr[i] == NULL) break;
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
   int max = 0;
   int maxindex = 0;
   for(int i = 0; i < MAXSTORE; i++){
      if(arr[i] == NULL) break;
      if(storagecount[i] > max){
         max = storagecount[i];
         maxindex = i;
         //printf("Index Max = %d, count = %d\n", i, count[i]);
      }
   }
   //printf("Most: %s", arr[maxindex]);
   most = storage[maxindex];
   mostchar = storagecount[maxindex];
}

int main(){
   pthread_t fileread;
   pthread_t tid[4];
   //char *test[10] = {"testing","testing","testing","derpy","blgawgaw","aefeawfewa", "fewafa","feafa","feawfaw","efwa"};
   pthread_create(&fileread, NULL, reading, "file.txt");
   pthread_join(fileread, NULL);
   // for(int i = 0; i < THREADS; i++){
   //    pthread_create(&tid[i], NULL, , test);
   // }
   // for(int i = 0; i < THREADS; i++){
   //    pthread_join(tid[i],NULL);
   // }
   Mapping(test);
   LongestWord(storage);
   ShortestWord(storage);
   MostCommonWord(storage);
   // for(int i = 0; i < TOTAL; i++){
   //    if(test[i] == NULL) break;
   //    printf("listing: %s \n", test[i]);
   // }

   for(int i = 0; i < MAXSTORE; i++){
      if(storage[i] == NULL) break;
      printf("In storage: %s with counts of %d\n", storage[i], storagecount[i]);
   }
   printf("Divided %d \n",DIVIDED);
   printf("Longest Word: %s With %d Characters\n", longest, longestchar);
   printf("Shortest Word: %s With %d Characters\n", shortest, shortestchar);
   printf("Most Word: %s Appeared %d Times\n", most, mostchar);
   return 0;
}