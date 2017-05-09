// Implementation of push-relabel algorithm in C++.
// For more information on algorithm visit
// https://en.wikipedia.org/wiki/Push-relabel_maximum_flow_algorithm
// link coming...

//================================================================================================
//
//        LIBRARIES AND DEFINITIONS
//
//================================================================================================

#include<iostream>
#include<vector>
#include<climits>
#include<cstdio>

using namespace std;

// Vertex and Edge are both represented as a struct.
struct Vertex
{
    int h, e;
    
    Vertex(int h, int e)
    {
        this->h = h;
        this->e = e;
    }
};

struct Edge
{
    int u, v;
    int c;
    int f;
    
    Edge(int u, int v, int c, int f)
    {
        this->u = u;
        this->v = v;
        this->c = c;
        this->f = f;
    }
};

// Prototypes.
int push_relabel();
void init_preflow();
int push(int u);
void relabel(int u);
int get_overflowing_vertex();
void update_reverse_edge(int i, int delta);
void populate_graph();

// Global variables.
int V; // Number of vertices.
vector<Vertex> vertices;
vector<Edge> edges;

//================================================================================================
//
//        MAIN
//
//================================================================================================

// Main function used for testing the algorithm.
int main()
{
    // Read from standard input (or pipe a file) and
    // populate edges and vertices.
    populate_graph();
    
    // Execute push-relabel algorithm and print result.
    cout << "Maximum flow through given network is " << push_relabel() << endl;
}

//================================================================================================
//
//        ALGORITHM
//
//================================================================================================

// The push-relabel algorithm function. It initializes
// preflow and executes either push or relabel operation
// while there is an overflowing vertex (vertex with excess
// flow) that is not source nor terminal (sink).
int push_relabel()
{
    // Initialize preflow.
    init_preflow();
    
    // While there is a vertex with overflow.
    while (get_overflowing_vertex() != -1)
    {
        int u = get_overflowing_vertex();
        
        // Push if you can, if not, relabel the overflowing vertex.
        if (!push(u))
            relabel(u);
    }
    
    // Return the overflow of last vertex (sink).
    return vertices[V-1].e;
}

// Initializes preflow. It sets the height of source to
// number of vertices, pushes flow over all edges from source
// and adds edges in residual graph.
void init_preflow()
{
    // Set height of source to number of vertices.
    vertices[0].h = V;
    
    for (int i = 0; i < edges.size(); i++)
    {
        // If the edge starts in source (source is 0)...
        if (edges[i].u == 0)
        {
            // Saturate the edge.
            edges[i].f = edges[i].c;
            
            // Set excess flow of vertex adjacent to source
            // to flow we just pushed over the edge.
            vertices[edges[i].v].e = edges[i].f;
            
            // Add edge from adjacent to source in residual graph
            // and set its capacity to 0 and its flow to negative
            // flow that we just pushed from source to adjacent.
            edges.push_back(Edge(edges[i].v, 0, 0, -edges[i].f));
        }
    }
}

// Push operation. First check if the edge starts in u, if the
// edge is not saturated and if the height of u is higher than
// the height of adjacent vertex. If so, execute the push operation.
int push(int u)
{
    for (int i = 0; i < edges.size(); i++)
    {
        // Assert that requirements are fulfilled.
        if (edges[i].u == u && edges[i].f < edges[i].c && vertices[u].h == vertices[edges[i].v].h +1)
        {
            // We can only push the minimum of excess in u and the
            // remaining capacity of edge between u and adjacent.
            int delta = min(vertices[u].e, edges[i].c - edges[i].f);
            
            // The excess of u is smaller for the ammount we pushed.
            vertices[u].e -= delta;
        
            // We have more excess in adjacent.
            vertices[edges[i].v].e += delta;
        
            // Flow over the edge is greater.
            edges[i].f += delta;
        
            // Update flow over the edge from adjacent to u.
            // If the edge does not exist, add it in residual graph.
            update_reverse_edge(i, delta);
        
            // Push successful.
            return 1;
        }
    }
    
    // We could not find the edge over which we could push.
    return 0;
}

// Relabel operation. Set the height of u to minimum of heights of adjacent vertices plus 1.
void relabel(int u)
{
    // Make minimum height of adjacent vertices something big.
    int min_height_adj = INT_MAX;
    
    for (int i = 0; i < edges.size(); i++)
    {
        // Assert we have the edge that starts in u, has flow that is smaller
        // than its capacity. If the height of adjacent is smaller than current
        // minimum height of adjacent, then change the heights.
        if (edges[i].u == u && edges[i].f < edges[i].c && vertices[edges[i].v].h < min_height_adj)
            min_height_adj = vertices[edges[i].v].h;
    }
    
    // Change the height of u to minimum height of adjacent vertices plus 1.
    vertices[u].h = min_height_adj + 1;
}

//================================================================================================
//
//        SUPPORTING FUNCTIONS
//
//================================================================================================

// Finds the vertex with excess and return its index.
// If there are no such vertices, return sentinel.
int get_overflowing_vertex()
{
    for (int u = 1; u < V-1; u++)
    {
        if (vertices[u].e > 0)
            return u;
    }
    
    return -1;
}

// Updates flow on reverse edge. If the edge does not exist,
// create an edge in residual flow.
void update_reverse_edge(int i, int delta)
{
    for (int j = 0; j < edges.size(); j++)
    {
        // Check if the edge is reverse.
        if (edges[i].u == edges[j].v && edges[i].v == edges[j].u)
        {
            // Subtract the flow we pushed and return.
            edges[j].f -= delta;
            return;
        }
    }
    
    // Add edge in residual graph with capacity of flow we pushed.
    edges.push_back(Edge(edges[i].v, edges[i].u, 0, -delta));
}


// Reads graph data from standard input and populates vectors.
void populate_graph()
{
    scanf("%d\n", &V);
    for (int i = 0; i < V; i++)
        vertices.push_back(Vertex(0, 0));
    
    int u, v, c;
    while (scanf("%d %d %d\n", &u, &v, &c) != EOF)
        edges.push_back(Edge(u, v, c, 0));
}

