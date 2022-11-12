/**Failing is always better than never trying*/
/**Winners never quit and quitters never win.*/

#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define inf 1e9
#define pb push_back
#define pii pair<int,int>
#define endl '\n'

#define ll long long
const ll maxnodes = 10005;

ll nodes = maxnodes, src, dest;
ll dist[maxnodes], q[maxnodes], work[maxnodes];
int vis[maxnodes];

vector<int>adj[maxnodes],level[2];
void dfs(int node,int par,int h)
{
    level[h%2].pb(node);
    vis[node]=1;
    for(auto son:adj[node])
    {
        if(son==par)
            continue;
        dfs(son,node,h+1);
    }
}

struct Edge
{
    ll to, rev;
    ll f, cap;
};

vector<Edge> g[maxnodes];

void addEdge(ll s, ll t, ll cap)
{
    Edge a = {t, g[t].size(), 0, cap};
    Edge b = {s, g[s].size(), 0, 0};
    g[s].push_back(a);
    g[t].push_back(b);
}

bool dinic_bfs()
{
    fill(dist, dist + nodes, -1);

    dist[src] = 0;
    ll index = 0;
    q[index++] = src;

    for (ll i = 0; i < index; i++)
    {
        ll u = q[i];
        for (ll j = 0; j < (ll) g[u].size(); j++)
        {
            Edge &e = g[u][j];
            if (dist[e.to] < 0 && e.f < e.cap)
            {
                dist[e.to] = dist[u] + 1;
                q[index++] = e.to;
            }
        }
    }
    return dist[dest] >= 0;
}

ll dinic_dfs(ll u, ll f)
{
    if (u == dest)
        return f;

    for (ll &i = work[u]; i < (ll) g[u].size(); i++)
    {
        Edge &e = g[u][i];

        if (e.cap <= e.f)
            continue;

        if (dist[e.to] == dist[u] + 1)
        {
            ll flow = dinic_dfs(e.to, min(f, e.cap - e.f));
            if (flow > 0)
            {
                e.f += flow;
                g[e.to][e.rev].f -= flow;
                return flow;
            }
        }
    }
    return 0;
}

ll maxFlow(ll _src, ll _dest)
{
    src = _src;
    dest = _dest;
    ll result = 0;
    while (dinic_bfs())
    {
        fill(work, work + nodes, 0);
        while (ll delta = dinic_dfs(src, inf))
            result += delta;
    }
    return result;
}
void all_clear()
{
    for(int i=1; i<maxnodes; i++)
    {
        g[i].clear();
        adj[i].clear();
        dist[i]=q[i]=work[i]=0;
        vis[i]=0;
    }
    level[0].clear();
    level[1].clear();
}

/// addEdge(u, v, C);    edge from u to v. Capacity is C
/// maxFlow(s, t);  max flow from s to t
///Find max bipartite matching by adding dummy source and sink.

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin>>t;
    for(int tc=1; tc<=t; tc++)
    {
        int n,m;
        cin>>n>>m;
        for(int i=0; i<m; i++)
        {
            int x,y;
            cin>>x>>y;
            adj[x].pb(y);
            adj[y].pb(x);
        }
        int srcc=n+1,sink=n+2;
        ll sub=0;
        for(int i=1; i<=n; i++)
        {
            if(vis[i])
                continue;
            level[0].clear();
            level[1].clear();
            dfs(i,0,0);
            for(auto j:level[0])
            {
                for(auto son:adj[j])
                    addEdge(j,son,1);
            }
            for(auto j:level[0])
                addEdge(srcc,j,1);
            for(auto j:level[1])
                addEdge(j,sink,1);
            sub+=maxFlow(srcc,sink);
            g[srcc].clear();
            g[sink].clear();
        }


        ll res=n-sub;
        cout<<"Case "<<tc<<": ";
        cout<<res<<endl;
        all_clear();
    }
}
///Debug tips : Look for corner logic that is not handled.
