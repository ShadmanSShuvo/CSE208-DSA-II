#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

vector<long long> dijkstra(int n, int src,
                           vector<vector<pair<int, int>>> &adj) {
    // adj[u] = vector of {v, w}
    vector<long long> dist(n, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>>
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

    for (long long d : dist)
        cout << d << " ";
    cout << "\n";
}
