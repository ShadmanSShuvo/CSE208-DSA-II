#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

class EdmondsKarp{
    int n;
    vector<vector<int>>capacity;
    vector<vector<int>>adj;
    public:
    EdmondsKarp(int n){
        this->n = n;
        capacity.resize(n,vector<int>(n,0));
        adj.resize(n,vector<int>());
    }
    void addEdge(int u,int v,int c){
        capacity[u][v] += c;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    int bfs(int u,int t,vector<int>&parent){
        fill(parent.begin(),parent.end(),-1);
        parent[u] = -2;
        queue<pair<int,int>>q;
        q.push({u,INT_MAX});
        while(!q.empty()){
            auto front = q.front();
            int u = front.first;
            int flow = front.second;
            q.pop();
            for(auto v:adj[u]){
                if(parent[v] == -1 and capacity[u][v] > 0){
                    parent[v] = u;
                    int new_flow = min(flow,capacity[u][v]);
                    if(v == t)return new_flow;
                    q.push({v,new_flow});
                }
            }
        }
        return 0;
    }
    int maxFlow(int s,int t){
        int flow = 0;
        vector<int>parent(n);
        while(true){
            int pushed = bfs(s,t,parent);
            if(pushed == 0)break;
            flow += pushed;
            int curr = t;
            while(curr!=s){
                int prev = parent[curr];
                capacity[prev][curr] -= pushed;
                capacity[curr][prev] += pushed;
                curr = prev;
            }
        }
        return flow;
    }
};

int main(){
    int p,q,r, k,m;
    cin >> p >> q >> r;
    vector<int> credits(p);
    vector<int>limits(q);
    vector<int>capacity(r);

    for(int i=0;i<p;i++){          
        cin >> credits[i];
    }
    for(int i=0;i<q;i++){           
        cin >> limits[i];
    }
    for(int i=0;i<r;i++){          
        cin >> capacity[i];
    }
    cin >> k;
    vector<pair<int,int>> stdcourse(k);
    for(int i=0;i<k;i++){ 
        int c, d;         
        cin >>c >> d;
         stdcourse[i].first=c-1;
         stdcourse[i].second=d-1;
    }
    cin >> m;
    vector<pair<int,int>> tcourse(m);
    for(int i=0;i<m;i++){ 
        int c, d;         
        cin >>c >> d;
         tcourse[i].first=c-1;
         tcourse[i].second=d-1;
    }

    EdmondsKarp ek1(p+r+2); 
    EdmondsKarp ek2(q+r+2); 
    EdmondsKarp ek3(p+q+r+2); 
    for(int i=0;i<k;i++){          
        ek1.addEdge(stdcourse[i].first,stdcourse[i].second,1);
    }
    for(int i=0;i<p;i++){          
        ek1.addEdge(p+r,stdcourse[i].first,1e9);
    }
    for(int i=0;i<r;i++){          
        ek1.addEdge(stdcourse[i].second,p+r+1,1e9);
    }

    for(int i=0;i<m;i++){          
        ek2.addEdge(tcourse[i].first,tcourse[i].second,1);
    }
    for(int i=0;i<q;i++){          
        ek1.addEdge(q+r,tcourse[i].first,1e9);
    }
    for(int i=0;i<p;i++){          
        ek1.addEdge(stdcourse[i].second,q+r+1,1e9);
    }

    // ek3
    for(int i=0;i<k;i++){          
        ek3.addEdge(stdcourse[i].first,stdcourse[i].second,1);
    }
    for(int i=0;i<p;i++){          
        ek3.addEdge(p+q+r,stdcourse[i].first,1e9);
    }
    // for(int i=0;i<r;i++){          
    //     ek3.addEdge(stdcourse[i].second,p+q+r,1e9);
    // }

    for(int i=0;i<m;i++){          
        ek3.addEdge(tcourse[i].first + p,tcourse[i].second+ p+ r,1);
    }
    for(int i=0;i<q;i++){          
        ek3.addEdge(p+q+r+1,tcourse[i].first + p+r,1e9);
    }
    // for(int i=0;i<p;i++){          
    //     ek3.addEdge(stdcourse[i].second,q+r+1,1e9);
    // }

    

    int flow1 = ek1.maxFlow(p+r,p+r+1);  
    int flow2 = ek2.maxFlow(q+r,q+r+1);
    int flag1=1;
    int flag2=1;
    int flag3=1;
    for(int i=0; i< p ; i++){
        if(ek3.maxflow(i, p+q+r+1)!=credits[i])
            flag1=0;
    }
    for(int i=0; i< q ; i++){
        if(ek3.maxflow(p+i, p+q+r+1)<=limits[i])
            flag2=0;
    }
    for(int i=0; i< r ; i++){
        if(ek3.maxflow(p+r+i, p+q+r+1)<=capacity[i])
            flag3=0;
    }
    if(!flow1 || !flow2 || !flag3)     
    cout << "NO" << endl;

    else
        cout << "YES " << flow1;
}