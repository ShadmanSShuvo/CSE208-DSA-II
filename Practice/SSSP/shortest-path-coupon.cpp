#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, ll>>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
    }

    // dist[node][state] -> state = 0 (coupon not used), 1 (coupon used)
    vector<vector<ll>> dist(n + 1, vector<ll>(2, INF));
    dist[1][0] = 0;

    priority_queue<pair<ll, pair<int, int>>, vector<pair<ll, pair<int, int>>>,
                   greater<pair<ll, pair<int, int>>>>
        pq;

    pq.push({0, {1, 0}});

    while (!pq.empty()) {
        auto [d, info] = pq.top();
        pq.pop();
        auto [u, used] = info;

        if (d != dist[u][used])
            continue;

        for (auto &[v, cost] : adj[u]) {
            // 1. Move without using coupon
            if (dist[v][used] > d + cost) {
                dist[v][used] = d + cost;
                pq.push({dist[v][used], {v, used}});
            }

            // 2. Use coupon here (only if unused)
            if (!used) {
                ll halfCost = cost / 2;
                if (dist[v][1] > d + halfCost) {
                    dist[v][1] = d + halfCost;
                    pq.push({dist[v][1], {v, 1}});
                }
            }
        }
    }

    cout << min(dist[n][0], dist[n][1]) << "\n";
    return 0;
}
