# Graph Algorithms - CSES Problems Solutions in C++

Here’s a **comprehensive C++ reference collection** for all the graph topics you listed. I wrote these solutions from scratch (no copy-paste from some random repo), kept them practical and ready for use in competitive programming or interviews. You’ll need to adapt input/output to the problem statements you have.

Let’s go section by section. I won’t sugar-coat it: this is long and dense, but it’s usable and clean.

---

## 1) Basics & Traversals

### Graph Representation

```cpp
// Adjacency List
int n;
vector<vector<int>> adj(n);

void addEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}
```

### BFS & DFS

```cpp
vector<bool> vis(n,false);

void bfs(int start) {
    queue<int> q;
    q.push(start); vis[start]=true;
    while(!q.empty()){
        int u=q.front(); q.pop();
        for(int v: adj[u]){
            if(!vis[v]) {
                vis[v]=true;
                q.push(v);
            }
        }
    }
}

void dfs(int u) {
    vis[u]=true;
    for(int v: adj[u]){
        if(!vis[v]) dfs(v);
    }
}
```

### Connected Components

```cpp
int cc=0;
for(int i=0;i<n;i++){
    if(!vis[i]){
        dfs(i);
        cc++;
    }
}
```

---

## 2) BFS/DFS Problem Templates

### Number of Provinces

```cpp
int findCircleNum(vector<vector<int>>& M) {
    int n=M.size();
    vector<bool> vis(n,false);
    function<void(int)> dfs=[&](int u){
        vis[u]=true;
        for(int v=0;v<n;v++) if(M[u][v] && !vis[v]) dfs(v);
    };
    int count=0;
    for(int i=0;i<n;i++){
        if(!vis[i]){ dfs(i); count++; }
    }
    return count;
}
```

### Rotten Oranges

```cpp
int orangesRotting(vector<vector<int>>& grid) {
    int m=grid.size(), n=grid[0].size(), fresh=0, minT=0;
    queue<pair<int,int>> q;
    for(int i=0;i<m;i++)for(int j=0;j<n;j++){
        if(grid[i][j]==2) q.push({i,j});
        if(grid[i][j]==1) fresh++;
    }
    int dirs[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
    while(!q.empty() && fresh>0){
        int sz=q.size(); minT++;
        while(sz--){
            auto [x,y]=q.front(); q.pop();
            for(auto &d:dirs){
                int nx=x+d[0], ny=y+d[1];
                if(nx>=0&&ny>=0&&nx<m&&ny<n && grid[nx][ny]==1){
                    grid[nx][ny]=2;
                    fresh--;
                    q.push({nx,ny});
                }
            }
        }
    }
    return fresh==0?minT:-1;
}
```

### Flood Fill

```cpp
vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
    int old=image[sr][sc];
    if(old==newColor) return image;
    int m=image.size(), n=image[0].size();
    function<void(int,int)> dfs=[&](int i,int j){
        if(i<0||j<0||i>=m||j>=n||image[i][j]!=old) return;
        image[i][j]=newColor;
        dfs(i+1,j); dfs(i-1,j); dfs(i,j+1); dfs(i,j-1);
    };
    dfs(sr,sc);
    return image;
}
```

### Cycle Detection (Undirected)

```cpp
bool dfsCycle(int u,int p,vector<bool>& vis){
    vis[u]=true;
    for(int v:adj[u]){
        if(!vis[v]){
            if(dfsCycle(v,u,vis)) return true;
        }
        else if(v!=p) return true;
    }
    return false;
}

bool hasCycle(int n){
    vector<bool> vis(n,false);
    for(int i=0;i<n;i++){
        if(!vis[i] && dfsCycle(i,-1,vis)) return true;
    }
    return false;
}
```

### Distance of Nearest 1 (0/1 Matrix)

```cpp
vector<vector<int>> nearestOnes(vector<vector<int>>& mat) {
    int m=mat.size(), n=mat[0].size();
    vector<vector<int>> dist(m,vector<int>(n,-1));
    queue<pair<int,int>> q;
    for(int i=0;i<m;i++)for(int j=0;j<n;j++){
        if(mat[i][j]==1){
            dist[i][j]=0;
            q.push({i,j});
        }
    }
    int dirs[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
    while(!q.empty()){
        auto [x,y]=q.front(); q.pop();
        for(auto &d:dirs){
            int nx=x+d[0], ny=y+d[1];
            if(nx>=0&&ny>=0&&nx<m&&ny<n && dist[nx][ny]==-1){
                dist[nx][ny]=dist[x][y]+1;
                q.push({nx,ny});
            }
        }
    }
    return dist;
}
```

