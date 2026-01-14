#include <bits/stdc++.h>

using namespace std;

const long long INF = 1e15;

int main() {
    int n, m, t;
    cin >> n >> m >> t;

    vector<vector<long long>> dist(n + 1, vector<long long>(n + 1, INF));

    for (int i = 1; i <= n; i++)
        dist[i][i] = 0;

    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        dist[u][v] = min(dist[u][v], w);
        dist[v][u] = min(dist[v][u], w);
    }

   

    
    // ... (Initial distance matrix setup and edge inputs)

    // Standard Floyd-Warshall to get all-pairs shortest paths
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    int M; // The must-visit node
    cin >> M;

    int q;
    cin >> q;
    while (q--) {
        int u, v;
        cin >> u >> v;

        // Force the path to go through M: u -> M -> v
        long long total_dist = dist[u][M] + dist[M][v];

        if (dist[u][M] >= INF || dist[M][v] >= INF)
            cout << "-1\n";
        else
            cout << total_dist << "\n";
    }
    

    return 0;
}