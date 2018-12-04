/*
Author: Rohithram R | EE16B031
Assignment 5: Finding Island Capitals from given graph whose nodes are towns connected by roads
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define max_char 20


// Note : Code for implementing Dijkstra is referred from geeksforgeek.com

// A utility function to find the vertex with minimum distance value, from 
// the set of vertices not yet included in shortest path tree 
int minDistance(int dist[], bool visited[], int N) 
{ 
   // Initialize min value 
   int min = INT_MAX, min_index; 
   int v=0;

   for (v = 0; v < N; v++) 
   {
     if (visited[v] == false && dist[v] <= min) 
         min = dist[v], min_index = v; 
   }

   return min_index; 
} 


// Function that implements Dijkstra's single source shortest path algorithm 
// for a distances represented using adjacency matrix representation 
// dist is 2D array to store shortest path of each vertex from every other . its symmetric matrix
int find_shortest_path(int** distances,int **dist,int N,int M,int s){

    int count=0,v=0,i=0;
    bool *visited;
    visited = (bool*)malloc(sizeof(bool)*N);

    for(i=0;i<N;i++)
        visited[i] = false;

    // Distance of source vertex from itself is always 0 
    dist[s][s] = 0; 

    // Find shortest path for all vertices 
    for (count = 0; count < N-1; count++) 
    {   
        // Pick the minimum distance vertex from the set of vertices not 
        // yet processed. u is always equal to src in the first iteration. 
        int u = minDistance(dist[s], visited,N); 
        // Mark the picked vertex as processed 
        visited[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex. 
        for (v = 0; v < N; v++) 
        {
            // Update dist[v] only if is not in visited, there is an edge from  
            // u to v, and total weight of path from src to  v through u is  
            // smaller than current value of dist[v] 
            if (!visited[v] && distances[u][v]!=-1 && dist[s][u] != INT_MAX){
                int tot_dist = dist[s][u]+distances[u][v];
                if(tot_dist<dist[s][v]){
                    dist[s][v] = tot_dist; 
                }
            }
        }
    }
    return 0;
}

// Helper function for inbuilt qsort to sort the capital towns in alphabetical order
int compare_capitals(const void *town1, const void *town2)
{
    return strcmp(* (char * const *) town1, * (char * const *) town2);
}

//Function to find capitals of group of islands represented by a graph
int find_capitals(int **distances,char** towns, int N, int M){

    int i=0,j=0,k=0,u=0;
    int *components[N];

    int *dist[N];
    bool *visited;

    visited = (bool*)malloc(sizeof(bool)*N);

    for(i=0;i<N;i++){
        dist[i] = (int*)malloc(sizeof(int)*N);
        components[i] = (int*)malloc(sizeof(int)*N);
    }

    // Initialize all distances as INFINITE and visited[] as false 
    for (i = 0; i < N; i++){
        visited[i] = false; 
        for(j=0;j<N;j++){
            dist[i][j] = INT_MAX;
            components[i][j] = -1;
        }
    }

    k=0;
    for(i=0;i<N;i++){
        if(visited[i]==false){
            find_shortest_path(distances,dist,N,M,i);
            u=0;
            for(j=0;j<N;j++){
                if(dist[i][j]!=INT_MAX){
                    components[k][u] = j;
                    if(i!=j){
                        find_shortest_path(distances,dist,N,M,j);
                    }
                    visited[j] = true;                     
                    u++;
                }
            }
            k++;
        }
    }

    double *sum_dist = (double*) malloc(sizeof(double)*N);

    for(i=0;i<N;i++){
        k=0;
        for(j=0;j<N;j++){
            if(dist[i][j]!=INT_MAX){
                sum_dist[i]+=dist[i][j];
                k++;
            }
        }
        sum_dist[i] = (double) sum_dist[i]/k;
    }

    double smallest_dist;
    char** capital_towns;
    capital_towns = malloc(N*sizeof(char*));
    k=0;
    for(i=0;i<N;i++){
        smallest_dist = (double) INT_MAX;
        int cap_idx = -1;
        char *temp_cap_name;
        temp_cap_name = (char*)malloc(sizeof(char)*max_char);

        for(j=0;j<N;j++){
            if(components[i][j]!=-1){
                if(sum_dist[components[i][j]]<=smallest_dist){
                    smallest_dist = sum_dist[components[i][j]];
                    cap_idx = components[i][j];
                    if(strcmp(temp_cap_name,towns[components[i][j]])<0){
                        strcpy(temp_cap_name,towns[components[i][j]]);
                        cap_idx = components[i][j];
                    }
                }
            }
        }
        
        if(cap_idx!=-1){
            capital_towns[k] = malloc(sizeof(char)*max_char);
            strcpy(capital_towns[k],towns[cap_idx]);
            k++;
        }

        free(temp_cap_name);
    }

    qsort(capital_towns,k,sizeof(char*),compare_capitals);

    for (i=0;i<k;i++)
	{
		printf("%s\n",capital_towns[i]);
        free(capital_towns[i]);
	}

    free(visited);
    free(sum_dist);
    for(i=0;i<N;i++){
        free(dist[i]);
        free(components[i]);
    }

    return 0;
}

//Searching towns array to find if it already exists else to add newly.
int search_town(char** towns,char* town, int N){
    int i=0;
    
    for(i=0;i<N;i++){
        if(strcmp(towns[i],town)==0){
            return i;
        }

    }
    return -1;
}

int main(){
    
    int i=0,j=0,k=0;
    int N,M;                             // no of towns and no of edges present in graph

    scanf("%d\t%d\n",&N,&M);

    // Adjacency matrix for distances between each town
    int* distances[N] ;

    // declaring array for town names
    char** towns;
    towns = malloc(N*sizeof(char*));

    for(i=0;i<N;i++){
        distances[i] = (int*)malloc(sizeof(int)*N);
        towns[i] = malloc(sizeof(char)*max_char);
    }

    // Initialising all distances as -1
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            distances[i][j] = -1;
        }
    }

    char *temp1,*temp2;
    int d;
   
    
    for(i=0;i<M;i++){

        temp1 = (char*) malloc(sizeof(char)*max_char);
        temp2 = (char*) malloc(sizeof(char)*max_char);

        scanf("%s %s %d",temp1,temp2,&d);

        int t1 =search_town(towns,temp1,k);
        int t2 =search_town(towns,temp2,k);

        if(t1==-1){
            strcpy(towns[k],temp1);
            t1 = k;
            k++;
        }
        if(t2==-1){
            strcpy(towns[k],temp2);
            t2 = k;
            k++;
        }

        distances[t1][t2] = d;
        distances[t2][t1] = d;
        free(temp2);
        free(temp1);
        
    }

    find_capitals(distances,towns,N,M);
   
    for(i=0;i<N;i++){
        free(distances[i]);
        free(towns[i]);
    }

    return 0;
}