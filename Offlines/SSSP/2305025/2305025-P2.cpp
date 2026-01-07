#include <bits/stdc++.h>
using namespace std;

void bellman_ford(int V, vector<tuple<int, int, long long>> &edges) {
    const long long INF = 1e18;
    vector<long long> dist(V + 1, INF);
    dist[1] = 0;
    vector<int> parent(V + 1, -1);

    int x = -1;

    for (int i = 1; i <= V; i++) {
        x = -1;
        for (auto &e : edges) {
            int u, v;
            long long w;
            tie(u, v, w) = e;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                x = v;
            }
        }
    }

    if (x == -1) {
        cout << -1 << "\n";
        return;
    }

    for (int i = 0; i < V; i++)
        x = parent[x];

    vector<int> cycle;
    int cur = x;
    do {
        cycle.push_back(cur);
        cur = parent[cur];
    } while (cur != x);

    reverse(cycle.begin(), cycle.end());

    for (int v : cycle)
        cout << v << " ";
    cout << "\n";
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<tuple<int, int, long long>> edges;
    for (int i = 0; i < m; i++) {
        int a, b;
        long long c;
        cin >> a >> b >> c;
        edges.push_back({a, b, c});
    }

    bellman_ford(n, edges);
    return 0;
}