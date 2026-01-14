#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e15;

struct Edge {
    int u, v;
    long long w;
};

int main() {
    int n, m;
    if (!(cin >> n >> m))
        return 0;

    vector<vector<long long>> dist(n + 1, vector<long long>(n + 1, INF));
    vector<Edge> edges;

    for (int i = 1; i <= n; i++)
        dist[i][i] = 0;

    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        dist[u][v] = min(dist[u][v], w);
        edges.push_back({u, v, w}); // Store edge for discount check
    }

    // Standard Floyd-Warshall [cite: 31]
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    int q;
    cin >> q;
    while (q--) {
        int start, end;
        cin >> start >> end;

        long long min_cost = dist[start][end];

        // Try halving each edge in the delivery network [cite: 32, 33]
        for (auto &e : edges) {
            if (dist[start][e.u] < INF && dist[e.v][end] < INF) {
                long long discounted_path =
                    dist[start][e.u] + (e.w / 2) + dist[e.v][end];
                min_cost = min(min_cost, discounted_path);
            }
        }

        if (min_cost >= INF)
            cout << "-1\n";
        else
            cout << min_cost << "\n";
    }

    return 0;
}