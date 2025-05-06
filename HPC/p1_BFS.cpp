#include <bits/stdc++.h>
#include <vector>
#include <omp.h>

using namespace std;

class Graph {
    int V;
    public:
    vector<vector<int>> adj;
        Graph(int V) {
            this->V = V;
            adj.resize(V);
        }
        void addEdge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // Parallel BFS function using OpenMP
        void parallelBFS(const vector<vector<int>>& graph, int start) {
            
            int n = graph.size();
            vector<bool> visited(n, false);   // Marks if a node has been visited
            vector<int> frontier;             // Current frontier (nodes at current level)
            
            // Start the BFS with the starting node.
            frontier.push_back(start);
            visited[start] = true;

            int level = 0;
            while (!frontier.empty()) {
                // Print the current level nodes
                cout << "Level " << level << ": ";
                for (int node : frontier) {
                    cout << node << " ";
                }
                cout << "\n";

                // Container for the next level frontier
                vector<int> next_frontier;

                // Parallel region: each thread works on a chunk of the frontier.
                #pragma omp parallel 
                {
                    // To show parallel openMP is working and multiple threads running (1) 
                    // int thread_id = omp_get_thread_num();

                    // Each thread maintains a local list of nodes discovered
                    vector<int> local_next;
                    // Distribute the current frontier across threads.
                    #pragma omp for nowait
                    for (int i = 0; i < frontier.size(); i++) {
                        int current = frontier[i];

                        // To show parallel openMP is working and multiple threads running (2) 
                        // print thread
                        // cout << "Thread: " << thread_id << " processing node: " << current << endl;
                        // To run this: g++ filename.cpp -fopenmp

                        // Visit all neighbors of current node
                        for (int neighbor : graph[current]) {
                            // Use a critical section to check and update visited.
                            // (This simple approach works well for teaching/learning purposes.
                            // In production code, you might consider more efficient parallel data structures.)
                            #pragma omp critical
                            {
                                if (!visited[neighbor]) {
                                    visited[neighbor] = true;
                                    local_next.push_back(neighbor);
                                }
                            }
                        }
                    }
                    // Merge each thread's local discovered nodes into the shared next frontier.
                    #pragma omp critical
                    {
                        next_frontier.insert(next_frontier.end(), local_next.begin(), local_next.end());
                    }
                }
                // Update the frontier to the next level and increment the level counter.
                frontier = next_frontier;
                level++;
            }
        };

        void normalBFS(vector<vector<int>> graph, int start) {
            queue<int> q;
            int n = graph.size();
            vector<bool> visited(n, false);
            visited[start] = true;
            q.push(start);
            while(!q.empty()) {
                int value = q.front();
                cout << value << " ";
                q.pop();
                for(auto it : graph[value]) {
                    if(visited[it] == false) {
                        q.push(it);
                        visited[it] = true;
                    }
                }
            }

        }

};


int main() {
    // Define an undirected graph using an adjacency list.
    // This graph example has 7 nodes (0 to 6) arranged as follows:
    //
    //          0
    //         / \
    //        1   2
    //       / \  / \
    //      3   4 5  6
    //
    // Node connections:
    // 0: 1, 2
    // 1: 0, 3, 4
    // 2: 0, 5, 6
    // 3: 1
    // 4: 1
    // 5: 2
    // 6: 2

    // vector<vector<int>> graph = {
    //     {1, 2},        // Neighbors of node 0
    //     {0, 3, 4},     // Neighbors of node 1
    //     {0, 5, 6},     // Neighbors of node 2
    //     {1},           // Neighbors of node 3
    //     {1},           // Neighbors of node 4
    //     {2},           // Neighbors of node 5
    //     {2}            // Neighbors of node 6
    // };
    
    int v, e;
    cout << "Enter vettices and edges : ";
    cin >> v >> e;
    Graph g(v);
    for(int i=0; i<e; i++)
    {
        int v, u;
        cout << "Enter edges from v to u :  ";
        cin >> v >> u;
        g.addEdge(v, u);
    }
    
    int start = 0;  // Starting node for BFS
    auto p_start = omp_get_wtime();
    g.parallelBFS(g.adj, start);
    auto p_end = omp_get_wtime();
    int p_time = p_end - p_start;

    cout << "parallel time " << p_time << endl;

    auto n_start = omp_get_wtime();
    g.normalBFS(g.adj, start);
    auto n_end = omp_get_wtime();
    int n_time = n_end - n_start;
    cout << "normmal time " << n_time << endl;
    return 0;
}

/*
Input Explanation (Graph as defined above):

- Total nodes: 7 (labeled 0 through 6)
- Edges (undirected):
    0 -- 1
    0 -- 2
    1 -- 3
    1 -- 4
    2 -- 5
    2 -- 6

Expected BFS traversal starting at node 0:
Level 0: 0 
Level 1: 1 2 
Level 2: 3 4 5 6
*/
