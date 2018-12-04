
/* Name : Rohithram R | EE16B031
   Date : 11-11-2018
   Question : 4
   Aim  : To make a tour around given cities in order with minimum total length.
*/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "graph.h"

/*
References : 
1. Referred from www.geeksforgeeks.com
*/

// Algorithm Explained.
/*
Algorithm used: Dijkstra Algo to find the shortest path between two cities.
1. We use greedy approach to solve this problem, by starting from the first city given
and finds the shortest path in TN graph to destination being the next city in the order given.

2. We proceed like this from each city and do dijkstra to find shortest path to next city
and stores path using predecessor nodes and to traverse back along the same path successor nodes.

3.Using the stored path between each pair of cities,we traverse the tour  in minimum total length!

4.Time complexity : for k cities we have to do dijkstra so O((K+E)log(N)). where K is cities
in the tour and E is no of edges in the total graph. N is total no of nodes in TN graph.
*/

// Implementation
/*
1. Have used 'graph.h' file as header to read the Tn_graph and then transformed that graph
into a transformed Graph Data structure to support Adjacency list for implementing 
Dijkstra and removed unwanted things like lat,longitude,type of edges.
*/


/*
Note:
Outline of Implementing Graphs with adjacency list is referred from www.geeksforgeeks.com
*/

// A structure to represent a City as a node in adjacency list form 
struct CityNode{ 
	int destination;		// Destination vertex for this vertex in the adjacency list
	double weight;			// Weight of the edge
	struct CityNode* next;	// Pointer to next node in the linkedlist
}; 


// Structure to represent the adjacency list 
typedef struct{ 
	CityNode *head; 		// pointer to head node of linkedlist 
} AdjList; 


// Structure which represent a graph. 
typedef struct{ 
	int V; 
	AdjList* array; 
} CityGraph; 


// Function to create a new adjacency list node 
CityNode* CreateCityNode(int destination, double weight) 
{ 
	CityNode* newCityNode = (CityNode*) malloc(sizeof(CityNode)); 
	newCityNode->destination = destination; 
	newCityNode->weight = weight; 
	newCityNode->next = NULL; 
	return newCityNode; 
} 


CityGraph* createGraph(int V) 
{ 
	CityGraph* citygraph = (CityGraph*) malloc(sizeof(CityGraph)); 
	citygraph->V = V; 

	// Create an array of adjacency lists
	citygraph->array = (AdjList*) malloc(V*sizeof(AdjList)); 

	for (int i = 0; i < V; ++i) 
		citygraph->array[i].head = NULL; 

	return citygraph; 
} 

// Adds an edge to the graph 
void addEdge(CityGraph* citygraph, int src, int destination, double weight) 
{ 
	CityNode* newCityNode = CreateCityNode(destination, weight); 
	newCityNode->next = citygraph->array[src].head; 
	citygraph->array[src].head = newCityNode; 

	newCityNode = CreateCityNode(src, weight); 
	newCityNode->next = citygraph->array[destination].head; 
	citygraph->array[destination].head = newCityNode; 
} 

typedef struct{ 
	int v; 
	double dist; 
} MinHeapNode; 


typedef struct{ 
	int size;	 					// Number of heap nodes present currently 
	int capacity; 					// Capacity of minheap 
	int *pos;
	MinHeapNode **array;
} MinHeap; 


/*
NOTE: Referred from www.geeksforgeeks.com
*/

MinHeapNode* newMinHeapNode(int v, double dist) 
{ 
	MinHeapNode* minHeapNode = (MinHeapNode*) malloc(sizeof(MinHeapNode)); 
	minHeapNode->v = v; 
	minHeapNode->dist = dist;

	return minHeapNode; 
} 


MinHeap* createMinHeap(int capacity) 
{ 
	MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap)); 
	minHeap->pos = (int *)malloc(capacity*sizeof(int)); 
	minHeap->size = 0; 
	minHeap->capacity = capacity; 
	minHeap->array = (MinHeapNode**) malloc(capacity*sizeof(MinHeapNode*)); 

	return minHeap; 
} 

void swapNode(MinHeapNode** node1, MinHeapNode** node2) 
{ 
	MinHeapNode* temp = *node1; 
	*node1 = *node2; 
	*node2 = temp; 
} 

void minHeapify(MinHeap* minHeap, int i) 
{ 
	int smallest, left, right; 
	smallest = i; 

	left = 2*i + 1; 
	right = 2*i + 2; 

	if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist ) 
	    smallest = left; 

	if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist ) 
	    smallest = right; 

	if (smallest != i){

		// The nodes to be swapped in minheap 
		MinHeapNode *smallestNode = minHeap->array[smallest]; 
		MinHeapNode *iNode = minHeap->array[i]; 

		minHeap->pos[smallestNode->v] = i; 
		minHeap->pos[iNode->v] = smallest; 

		// Swap nodes 
		swapNode(&minHeap->array[smallest], &minHeap->array[i]); 
		minHeapify(minHeap, smallest); 
	} 
} 


// Function to check if the minHeap is empty
int isEmpty(MinHeap* minHeap) 
{ 
	return minHeap->size == 0; 
} 


