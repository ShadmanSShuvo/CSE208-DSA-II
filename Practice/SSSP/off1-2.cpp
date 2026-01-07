#include <bits/stdc++.h>
using namespace std;

vector<int> dijkstra(int n, int src,
                     const vector<vector<pair<int, int>>> &adj) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    vector<int> dist(n + 1, 1e9);
    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [dis, node] = pq.top();
        pq.pop();
        if (dis > dist[node])
            continue;

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
    vector<int> from, to, cost;

    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
        adj[b].push_back({a, c}); // undirected
        from.push_back(a);
        to.push_back(b);
        cost.push_back(c);
    }

    auto dist1 = dijkstra(n, 1, adj); // shortest from 1
    auto distN = dijkstra(n, n, adj); // shortest to n (same in undirected)

    int ans = dist1[n]; // at least the normal shortest path

    for (int i = 0; i < m; i++) {
        int a = from[i], b = to[i], c = cost[i];
        // Try using half cost on edge a-b
        if (dist1[a] < 1e9 && distN[b] < 1e9) {
            ans = min(ans, dist1[a] + (c + 1) / 2 + distN[b]);
        }
        // Try using half cost on edge b-a (same edge)
        if (dist1[b] < 1e9 && distN[a] < 1e9) {
            ans = min(ans, dist1[b] + (c + 1) / 2 + distN[a]);
        }
    }

    cout << ans << "\n";
}
