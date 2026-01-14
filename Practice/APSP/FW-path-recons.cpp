#include <bits/stdc++.h>
using namespace std;

static const long long INF = (long long)4e18;

vector<int> get_path(int u, int v, const vector<vector<int>> &nxt) {
    if (nxt[u][v] == -1)
        return {}; // no path

    vector<int> path;
    path.push_back(u);
    while (u != v) {
        u = nxt[u][v];
        path.push_back(u);
    }
    return path;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<long long>> dist(n, vector<long long>(n, INF));
    vector<vector<int>> nxt(n, vector<int>(n, -1));

    // init
    for (int i = 0; i < n; i++) {
        dist[i][i] = 0;
        nxt[i][i] = i;
    }

    // edges
    for (int i = 0; i < m; i++) {
        int a, b;
        long long c;
        cin >> a >> b >> c;

        // Keep best edge only
        if (c < dist[a][b]) {
            dist[a][b] = c;
            nxt[a][b] = b;
        }
    }

    // Floyd + path update
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (dist[i][k] >= INF)
                continue;
            for (int j = 0; j < n; j++) {
                if (dist[k][j] >= INF)
                    continue;

                long long cand = dist[i][k] + dist[k][j];
                if (cand < dist[i][j]) {
                    dist[i][j] = cand;
                    nxt[i][j] =
                        nxt[i][k]; // first step from i towards j goes through k
                }
            }
        }
    }

    int u, v;
    cin >> u >> v;

    if (dist[u][v] >= INF) {
        cout << "No path\n";
        return 0;
    }

    cout << "Distance: " << dist[u][v] << "\n";

    vector<int> path = get_path(u, v, nxt);
    cout << "Path: ";
    for (int x : path)
        cout << x << " ";
    cout << "\n";
}

