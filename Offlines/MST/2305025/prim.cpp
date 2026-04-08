#include<bits/stdc++.h>
using namespace std;


int main(){
    int m, n;
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n);
    vector<pair<int, int>> parent(n, {-1, 0});
    
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> vis(n,0);
    pq.push({0,0});
    int sum = 0;
    while (!pq.empty())
    {
        auto it = pq.top();
        pq.pop();
        int v = it.first;
        int wt = it.second;
        if(vis[v]==1) continue;

        vis[v] = 1;
        sum += wt;
        for(auto it: adj[v]){
            int adjNode = it.first;
            int edW = it.second;
            if(vis[adjNode]==0){
                pq.push({edW, adjNode});
                parent[adjNode] = {v, edW};
            }
        }
    }
    cout << sum << endl;
    for (auto it: parent)
    {
        cout << it.first << " " << it.second << "\n";
    }
    
    
    return 0;
}