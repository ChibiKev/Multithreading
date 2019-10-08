#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define TOTAL 50000000 // Total Number of Words in Text File
#define THREADS 2 // Total Number of Threads
#define MAXSTORE 20 // Total Number Going Into Map
#define BUFSIZE 15 // BUFSIZE For Allocation
#define DIVIDED TOTAL/THREADS // Split The Work Between Threads

bool check(char* first, char* second); // Used to Map, Check if Two Words Are The Same
void LongestWord(char* arr[]); // Longest Word Function
void ShortestWord(char* arr[]); // Shortest Word Function
void MostCommonWord(char* arr[]); // Most Common Word Function

char *test[TOTAL]; // Storage For The Text File, Into an Array
char *longest; // Longest Word Goes Here
int longestchar; // Length of Longest Word Goes Here
char *shortest; // Shortest Word Goes Here
int shortestchar; // Length of Shortest Word Goes Here
int store; // Current Number That is Stored Into Storage.
char *storage[MAXSTORE]; // Storage For The Map Function, Only Stores Unique Words Found in Test
int storagecount[MAXSTORE] = {}; // Storage For The Map Function. Only Stores the Count of Each Individual Unique Word.
char *most; // Most Word Appeared Goes Here
int mostchar; // Total Count of Most Word Goes Here

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // Setting Up MUTEX

struct arg_struct{ // Setting Up Struct For Mapping Argument. For Ranging (To Split Work For Threading)
   int arg1; // First Argument, From.
   int arg2; // Second Argument, To.
};

void *reading(void *arg){ // Read File Function
   char *val_p = (char *)arg; // Extracting Parameters
   FILE *pointer_file; // Setting File Pointer
   pointer_file = fopen(val_p, "r"); // Reading File
   int i = 0; // Initialize Index of Array
   test[i] = malloc(BUFSIZE); // Allocating Array
   while(fgets(test[i], BUFSIZE,pointer_file) != NULL){ // Adding File Into Array
      strtok(test[i],"\n"); // Remove The Next Line (\n) That's Found In The File
      i++; // Increase Index
      test[i] = malloc(BUFSIZE); // Allocate Array
      if (i == TOTAL){ // At Total Amount
         break; // Break Out, Don't Need to Store Anymore, We're Done.
      }
   }
   fclose(pointer_file); // Close The File.
   pthread_exit(0); // Exit Thread.
}

bool check(char* first, char* second){ // Check to See if Two Words Are Equal
   int firstlen = strlen(first); // Get Size of First Word
   int secondlen = strlen(second); // Get Size of Second Word
   int found = 1; // Set Found to true (1).
   if (firstlen != secondlen){ // When The Size Are Not Equal.
      found = 0; // Set Found to False (0).
   }
   else{ // If the Size Are Equal.
      for(int i = 0; i < firstlen; i++){ // Earch Each Character
         if(first[i] != second[i]){ // If The Characters of The Two Words Are Not Equal
            found = 0; // Set Found to False (0).
            break; // Break Out. We're Done.
         }
      }
   }
   return found; // Return Found, Either 0 or 1.
}

void *Mapping(void *arguments){ // Mapping Thread. Takes Argument From # to #.
   struct arg_struct *args = arguments; // Extract Parameters
   int firstarg = args -> arg1; // First Argument Extracted From Parameters.
   int secondarg = args -> arg2; // Second Argument Extracted From Parameters.
   for(int i = firstarg; i < secondarg; i++){ // Run From First Argument To Second Argument.
      int found = 0; // Found is Set to False (0).
      for(int j = 0; j < store; j++){ // Check to See if It's Been Stored Into The Map Yet.
         bool value = check(storage[j],test[i]); // Check To See if It's Found in The Map.
         if(value){ // If It's Found in The Map, Storage
            pthread_mutex_lock(&mutex1); // Critical Section
            storagecount[j]++; // Storage Count Increase by 1.
            free(test[i]); // Free the Allocation Where It's Been Found
            pthread_mutex_unlock(&mutex1); // End of Critical Section
            found = 1; // Set Found to True (1).
            break; // Break Out Since It's Found.
         }  
      }
      if (found == 0){ // If It's Not Found
         pthread_mutex_lock(&mutex1); // Critical Section
         storage[store]=test[i]; // Store Word Into Map, Storage.
         storagecount[store]++; // Set Up Count For The Word.
         store++; // Increase Number Stored in Storage.
         pthread_mutex_unlock(&mutex1); // End of Critical Section.
      }
      
   }
   pthread_exit(0); // Exit Thread.
}