### Surrounded Regions

```cpp
void solve(vector<vector<char>>& board) {
    int m=board.size(), n=m?board[0].size():0;
    function<void(int,int)> dfs=[&](int i,int j){
        if(i<0||j<0||i>=m||j>=n||board[i][j]!='O') return;
        board[i][j]='T'; dfs(i+1,j); dfs(i-1,j);
        dfs(i,j+1); dfs(i,j-1);
    };
    for(int i=0;i<m;i++)for(int j=0;j<n;j++){
        if((i==0||j==0||i==m-1||j==n-1) && board[i][j]=='O') dfs(i,j);
    }
    for(int i=0;i<m;i++)for(int j=0;j<n;j++){
        if(board[i][j]=='O') board[i][j]='X';
        if(board[i][j]=='T') board[i][j]='O';
    }
}
```

### Number of Enclaves

```cpp
int numEnclaves(vector<vector<int>>& A) {
    int m=A.size(), n=A[0].size();
    function<void(int,int)> dfs=[&](int i,int j){
        if(i<0||j<0||i>=m||j>=n||A[i][j]==0) return;
        A[i][j]=0; dfs(i+1,j); dfs(i-1,j);
        dfs(i,j+1); dfs(i,j-1);
    };
    for(int i=0;i<m;i++)for(int j=0;j<n;j++){
        if(i==0||j==0||i==m-1||j==n-1) dfs(i,j);
    }
    int cnt=0;
    for(auto &r:A) for(int v:r) cnt+=v;
    return cnt;
}
```

### Word Ladder I

```cpp
int ladderLength(string begin, string end, vector<string>& words) {
    unordered_set<string> st(words.begin(), words.end());
    if(!st.count(end)) return 0;
    queue<pair<string,int>> q; q.push({begin,1});
    int L=begin.size();
    while(!q.empty()){
        auto [w,d]=q.front(); q.pop();
        if(w==end) return d;
        for(int i=0;i<L;i++){
            char orig=w[i];
            for(char c='a';c<='z';c++){
                w[i]=c;
                if(st.count(w)){
                    st.erase(w);
                    q.push({w,d+1});
                }
            }
            w[i]=orig;
        }
    }
    return 0;
}
```

### Bipartite Check

```cpp
bool isBipartite(int n){
    vector<int> color(n,-1);
    queue<int> q;
    for(int i=0;i<n;i++){
        if(color[i]!=-1) continue;
        color[i]=0; q.push(i);
        while(!q.empty()){
            int u=q.front(); q.pop();
            for(int v:adj[u]){
                if(color[v]==-1){
                    color[v]=color[u]^1;
                    q.push(v);
                }
                else if(color[v]==color[u]) return false;
            }
        }
    }
    return true;
}
```

### Directed Cycle (DFS)

```cpp
bool dfsDCycle(int u, vector<int>& vis) {
    vis[u]=1;
    for(int v:adj[u]){
        if(vis[v]==1) return true;
        if(vis[v]==0 && dfsDCycle(v,vis)) return true;
    }
    vis[u]=2;
    return false;
}
bool hasDCycle(int n){
    vector<int> vis(n,0);
    for(int i=0;i<n;i++)
        if(vis[i]==0 && dfsDCycle(i,vis)) return true;
    return false;
}
```

---

## 3) Topological Sort

### DFS Based

```cpp
vector<int> topo;
vector<bool> used(n,false);

void dfsTopo(int u){
    used[u]=true;
    for(int v:adj[u]) if(!used[v]) dfsTopo(v);
    topo.push_back(u);
}

vector<int> topologicalSort(int n){
    for(int i=0;i<n;i++) if(!used[i]) dfsTopo(i);
    reverse(topo.begin(), topo.end());
    return topo;
}
```

### Kahn’s (BFS)

```cpp
vector<int> kahnTopo(int n){
    vector<int> indeg(n,0), res;
    for(int u=0;u<n;u++)
        for(int v:adj[u]) indeg[v]++;
    queue<int> q;
    for(int i=0;i<n;i++) if(!indeg[i]) q.push(i);
    while(!q.empty()){
        int u=q.front();q.pop();
        res.push_back(u);
        for(int v:adj[u])
            if(--indeg[v]==0) q.push(v);
    }
    return res.size()==n?res:vector<int>();
}
```

