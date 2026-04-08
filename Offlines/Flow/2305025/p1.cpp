#include <bits/stdc++.h>

using namespace std;

const int INF = 1e9;


struct Edge {
    int v;
    int capacity;
    int flow;
    int rev_index;
    int original_id;

};


vector<vector<Edge>> adj;
int N, M;
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
    vector<int> parent_edge_index(N);
    vector<int> parent_node(N);

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

int main() {

    cin >> N >> M;

    adj.resize(N);



    vector<pair<int, int>> input_edges_tracker;

    for (int i = 0; i < M; i++) {
        int u, v, c;
        cin >> u >> v >> c;

        Edge forward = {v, c, 0, (int)adj[v].size(), i};

        Edge backward = {u, 0, 0, (int)adj[u].size(), -1};

        adj[u].push_back(forward);
        input_edges_tracker.push_back({u, (int)adj[u].size() - 1});
        adj[v].push_back(backward);
    }

    cin >> s >> t;

    cout << max_flow() << endl;


    for (int i = 0; i < M; i++) {
        int u = input_edges_tracker[i].first;
        int idx = input_edges_tracker[i].second;
        Edge &e = adj[u][idx];


        cout << u << " " << e.v << " " << e.flow << "/" << e.capacity << endl;
    }

    return 0;
}
