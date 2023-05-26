#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

class Graph
{
public:
    vector<vector<int>> graph;
    vector<bool> visited;
    int vertices = 0;
    int edges = 0;

    Graph()
    {
        cout << "\nEnter number of nodes: ";
        cin >> vertices;
        cout << "\nEnter number of edges: ";
        cin >> edges;

        graph.assign(vertices, vector<int>());
        int x, y;
        for (int i = 0; i < edges; ++i)
        {
            cout << "\nEnter edge: ";
            cin >> x >> y;
            addEdge(x, y);
        }
    }

    void addEdge(int x, int y)
    {
        graph[x].push_back(y);
        graph[y].push_back(x);
    }

    void printGraph()
    {
        for (int i = 0; i < vertices; ++i)
        {
            cout << i << " -> ";
            for (auto j = graph[i].begin(); j != graph[i].end(); j++)
                cout << *j << " ";
            cout << endl;
        }
    }

    void initializeVisited()
    {
        visited.assign(vertices, false);
    }

    void bfs(int i)
    {
        queue<int> q;
        q.push(i);
        visited[i] = true;

        while (!q.empty())
        {
            int curr = q.front();
            cout << curr << " ";
            q.pop();

            for (int j = 0; j < graph[curr].size(); ++j)
            {
            	int x = graph[curr][j];
                if (!visited[x])
                {
                    visited[x] = true;
                    q.push(x);
                }
            }
        }
    }

    void parallel_bfs(int i)
    {
        queue<int> q;
        q.push(i);
        visited[i] = true;

        while (!q.empty())
        {
            int curr = q.front();
            cout << curr << " ";
#pragma omp critical
            q.pop();

#pragma omp parallel for
            for (int j = 0; j < graph[curr].size(); ++j)
            {
            	int x = graph[curr][j];
                if (!visited[x])
#pragma omp critical
                {
                    visited[x] = true;
                    q.push(x);
                }
            }
        }
    }
};

int main()
{
    Graph g;
    cout << "\nAdjacency List:" << endl;
    g.printGraph();
    cout << endl;

    g.initializeVisited();
    cout << "\nNormal BFS: ";
    auto start = chrono::high_resolution_clock::now();
    g.bfs(0);
    auto end = chrono::high_resolution_clock::now();
    cout << "\nTime taken = " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;

    g.initializeVisited();
    cout << "\nParallel BFS: ";
    start = chrono::high_resolution_clock::now();
    g.parallel_bfs(0);
    end = chrono::high_resolution_clock::now();
    cout << "\nTime taken = " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;
}