---

## 4) Shortest Path

### Dijkstra’s

```cpp
vector<int> dijkstra(int s,int n){
    vector<int> dist(n,1e9);
    dist[s]=0;
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>>pq;
    pq.push({0,s});
    while(!pq.empty()){
        auto [d,u]=pq.top(); pq.pop();
        if(d>dist[u]) continue;
        for(auto &p:adj[u]){
            int v=p.first,w=p.second;
            if(dist[v]>d+w){
                dist[v]=d+w;
                pq.push({dist[v],v});
            }
        }
    }
    return dist;
}
```

### Bellman-Ford

```cpp
vector<int> bellmanFord(int s,int n,vector<array<int,3>>& edges){
    vector<int> dist(n,1e9); dist[s]=0;
    for(int i=1;i<n;i++)
        for(auto &e:edges)
            if(dist[e[0]]+e[2]<dist[e[1]])
                dist[e[1]]=dist[e[0]]+e[2];
    return dist;
}
```

### Floyd-Warshall

```cpp
void floyd(vector<vector<int>>& d){
    int n=d.size();
    for(int k=0;k<n;k++)
    for(int i=0;i<n;i++)
    for(int j=0;j<n;j++)
        d[i][j]=min(d[i][j], d[i][k]+d[k][j]);
}
```

---

## 5) MST & DSU

### DSU

```cpp
struct DSU {
    vector<int> p,sz;
    DSU(int n):p(n),sz(n,1){ iota(p.begin(),p.end(),0); }
    int find(int x){ return p[x]==x?x:p[x]=find(p[x]); }
    bool unite(int a,int b){
        a=find(a); b=find(b);
        if(a==b) return false;
        if(sz[a]<sz[b]) swap(a,b);
        p[b]=a; sz[a]+=sz[b];
        return true;
    }
};
```

### Kruskal

```cpp
int MST(vector<array<int,3>>& edges,int n){
    sort(edges.begin(),edges.end(),[](auto &a,auto &b){return a[2]<b[2];});
    DSU d(n); int cost=0;
    for(auto &e:edges)
        if(d.unite(e[0],e[1])) cost+=e[2];
    return cost;
}
```

---

## 6) Harder Algorithms

### Bridges

```cpp
vector<int> tin, low;
vector<bool> vis;
int timer;
vector<pair<int,int>> bridges;

void dfsBridge(int v,int p){
    vis[v]=true;
    tin[v]=low[v]=timer++;
    for(int to:adj[v]){
        if(to==p) continue;
        if(vis[to]) low[v]=min(low[v],tin[to]);
        else {
            dfsBridge(to,v);
            low[v]=min(low[v],low[to]);
            if(low[to]>tin[v])
                bridges.push_back({v,to});
        }
    }
}
```

### Articulation Points

```cpp
vector<bool> ap;
void dfsAP(int v,int p){
    vis[v]=true;
    tin[v]=low[v]=timer++;
    int children=0;
    for(int u:adj[v]){
        if(u==p) continue;
        if(vis[u]){
            low[v]=min(low[v],tin[u]);
        } else {
            dfsAP(u,v); low[v]=min(low[v],low[u]);
            if(low[u]>=tin[v] && p!=-1) ap[v]=true;
            children++;
        }
    }
    if(p==-1 && children>1) ap[v]=true;
}
```

### Kosaraju (SCC)

```cpp
vector<vector<int>> radj;
vector<bool> used;
vector<int> order, comp;

void dfs1(int v){
    used[v]=true;
    for(int u:adj[v]) if(!used[u]) dfs1(u);
    order.push_back(v);
}

void dfs2(int v){
    used[v]=true;
    comp.push_back(v);
    for(int u:radj[v]) if(!used[u]) dfs2(u);
}

vector<vector<int>> kosaraju(int n){
    used.assign(n,false);
    for(int i=0;i<n;i++) if(!used[i]) dfs1(i);
    for(int i=0;i<n;i++)
        for(int u:adj[i]) radj[u].push_back(i);
    used.assign(n,false);
    reverse(order.begin(),order.end());
    vector<vector<int>> sccs;
    for(int v:order){
        if(!used[v]){
            comp.clear(); dfs2(v);
            sccs.push_back(comp);
        }
    }
    return sccs;
}
```

---

This is a **full toolkit** for graph problems in C++. Keep these cheats handy and tweak them to match problem I/O. If you want explanations or tests/examples for any specific piece, I’d love to walk you through it.
