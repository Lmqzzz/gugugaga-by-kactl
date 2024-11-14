template <const int N, class T>
class ScalingDinic_t {
 public:
  const bool SCALING = N > 32;
  T lim = 1;
  const T INF = numeric_limits<T>::max();

  class edge_t {
   public:
    int to;
    T capa, flow;
    edge_t(int to = 0, T capa = 0, T flow = 0) : to(to), capa(capa), flow(flow) {}
  };

  int dist[N], cur[N];
  vector<int> adj[N];
  vector<edge_t> edge;

  void addEdge(const int &u, const int &v, T capa, bool is_directed = true) {
    adj[u].emplace_back(edge.size());
    edge.emplace_back(v, capa);
    adj[v].emplace_back(edge.size());
    edge.emplace_back(u, is_directed ? 0 : capa);
  }

  bool bfs(const int &s, const int &t) {
    memset(dist, -1, sizeof dist);
    queue<int> q;
    q.emplace(s);
    dist[s] = 0;
    while (q.size()) {
      int u = q.front();
      q.pop();
      for (const int &id : adj[u]) {
        const int v = edge[id].to;
        if (dist[v] != -1) continue;
        if (edge[id].capa - edge[id].flow >= lim) {
          dist[v] = dist[u] + 1;
          q.emplace(v);
        }
      }
    }
    return dist[t] != -1;
  }

  T dfs(const int &u, const int &t, const T &flow) {
    if (u == t || !flow) return flow;
    for (int &i = cur[u]; i < adj[u].size(); i++) {
      const int id = adj[u][i];
      const int v = edge[id].to;
      if (dist[v] != dist[u] + 1) continue;
      if (T delta = dfs(v, t, min(flow, edge[id].capa - edge[id].flow))) {
        edge[id].flow += delta;
        edge[id ^ 1].flow -= delta;
        return delta;
      }
    }
    return 0;
  }

  int64_t maxFlow(const int &s, const int &t) {
    int64_t ans = 0;
    for (lim = SCALING ? (1 << 30) : 1; lim; lim >>= 1) {
      while (bfs(s, t)) {
        memset(cur, 0, sizeof cur);
        while (T flow = dfs(s, t, INF)) {
          ans += flow;
        }
      }
    }
    return ans;
  }
};
