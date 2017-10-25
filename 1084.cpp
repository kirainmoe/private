#include <cstdio>
#include <cstring>
#include <algorithm>
#define LL long long
using namespace std;
const int MAXN = 50050;
const int INF = 0x3f3f3f3f;

struct Edge {
    int last, next;
    LL val;
    Edge(int n = 0, int v = 0): next(n), val(v) {}
};

struct Army {
    int idx, rest;
    bool operator < (const Army &a) const {
        return rest < a.rest;
    }
};

struct Node {
    int dist, idx;
    bool operator < (const Node &n) const {
        return dist < n.dist;
    }
};

int n, m;
int cnt = 0, armyCnt = 0;
int rootSonCount = 0;
int G[MAXN];
int dist[MAXN][25], ancestor[MAXN][25];
int army[MAXN];
int depth[MAXN];
int pos[MAXN];
int rest[MAXN];
int sonOfRoot[MAXN];
bool coverable[MAXN];

LL maxWeight, maxDepth;
LL canReachCount = 0, cannotCover = 0;
Edge e[MAXN << 1];
Army a[MAXN];
Node r[MAXN];

void addEdge(int u, int v, int w)
{
    e[cnt] = Edge(v, w);
    e[cnt].last = G[u];
    G[u] = cnt;
    cnt++;

    e[cnt] = Edge(u, w);
    e[cnt].last = G[v];
    G[v] = cnt;
    cnt++;
}

void process(int root, int dep, int father, int d)
{
    ancestor[root][0] = father;
    depth[root] = 0;
    dist[root][0] = d;
    maxDepth = max((LL)dep, maxDepth);
    for (int i = 1; (1 << i) <= dep; i++)
    {
        ancestor[root][i] = ancestor[ancestor[root][i-1]][i-1];
        dist[root][i] = dist[ancestor[root][i-1]][i-1];
    }
    for (int i = G[root]; i; i = e[i].next)
    {
        int next = e[i].next, w = e[i].val;
        if (!depth[next]) {
            process(next, dep + 1, root, w);
        }
    }
}

bool check(LL limit)
{
    memset(coverable, 0, sizeof coverable);
    memset(a, 0, sizeof a);
    memset(rest, 0, sizeof rest);

    canReachCount = 0, cannotCover = 0;

    for (int i = 0; i < m; i++)
    {
        LL cur = limit;
        pos[i] = army[i];
        for (int j = 20; j >= 0; j--)
        {
            if (dist[pos[i]][j] <= cur && ancestor[pos[i]][j] != 0) {
                cur -= dist[pos[i]][j];
                pos[i] = ancestor[pos[i]][j];
            }
        }
        rest[i] = cur;
        // cannot reach root
        if (pos[i] != 1) {
            coverable[army[i]] = 1;
        } else {
            canReachCount++;
            a[canReachCount].rest = rest[i];
            a[canReachCount].idx = army[i];
        }
    }

    for (int i = 0; i < rootSonCount; i++)
    {
        if (!coverable[sonOfRoot[i]]) {
            cannotCover++;
            r[cannotCover].dist = dist[sonOfRoot[i]][0];
            r[cannotCover].idx = sonOfRoot[i];
        }
    }

    if (canReachCount < cannotCover) {
        return false;
    }

    sort(a, a + canReachCount);
    sort(r, r + cannotCover);

    for (int i = 0; i < canReachCount; i++)
    {
        if (!coverable[a[i].idx]) {
            coverable[a[i].idx] = 1;
        } else {
            // ???
            for (int j = 0; j < cannotCover && coverable[r[j].idx]; j++)
            {
                if (r[j].dist < a[i].rest) {
                    coverable[r[j].idx] = 1;
                }
            }
        }
    }
}

LL binary()
{
    LL left = 1, right = maxWeight * (maxDepth + 1), mid;
    while (left < right)
    {
        mid = (left + right) >> 1;
        if (check(mid)) {
            mid = right;
        } else {
            left = mid;
        }
    }
    return right;
}

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n-1; i++)
    {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        addEdge(u, v, w);
        maxWeight = max(maxWeight, (LL)w);
        if (u == 1) {
            sonOfRoot[rootSonCount++] = v;
        }
    }

    scanf("%d", &m);
    if (rootSonCount > m) {
        printf("-1");
        return 0;
    }
    for (int i = 0; i < m; i++)
    {
        scanf("%d", &army[i]);
    }
    int root = 1;
    process(root, 1, root, 0);
    LL ans = binary();
    return 0;
}
