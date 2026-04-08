#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj(n);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;

        // if input nodes are 1-based, uncomment next 2 lines:
        // u--;
        // v--;

        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>>
        pq;

    vector<int> vis(n, 0);
    vector<int> parent(n, -1);
    vector<int> key(n, 1e9);

    int start = 0; // or 1 depending on input
    key[start] = 0;
    pq.push({0, start});

    long long sum = 0;

    while (!pq.empty()) {
        auto [wt, v] = pq.top();
        pq.pop();

        if (vis[v])
            continue;

        vis[v] = 1;
        sum += wt;

        for (auto [to, w] : adj[v]) {
            if (!vis[to] && w < key[to]) {
                key[to] = w;
                parent[to] = v;
                pq.push({w, to});
            }
        }
    }

    cout << sum << "\n";
    for (int i = 0; i < n; i++) {
        cout << parent[i] << " ";
    }
    cout << "\n";

    return 0;
}