// Function to extract minimum node from minheap and update (minheapify) the heap. 
MinHeapNode* extractMin(MinHeap* minHeap) 
{ 
	if (isEmpty(minHeap)) 
		return NULL; 

	// Store the root node 
	MinHeapNode* root = minHeap->array[0]; 

	// Replace root node with last node 
	MinHeapNode* lastNode = minHeap->array[minHeap->size - 1]; 
	minHeap->array[0] = lastNode; 

	// Update position of last node 
	minHeap->pos[root->v] = minHeap->size-1; 
	minHeap->pos[lastNode->v] = 0; 

	// Reduce heap size and heapify root 
	--minHeap->size; 
	minHeapify(minHeap, 0); 

	return root; 
} 

// Function to decrease dist value(distance) of a given vertex v. 
void decreaseKey(MinHeap* minHeap, int v, double dist) 
{ 
	// Get the index of v in heap array 
	int i = minHeap->pos[v]; 

	minHeap->array[i]->dist = dist; 

	while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) 
	{ 
		minHeap->pos[minHeap->array[i]->v] = (i-1)/2; 
		minHeap->pos[minHeap->array[(i-1)/2]->v] = i; 
		swapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]); 

		i = (i - 1) / 2; 
	} 
} 

bool isPresent(MinHeap *minHeap, int v) 
{ 
    if (minHeap->pos[v] < minHeap->size) 
	    return true;
    else
        return false; 
} 


/*
NOTE:Referred from www.geeksforgeeks.com.
*/

// Function to calulate Single Source Shortest path using Dijkstra's algorithm. 
void dijkstra(CityGraph* citygraph, int src, double* dist, int* predecessor_nodes) 
{ 

	int V = citygraph->V;					// Get the number of vertices in citygraph

	MinHeap* minHeap = createMinHeap(V); 


	//Initialising distance of all vertices to be infinity.
	for (int v = 0; v < V; ++v) 
	{ 
		dist[v] = INT_MAX; 
		minHeap->array[v] = newMinHeapNode(v, dist[v]); 
		minHeap->pos[v] = v; 
	} 

	minHeap->array[src] = newMinHeapNode(src, dist[src]); 
	minHeap->pos[src] = src; 
	dist[src] = 0; 
	decreaseKey(minHeap, src, dist[src]); 

	minHeap->size = V; 

	while (!isEmpty(minHeap)) 
	{ 

		MinHeapNode* minHeapNode = extractMin(minHeap); 

		int u = minHeapNode->v;                         // Store the extracted vertex number 


		CityNode* temp = citygraph->array[u].head; 
		while (temp != NULL)
		{ 
			int v = temp->destination; 
			
			if (isPresent(minHeap, v) && dist[u] != INT_MAX && temp->weight + dist[u] < dist[v]) 
			{ 
				dist[v] = dist[u] + temp->weight;
                predecessor_nodes[v] = u;               // to track the path of the shortest path
				decreaseKey(minHeap, v, dist[v]);       // update distance in minheap
			} 
			temp = temp->next; 
		} 
	} 
} 


// Function to make a tour in given city order.
int make_a_tour(CityGraph* citygraph,double *dist,int *predecessor_nodes,int *successor_nodes,
									 int *city_id,int no_of_cities){

	int counter,v,k=0;

	for (int i=0;i<no_of_cities;i++){

		// to find shortest path from current city
        dijkstra(citygraph,city_id[i],dist,predecessor_nodes);

        counter = 0;

        // if i is last city then the next city should be starting point
        // so this condition or else normal i+1 the city.
        if(i!=no_of_cities-1){
        	k = i+1;
        }
        else{
        	k=0;
        }

        // to assign parent in the correct order of printing
        for (v=city_id[k];v!=city_id[i];v=predecessor_nodes[v]){
		    successor_nodes[counter++] = predecessor_nodes[v];
        }
        
        // printing the path traversed
        for (int j=counter-1;j>=0;j--)
            printf("%d\n",successor_nodes[j]);
    
    }

    //print starting city
    printf("%d\n",city_id[0]);

    return 0;
}


int main()
{

	int no_of_cities;

    scanf("%d\n",&no_of_cities);

    // Reading graph using 'graph.h'
    Graph TN_graph;
    TN_graph.read_graph("./tn_graph.txt");
    
    int total_nodes = TN_graph.no_nodes;

    // to store the parents of cities explored
    int *predecessor_nodes = (int*) malloc(sizeof(int)*total_nodes);

    for(int i=0;i<total_nodes;i++)
        predecessor_nodes[i] = -1;

    // to store the children of current city explored
    int *successor_nodes = (int*) malloc(sizeof(int)*total_nodes);

    // shortest distance of each city from source
    double *dist = (double*) malloc(sizeof(double)*total_nodes);

    // Array of node ids of the given cities to make a tour
    int *city_id = (int*) malloc(sizeof(int)*no_of_cities);            

    for (int j=0;j<no_of_cities;j++)
        scanf("%d\n",&city_id[j]);
    
    // Now graph read is transformed into customized structure which will
    // support Adjacency list to implement Dijkstra efficiently.
    // Also to remove unwanted info like type of edges, lat,longitude,etc.
    // Create a graph Data structure using Adjacency list
    CityGraph* citygraph = createGraph(total_nodes);

    //Adding edges to transformed graph
    for (int i=0;i<TN_graph.no_edges;i++){

        addEdge(citygraph,TN_graph.edges[i].nodes[0],
        	TN_graph.edges[i].nodes[1],TN_graph.edges[i].distance);
    }


    make_a_tour(citygraph,dist, predecessor_nodes, successor_nodes,city_id, no_of_cities);

 
    return 0;
}


