#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = 1e18;

void dijkstra(int src, vector<vector<pair<int, ll>>> &g,
              vector<vector<ll>> &dist) {

    priority_queue<tuple<ll, int, int>, vector<tuple<ll, int, int>>, greater<>>
        pq;

    dist[src][0] = 0;
    pq.push({0, src, 0});

    while (!pq.empty()) {
        auto [d, u, used] = pq.top();
        pq.pop();
        if (d > dist[u][used])
            continue;

        for (auto [v, w] : g[u]) {
            if (dist[v][used] > d + w) {
                dist[v][used] = d + w;
                pq.push({dist[v][used], v, used});
            }
            if (used == 0) {
                ll nd = d + w / 2;
                if (dist[v][1] > nd) {
                    dist[v][1] = nd;
                    pq.push({nd, v, 1});
                }
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, ll>>> adj(n + 1), radj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
        radj[b].push_back({a, c});
    }

    int x, y;
    ll w;
    cin >> x >> y >> w; // mandatory edge

    vector<vector<ll>> d1(n + 1, vector<ll>(2, INF));
    vector<vector<ll>> d2(n + 1, vector<ll>(2, INF));

    dijkstra(1, adj, d1);
    dijkstra(n, radj, d2);

    ll ans = INF;

    // coupon before or after
    for (int a = 0; a <= 1; a++) {
        for (int b = 0; b <= 1; b++) {
            if (a + b <= 1) {
                ans = min(ans, d1[x][a] + w + d2[y][b]);
            }
        }
    }

    // coupon on mandatory edge
    ans = min(ans, d1[x][0] + w / 2 + d2[y][0]);

    cout << ans << "\n";
    return 0;
}
