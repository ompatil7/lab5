#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

// Parallel DFS function using OpenMP
void parallelDFS(const vector<vector<int>>& graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);   // Marks if a node has been visited
    stack<int> dfs_stack;             // DFS stack
    
    // Start the DFS with the starting node.
    dfs_stack.push(start);
    visited[start] = true;
    
    int depth = 0;
    while (!dfs_stack.empty()) {
        // Print the current depth nodes
        cout << "Depth " << depth << ": ";
        
        // Create a vector of current nodes to process in parallel
        vector<int> current_nodes;
        int stack_size = dfs_stack.size();
        
        for (int i = 0; i < stack_size; i++) {
            int node = dfs_stack.top();
            dfs_stack.pop();
            current_nodes.push_back(node);
            cout << node << " ";
        }
        cout << "\n";
        
        // Container for the next batch of nodes
        vector<int> next_nodes;

        // Parallel region: each thread works on a node from current_nodes
        #pragma omp parallel
        {
            // To show parallel OpenMP is working and multiple threads running (1) 
            // int thread_id = omp_get_thread_num();

            // Each thread maintains a local list of nodes discovered
            vector<int> local_next;
            
            // Distribute the current nodes across threads
            #pragma omp for nowait
            for (int i = 0; i < current_nodes.size(); i++) {
                int current = current_nodes[i];

                // To show parallel OpenMP is working and multiple threads running (2) 
                // print thread
                // cout << "Thread: " << thread_id << " processing node: " << current << endl;
                // To run this: g++ filename.cpp -fopenmp

                // Visit all neighbors of current node (in reverse to maintain DFS order)
                for (int j = graph[current].size() - 1; j >= 0; j--) {
                    int neighbor = graph[current][j];
                    // Use a critical section to check and update visited
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            local_next.push_back(neighbor);
                        }
                    }
                }
            }
            
            // Merge each thread's local discovered nodes into the shared next nodes
            #pragma omp critical
            {
                next_nodes.insert(next_nodes.end(), local_next.begin(), local_next.end());
            }
        }
        
        // Add all new nodes to the stack (in reverse to maintain DFS order)
        for (int i = next_nodes.size() - 1; i >= 0; i--) {
            dfs_stack.push(next_nodes[i]);
        }
        
        depth++;
    }
}

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

    vector<vector<int>> graph = {
        {1, 2},        // Neighbors of node 0
        {0, 3, 4},     // Neighbors of node 1
        {0, 5, 6},     // Neighbors of node 2
        {1},           // Neighbors of node 3
        {1},           // Neighbors of node 4
        {2},           // Neighbors of node 5
        {2}            // Neighbors of node 6
    };

    int start = 0;  // Starting node for DFS
    parallelDFS(graph, start);

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

Expected DFS traversal starting at node 0:
(One possible traversal, actual order may vary)
Depth 0: 0
Depth 1: 2 1
Depth 2: 6 5 4 3
*/