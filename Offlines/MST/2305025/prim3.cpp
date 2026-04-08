#include<bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj(n);

    // ────────────────────────────────────────
    vector<pair<int, int>> parent(n, {-1, 0}); // ← FIX HERE
    // -1 means no parent yet
    // ────────────────────────────────────────

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    using pii = pair<int, int>;
    priority_queue<pii, vector<pii>, greater<pii>> pq;

    vector<int> vis(n, 0);
    vector<int> minDist(n, INT_MAX); // optional but good practice

    pq.push({0, 0}); // {weight, node}
    minDist[0] = 0;

    long long sum = 0;

    while (!pq.empty()) {
        auto [wt, v] = pq.top();
        pq.pop();

        if (vis[v])
            continue;
        vis[v] = 1;
        sum += wt;

        for (auto [to, w] : adj[v]) {
            if (!vis[to] && w < minDist[to]) { // ← important Prim optimization
                minDist[to] = w;
                pq.push({w, to});
                parent[to] = {v, w};
            }
        }
    }

    cout << sum << '\n';

    // Print edges of the MST (except root)
    for (int i = 1; i < n; i++) {
        if (parent[i].first != -1) {
            cout << parent[i].first << " " << i << " " << parent[i].second
                 << '\n';
        }
    }

    return 0;
}