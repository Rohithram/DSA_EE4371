/*
Author: Rohithram | EE16B031
Assignment 6: Clock Tree Routing
Note: Comment out printf line after printing edges in function clock_route
to view total length of the MST.
*/

//Algo Explained:
/* 
1.we add clock and N flipflops to graph which uses Adjacency List representation
2.And then we connect each flipflop to every other flipflop since graph is unconnected
initially , so we need to explore all possibilities.
3.And we add edges in both directions since its undirected graph
4.Now we run PRIMs Algorithm to construct Minimum spanning tree with manhattan distance
as metric to find distance
5.We dont any new nodes, since distance is calculated using manhattan metric instead of
Euclidean distance	


*/

#include <bits/stdc++.h>
#include <stdio.h> 
#include <stdlib.h> 

//Function to calculate manhattan distance between two nodes.
int find_man_dist(int x1, int y1,int x2, int y2){
    return abs(x1-x2)+abs(y1-y2);
}

//Note:  Prim's algorithm using adjacency list representation is referred 
// from www.geeksforgeeks.com


// A structure to represent a node in adjacency list 
struct AdjListNode { 
    int x_pos;    // x coordinate of node
    int y_pos;	  // y coordinate of node
   	int destination;
   	int weight;    // Manhattan distance between flipflops
	struct AdjListNode* next; 
}; 

// A structure to represent an adjacency list 
struct AdjList { 
	struct AdjListNode* head; // pointer to head node of list 
}; 

// A structure to represent a graph.
struct Graph { 
	int V; 
	struct AdjList* array; 
}; 

struct AdjListNode* newAdjListNode( int x_pos, int y_pos,int destination, int weight) 
{ 
	struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode)); 
	newNode->x_pos = x_pos;
    newNode->y_pos = y_pos;
	newNode->destination = destination; 
	newNode->weight = weight; 
	newNode->next = NULL; 
    
	return newNode; 
} 

struct Graph* createGraph(int V) 
{ 
	struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph)); 
	graph->V = V; 

	// Create an array of adjacency lists. Size of array will be V 
	graph->array = (struct AdjList*)malloc(V * sizeof(struct AdjList)); 

	for (int i = 0; i < V; ++i) 
		graph->array[i].head = NULL; 

	return graph; 
} 

// Note : Minheap is referred from www.geeksforgeeks.com

// Structure to represent a min heap node 
struct MinHeapNode { 
	int v; 
	int key; 
}; 

// Structure to represent a min heap 
struct MinHeap { 
	int size; // Number of heap nodes present currently 
	int capacity;  
	int* pos; 
	struct MinHeapNode** array; 
}; 

// A utility function to create a new Min Heap Node 
struct MinHeapNode* newMinHeapNode(int v, int key) 
{ 
	struct MinHeapNode* minHeapNode = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode)); 
	minHeapNode->v = v; 
	minHeapNode->key = key; 
	return minHeapNode; 
} 

// A utilit function to create a Min Heap 
struct MinHeap* createMinHeap(int capacity) 
{ 
	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap)); 
	minHeap->pos = (int*)malloc(capacity * sizeof(int)); 
	minHeap->size = 0; 
	minHeap->capacity = capacity; 
	minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*)); 
	return minHeap; 
} 

// A utility function to swap two nodes of min heap. Needed for min heapify 
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) 
{ 
	struct MinHeapNode* t = *a; 
	*a = *b; 
	*b = t; 
} 

void minHeapify(struct MinHeap* minHeap, int idx) 
{ 
	int smallest, left, right; 
	smallest = idx; 
	left = 2 * idx + 1; 
	right = 2 * idx + 2; 

	if (left < minHeap->size && minHeap->array[left]->key < minHeap->array[smallest]->key) 
		smallest = left; 

	if (right < minHeap->size && minHeap->array[right]->key < minHeap->array[smallest]->key) 
		smallest = right; 

	if (smallest != idx) { 
		// The nodes to be swapped in min heap 
		MinHeapNode* smallestNode = minHeap->array[smallest]; 
		MinHeapNode* idxNode = minHeap->array[idx]; 

		// Swap positions 
		minHeap->pos[smallestNode->v] = idx; 
		minHeap->pos[idxNode->v] = smallest; 

		// Swap nodes 
		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]); 

		minHeapify(minHeap, smallest); 
	} 
} 

// A utility function to check if the given minHeap is ampty or not 
int isEmpty(struct MinHeap* minHeap) 
{ 
	return minHeap->size == 0; 
} 

struct MinHeapNode* extractMin(struct MinHeap* minHeap) 
{ 
	if (isEmpty(minHeap)) 
		return NULL; 

	// Store the root node 
	struct MinHeapNode* root = minHeap->array[0]; 

	// Replace root node with last node 
	struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1]; 
	minHeap->array[0] = lastNode; 

	// Update position of last node 
	minHeap->pos[root->v] = minHeap->size - 1; 
	minHeap->pos[lastNode->v] = 0; 

	// Reduce heap size and heapify root 
	--minHeap->size; 
	minHeapify(minHeap, 0); 

	return root; 
} 

