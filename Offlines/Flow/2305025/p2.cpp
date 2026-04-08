#include <bits/stdc++.h>

using namespace std;

const int INF = 1e9;

struct Edge {
    int v;
    int capacity;
    int flow;
    int rev_index;
};

vector<vector<Edge>> adj;
int num_nodes;
int s, t;


int bfs(vector<int> &parent_edge_index, vector<int> &parent_node) {
    fill(parent_node.begin(), parent_node.end(), -1);
    fill(parent_edge_index.begin(), parent_edge_index.end(), -1);

    queue<pair<int, int>> q;
    q.push({s, INF});
    parent_node[s] = s;

    while (!q.empty()) {
        int u = q.front().first;
        int flow = q.front().second;
        q.pop();

        if (u == t)
            return flow;

        for (int i = 0; i < adj[u].size(); i++) {
            Edge &e = adj[u][i];
            if (parent_node[e.v] == -1 && e.capacity > e.flow) {
                parent_node[e.v] = u;
                parent_edge_index[e.v] = i;
                int new_flow = min(flow, e.capacity - e.flow);
                q.push({e.v, new_flow});
            }
        }
    }
    return 0;
}


int max_flow() {
    int max_f = 0;
    vector<int> parent_edge_index(num_nodes);
    vector<int> parent_node(num_nodes);

    int path_flow;
    while ((path_flow = bfs(parent_edge_index, parent_node))) {
        max_f += path_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent_node[cur];
            int index = parent_edge_index[cur];
            adj[prev][index].flow += path_flow;
            int rev_idx = adj[prev][index].rev_index;
            adj[cur][rev_idx].flow -= path_flow;
            cur = prev;
        }
    }
    return max_f;
}

void add_edge(int u, int v, int cap) {
    Edge forward = {v, cap, 0, (int)adj[v].size()};
    Edge backward = {u, 0, 0, (int)adj[u].size()};
    adj[u].push_back(forward);
    adj[v].push_back(backward);
}

int main() {

    int N, K, M;
    cin >> N >> K >> M;

    s = N;
    t = N + 1;
    num_nodes = N + 2;
    adj.resize(num_nodes);


    for (int i = 0; i < K; i++) {
        add_edge(s, i, 1);
    }


    for (int i = K; i < N; i++) {
        add_edge(i, t, 1);
    }


    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        add_edge(u, v, 1);
    }


    int total_matches = max_flow();
    cout << total_matches << endl;



    for (int u = 0; u < K; u++) {
        for (const auto &e : adj[u]) {
            if (e.flow == 1 && e.v >= K && e.v < N) {
                cout << u << " " << e.v << endl;
            }
        }
    }

    return 0;
}
