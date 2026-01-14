#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = (1LL << 60);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, ll>>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        ll c;
        cin >> u >> v >> c;
        adj[u].push_back({v, c});
    }

    // dist[u][0] = coupon not used yet
    // dist[u][1] = coupon already used
    vector<vector<ll>> dist(n + 1, vector<ll>(2, INF));

    // (distance, node, usedCoupon)
    priority_queue<tuple<ll, int, int>, vector<tuple<ll, int, int>>,
                   greater<tuple<ll, int, int>>>
        pq;

    dist[1][0] = 0;
    pq.push({0, 1, 0});

    while (!pq.empty()) {
        auto [d, u, used] = pq.top();
        pq.pop();

        if (d != dist[u][used])
            continue; // stale

        for (auto [v, c] : adj[u]) {
            // move without using coupon
            if (d + c < dist[v][used]) {
                dist[v][used] = d + c;
                pq.push({dist[v][used], v, used});
            }

            // if coupon not used yet, use it on this edge
            if (used == 0) {
                ll discounted = c / 2; // floor(c/2)
                if (d + discounted < dist[v][1]) {
                    dist[v][1] = d + discounted;
                    pq.push({dist[v][1], v, 1});
                }
            }
        }
    }

    cout << min(dist[n][0], dist[n][1]) << "\n";
    return 0;
}
