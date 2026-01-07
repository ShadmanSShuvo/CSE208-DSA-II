#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

vector<int> dijkstra(int n, int src, vector<vector<pair<int, int>>> &adj) {
    // adj[u] = vector of {v, w}
    vector<int> dist(n, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>>
        pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d != dist[u])
            continue;

        for (auto &edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n);

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        // for undirected: adj[v].push_back({u, w});
    }

    int src;
    cin >> src;

    auto dist = dijkstra(n, src, adj);

    for (int d : dist)
        cout << d << " ";
    cout << "\n";
}
