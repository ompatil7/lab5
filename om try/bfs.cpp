#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
using namespace std;

class Graph {
public:
    int V;
    vector<vector<int>> adj;
    
    Graph(int numVertices) {
        V = numVertices;
        adj.resize(V);
    }
    
    // Add an undirected edge
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void parallelBFS(int start) {
        int n = adj.size();
        vector<int> visited(n, false);
        
        vector<int> frontier;
        frontier.push_back(start);
        visited[start] = true;

        int level = 0;
        while(!frontier.empty())
        {
            cout << "Level " << level;
            
            for(int node : frontier) {
                cout << " " << node;
            }
            cout << endl;

            vector<int> next_frontier;

            #pragma omp parallel
            {
                vector<int> local_next;
                
                #pragma omp for nowait
                for(int i=0; i<frontier.size(); i++)
                {
                    int current = frontier[i];

                    for(int neigbor : adj[current]) {


                        #pragma omp critical
                        {
                            if(!visited[neigbor]){
                                visited[neigbor] = true;
                                local_next.push_back(neigbor);
                            }
                        }
                    }
                }
                #pragma omp critical
                {
                    next_frontier.insert(next_frontier.end(), local_next.begin(), local_next.end());
                }
            }
            level++;
            frontier = next_frontier;
        }
    }

    void BFS(int start)
    {
        int n = adj.size();
        vector<int> visited(n, false);

        queue<int> q;
        q.push(start);
        visited[start] = true;

        while(!q.empty())
        {
            int node = q.front();
            q.pop();
            cout << node << " ";
            
            for(int n : adj[node]) {
                if(!visited[n]) {
                    visited[n] = true;
                    q.push(n);
                }
            }
        }
    }
};

int main() {
    int numVertices, numEdges;
    cout << "Enter number of vertices and edges: ";
    cin >> numVertices >> numEdges;
    Graph g(numVertices);

    for (int i = 0; i < numEdges; i++) {
        int u, v;
        cout << "Enter edge (u v): ";
        cin >> u >> v;
        // Use addEdge(u, v) to preserve the intended order.
        g.addEdge(u, v);
    }
    
    int start;
    cout << "Enter start vertex: ";
    cin >> start;
    auto st = chrono::high_resolution_clock::now();
    g.parallelBFS(start);
    auto et = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durEt = et - st;

    cout << "timme " << durEt.count() << endl;


    auto st2 = chrono::high_resolution_clock::now();
    g.BFS(start);
    auto et2 = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durEt2 = et2 - st2;

    cout << "timme " << durEt2.count();
    return 0;
}
