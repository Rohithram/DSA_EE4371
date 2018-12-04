/*
Author: Rohithram R | EE16B031
Date: 15-08-2018
Program to find anagrams from a sequence of words in a dictionary
Procedure to run:
gcc anagrams.c
./a.out < input.txt
*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//Some of the assumptions taken
#define max_word_len 30
#define max_anagrams 5000
#define max_words_per_anagram 30

//function declaration
void qsort_each_word(char word[],int, int);
void qsort_indices(int array[],int anagstart[], int anagend[],int,int);
void qsort_words(char words[][max_word_len],int [], int, int);

void main()
{   
    int no_of_words;

    scanf("%d",&no_of_words);                      // get no.of words in the dictionary
    char words[no_of_words][max_word_len];         // array containing all words in the input
    char sorted_char_level[no_of_words][max_word_len]; //array containing all words in alphabetical order
    int sorted_indices[no_of_words];               //array of indexes to track the original order
    int flag = -1,i=0,ctr = 0,j=0,l=0,k=0;

    // Read the words line by line into the array
    for(j=0;j<no_of_words;j++)
    {   
        scanf("%s",words[j]);
        strcpy(sorted_char_level[j],words[j]);
            
        // Sort the letters in each word of the array in ascending alphabetical order
        qsort_each_word(sorted_char_level[j],0,strlen(sorted_char_level[j])-1);
        sorted_indices[j] = j;
    }

    // Sort the words of the array in alphabetical order and update it in 'sorted_char_level' array itself
    qsort_words(sorted_char_level,sorted_indices,0,no_of_words-1);
    
   
    //Arrays to track the starting and ending indexes of anagrams
    int anagstart[max_anagrams];
    int anagend[max_anagrams];
    int no_of_anagrams = 0;
   
   //Code to find the anagrams after sorting
    i=0;
    while(i<no_of_words){
        flag = -1;                //marked -1 to tell no anagrams are there initially
        j=i+1;
        while(strcmp(sorted_char_level[i],sorted_char_level[j])==0){
            flag=i;               // marks the start index of the anagram
            j++;
        }

        if(flag!=-1){   
            anagstart[ctr] = flag;
            anagend[ctr] = j-1;
            i = j-1;               // move the index to next index to current set of anagrams
            ctr++;                 //ctr is incremented for each set of anagrams found
        }else{
            i++;
        }
    }

    no_of_anagrams = ctr;
    printf("%d\n",no_of_anagrams); // no of anagrams found

    //code to print the anagrams in the given format
    int temp_sort_anagram_indexes[no_of_anagrams],words_per_anag=0;

    for(i=0;i<no_of_anagrams;i++){
        //finding the index of word which occurs first in input order in a set of anagrams
        int min_index_per_set = sorted_indices[anagstart[i]];
        words_per_anag = anagend[i]-anagstart[i]+1;
        for(k=0;k<words_per_anag;k++){
            if(sorted_indices[anagstart[i]+k]<=min_index_per_set){
                min_index_per_set = sorted_indices[anagstart[i]+k];
            }
        }
        temp_sort_anagram_indexes[i] = min_index_per_set;
    }
    
    
    //sorting the anagrams according to its original order
    qsort_indices(temp_sort_anagram_indexes,anagstart,anagend,0,no_of_anagrams-1);

    for(i=0;i<no_of_anagrams;i++){
            k = 0;
            //temporary array to sort the indices in set of anagrams so that the original order is maintained
            int temp_sorted_indexes[max_words_per_anagram];
            char temp_sorted_words[max_words_per_anagram][max_word_len];

            
            for(j=anagstart[i];j<=anagend[i];j++){
                temp_sorted_indexes[k] = sorted_indices[j];
                strcpy(temp_sorted_words[k],words[sorted_indices[j]]);
                k++;
            }

            //sorting the words in a set of anagrams in alphabetical order 
            //using quick sort
            qsort_words(temp_sorted_words,temp_sorted_indexes,0,anagend[i]-anagstart[i]);
            printf("%d\n",anagend[i]-anagstart[i]+1);
            for(l=0;l<anagend[i]-anagstart[i]+1;l++){
                printf("%s\n",words[temp_sorted_indexes[l]]);
            }
    }
}


// A utility function to swap two characters in word
void swap(char* a, char* b)
{
    char t = *a;
    *a = *b;
    *b = t;
}

// A utility function to swap two indices
void swap_indices(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

// A utility function to swap two words in array
void swap_words(char a[], char b[])
{
    char t[max_word_len];
    strcpy(t,a);
    strcpy(a,b);
    strcpy(b,t);
}

 
/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot 

*/

int partition_word (char word[], int low, int high)
{
    int pivot = word[high];    // pivot
    int i = (low - 1);         // Index of smaller element
    int j;
    for (j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (word[j] <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&word[i], &word[j]);
        }
    }
    swap(&word[i + 1], &word[high]);
    return (i + 1);
}
 

/* The function that implements QuickSort
  word[] --> word to be sorted,
  low  --> Starting index,
  high  --> Ending index */

void qsort_each_word(char word[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[pi] is now
           at right place */
        int pi = partition_word(word, low, high);
 
        // Separately sort elements before
        // partition and after partition
        qsort_each_word(word, low, pi - 1);
        qsort_each_word(word, pi + 1, high);
    }

}

/*
Function to quick sort the indexes
Takes array of indexes of words in original input array to sort and also sorts the anagram starting indexes
and ending indexes accordingly using quick sort
*/
void qsort_indices(int array[], int anagstart[], int anagend[],int first_line, int last_line)
{
    int i,last;
    int pivot = (first_line + last_line)/2 ;
    if(first_line>=last_line){
        return ; 
    }

    swap_indices(&array[first_line],&array[pivot]);
    swap_indices(&anagstart[first_line],&anagstart[pivot]);
    swap_indices(&anagend[first_line],&anagend[pivot]);   

    last = first_line;
    for(i=first_line+1;i<=last_line;i++){
        if((array[i] < array[first_line])){
            last+=1;
            swap_indices(&array[last],&array[i]);
            swap_indices(&anagstart[last],&anagstart[i]);
            swap_indices(&anagend[last],&anagend[i]);  
        }
    }

    swap_indices(&array[first_line],&array[last]);
    swap_indices(&anagstart[first_line],&anagstart[last]);
    swap_indices(&anagend[first_line],&anagend[last]);

    qsort_indices(array,anagstart,anagend,first_line,last-1);
    qsort_indices(array,anagstart,anagend,last+1,last_line);
}


/* Function to quick sort the words in dicitionary in alphabetical order
Takes array of words and indexes of that and start index and end index as inputs
indexes is passed as argument to track the original position of the word in input even 
after sorting*/
void qsort_words(char array[][max_word_len],int sorted_indices[], int first_line, int last_line)
{

    int i,last;
    int pivot = (first_line + last_line)/2 ;
    if(first_line>=last_line){
        return ; 
    }

    swap_words(array[first_line],array[pivot]);
    swap_indices(&sorted_indices[first_line],&sorted_indices[pivot]);
    last = first_line;
    for(i=first_line+1;i<=last_line;i++){
        if(strcmp(array[i],array[first_line])<0){
            last+=1;
            swap_words(array[last],array[i]);
            swap_indices(&sorted_indices[last],&sorted_indices[i]);
        }
    }

    swap_words(array[first_line],array[last]);
    swap_indices(&sorted_indices[first_line],&sorted_indices[last]);
    
    qsort_words(array,sorted_indices,first_line,last-1);
    qsort_words(array,sorted_indices,last+1,last_line);

    return ;
}
