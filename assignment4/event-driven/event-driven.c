/*
Assignment - 4
Name : Rohithram R 
Rollno : EE16B031
Aim : To simulate the execution of a set of tasks that are given implementing priority queues using Binary Heap datastructure.
Date : 22-09-2018
*/

//importing header files
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

// Definig long int as dtype since the numbers used are very large
typedef long int dtype;

// Task is structure to store end time, start time and whether task is currently active or not
typedef struct{
    dtype start_time;
    dtype end_time;
    bool isactive;
}Task;

// End_time_heap is MinHeap of endtime of each tasks
typedef struct{
    dtype heap_size;        // size of the heap
    dtype* end_time_arr;    // array of end time of each task
    dtype* indexes;         // array of indexes of each end time to track from which task it belongs to.
} End_time_heap;

// Its just a structure to return two values together from a extract_min_end_time function
typedef struct{
    dtype min_end_time;     // minimum end time
    dtype task_index;       // corresponding task no of the minimum end time
} Extract_min_outputs;


// An utility function to swap two task's endtime
void swap_end_times(dtype *x, dtype *y)
{
    dtype temp = *x;
    *x = *y;
    *y = temp;
}

// A function to initialise new tasks as read from inputs
void new_task(Task* task,int start_time, int duration){
    (*task).start_time = start_time;
    (*task).end_time = (*task).start_time+duration;
    (*task).isactive = false;
}

// A function to heapify the created end_time_heap to follow the min_heap property
//Source :  The overall idea of the code implementing heap is referred from CLRS text book 6th chapter - Heap

void min_heapify(End_time_heap* end_time_heap,dtype i){

    dtype left = 2*i+1;   // Left child
    dtype right = 2*i+2;  // Right child
    dtype least = i;      // current node 

    // checking whether left child is lesser than parent
    if(left < (*end_time_heap).heap_size && (*end_time_heap).end_time_arr[left] < (*end_time_heap).end_time_arr[least]){
        least = left;
    }
    // checking whether the right child is lesser than the current least element
    if(right < (*end_time_heap).heap_size && (*end_time_heap).end_time_arr[right] < (*end_time_heap).end_time_arr[least]){
        least = right;
    }
    
    // if least is not parent we swap them and call function recursively to heapify the subtree below the swapped node.
    if(least!=i){
        swap_end_times(&(*end_time_heap).end_time_arr[i],&(*end_time_heap).end_time_arr[least]);
        // Swapping the indexes of the tasks to keep track of which end time belongs to which task
        swap_end_times(&(*end_time_heap).indexes[i],&(*end_time_heap).indexes[least]);
        min_heapify(end_time_heap,least);   
    }
}


// constructing a Minimum Binary Heap data structure, which will hold the least endtypeime as the root node
End_time_heap* constructMinHeap(dtype end_times[], dtype N){
    
    dtype i;

    // Dynamic memory allocation 
    End_time_heap* end_time_heap = (End_time_heap*) malloc(sizeof(End_time_heap));
    (*end_time_heap).heap_size = N;                             // initialize size of heap
    (*end_time_heap).end_time_arr = end_times;                  // Assign address of first element of array
    
    //Allocating memory to track indexes of end_time of tasks while heapifying the end times
    end_time_heap->indexes = (dtype *)malloc(sizeof(dtype)*N);  

    for(i=0;i<N;i++){
        (*end_time_heap).indexes[i] = i;
    }

    // Start from bottommost and rightmost internal mode and heapify all
    // internal modes in bottom up way
    for (i = ((*end_time_heap).heap_size - 2) / 2; i >= 0; --i)
        min_heapify(end_time_heap, i);

    return end_time_heap;
}

// Function which returns minimum end time and the corresponding task index and updates the heap accordingly.
Extract_min_outputs* extract_min_end_time(End_time_heap* end_time_heap){

    dtype min_end_time = -1;
    dtype task_index = -1;
    // structure used to return two values from this function
    Extract_min_outputs* extract_min_outputs = (Extract_min_outputs*) malloc(sizeof(Extract_min_outputs));
    
    // underflow error
    if((*end_time_heap).heap_size < 1){
        extract_min_outputs->min_end_time = min_end_time;
        extract_min_outputs->task_index = task_index;           
        return extract_min_outputs;
    }

    // root node is the minimum element in minheap and updating indexes correspondingly
    min_end_time = (*end_time_heap).end_time_arr[0];
    task_index = (*end_time_heap).indexes[0];

    (*end_time_heap).end_time_arr[0] = (*end_time_heap).end_time_arr[(*end_time_heap).heap_size-1];
    (*end_time_heap).indexes[0] = (*end_time_heap).indexes[(*end_time_heap).heap_size-1];
    (*end_time_heap).heap_size-=1;

    // After removing the root node and swapping with last one, we heapify the subtree below it now.
    min_heapify(end_time_heap,0);

    extract_min_outputs->min_end_time = min_end_time;
    extract_min_outputs->task_index = task_index;

    return extract_min_outputs;
}


