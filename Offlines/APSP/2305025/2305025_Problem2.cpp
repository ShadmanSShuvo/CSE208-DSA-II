#include <bits/stdc++.h>

using namespace std;

int main() {
    int n, m;
    cin >> n;

    map<string, int> currencies;
    vector<string> currency_names(n);

    for (int i = 0; i < n; i++) {
        cin >> currency_names[i];
        currencies[currency_names[i]] = i;
    }

    vector<vector<double>> dist(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; i++) {
        dist[i][i] = 1.0;
    }

    cin >> m;

    for (int i = 0; i < m; i++) {
        string u_str, v_str;
        double w;
        cin >> u_str >> w >> v_str;
        int u = currencies[u_str];
        int v = currencies[v_str];
        dist[u][v] = max(dist[u][v], w);
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                dist[i][j] = max(dist[i][j], dist[i][k] * dist[k][j]);
            }
        }
    }

    bool flag = false;
    for (int i = 0; i < n; i++) {
        if (dist[i][i] > 1.0) {
            flag = true;
            break;
        }
    }

    if (flag)
        cout << "Yes\n";
    else
        cout << "No\n";

    return 0;
}