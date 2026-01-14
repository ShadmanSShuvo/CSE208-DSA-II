#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e15;

int main() {
    int n, m;
    if (!(cin >> n >> m))
        return 0;

    vector<vector<long long>> dist(n + 1, vector<long long>(n + 1, INF));

    for (int i = 0; i <= n; i++)
        dist[i][i] = 0;

    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        dist[u][v] = min(dist[u][v], w);
    }

    int Y, Z;
    cin >> Y >> Z;

    for (int k = 0; k <= n; k++) {
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= n; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    int q;
    cin >> q;
    while (q--) {
        int u, v;
        cin >> u >> v;
        long long pathY = (dist[u][Y] < INF && dist[Y][v] < INF)
                              ? (dist[u][Y] + dist[Y][v])
                              : INF;
        long long pathZ = (dist[u][Z] < INF && dist[Z][v] < INF)
                              ? (dist[u][Z] + dist[Z][v])
                              : INF;

        long long ans = min(pathY, pathZ);
        if (ans >= INF)
            cout << "-1\n";
        else
            cout << ans << "\n";
    }
    return 0;
}