//function to simulate the set of events and to print relevant metrics as required
void simulate_events(Task* total_tasks,End_time_heap* end_time_heap,dtype N){

    dtype i=0;
    double avg_active_tasks=0.0;                        // Average no of active tasks for entire duration
    int active_tasks=0,max_active_tasks = 0;         // current no of active tasks and maximum no of active tasks @ any instant.
    dtype simulation_end_time = 0;                   // Simulation end time
    dtype segment_low_idx = 0,segment_upper_idx = 0; // To keep track of time interval between active tasks
    dtype min_end_time = 1,min_start_time = 0;       
    dtype task_index = 0;                            // Task index is used to track the corresponding task of each endtime

    //To parse the outputs returned from extract min end time function
    Extract_min_outputs* extract_min_outputs;

    extract_min_outputs = extract_min_end_time(end_time_heap);
    min_end_time = extract_min_outputs->min_end_time;
    task_index = extract_min_outputs->task_index;
    
    // Since tasks are arranged in ascending order of start time , it already follows the minimum heap property
    // So only end time of each tasks is heapified to efficiently implement priority queue
    for(i=0;i<N;i++){               
        
        min_start_time = total_tasks[i].start_time;
        
        
        if(min_start_time < min_end_time || (min_start_time > min_end_time && total_tasks[task_index].isactive==false)){

            segment_upper_idx = min_start_time;
            avg_active_tasks+=(segment_upper_idx-segment_low_idx)*active_tasks;
            active_tasks+=1;
            total_tasks[i].isactive = true;
            segment_low_idx = segment_upper_idx;

        }else if(min_end_time < min_start_time && total_tasks[task_index].isactive){
            
            segment_upper_idx = min_end_time;
            avg_active_tasks+=(segment_upper_idx-segment_low_idx)*active_tasks;
            active_tasks-=1;
            total_tasks[task_index].isactive = false;
            segment_low_idx = segment_upper_idx;

            //To get next min end time after ending one task
            extract_min_outputs = extract_min_end_time(end_time_heap);
            min_end_time = extract_min_outputs->min_end_time;
            task_index = extract_min_outputs->task_index;
            // We decrease i since starttime was greater than end time, so we need to check it again with next
            // minimum end time and so on...
            i-=1;                                        
        
        }
        else if(min_start_time == min_end_time && total_tasks[task_index].isactive){
            
            total_tasks[i].isactive=true;
            total_tasks[task_index].isactive = false;
            
            extract_min_outputs = extract_min_end_time(end_time_heap);
            min_end_time = extract_min_outputs->min_end_time;
            task_index = extract_min_outputs->task_index;
        }

        // finding max active tasks at any instant
        if(active_tasks>max_active_tasks){
            max_active_tasks = active_tasks;
        }
    }

    // When all the tasks are started, we end tasks which are pending based on endtime
    while((*end_time_heap).heap_size >= 0 && min_end_time!=-1 && total_tasks[task_index].isactive){
        
        segment_upper_idx = min_end_time;
        avg_active_tasks+=(segment_upper_idx-segment_low_idx)*active_tasks;
        active_tasks-=1;
        total_tasks[task_index].isactive = false;
        segment_low_idx = segment_upper_idx;

        extract_min_outputs = extract_min_end_time(end_time_heap);
        min_end_time = extract_min_outputs->min_end_time;
        task_index = extract_min_outputs->task_index;

        // finding max active tasks at any instant
        if(active_tasks>max_active_tasks){
            max_active_tasks = active_tasks;
        }
    }
    
    simulation_end_time = (segment_upper_idx);                  // Simulation end time
    avg_active_tasks = avg_active_tasks/simulation_end_time;    // average no of active tasks during entire duration


    //printing the relevant metrics of the event simulator in the format needed
    printf("%ld\n",simulation_end_time);
    printf("%d\n",max_active_tasks);
    printf("%.4f\n",avg_active_tasks);
}



int main(){
    dtype i,N;                                            // Total no of tasks
    int dur=1;                                            // just a variable to read in input
    dtype start=0;                                        
    dtype* end_times;                                     // array of end time of each task
    End_time_heap* end_time_heap;                         // minHeap of end time of each task

    //Read in no of tasks to be simulated
    scanf("%ld\n",&N);

    // Dynamic memory allocation of Array of structure of tasks.
    Task* total_tasks = (Task*) malloc(sizeof(Task)*N);
    
    end_times = (dtype*) malloc(sizeof(dtype)*N);

    //Read in each task line by line and create new task in structure format and end time array
    for(i=0;i<N;i++){
        scanf("%ld %d\n",&start,&dur);
        new_task(&total_tasks[i],start,dur);
        end_times[i] = total_tasks[i].end_time;
    }

    //Creating minHeap of end time of each task
    end_time_heap = constructMinHeap(end_times,N);
    
    //Simulating the set of events given
    simulate_events(total_tasks,end_time_heap,N);

    return 0;
}