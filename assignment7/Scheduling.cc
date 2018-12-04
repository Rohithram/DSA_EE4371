/*
Author: Rohithram | EE16B031
Assignment 7: Task Scheduling
My code gives 61 violations for test input of 1000 tasks given
*/

/* we sort tasks based on release time if equal then based on margin which is defined as
deadline - (release time + duration time). if equal then we sort them based on earliest
deadline.

Thus we perform tasks in this priority order : least margin , if equal then
those with earlier deadlines are executed first*/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <bits/stdc++.h>

using namespace std;
#define t_MAX 99999

typedef struct{

	int id;
	int r_time;   // release time
	int deadline; // deadline 
	int duration; // duration
	bool is_done = false; // whether task is done are not
	int margin;   // margin = deadline - (r_time + duration)

}task;
	

// Compare wrote in format that qsort (library function) would take
int compare(const void *a, const void *b)  
{ 	
	// priority variables which ll be used to sort two tasks
	int pr1,pr2;

    task t1 = *(task * )a; 
    task t2 = *(task * )b;

    //first sort according to release times
    pr1 = t1.r_time;
    pr2 = t2.r_time;

    //if equal , then margin
    if(pr1 == pr2){
    	pr1 = t1.margin;
    	pr2 = t2.margin;
    }  

    //if again equal, then earliest deadline
    if(pr1 == pr2){
    	pr1 = t1.deadline;
    	pr2 = t2.deadline;
    }  

    return pr1 - pr2; 
} 

int find_best_task(task * tasks,int current_time, int N){

	int best_task_id = -1, min_margin = t_MAX;
	int violated_task_id = -1;

	for (int i = 0; i < N; i++)
		{
			// to consider tasks whose start time is less than current time
			if(tasks[i].r_time > current_time){
				break;
			}

			// Task which can't be completed without violation
			if(!tasks[i].is_done && tasks[i].margin<0){
				violated_task_id = tasks[i].id;
			}
			
			// Tasks which can be done without violations
			else if(!tasks[i].is_done && tasks[i].margin >= 0){

				if(tasks[i].margin == min_margin){
					if(tasks[i].deadline < tasks[best_task_id].deadline){
						best_task_id = tasks[i].id;
					}
				}
				else if(tasks[i].margin < min_margin){
					best_task_id = tasks[i].id;
					min_margin = tasks[i].margin;
				}
				
				
			}
			

		}

		if(best_task_id == -1){
			best_task_id = violated_task_id;
		}

		return best_task_id;
}

int Schedule_the_tasks(task* tasks,int N){

	int tasks_compltd = 0,current_time = 1,best_task_id;

	// to keep track executing tasks
	vector<int> task_manager;

	// sort based on given priority order :
	// 1.start time, 2.margin, 3.deadline
	qsort(tasks, N, sizeof(task), compare);

	while(tasks_compltd < N){

		best_task_id = find_best_task(tasks,current_time,N);

		for (int i = 0; i < N; i++)
		{
			if(tasks[i].r_time > current_time){
				break;
			}

			// task to be done now
			if(tasks[i].id == best_task_id){
				tasks[i].duration--;
				// Task is completed
				if(tasks[i].duration == 0){
					tasks[i].is_done = true; 
					tasks_compltd++;
				}
			}
			else if(!tasks[i].is_done)
				tasks[i].margin--;
				
		
			tasks[i].r_time++;
		}

		current_time++;

		task_manager.push_back(best_task_id);
	}


	for (int i = 0; i < task_manager.size();i++){

		if(task_manager[i] != -1){
			int j = i;
			while(task_manager[j+1] == task_manager[j]){
				j++;
			}
			printf("%d %d %d\n",i+1,j+2,task_manager[i]);
			i = j;
		}

	}	

	return 0;
}


int main(){

	int N; // No of tasks to be scheduled
	
	scanf("%d",&N);


	//temporary variables for initialisation
	int r_time,dur,deadline;

	//memory allocation for array of structure task
	task * tasks = (task*) malloc(sizeof(task)*N);

	for (int i = 0; i < N; i++){	
		
		scanf("%d %d %d",&r_time,&dur,&deadline);

		// given an id for each task to track while sorting
		tasks[i].id = i+1;

		tasks[i].r_time = r_time;
		tasks[i].duration = dur;
		tasks[i].deadline = deadline;
		tasks[i].is_done = false;

		tasks[i].margin = tasks[i].deadline - (tasks[i].r_time + tasks[i].duration);

	}
	//Function to schedule the tasks
	Schedule_the_tasks(tasks,N);
	
	return 0;
}