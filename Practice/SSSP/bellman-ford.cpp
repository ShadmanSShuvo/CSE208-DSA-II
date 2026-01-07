#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

struct Edge {
    int u, v, w;
};

int main() {
    ifstream cin("input.txt");
    ofstream cout("output.txt");
    int n, m;
    cin >> n >> m;

    vector<Edge> edges(m);

    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    int src;
    cin >> src;

    vector<long long> dist(n, INF);
    dist[src] = 0;

    // Relax edges (n-1) times
    for (int i = 0; i < n - 1; i++) {
        for (auto &e : edges) {
            if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
            }
        }
    }

    // Check negative cycle
    bool neg_cycle = false;
    for (auto &e : edges) {
        if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
            neg_cycle = true;
            break;
        }
    }

    if (neg_cycle) {
        cout << "Negative cycle detected\n";
    } else {
        for (long long d : dist)
            cout << d << " ";
        cout << "\n";
    }
}
