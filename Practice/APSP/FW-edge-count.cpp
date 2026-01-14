#include <bits/stdc++.h>
using namespace std;

static const long long INF = (long long)4e18;
static const int INF_EDGES = (int)1e9;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<long long>> dist(n, vector<long long>(n, INF));
    vector<vector<int>> edges(n, vector<int>(n, INF_EDGES));

    // init
    for (int i = 0; i < n; i++) {
        dist[i][i] = 0;
        edges[i][i] = 0;
    }

    // input edges (directed graph)
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;

        // keep best direct edge
        if (w < dist[u][v]) {
            dist[u][v] = w;
            edges[u][v] = 1;
        }
        // if multiple edges same cost, keep smaller edge count (still 1)
        else if (w == dist[u][v]) {
            edges[u][v] = min(edges[u][v], 1);
        }
    }

    // Floyd–Warshall with edge count tie-break
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (dist[i][k] >= INF)
                continue;
            for (int j = 0; j < n; j++) {
                if (dist[k][j] >= INF)
                    continue;

                long long candDist = dist[i][k] + dist[k][j];
                int candEdges = edges[i][k] + edges[k][j];

                if (candDist < dist[i][j]) {
                    dist[i][j] = candDist;
                    edges[i][j] = candEdges;
                } else if (candDist == dist[i][j]) {
                    // tie: pick fewer edges
                    edges[i][j] = min(edges[i][j], candEdges);
                }
            }
        }
    }

    // Queries
    int q;
    cin >> q;
    while (q--) {
        int u, v;
        cin >> u >> v;

        if (dist[u][v] >= INF) {
            cout << "-1\n"; // unreachable
        } else {
            cout << dist[u][v] << " " << edges[u][v] << "\n";
        }
    }

    return 0;
}
