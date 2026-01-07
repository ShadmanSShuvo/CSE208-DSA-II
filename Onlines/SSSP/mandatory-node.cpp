#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
    long long weight;
};

struct State {
    long long cost;
    int u;
    int used; // 0 if coupon not used, 1 if used

    bool operator>(const State &other) const { return cost > other.cost; }
};

// Standard State-Space Dijkstra function
// Returns a 2D vector where dist[i][j] is min cost to node i with j coupons
// used
vector<vector<long long>> dijkstra(int start_node, int n,
                                   const vector<vector<Edge>> &adj) {
    vector<vector<long long>> dist(n + 1, vector<long long>(2, INF));
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[start_node][0] = 0;
    pq.push({0, start_node, 0});

    while (!pq.empty()) {
        State curr = pq.top();
        pq.pop();

        if (curr.cost > dist[curr.u][curr.used])
            continue;

        for (const auto &edge : adj[curr.u]) {
            // Option A: Don't use coupon on this specific edge
            if (dist[edge.to][curr.used] > curr.cost + edge.weight) {
                dist[edge.to][curr.used] = curr.cost + edge.weight;
                pq.push({dist[edge.to][curr.used], edge.to, curr.used});
            }

            // Option B: Use coupon on this specific edge (only if not used yet)
            if (curr.used == 0) {
                if (dist[edge.to][1] > curr.cost + (edge.weight / 2)) {
                    dist[edge.to][1] = curr.cost + (edge.weight / 2);
                    pq.push({dist[edge.to][1], edge.to, 1});
                }
            }
        }
    }
    return dist;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, k;
    // n: nodes, m: edges, k: mandatory node
    if (!(cin >> n >> m >> k))
        return 0;

    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    // 1. Calculate distances from Dhaka (1)
    vector<vector<long long>> dist_from_start = dijkstra(1, n, adj);

    // 2. Calculate distances from Mandatory Node (K)
    vector<vector<long long>> dist_from_k = dijkstra(k, n, adj);

    // 3. Find minimum cost passing through K
    // Path 1: Coupon used in Dhaka -> K segment
    long long path1 = dist_from_start[k][1] + dist_from_k[n][0];

    // Path 2: Coupon used in K -> London segment
    long long path2 = dist_from_start[k][0] + dist_from_k[n][1];

    long long result = min(path1, path2);

    if (result >= INF) {
        cout << -1 << endl; // If London is unreachable
    } else {
        cout << result << endl;
    }

    return 0;
}