// Function to decreasy key value of a given vertex v. This function 
// uses pos[] of min heap to get the current index of node in min heap 
void decreaseKey(struct MinHeap* minHeap, int v, int key) 
{ 
	// Get the index of v in heap array 
	int i = minHeap->pos[v]; 

	// Get the node and update its key value 
	minHeap->array[i]->key = key; 

	while (i && minHeap->array[i]->key < minHeap->array[(i - 1) / 2]->key) { 
		// Swap this node with its parent 
		minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2; 
		minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i; 
		swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]); 

		// move to parent index 
		i = (i - 1) / 2; 
	} 
} 

bool isInMinHeap(struct MinHeap* minHeap, int v) 
{ 
	if (minHeap->pos[v] < minHeap->size) 
		return true; 
	return false; 
} 


void addEdge(struct Graph* graph, int src, int src_x, 
			int src_y, int destination, int dest_x, int dest_y,int distance) 
{ 
	 
    // First we add forward edge from src to dest
	struct AdjListNode* newNode = newAdjListNode( dest_x, dest_y,destination, distance); 
	newNode->next = graph->array[src].head; 
	graph->array[src].head = newNode; 

	// Graph is undirected, so add an edge in reverse direction
	newNode = newAdjListNode(src_x, src_y,src, distance); 
	newNode->next = graph->array[destination].head; 
	graph->array[destination].head = newNode; 
} 



// Constructs MST using Prim's algorithm 
void PrimMST(struct Graph* graph, int parent[]) 
{ 
	int V = graph->V; 
	int key[V]; // Key used to pick minimum weight edge in cut 

	// minHeap represents set E 
	struct MinHeap* minHeap = createMinHeap(V); 

	// Initialize min heap with all vertices. Key value of 
	// all vertices (except 0th vertex) is initially infinite 
	for (int v = 1; v < V; ++v) { 
		parent[v] = -1; 
		key[v] = INT_MAX; 
		minHeap->array[v] = newMinHeapNode(v, key[v]); 
		minHeap->pos[v] = v; 
	} 

	// Make key value of 0th vertex as 0 so that it 
	// is extracted first 
	key[0] = 0; 
	minHeap->array[0] = newMinHeapNode(0, key[0]); 
	minHeap->pos[0] = 0; 

	// Initially size of min heap is equal to V 
	minHeap->size = V; 

	while (!isEmpty(minHeap)) { 
		// Extract the vertex with minimum key value 
		struct MinHeapNode* minHeapNode = extractMin(minHeap); 
		int u = minHeapNode->v; // Store the extracted vertex number 

		
		struct AdjListNode* pCrawl = graph->array[u].head; 
		while (pCrawl != NULL) { 
			int v = pCrawl->destination; 

			if (isInMinHeap(minHeap, v) && pCrawl->weight < key[v]) { 
				key[v] = pCrawl->weight; 
				parent[v] = u; 
				decreaseKey(minHeap, v, key[v]); 
			} 
			pCrawl = pCrawl->next; 
		} 
	} 

	
} 


int clock_route(int clock_x,int clock_y,int grid_x,int grid_y, int n_flops){

	int x1,y1,x2,y2;

	//vector to store coordinates of clock and flipflops
    int *pos_x = (int*) malloc(sizeof(int)*(n_flops+1));
    int* pos_y = (int*)malloc(sizeof(int)*(n_flops+1));  

    //updating 0th index to position of clock
    pos_x[0] = clock_x;
    pos_y[0] = clock_y;

    int distance;
    // N+1 nodes since it clock also in the graph
	struct Graph* graph = createGraph(n_flops+1); 
	// from index 1 pos_x and pos_y stores coordinates of flipflops
    for (int i = 1; i <= n_flops;i++)
    {
        scanf("%d %d\n",&pos_x[i],&pos_y[i]);

        //Loop to add edges from current node to all existing nodes in graph
        for (int j = 0;j < i; j++){
        	
    		distance = find_man_dist(pos_x[i],pos_y[i],pos_x[j],pos_y[j]);

            addEdge(graph,i, pos_x[i],pos_y[i],j,pos_x[j],pos_y[j],distance);
        }
    }
  
  	// array to store the parent of each node
  	// used to print the edges in final MST.
    int * parent = (int*)malloc(sizeof(int)*(n_flops+1));

    // Constructs MST using prims algo with minheap
	PrimMST(graph,parent); 
    
	//Since no new nodes are added
	printf("0\n");
	//no of edges will be no of flipflops
	printf("%d\n",n_flops);

	//total length tree spans
	int total_length =0;

	for (int i = 1; i <= n_flops;i++)
	{	
		x1 = pos_x[i];
		// position of parent of current node
		x2 = pos_x[parent[i]];
		
		y1 = pos_y[i];
		// position of parent of current node
		y2 = pos_y[parent[i]];
		
		total_length+=find_man_dist(x1,y1,x2,y2);
		// printing edges in final MST
		printf("%d %d\n",i,parent[i]);  
	}

	// total length tree spans
	// printf("%d\n",total_length);

    return 0;
}

int main() 
{ 

	// gridsize in x and y directions
	int grid_x,grid_y;

    scanf("%d %d\n",&grid_x,&grid_y);

    // position of clock
    int clock_x,clock_y;
    scanf("%d %d\n",&clock_x,&clock_y);

    int n_flops;
    scanf("%d\n",&n_flops);

    // function for clockrouting with min manhattan distance
    clock_route(clock_x,clock_y,grid_x,grid_y,n_flops);


   return 0; 
} 
