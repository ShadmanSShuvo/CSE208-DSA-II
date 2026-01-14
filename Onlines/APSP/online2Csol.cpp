#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e15;

int main() {
    int n, m;
    if (!(cin >> n >> m))
        return 0;

    vector<vector<long long>> dist(n, vector<long long>(n, INF));
    vector<vector<long long>> distDisc(n, vector<long long>(n, INF));

    for (int i = 0; i < n; i++)
        dist[i][i] = distDisc[i][i] = 0;

    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        // Directed graph as per problem statement
        dist[u][v] = min(dist[u][v], w);
        distDisc[u][v] = min(distDisc[u][v], max(0LL, w - 1));
    }

    int V;
    cin >> V;

    // Standard Floyd-Warshall
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                if (distDisc[i][k] < INF && distDisc[k][j] < INF)
                    distDisc[i][j] =
                        min(distDisc[i][j], distDisc[i][k] + distDisc[k][j]);
            }
        }
    }

    int q;
    cin >> q;
    while (q--) {
        int u, v;
        cin >> u >> v;
        long long original = dist[u][v];
        long long discounted = (distDisc[u][V] < INF && distDisc[V][v] < INF)
                                   ? (distDisc[u][V] + distDisc[V][v])
                                   : INF;

        long long ans = min(original, discounted);
        if (ans >= INF)
            cout << "-1\n";
        else
            cout << ans << "\n";
    }
    return 0;
}