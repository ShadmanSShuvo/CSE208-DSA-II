#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Edge {
    int a, b;
    ll w;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].a >> edges[i].b >> edges[i].w;
    }

    const ll INF = LLONG_MAX / 4;
    vector<ll> dist(n + 1, INF);
    vector<int> parent(n + 1, -1);

    dist[1] = 0; // start anywhere; cycle detection doesn't depend on source
    int x = -1;

    // Standard Bellman–Ford: n rounds
    for (int i = 0; i < n; i++) {
        x = -1;
        for (auto &e : edges) {
            if (dist[e.a] < INF && dist[e.b] > dist[e.a] + e.w) {
                dist[e.b] = dist[e.a] + e.w;
                parent[e.b] = e.a;
                x = e.b;
            }
        }
    }

    if (x == -1) {
        cout << -1 << "\n";
        return 0; // No negative cycle
    }

    // Recover cycle
    int v = x;
    for (int i = 0; i < n; i++)
        v = parent[v]; // move deep inside the cycle

    vector<int> cycle;
    int cur = v;

    // walk until the cycle repeats
    while (true) {
        cycle.push_back(cur);
        cur = parent[cur];
        if (cur == v) {
            cycle.push_back(v);
            break;
        }
    }

    reverse(cycle.begin(), cycle.end());

    for (int node : cycle)
        cout << node << " ";
    cout << "\n";

    return 0;
}
