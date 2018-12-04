/*
Author: Rohithram | EE16B031
Assignment 8: Supply and delivery optimization - Problem 1
*/

//Algo Explained:
/* 
1.We first read tn_graph using map header given and use only required info in the 
Residual graph created.
2.We use Dinic's Max Flow algorithm to transport the maximum number of units per hour from
A to B.
*/

#include <bits/stdc++.h> 
#include "graph.h"

using namespace std; 

// Function return capacity of road based on road type
int capacity(char road_type){
	int cap;
	if(road_type=='p')
		cap = 100;
	else if(road_type=='s')
		cap = 50;
	else if(road_type=='t')
		cap = 20;

	return cap;
}

//Note : 
// Dinic Max Flow algorithm is referred from www.geeksforgeeks.com

// A structure to represent a edge between 
// two vertex 
struct Road 
{ 
	int v ; // Vertex v (or "to" vertex) 
			// of a directed edge u-v

	int flow ; // flow of data in edge 

	int C; // capacity 

	int rev ; // To store index of reverse 
			// edge in adjacency list so that 
			// we can quickly find it. 
}; 

// Residual Resid_Graph 
class Resid_Graph 
{ 
	int V; // number of vertex 
	int *level ; // stores level of a node 


public : 
	// Adjaceny list
	vector< Road > *adj; 

	Resid_Graph(int V) 
	{ 
		adj = new vector<Road>[V]; 
		this->V = V; 
		level = new int[V]; 
	} 

	// add edge to the graph 
	void addRoad(int u, int v, int C) 
	{ 
		// Forward edge : 0 flow and C capacity 
		Road a{v, 0, C, adj[v].size()}; 
		// Back edge : 0 flow and 0 capacity 
		Road b{u, 0, 0, adj[u].size()}; 
		adj[u].push_back(a); 
		adj[v].push_back(b); // reverse edge 
	} 

	bool BFS(int s, int t); 
	int sendFlow(int s, int flow, int t, int ptr[]); 
	int DinicMaxflow(int s, int t); 
}; 

// Finds if more flow can be sent from s to t. 
// Also assigns levels to nodes. 
bool Resid_Graph::BFS(int s, int t) 
{ 
	for (int i = 0 ; i < V ; i++) 
		level[i] = -1; 

	level[s] = 0; // Level of source vertex 

	// Create a queue, enqueue source vertex 
	// and mark source vertex as visited here 
	list< int > q; 
	q.push_back(s); 

	vector<Road>::iterator i ; 

	while (!q.empty()) 
	{ 
		int u = q.front(); 
		q.pop_front(); 

		for (i = adj[u].begin(); i != adj[u].end(); i++) 
		{ 
			Road &e = *i; 
			
			if (level[e.v] < 0 && e.flow < e.C) 
			{ 	
			
				// Level of current vertex is, 
				// level of parent + 1 
				level[e.v] = level[u] + 1; 
				q.push_back(e.v); 
			} 
		} 
	} 

	// IF we can not reach to the sink we 
	// return false else true 
	return level[t] < 0 ? false : true ; 
} 

// flow : Current flow send by parent function call 
// start : array to keep track of next edge to be explored. 
//		 start[i] stores count of edges explored 
//		 from i.
// u is source and t is sink
int Resid_Graph::sendFlow(int u, int flow, int t, int start[]) 
{ 
	// Sink reached 
	if (u == t) 
		return flow; 

	// Traverse all adjacent edges one -by - one. 
	for ( ; start[u] < adj[u].size(); start[u]++) 
	{ 
		// Pick next edge from adjacency list of u 
		Road &e = adj[u][start[u]]; 
									
		if (level[e.v] == level[u]+1 && e.flow < e.C) 
		{ 
			// find minimum flow from u to t 
			int curr_flow = min(flow, e.C - e.flow); 

			int temp_flow = sendFlow(e.v, curr_flow, t, start); 

			// flow is greater than zero 
			if (temp_flow > 0) 
			{ 
				// add flow to current edge 
				e.flow += temp_flow; 

				// subtract flow from reverse edge 
				// of current edge 
				adj[e.v][e.rev].flow -= temp_flow;
				return temp_flow; 
			} 
		} 
	} 

	return 0; 
} 

// Returns maximum flow in graph 
int Resid_Graph::DinicMaxflow(int s, int t) 
{ 
	// Corner case 
	if (s == t) 
		return -1; 

	int total = 0; // Initialize result 
	
	// Augment the flow while there is path 
	// from source to sink 
	while (BFS(s, t) == true) 
	{ 
		// store how many edges are visited 
		// from V { 0 to V } 
		int *start = new int[V+1]; 

		// while flow is not zero in graph from S to D 
		while (int flow = sendFlow(s, INT_MAX, t, start)) 

			// Add path flow to overall flow 
			total += flow; 
	} 

	// To print the edges in the paths which gives max flow.
	for (int u=0;u<V;u++)
    {
        for (int j=0;j<adj[u].size();j++)
        {   
            if (adj[u][j].flow > 0){
                printf("%d %d %d\n",u,adj[u][j].v,adj[u][j].flow);
            }
        }
    }

	// return maximum flow 
	return total; 
} 


int main() 
{ 
	// Reading graph using 'graph.h'
    Graph tn_graph;
    tn_graph.read_graph("./tn_graph.txt");

	Resid_Graph tn_res_graph(tn_graph.no_nodes);

	//capacity of a road
	int cap;

	//source and sink
	int s,t;

	scanf("%d",&s);
	scanf("%d",&t);


	for(int i=0;i<tn_graph.no_edges;i++){

		cap = capacity(tn_graph.edges[i].type);
		//Since the tn graph is undirected , edges are twice to residual graph,
		//inorder to account for undirectedness.
		tn_res_graph.addRoad(tn_graph.edges[i].nodes[0],tn_graph.edges[i].nodes[1],cap);
		tn_res_graph.addRoad(tn_graph.edges[i].nodes[1],tn_graph.edges[i].nodes[0],cap);

	}

	int max_flow = tn_res_graph.DinicMaxflow(s, t); 
	// printf("Maximum flow %d\n", max_flow);
	return 0; 
} 