void LongestWord(char* arr[]){ // Longest Word Function
   int maxlength = strlen(arr[0]); // Set First Word As Longest Word. Get Length of First Word
   int maxindex = 0; // Set First Word as Longest Word. Get Index of First Word.
   for(int i = 1; i < MAXSTORE; i++){ // Start at Index 1 to Total In Map.
      if(arr[i] == NULL) break; // If There's Nothing In Map, Break.
      int length = strlen(arr[i]); // Get Length of Word
      if(length > maxlength){ // If New Word Length Is Larger Than The Max Length
         maxlength = length; // Set Max Length = to New Word Length
         maxindex = i; // Get Index of The New Word
      }
   }
   longest = arr[maxindex]; // Longest Word = Array[MaxIndex]
   longestchar = maxlength; // Longest Character = Max Length
}

void ShortestWord(char* arr[]){ // Shortest Word Function
   int minlength = strlen(arr[0]); // Set First Word As Shortest Word. Get Length of First Word
   int minindex = 0; // Set First Word As Shortest Word. Get Index of First Word.
   for(int i = 1; i < MAXSTORE; i++){ // Start at Index 1 to Total In Map.
      if(arr[i] == NULL) break; // There's Nothing In Map, Break.
      int length = strlen(arr[i]); // Get Length of Word
      if(length < minlength){ // If New Word Length Is Shorter Than The Min Length
         minlength = length; // Set Min Length = to New Word Length
         minindex = i; // Get Index of The New Word
      }
   }
   shortest = arr[minindex]; // Shortest Word = Array[MinIndex]
   shortestchar = minlength; // Shorest Character = Min Length
}

void MostCommonWord(char* arr[]){ // Most Common Word Function
   int max = 0; // Initialize Max to 0.
   int maxindex = 0; // Initialze Index of Max to 0.
   for(int i = 0; i < MAXSTORE; i++){ // Search Through Map.
      if(arr[i] == NULL) break; // There's Nothing In Map, Break.
      if(storagecount[i] > max){ // If The Amount in Storage Count is Greater Than Max
         max = storagecount[i]; // Set Max Equal To Value of Storage Count
         maxindex = i; // Record Index of Max Storage Count
      }
   }
   most = storage[maxindex]; // Most Words is at Storage[MaxIndex]
   mostchar = storagecount[maxindex]; // Amount of Words is at StorageCount[MaxIndex]
}

int main(){
   pthread_t fileread; // Reading Thread
   pthread_t tid[THREADS]; // Thread For Efficiency
   pthread_create(&fileread, NULL, reading, "file.txt"); // Read The Words to The Same List by a Single Thread
   pthread_join(fileread, NULL); // Join to Get Words Into Test Array
   struct arg_struct args; // Structure For The First Arguments
   args.arg1 = 0; // Setting it From 0
   args.arg2 = DIVIDED; // Setting it To Half of Total Since Two Threads
   struct arg_struct argss; // Structures For Second Arguments
   argss.arg1 = DIVIDED; // Setting it From Half of Total
   argss.arg2 = TOTAL; // Setting it To Total
   pthread_mutex_init(&mutex1,NULL); // Initializing Mutex
   pthread_create(&tid[0], NULL, Mapping, (void *) &args); // First Thread, From 0 to Total/2
   pthread_create(&tid[1], NULL, Mapping, (void *) &argss); // Second Thread, From Total/2 to Total

   for(int i = 0; i < THREADS; i++){ // Joining of The Threads
      pthread_join(tid[i],NULL); // Joining
   }
   pthread_mutex_destroy(&mutex1); // Destroying Mutex
   LongestWord(storage); // Run Longest Word Function Using Result of Map
   ShortestWord(storage); // Run Shortest Word Function Using Result of Map
   MostCommonWord(storage); // Run Most Common Word Function Using Result of Map

   for(int i = 0; i < MAXSTORE; i++){ // Print out Results of Map
      if(storage[i] == NULL) break; // There's Nothing In Map. Break.
      printf("In storage: %s with counts of %d\n", storage[i], storagecount[i]); // Print Out Map
   }
   printf("Longest Word: %s With %d Characters\n", longest, longestchar); // Longest Word
   printf("Shortest Word: %s With %d Characters\n", shortest, shortestchar); // Shortest Word
   printf("Most Word: %s Appeared %d Times\n", most, mostchar); // Most Word
   return 0;
}