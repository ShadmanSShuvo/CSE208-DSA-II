/**
 * Solution for Flow Online B1+B2
 * Problem: Identify flyovers that strictly increase max flow.
 * Algorithm: Dinic's Algorithm + Residual Graph Analysis
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

// Edge structure for Flow Network
struct Edge {
    int v;                  // Destination vertex
    long long capacity;     // Total capacity
    long long flow;         // Current flow
    int rev;                // Index of the reverse edge in adj[v]
};

// Global Graph Variables
vector<vector<Edge>> adj;
vector<int> level;
vector<int> ptr;
int N;

// Add a directed edge with capacity
void add_edge(int u, int v, long long cap) {
    Edge a = {v, cap, 0, (int)adj[v].size()};
    Edge b = {u, 0, 0, (int)adj[u].size()}; // Backward edge with 0 capacity
    adj[u].push_back(a);
    adj[v].push_back(b);
}

// BFS for Dinic's Algorithm (Builds Level Graph)
bool bfs(int s, int t) {
    fill(level.begin(), level.end(), -1);
    level[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (const auto& e : adj[u]) {
            if (e.capacity - e.flow > 0 && level[e.v] == -1) {
                level[e.v] = level[u] + 1;
                q.push(e.v);
            }
        }
    }
    return level[t] != -1;
}

// DFS for Dinic's Algorithm (Finds Augmenting Paths in Level Graph)
long long dfs(int u, int t, long long pushed) {
    if (pushed == 0 || u == t) return pushed;
    for (int& cid = ptr[u]; cid < adj[u].size(); ++cid) {
        auto& e = adj[u][cid];
        if (level[u] + 1 != level[e.v] || e.capacity - e.flow == 0) continue;
        long long tr = dfs(e.v, t, min(pushed, e.capacity - e.flow));
        if (tr == 0) continue;
        e.flow += tr;
        adj[e.v][e.rev].flow -= tr;
        return tr;
    }
    return 0;
}

// Dinic's Algorithm Driver
long long dinic(int s, int t) {
    long long flow = 0;
    while (bfs(s, t)) {
        fill(ptr.begin(), ptr.end(), 0);
        while (long long pushed = dfs(s, t, INF)) {
            flow += pushed;
        }
    }
    return flow;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int M;
    if (!(cin >> N >> M)) return 0;

    // Resize vectors (1-based indexing)
    adj.assign(N + 1, vector<Edge>());
    level.resize(N + 1);
    ptr.resize(N + 1);

    // Reading existing roads
    for (int i = 0; i < M; ++i) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        add_edge(u, v, c);
    }

    // 1. Run Max Flow on the initial graph
    dinic(1, N);

    // 2. Identify set of nodes reachable FROM Source (1) in Residual Graph
    //    Condition: capacity - flow > 0
    vector<bool> reachable_from_source(N + 1, false);
    queue<int> q_s;

    q_s.push(1);
    reachable_from_source[1] = true;

    while(!q_s.empty()){
        int u = q_s.front();
        q_s.pop();
        for(const auto& e : adj[u]){
            if(e.capacity - e.flow > 0 && !reachable_from_source[e.v]){
                reachable_from_source[e.v] = true;
                q_s.push(e.v);
            }
        }
    }

    // 3. Identify set of nodes that can REACH Sink (N) in Residual Graph
    //    We perform a reverse BFS starting from Sink.
    //    We construct a temporary reverse residual graph for traversal.
    //    If Edge u->v exists in residual (cap - flow > 0), we add v->u to reverse graph.
    vector<vector<int>> rev_residual_adj(N + 1);
    for(int u = 1; u <= N; ++u){
        for(const auto& e : adj[u]){
            if(e.capacity - e.flow > 0){
                rev_residual_adj[e.v].push_back(u);
            }
        }
    }

    vector<bool> can_reach_sink(N + 1, false);
    queue<int> q_t;

    q_t.push(N);
    can_reach_sink[N] = true;

    while(!q_t.empty()){
        int u = q_t.front();
        q_t.pop();
        for(int v : rev_residual_adj[u]){
            if(!can_reach_sink[v]){
                can_reach_sink[v] = true;
                q_t.push(v);
            }
        }
    }

    // 4. Process Proposals
    int P;
    cin >> P;
    vector<int> result_indices;

    for(int i = 0; i < P; ++i){
        int u, v;
        long long c;
        cin >> u >> v >> c;

        // A new edge u->v increases flow if S -> ... -> u exists AND v -> ... -> T exists
        if(reachable_from_source[u] && can_reach_sink[v]){
            result_indices.push_back(i + 1); // Store 1-based index
        }
    }

    // 5. Output
    if(result_indices.empty()){
        cout << "None" << endl;
    } else {
        for(size_t i = 0; i < result_indices.size(); ++i){
            cout << result_indices[i] << (i == result_indices.size() - 1 ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}
