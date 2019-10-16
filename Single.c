#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TOTAL 50000000 // Total Number of Words in Text File
#define MAXSTORE 20 // Total Number Going Into Map
#define BUFSIZE 15 // BUFSIZE For Allocation

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

void reading(char *arg){ // Read File Function
   FILE *pointer_file; // Setting File Pointer
   pointer_file = fopen(arg, "r"); // Reading File
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

void Mapping(int firstarg, int secondarg){ // Mapping Function. Takes Argument From # to #.
   for(int i = firstarg; i < secondarg; i++){ // Run From First Argument To Second Argument.
      int found = 0; // Found is Set to False (0).
      for(int j = 0; j < store; j++){ // Check to See if It's Been Stored Into The Map Yet.
         bool value = check(storage[j],test[i]); // Check To See if It's Found in The Map.
         if(value){ // If It's Found in The Map, Storage
            storagecount[j]++; // Storage Count Increase by 1.
            free(test[i]); // Free the Allocation Where It's Been Found
            found = 1; // Set Found to True (1).
            break; // Break Out Since It's Found.
         }  
      }
      if (found == 0){ // If It's Not Found
         storage[store]=test[i]; // Store Word Into Map, Storage.
         storagecount[store]++; // Set Up Count For The Word.
         store++; // Increase Number Stored in Storage.
      }
      
   }
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
   reading("file.txt"); // Read Text File
   Mapping(0,TOTAL); // Map the Text File
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