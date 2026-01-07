#include <bits/stdc++.h>
#include <iomanip>
using namespace std;

const long long INF = 1e15;

int main() {
    ifstream cin("input.txt"); 
    ofstream cout("output.txt");
    int n, m;
    cin >> n >> m;

    vector<vector<long long>> dist(n, vector<long long>(n, INF));

    for (int i = 0; i < n; i++)
        dist[i][i] = 0;

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        dist[u][v] = min(dist[u][v], (long long)w);
        // for undirected: dist[v][u] = min(dist[v][u], (long long)w);
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    // Print result
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist[i][j] >= INF / 2)
                cout << "INF ";
            else
                cout << setw(2) << dist[i][j] << " ";
        }
        cout << "\n";
    }
}
