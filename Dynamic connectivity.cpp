#include <bits/stdc++.h>

using namespace std;
typedef pair < int , int > ipair;
const int N = 100321;

int dsuP[N], dsuR[N];

vector < pair < int*, int > > hist;


int dsuRoot(int v)
{
    while (dsuP[v] != -1)
        v = dsuP[v];
    return v;
}


void dsuMerge(int a, int b)
{
    a = dsuRoot(a);
    b = dsuRoot(b);
    if (a == b)
        return;
    if (dsuR[a] > dsuR[b])
    {
        hist.emplace_back(&dsuP[b], dsuP[b]);
        dsuP[b] = a;
    } else if (dsuR[a] < dsuR[b])
    {
        hist.emplace_back(&dsuP[a], dsuP[a]);
        dsuP[a] = b;
    } else
    {
        hist.emplace_back(&dsuP[a], dsuP[a]);
        hist.emplace_back(&dsuR[b], dsuR[b]);
        dsuP[a] = b;
        ++dsuR[b];
    }
}

struct Query
{
    int u, v;
    bool answer
    string t;
};
int n, m;
Query q[N];


vector < ipair > t[N*4];


void addEdge(int l, int r, ipair uv, int c, int cl, int cr)
{
    if (l > cr || r < cl)
        return;
    if (l <= cl && cr <= r)
    {
        t[c].push_back(uv);
        return;
    }
    int mid = (cl + cr) / 2;
    addEdge(l, r, uv, c*2+1, cl, mid);
    addEdge(l, r, uv, c*2+2, mid+1, cr);
}


void go(int c, int cl, int cr)
{
    int startSize = hist.size();
   
    for (ipair uv : t[c])
        dsuMerge(uv.first, uv.second);

    if (cl == cr)
    {
        
        if (q[cl].t == 3)
            q[cl].answer = (dsuRoot(q[cl].u) == dsuRoot(q[cl].v));
    } else {
        int mid = (cl + cr) / 2;
        go(c*2+1, cl, mid);
        go(c*2+2, mid+1, cr);
    }


    while ((int)hist.size() > startSize)
    {
        *hist.back().first = hist.back().second;
        hist.pop_back();
    }
}

int main()
{
    ios::sync_with_stdio(false);

    cin >> n >> m;
    for (int i = 0; i < n; ++i) 
        dsuP[i] = -1;

    set < pair < ipair, int > > edges;
    for (int i = 0; i < m; ++i)
    {
        cin >> q[i].t >> q[i].u >> q[i].v;
        if (q[i].u > q[i].v) swap(q[i].u, q[i].v);
        if (q[i].t == "ADD")
            edges.emplace(ipair(q[i].u, q[i].v), i);
        else if (q[i].t == "DEL")
        {
            auto iter = edges.lower_bound(make_pair(ipair(q[i].u, q[i].v), 0));
            addEdge(iter->second, i, iter->first, 0, 0, m - 1);
            edges.erase(iter);
        }
    }
    for (auto e : edges)
        addEdge(e.second, m - 1, e.first, 0, 0, m - 1);
    
    go(0, 0, m - 1);
    for (int i = 0; i < m; ++i)
        if (q[i].t == "GET")
        {
            if (q[i].answer)
                cout << "YES\n";
            else
                cout << "NO\n";
        }

    return 0;
