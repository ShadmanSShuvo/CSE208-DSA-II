#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, weight;
    Edge(int a, int b, int c) : u(a), v(b), weight(c) {}
    bool operator<(const Edge &e) const { return weight < e.weight; }
};

struct PQNode {
    int vertex, weight;
    bool operator>(const PQNode &p) const { return weight > p.weight; }
};

// Disjoint Set Union implementation
class DSU {
  private:
    vector<int> parent;
    vector<int> rank;

  public:
    DSU(int size) {
        parent.resize(size);
        rank.resize(size, 0);
        for (int i = 0; i < size; i++) {
            parent[i] = i;
        }
    }

    int find(int node) {
        if (parent[node] != node) {
            parent[node] = find(parent[node]);
        }
        return parent[node];
    }

    bool unite(int a, int b) {
        int root_a = find(a);
        int root_b = find(b);

        if (root_a == root_b)
            return false;

        if (rank[root_a] < rank[root_b]) {
            parent[root_a] = root_b;
        } else if (rank[root_a] > rank[root_b]) {
            parent[root_b] = root_a;
        } else {
            parent[root_b] = root_a;
            rank[root_a]++;
        }
        return true;
    }
};

void primMST(vector<vector<Edge>> &adj, int vertices, int start) {
    vector<bool> visited(vertices, false);
    vector<int> parent(vertices, -1);
    vector<int> minCost(vertices, INT_MAX);

    priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;

    minCost[start] = 0;
    pq.push({start, 0});

    long long total = 0;
    vector<pair<int, int>> result;

    while (!pq.empty()) {
        int current = pq.top().vertex;
        pq.pop();

        if (visited[current])
            continue;

        visited[current] = true;
        total += minCost[current];

        if (parent[current] != -1) {
            result.push_back({parent[current], current});
        }

        for (const Edge &e : adj[current]) {
            int next = e.v;
            int cost = e.weight;

            if (!visited[next] && cost < minCost[next]) {
                minCost[next] = cost;
                parent[next] = current;
                pq.push({next, cost});
            }
        }
    }

    cout << "Total weight " << total << endl;
    cout << "Root node " << start << endl;
    for (auto &edge : result) {
        cout << edge.first << " " << edge.second << endl;
    }
}

void kruskalMST(vector<Edge> &edgeList, int vertices) {
    sort(edgeList.begin(), edgeList.end());

    DSU dsu(vertices);
    long long total = 0;
    vector<pair<int, int>> result;

    for (Edge &e : edgeList) {
        if (dsu.unite(e.u, e.v)) {
            total += e.weight;
            result.push_back({e.u, e.v});

            if (result.size() == vertices - 1)
                break;
        }
    }

    cout << "Total weight " << total << endl;
    for (auto &edge : result) {
        cout << edge.first << " " << edge.second << endl;
    }
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<Edge>> adj(n);
    vector<Edge> edgeList;

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;

        adj[u].push_back(Edge(u, v, w));
        adj[v].push_back(Edge(v, u, w));

        edgeList.push_back(Edge(u, v, w));
    }

    int root;
    cin >> root;

    primMST(adj, n, root);

    cout << endl;

    kruskalMST(edgeList, n);

    return 0;
}
