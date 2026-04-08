#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

void solve() {
    int n;
    // Read number of currencies [cite: 12]
    if (!(cin >> n))
        return;

    map<string, int> name_to_idx;
    vector<string> idx_to_name(n);

    // Read currency names and store their input order [cite: 13, 14]
    for (int i = 0; i < n; i++) {
        cin >> idx_to_name[i];
        name_to_idx[idx_to_name[i]] = i;
    }

    // Initialize distance matrix
    // dist[i][j] stores the maximum multiplier from currency i to j
    vector<vector<double>> dist(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; i++) {
        dist[i][i] = 1.0;
    }

    int m;
    cin >> m; // Read number of exchange rates [cite: 15]
    for (int i = 0; i < m; i++) {
        string u_str, v_str;
        double rate;
        cin >> u_str >> rate >> v_str; // [cite: 17, 18, 19]
        int u = name_to_idx[u_str];
        int v = name_to_idx[v_str];
        // Keep the best rate if multiple exist [cite: 18]
        dist[u][v] = max(dist[u][v], rate);
    }

    // Floyd-Warshall optimized for maximum product
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] > 0 && dist[k][j] > 0) {
                    dist[i][j] = max(dist[i][j], dist[i][k] * dist[k][j]);
                }
            }
        }
    }

    bool any_arbitrage = false;
    // Check and print in the order they appeared in input
    for (int i = 0; i < n; i++) {
        // A currency admits arbitrage if it can return to itself with > 1.0
        if (dist[i][i] > 1.0) {
            cout << idx_to_name[i] << endl;
            any_arbitrage = true;
        }
    }

    if (!any_arbitrage) {
        cout << "No Arbitrage" << endl; // [cite: 25]
    }
}

int main() {
    // The problem implies a single test case per run based on the PDF format,
    // but can be wrapped in while(cin >> n) if multiple cases are expected.
    solve();
    return 0;
}