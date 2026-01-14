#include <bits/stdc++.h>
using namespace std;

class Solution {
  public:
    int findTheCity(int n, vector<vector<int>> &edges, int distanceThreshold) {
        const int INF = 1e9;
        vector<vector<int>> dist(n, vector<int>(n, INF));

        for (int i = 0; i < n; i++)
            dist[i][i] = 0;

        // Undirected edges
        for (auto &e : edges) {
            int u = e[0], v = e[1], w = e[2];
            dist[u][v] = min(dist[u][v], w);
            dist[v][u] = min(dist[v][u], w);
        }

        // Floyd–Warshall
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                if (dist[i][k] == INF)
                    continue;
                for (int j = 0; j < n; j++) {
                    if (dist[k][j] == INF)
                        continue;
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }

        int bestCity = -1;
        int bestCnt = INF;

        for (int i = 0; i < n; i++) {
            int cnt = 0;
            for (int j = 0; j < n; j++) {
                if (i != j && dist[i][j] <= distanceThreshold)
                    cnt++;
            }

            // smallest cnt, tie -> largest index
            if (cnt < bestCnt || (cnt == bestCnt && i > bestCity)) {
                bestCnt = cnt;
                bestCity = i;
            }
        }

        return bestCity;
    }
};
