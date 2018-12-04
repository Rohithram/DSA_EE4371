/*
Author: Rohithram | EE16B031
Assignment 9: Supply and delivery optimization - Problem 2
*/

//Algo Explained:
/* 
1.We first read tn_graph using map header given and use only required info in the 
Residual graph created.
2.We use Dinic's Max Flow algorithm to transport the maximum number of units per hour from
A to B.
3. We obtain all paths from dinic's algo and store it in structure flowpath..
4. We find traversing time taken for each path and find min out of them and send the trucks, we repeat this
  splitting of trucks in paths which have ascending ordr of path lengths.
5. We reset all the paths status of being traversed to send more trucks than its capacity.
6. We can;t say this optimal, but it might be close to optimal since shortest traversing path
   will have least time taken to traverse alone, ignoring the time caused by bottleneck.
7. say if it's not optimal then, its case might have more tertiary roads in shortest path,
    but we see that tertiary have very less speed, so it can't have least time taken in the 1st case.
8. So we can approximate this as close to optimal algo!
*/

#include <bits/stdc++.h> 
#include "graph.h"

using namespace std; 

// Function return capacity of road based on road type
int capacity(char road_type,int prop[2]){
    // int *prop[2];
    if(road_type=='p')
    {
        prop[0] = 100;
        prop[1] = 60;
    }
    else if(road_type=='s'){
        prop[0] = 50;
        prop[1] = 40;
    }
    else if(road_type=='t'){
        prop[0] = 20;
        prop[1] = 30;
    }

    return 0;
}

//Note : 
// Dinic Max Flow algorithm is referred from www.geeksforgeeks.com

// A structure to represent a edge between 
// two vertex 
struct Road 
{ 
    int id;
    int v ; // Vertex v (or "to" vertex) 
            // of a directed edge u-v
    int flow ; // flow of data in edge 
    int C; // capacity 
    double length;
    int speed;
    int rev ; // To store index of reverse 
}; 


// Structure to represent edges with flows.
typedef struct 
{   
    int id;
    int path_id = -1;  // which path flow belong to
    int u;
    int v;
    int length;
    int speed;
    int flow;
    int C;

}flow_edge;


// Structure to represent all edges in a path 
typedef struct
{
    vector<flow_edge> path_edges;
    double time_taken=0;
    bool traversed = false;
}flow_path;


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
    void addRoad(int id,int u, int v, int C, int speed,double length) 
    { 
        // Forward edge : 0 flow and C capacity 
        Road a{id,v, 0, C, length,speed,adj[v].size()}; 
        // Back edge : 0 flow and 0 capacity 
        Road b{id,u, 0, 0,length,speed, adj[u].size()}; 
        adj[u].push_back(a); 
        adj[v].push_back(b); // reverse edge 
    } 

    bool BFS(int s, int t); 
    int sendFlow(int s, int flow, int t, int ptr[], vector<flow_edge> &flow_edges); 
    int DinicMaxflow(int s, int t,vector<flow_edge> &flow_edges); 
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
//       start[i] stores count of edges explored 
//       from i.
// u is source and t is sink
int Resid_Graph::sendFlow(int u, int flow, int t, int start[],vector<flow_edge> &flow_edges) 
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

            int temp_flow = sendFlow(e.v, curr_flow, t, start,flow_edges); 

            // flow is greater than zero 
            if (temp_flow > 0) 
            { 

                // add flow to current edge 
                e.flow += temp_flow; 
                 
                flow_edge f;
                f.u = u;
                f.v = e.v;
                f.length = e.length;
                f.id = e.id;
                f.flow = temp_flow;
                f.C = e.C;
                f.speed = e.speed;

                flow_edges.push_back(f);

                
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
int Resid_Graph::DinicMaxflow(int s, int t,vector<flow_edge> &flow_edges) 
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
        while (int flow = sendFlow(s, INT_MAX, t, start,flow_edges)) 

            // Add path flow to overall flow 
            total += flow; 
    } 

    // return maximum flow 
    return total; 
} 

//Function to segregate the random flow edges into different paths 
// And assigns path id to each edge in the path.
vector<flow_path> split_into_flow_paths(vector<flow_edge> &flow_edges, int s){
    
    vector<flow_path> flow_paths;

    int j;
    int ctr =0;
    for (int i = 0; i < flow_edges.size(); ++i)
    {
        flow_path p;

        j = i;
        while(flow_edges[j].u != s){
            flow_edges[j].path_id = ctr;
            p.path_edges.push_back(flow_edges[j]);
            j++;
        }

        flow_edges[j].path_id = ctr;
        p.path_edges.push_back(flow_edges[j]);
        flow_paths.push_back(p);
        ctr++;
        i = j;
    }

    return flow_paths;
}

