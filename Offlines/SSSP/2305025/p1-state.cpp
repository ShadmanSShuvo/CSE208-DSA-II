#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// Using long long to prevent overflow as prices can be large
const long long INF = 1e18;

struct Edge {
    int to;
    long long weight;
};

struct State {
    int u;
    long long cost;
    int couponUsed;

    // Min-priority queue needs to prioritize smaller costs
    bool operator>(const State &other) const { return cost > other.cost; }
};

int main() {
    // Optimize I/O
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m))
        return 0; // [cite: 17, 18, 19]

    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long c;
        cin >> u >> v >> c; // [cite: 22, 23]
        adj[u].push_back({v, c});
    }

    // dist[node][0] -> min cost to reach node without using coupon
    // dist[node][1] -> min cost to reach node having used the coupon
    vector<vector<long long>> dist(n + 1, vector<long long>(2, INF));
    priority_queue<State, vector<State>, greater<State>> pq;

    // Starting point: Dhaka (Airport 1) [cite: 20, 21]
    dist[1][0] = 0;
    pq.push({1, 0, 0});

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        int u = current.u;
        long long d = current.cost;
        int used = current.couponUsed;

        // Standard Dijkstra check to skip outdated entries
        if (d > dist[u][used])
            continue;

        for (auto &edge : adj[u]) {
            int v = edge.to;
            long long weight = edge.weight;

            // 1. Move to next airport WITHOUT using a coupon on this flight
            if (dist[v][used] > d + weight) {
                dist[v][used] = d + weight;
                pq.push({v, dist[v][used], used});
            }

            // 2. Move to next airport USING a coupon on this flight (if not
            // used yet)
            if (used == 0) {
                long long discountedPrice = weight / 2; //
                if (dist[v][1] > d + discountedPrice) {
                    dist[v][1] = d + discountedPrice;
                    pq.push({v, dist[v][1], 1});
                }
            }
        }
    }

    // The answer is the minimum cost to reach London (Airport n) [cite: 21, 27,
    // 28] with the coupon having been used at some point.
    cout << dist[n][1] << endl;

    return 0;
}