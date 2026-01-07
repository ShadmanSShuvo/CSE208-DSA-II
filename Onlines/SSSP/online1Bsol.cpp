#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int,int>>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
    }

    // dist[u][k] = min cost to reach u using k coupons
    vector<vector<ll>> dist(n + 1, vector<ll>(3, INF));

    // (cost, node, coupons_used)
    priority_queue<tuple<ll,int,int>,
                   vector<tuple<ll,int,int>>,
                   greater<>> pq;

    dist[1][0] = 0;
    pq.push({0, 1, 0});

    while (!pq.empty()) {
        auto [cost, u, used] = pq.top();
        pq.pop();

        if (cost > dist[u][used]) continue;

        for (auto [v, w] : adj[u]) {
            // 1️⃣ Don't use coupon
            if (dist[v][used] > cost + w) {
                dist[v][used] = cost + w;
                pq.push({dist[v][used], v, used});
            }

            // 2️⃣ Use coupon (if available)
            if (used < 2) {
                ll discounted = cost + w / 2;
                if (dist[v][used + 1] > discounted) {
                    dist[v][used + 1] = discounted;
                    pq.push({discounted, v, used + 1});
                }
            }
        }
    }

    ll answer = min({dist[n][0], dist[n][1], dist[n][2]});
    cout << answer << "\n";

    return 0;
}
