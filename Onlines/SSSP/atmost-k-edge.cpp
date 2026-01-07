#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, K;
    cin >> n >> m >> K;

    vector<vector<pair<int, ll>>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
    }

    // dist[node][edges_used]
    vector<vector<ll>> dist(n + 1, vector<ll>(K + 1, INF));

    priority_queue<tuple<ll, int, int>, vector<tuple<ll, int, int>>, greater<>>
        pq;

    dist[1][0] = 0;
    pq.push({0, 1, 0}); // cost, node, edges_used

    while (!pq.empty()) {
        auto [d, u, used] = pq.top();
        pq.pop();

        if (d > dist[u][used])
            continue;

        if (used == K)
            continue; // cannot use more edges

        for (auto [v, w] : adj[u]) {
            if (dist[v][used + 1] > d + w) {
                dist[v][used + 1] = d + w;
                pq.push({dist[v][used + 1], v, used + 1});
            }
        }
    }

    ll ans = INF;
    for (int i = 0; i <= K; i++) {
        ans = min(ans, dist[n][i]);
    }

    cout << ans << "\n";
    return 0;
}
