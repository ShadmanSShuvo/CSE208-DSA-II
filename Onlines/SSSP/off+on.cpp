#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, int> pli;

const ll INF = 1e18;

// ==================== OFFLINE 1: Base Problem (One Coupon)
// ====================
ll solveOffline1(int n, vector<vector<pair<int, ll>>> &graph) {
    // State: (node, coupon_used)
    // dist[node][0] = min cost to reach node without using coupon
    // dist[node][1] = min cost to reach node with coupon used
    vector<vector<ll>> dist(n + 1, vector<ll>(2, INF));
    priority_queue<pair<ll, pair<int, int>>, vector<pair<ll, pair<int, int>>>,
                   greater<pair<ll, pair<int, int>>>>
        pq;

    dist[1][0] = 0;
    pq.push({0, {1, 0}}); // {cost, {node, coupon_used}}

    while (!pq.empty()) {
        auto [cost, state] = pq.top();
        pq.pop();

        int u = state.first;
        int used = state.second;

        if (cost > dist[u][used])
            continue;

        for (auto [v, w] : graph[u]) {
            // Option 1: Don't use coupon on this edge
            if (dist[v][used] > dist[u][used] + w) {
                dist[v][used] = dist[u][used] + w;
                pq.push({dist[v][used], {v, used}});
            }

            // Option 2: Use coupon on this edge (if not already used)
            if (used == 0) {
                ll discounted = w / 2;
                if (dist[v][1] > dist[u][0] + discounted) {
                    dist[v][1] = dist[u][0] + discounted;
                    pq.push({dist[v][1], {v, 1}});
                }
            }
        }
    }

    return min(dist[n][0], dist[n][1]);
}

// ==================== ONLINE 1B: Two Coupons ====================
ll solveOnline1B(int n, vector<vector<pair<int, ll>>> &graph) {
    // State: (node, coupons_used)
    // dist[node][k] = min cost to reach node using k coupons
    vector<vector<ll>> dist(n + 1, vector<ll>(3, INF));
    priority_queue<pair<ll, pair<int, int>>, vector<pair<ll, pair<int, int>>>,
                   greater<pair<ll, pair<int, int>>>>
        pq;

    dist[1][0] = 0;
    pq.push({0, {1, 0}}); // {cost, {node, coupons_used}}

    while (!pq.empty()) {
        auto [cost, state] = pq.top();
        pq.pop();

        int u = state.first;
        int used = state.second;

        if (cost > dist[u][used])
            continue;

        for (auto [v, w] : graph[u]) {
            // Option 1: Don't use coupon on this edge
            if (dist[v][used] > dist[u][used] + w) {
                dist[v][used] = dist[u][used] + w;
                pq.push({dist[v][used], {v, used}});
            }

            // Option 2: Use a coupon on this edge (if we have coupons left)
            if (used < 2) {
                ll discounted = w / 2;
                if (dist[v][used + 1] > dist[u][used] + discounted) {
                    dist[v][used + 1] = dist[u][used] + discounted;
                    pq.push({dist[v][used + 1], {v, used + 1}});
                }
            }
        }
    }

    return min({dist[n][0], dist[n][1], dist[n][2]});
}