// Updates flow edges with amount flowing in them after each iteration
int update_path_edges(vector<flow_path> &flow_paths, int path_id,vector<int>& final_flow_edges,
                    Graph tn_graph,int flow_amount){

    double min_cap = 100.0;

    // update the flows in each edge
    for (int i = 0; i < flow_paths[path_id].path_edges.size(); ++i)
    {   

        // to find the bottleneck capacity along the path
        if(flow_paths[path_id].path_edges[i].C < min_cap){
            min_cap = flow_paths[path_id].path_edges[i].C;
        }

        final_flow_edges[flow_paths[path_id].path_edges[i].id] += flow_amount;
        tn_graph.edges[flow_paths[path_id].path_edges[i].id].nodes[0] = flow_paths[path_id].path_edges[i].u;
        tn_graph.edges[flow_paths[path_id].path_edges[i].id].nodes[1] = flow_paths[path_id].path_edges[i].v;
    }

    // printf("%.2f\n",(((flow_amount)/(min_cap))));

    // Adds time taken to send given flow amt in the path
    flow_paths[path_id].time_taken += (((flow_amount)/(min_cap))); 


    return 0;
}


// Function to split truck optimally and finds time taken to transport X trucks.
int split_truck_optim(int X,vector<flow_edge> &flow_edges,Graph tn_graph, int s){

    vector<int> path_lengths;
    vector<flow_path> flow_paths = split_into_flow_paths(flow_edges,s);
    vector<int> final_flow_edges(tn_graph.no_edges);
    int no_paths = flow_paths.size();

    double* path_times  = (double*)malloc(sizeof(double)*no_paths);

    // Calculating traversing time taken for each path.
    for(int k=0;k<flow_paths.size();k++){

        for(int i=0;i<flow_paths[k].path_edges.size();i++){
            // adding time taken by length of roads.
            path_times[k]+= (flow_paths[k].path_edges[i].length)/(flow_paths[k].path_edges[i].speed);

        }
    }


    int transported_trucks = 0;

    while(transported_trucks < X){

        // pick the path with minimum traversing time
        double min = INT_MAX;
        int min_path_id = -1;

        // finds shortest traversing path
        for (int i = 0; i < no_paths; ++i)
        {
            if(!flow_paths[i].traversed){


                if(path_times[i] < min){
                    min = path_times[i];
                    min_path_id = i;
                }
            }
        }

        // We reset all paths for the case where X > capacities of Roads.
        if(min_path_id==-1){
            for(int i=0;i<no_paths;i++){
                flow_paths[i].traversed = false;
            }
            continue;
        }

        flow_paths[min_path_id].traversed = true;

        // printf("No of paths %d\n",no_paths );
        // for(int k=0;k<flow_paths.size();k++){

        int flow_amount = flow_paths[min_path_id].path_edges[0].flow;
        if(flow_amount < (X-transported_trucks)){
            flow_amount = flow_amount;
        }
        else{
            flow_amount = X-transported_trucks;
        }

        update_path_edges(flow_paths,min_path_id,final_flow_edges,tn_graph,flow_amount);
        transported_trucks += flow_amount;
    
        // printf("%d\n",transported_trucks );
        
    }

    //Calculating Maximum Time Taken
    double max_timetaken = 0;    
    int max_time_id = -1;

    for(int k=0;k<flow_paths.size();k++){

        if(flow_paths[k].traversed || flow_paths[k].time_taken>0){
                // adding time taken by length of roads.
                flow_paths[k].time_taken += path_times[k];
            }

            // printf("time taken by path %d: %.2f\n",k+1,flow_paths[k].time_taken);

            if(flow_paths[k].time_taken > max_timetaken){
                max_time_id = k;    
                max_timetaken = flow_paths[k].time_taken;
            }
    }
    
    // printf("Max time taken : %.2f\n",max_timetaken );

    //Printing the Edges in the output format given.
    for (int i = 0; i < tn_graph.no_edges; ++i)
    {
        if(final_flow_edges[i] > 0){
            printf("%ld %ld %d\n",tn_graph.edges[i].nodes[0],tn_graph.edges[i].nodes[1],final_flow_edges[i]);
        }
    }

    return 0;   
}

int main() 
{ 
    // Reading graph using 'graph.h'
    Graph tn_graph;
    tn_graph.read_graph("./tn_graph.txt");

    Resid_Graph tn_res_graph(tn_graph.no_nodes);


    //source and sink
    int s,t;
    int X; 

    scanf("%d",&s);
    scanf("%d",&t);
    scanf("%d",&X);

    //road properties such as capacity and speed.
    int road_props[2];
    int cap,speed;

    for(int i=0;i<tn_graph.no_edges;i++){

        capacity(tn_graph.edges[i].type,road_props);
        cap = road_props[0];
        speed = road_props[1];
        //Since the tn graph is undirected , edges are twice to residual graph,
        //inorder to account for undirectedness.
        tn_res_graph.addRoad(i,tn_graph.edges[i].nodes[0],tn_graph.edges[i].nodes[1],cap,
                            speed,tn_graph.edges[i].distance);
        tn_res_graph.addRoad(i,tn_graph.edges[i].nodes[1],tn_graph.edges[i].nodes[0],cap,
                            speed,tn_graph.edges[i].distance);

    }

    vector<flow_edge> flow_edges;

    int max_flow = tn_res_graph.DinicMaxflow(s, t,flow_edges); 
    // printf("Maximum flow %d\n", max_flow);

    split_truck_optim(X,flow_edges,tn_graph,s);
    
    return 0; 
} 