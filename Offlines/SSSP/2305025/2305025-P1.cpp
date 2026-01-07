#include <bits/stdc++.h>
using namespace std;

#define ll long long
const ll INF = 9e18;

vector<ll> dijkstra(int n, int src, const vector<vector<pair<int, int>>> &adj) {
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>>
        pq;

    vector<ll> dist(n + 1, INF);
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
    vector<vector<pair<int, int>>> revAdj(n + 1);
    vector<tuple<int, int, int>> edges;

    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
        revAdj[b].push_back({a, c});
        edges.push_back({a, b, c});
    }

    vector<ll> dist1 = dijkstra(n, 1, adj);
    vector<ll> dist2 = dijkstra(n, n, revAdj);

    ll minDist = INF;

    for (auto edge : edges) {
        int a, b, c;
        tie(a, b, c) = edge;

        if (dist1[a] < INF && dist2[b] < INF) {
            minDist = min(minDist, dist1[a] + (c / 2) + dist2[b]);
        }
    }

    cout << minDist << "\n";
    return 0;
}