// ==================== ONLINE 1C: Mandatory Flight ====================
ll dijkstra(int start, int end, int n, vector<vector<pair<int, ll>>> &graph) {
    vector<ll> dist(n + 1, INF);
    priority_queue<pli, vector<pli>, greater<pli>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u])
            continue;

        for (auto [v, w] : graph[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    return dist[end];
}

ll solveOnline1C(int n, vector<vector<pair<int, ll>>> &graph, int x, int y,
                 ll w) {
    // We must use edge (x, y, w)
    // Find shortest path: 1 -> x, then use edge (x, y, w), then y -> n
    // We can apply coupon to any edge in the entire path

    // Calculate dist1[i] = shortest distance from node 1 to node i
    vector<ll> dist1(n + 1, INF);
    priority_queue<pli, vector<pli>, greater<pli>> pq;
    dist1[1] = 0;
    pq.push({0, 1});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist1[u])
            continue;
        for (auto [v, cost] : graph[u]) {
            if (dist1[v] > dist1[u] + cost) {
                dist1[v] = dist1[u] + cost;
                pq.push({dist1[v], v});
            }
        }
    }

    // Calculate distN[i] = shortest distance from node i to node n
    vector<vector<pair<int, ll>>> revGraph(n + 1);
    for (int u = 1; u <= n; u++) {
        for (auto [v, cost] : graph[u]) {
            revGraph[v].push_back({u, cost});
        }
    }

    vector<ll> distN(n + 1, INF);
    distN[n] = 0;
    pq.push({0, n});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > distN[u])
            continue;
        for (auto [v, cost] : revGraph[u]) {
            if (distN[v] > distN[u] + cost) {
                distN[v] = distN[u] + cost;
                pq.push({distN[v], v});
            }
        }
    }

    // Check if path exists
    if (dist1[x] == INF || distN[y] == INF) {
        return -1; // Not possible
    }

    // Option 1: Use coupon on mandatory edge
    ll opt1 = dist1[x] + w / 2 + distN[y];

    // Option 2: Use coupon on some edge in path 1->x
    // We need to track: dist1_with_coupon[i] = min cost to reach i using coupon
    vector<vector<ll>> dist1_coupon(n + 1, vector<ll>(2, INF));
    dist1_coupon[1][0] = 0;
    pq.push({0, 1});

    priority_queue<pair<ll, pair<int, int>>, vector<pair<ll, pair<int, int>>>,
                   greater<pair<ll, pair<int, int>>>>
        pq2;
    pq2.push({0, {1, 0}});

    while (!pq2.empty()) {
        auto [cost, state] = pq2.top();
        pq2.pop();
        int u = state.first, used = state.second;
        if (cost > dist1_coupon[u][used])
            continue;

        for (auto [v, c] : graph[u]) {
            if (dist1_coupon[v][used] > dist1_coupon[u][used] + c) {
                dist1_coupon[v][used] = dist1_coupon[u][used] + c;
                pq2.push({dist1_coupon[v][used], {v, used}});
            }
            if (used == 0 && dist1_coupon[v][1] > dist1_coupon[u][0] + c / 2) {
                dist1_coupon[v][1] = dist1_coupon[u][0] + c / 2;
                pq2.push({dist1_coupon[v][1], {v, 1}});
            }
        }
    }

    // Option 3: Use coupon on some edge in path y->n
    vector<vector<ll>> distN_coupon(n + 1, vector<ll>(2, INF));
    distN_coupon[n][0] = 0;
    pq2.push({0, {n, 0}});

    while (!pq2.empty()) {
        auto [cost, state] = pq2.top();
        pq2.pop();
        int u = state.first, used = state.second;
        if (cost > distN_coupon[u][used])
            continue;

        for (auto [v, c] : revGraph[u]) {
            if (distN_coupon[v][used] > distN_coupon[u][used] + c) {
                distN_coupon[v][used] = distN_coupon[u][used] + c;
                pq2.push({distN_coupon[v][used], {v, used}});
            }
            if (used == 0 && distN_coupon[v][1] > distN_coupon[u][0] + c / 2) {
                distN_coupon[v][1] = distN_coupon[u][0] + c / 2;
                pq2.push({distN_coupon[v][1], {v, 1}});
            }
        }
    }

    ll opt2 = dist1_coupon[x][1] + w + distN[y];
    ll opt3 = dist1[x] + w + distN_coupon[y][1];

    return min({opt1, opt2, opt3});
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Read which problem to solve
    cout << "Enter problem type (B for Online1B, C for Online1C): ";
    char problemType;
    cin >> problemType;

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, ll>>> graph(n + 1);

    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        graph[a].push_back({b, c});
    }

    if (problemType == 'B' || problemType == 'b') {
        ll result = solveOnline1B(n, graph);
        cout << result << endl;
    } else if (problemType == 'C' || problemType == 'c') {
        int x, y;
        ll w;
        cin >> x >> y >> w;
        ll result = solveOnline1C(n, graph, x, y, w);
        if (result == -1) {
            cout << "Not possible" << endl;
        } else {
            cout << result << endl;
        }
    }

    return 0;
}