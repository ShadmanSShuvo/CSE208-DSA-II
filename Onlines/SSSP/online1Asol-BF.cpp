#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = (1LL << 60);

struct Edge {
    int a, b;
    ll c;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].a >> edges[i].b >> edges[i].c;
    }

    // dp0 = coupon not used, dp1 = coupon used
    vector<ll> dp0(n + 1, INF), dp1(n + 1, INF);
    dp0[1] = 0;

    ll ans = INF;

    for (int step = 1; step <= k; step++) {
        vector<ll> new0 = dp0;
        vector<ll> new1 = dp1;

        for (auto &e : edges) {
            int u = e.a, v = e.b;
            ll c = e.c;

            // without coupon
            if (dp0[u] != INF) {
                new0[v] = min(new0[v], dp0[u] + c);
                // use coupon on this edge
                new1[v] = min(new1[v], dp0[u] + (c / 2));
            }

            // coupon already used
            if (dp1[u] != INF) {
                new1[v] = min(new1[v], dp1[u] + c);
            }
        }

        dp0.swap(new0);
        dp1.swap(new1);

        ans = min(ans, dp1[n]); // must have used coupon optimally (can also
                                // allow not using, but coupon helps)
    }

    // If coupon was never used, dp0[n] might be best (but coupon is optional
    // logically).
    ans = min(ans, dp0[n]);

    if (ans >= INF / 2)
        cout << "Not possible\n";
    else
        cout << ans << "\n";

    return 0;
}
