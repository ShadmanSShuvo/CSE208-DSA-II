#include <bits/stdc++.h>

using namespace std;

const long long INF = 1e15;

int main() {
    ifstream cin("input1.txt");
    ofstream cout("output1.txt");
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

    vector<pair<int, int>> queries(t);
    for (int i = 0; i < t; i++) {
        cin >> queries[i].first >> queries[i].second;
    }

    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    for (int i = 0; i < t; i++) {
        int u = queries[i].first;
        int v = queries[i].second;
        if (dist[u][v] >= INF / 2)
            cout << "-1\n";
        else
            cout << dist[u][v] << "\n";
    }

    return 0;
}