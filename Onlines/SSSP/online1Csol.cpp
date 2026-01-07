#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const long long INF = 1e18; // Use a large value for infinity

struct Edge {
    int to;
    long long weight;
};

struct State {
    int u;
    long long d;
    int couponUsed;
    // Min-priority queue based on distance
    bool operator>(const State &other) const { return d > other.d; }
};

// Modified Dijkstra to handle the "one coupon" state
void dijkstra(int start, int n, const vector<vector<Edge>> &adj,
              vector<vector<long long>> &dist) {
    dist.assign(n + 1, vector<long long>(2, INF));
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[start][0] = 0;
    pq.push({start, 0, 0});

    while (!pq.empty()) {
        State curr = pq.top();
        pq.pop();

        int u = curr.u;
        long long d = curr.d;
        int used = curr.couponUsed;

        if (d > dist[u][used])
            continue;

        for (auto &edge : adj[u]) {
            int v = edge.to;
            long long w = edge.weight;

            // Option A: Do not use a coupon on this specific edge
            if (dist[v][used] > d + w) {
                dist[v][used] = d + w;
                pq.push({v, dist[v][used], used});
            }

            // Option B: Use a coupon on this specific edge (if not used yet)
            if (used == 0) {
                if (dist[v][1] > d + (w / 2)) {
                    dist[v][1] = d + (w / 2);
                    pq.push({v, dist[v][1], 1});
                }
            }
        }
    }
}

int main() {
    int n, m;
    if (!(cin >> n >> m))
        return 0;

    vector<vector<Edge>> adj(n + 1), rev_adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        rev_adj[v].push_back(
            {u, w}); // Build reversed graph for backward Dijkstra
    }

    int x, y;
    long long w_mand;
    cin >> x >> y >> w_mand; // Mandatory flight details [cite: 24]

    vector<vector<long long>> dist1, distN;
    dijkstra(1, n, adj, dist1);     // Path costs from Dhaka
    dijkstra(n, n, rev_adj, distN); // Path costs to London

    long long ans = INF;

    // Check if the mandatory flight is reachable and can reach the destination
    if (dist1[x][0] != INF && distN[y][0] != INF) {
        // Case 1: Coupon used somewhere in the path 1 -> x
        ans = min(ans, dist1[x][1] + w_mand + distN[y][0]);

        // Case 2: Coupon used on the mandatory flight x -> y itself
        ans = min(ans, dist1[x][0] + (w_mand / 2) + distN[y][0]);

        // Case 3: Coupon used somewhere in the path y -> n
        ans = min(ans, dist1[x][0] + w_mand + distN[y][1]);
    }

    if (ans >= INF) {
        cout << "Not possible" << endl;
        [cite:25]
    } else {
        cout << ans << endl;
        [cite:28]
    }

    return 0;
}