

/*
3 4
1 2 3
2 3 1
1 3 7
2 1 5
*/
#include <bits/stdc++.h>
using namespace std;

vector<int> dijkstra(int n, int src,
                      const vector<vector<pair<int, int>>> &adj) {
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>>
        pq;

    vector<int> dist(n + 1, 1e9);
    dist[src] = 0;
    pq.push({0, src}); // {distance, node}

    while (!pq.empty()) {
        auto [dis, node] =
            pq.top(); // C++17 structured binding (or use .first/.second)
        pq.pop();

        if (dis > dist[node])
            continue; // Outdated entry

        for (auto [adjNode, wt] : adj[node]) {
            if (dist[node] + wt < dist[adjNode]) {
                dist[adjNode] = dist[node] + wt;
                pq.push({dist[adjNode], adjNode});
            }
        }
    }

    return dist;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj(n + 1);
    vector<vector<pair<int, int>>> revAdj(n + 1);
    vector<tuple<int, int, int>> edges;

    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
        revAdj[b].push_back({a, c});
        edges.push_back({a, b, c});
    }

    vector<int> dist1 = dijkstra(n, 1, adj);
    vector<int> dist2 = dijkstra(n, n, revAdj);
    for( auto &d: dist1 ) cout << d << " ";
    cout << "\n";
    for( auto &d: dist2 ) cout << d << " ";
    cout << "\n";
    int minDist = 1e9;
    for (auto edge : edges) {
        int a, b, c;
        tie(a, b, c) = edge;

        // Using half-off coupon on edge a->b
        if (dist1[a] < 1e9 && dist2[b] < 1e9) {
            minDist = min(minDist, dist1[a] + (c / 2) + dist2[b]);
        }

    }
    cout << minDist << "\n";

    return 0;
}