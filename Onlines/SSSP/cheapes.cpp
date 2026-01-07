#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, ll>>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
    }

    vector<vector<ll>> dist(n + 1, vector<ll>(2, INF));
    priority_queue<tuple<ll, int, int>, vector<tuple<ll, int, int>>, greater<>>
        pq;

    dist[1][0] = 0;
    pq.push({0, 1, 0}); // cost, node, couponUsed

    while (!pq.empty()) {
        auto [d, u, used] = pq.top();
        pq.pop();

        if (d > dist[u][used])
            continue;

        for (auto [v, w] : adj[u]) {
            // normal edge
            if (dist[v][used] > d + w) {
                dist[v][used] = d + w;
                pq.push({dist[v][used], v, used});
            }

            // use coupon
            if (used == 0) {
                ll nd = d + w / 2;
                if (dist[v][1] > nd) {
                    dist[v][1] = nd;
                    pq.push({nd, v, 1});
                }
            }
        }
    }

    cout << min(dist[n][0], dist[n][1]) << "\n";
    return 0;
}
