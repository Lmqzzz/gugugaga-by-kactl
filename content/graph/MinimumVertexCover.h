// author: lmqzzz
class maximumMatching_t {
 private:
  bool bfs() {
    queue<int> q;
    for (int i = 1; i <= n_left; i++) {
      if (mat[i] == 0) {
        q.emplace(i);
        d[i] = 0;
      } else {
        d[i] = 1e9;
      }
    }
    d[0] = 1e9;

    while (q.size()) {
      int u = q.front();
      q.pop();
      if (d[u] >= d[0]) {
        continue;
      }

      for (int v : adj[u]) {
        if (mini(d[rmat[v]], d[u] + 1)) {
          q.emplace(rmat[v]);
        }
      }
    }

    return d[0] != 1e9;
  }

  bool bpm(const int &u) {
    if (!u) {
      return 1;
    }

    for (int v : adj[u]) {
      if (d[rmat[v]] == d[u] + 1) {
        if (bpm(rmat[v])) {
          mat[u] = v;
          rmat[v] = u;
          return 1;
        }
      }
    }
    d[u] = 1e9;
    return 0;
  }

  void dfs(const int &u, const bool &_) {
    vis[u][_] = 1;
    for (int v : g[u][_]) {
      if (vis[v][_ ^ 1] == 0) {
        dfs(v, _ ^ 1);
      }
    }
  }

 public:
  static constexpr int NMAX = 100000;
  int mat[NMAX], rmat[NMAX], d[NMAX];
  bool vis[NMAX][2];
  int n_left, n_right;
  vector<int> adj[NMAX];
  vector<int> g[NMAX][2];

  maximumMatching_t(int n_left = 0, int n_right = 0)
      : n_left(n_left), n_right(n_right) {
    for (int i = 0; i <= n_left; i++) {
      vector<int>().swap(adj[i]);
    }
  }

  void addEdge(const int &u, const int &v) {
    adj[u].emplace_back(v);
  }

  int findMaximumMatching() {
    memset(mat, 0, (n_left + 1) * sizeof(int));
    memset(rmat, 0, (n_right + 1) * sizeof(int));

    int res = 0;

    while (bfs()) {
      for (int i = 1; i <= n_left; i++) {
        if (mat[i] == 0) {
          res += bpm(i);
        }
      }
    }

    return res;
  }

  vector<pair<int, bool> > minimumVertexCover(const bool &inverse = 0) {
    for (int i = 1; i <= n_left; i++) {
      vector<int>().swap(g[i][0]);
      vis[i][0] = 0;
    }
    for (int i = 1; i <= n_right; i++) {
      vector<int>().swap(g[i][1]);
      vis[i][1] = 0;
    }

    /// if it's a matching: edge from right->left
    /// otherwise: edge from left->right

    for (int i = 1; i <= n_left; i++) {
      for (int v : adj[i]) {
        if (v == mat[i]) {
          g[v][1].emplace_back(i);
        } else {
          g[i][0].emplace_back(v);
        }
      }
    }

    for (int i = 1; i <= n_left; i++) {
      if (mat[i] == 0) {
        dfs(i, 0);
      }
    }

    vector<pair<int, bool> > ans;

    for (int i = 1; i <= n_left; i++) {
      if (vis[i][0] == inverse) {
        ans.emplace_back(i, 0);
      }
    }

    for (int i = 1; i <= n_right; i++) {
      if (vis[i][1] != inverse) {
        ans.emplace_back(i, 1);
      }
    }

    return ans;
  }
};
