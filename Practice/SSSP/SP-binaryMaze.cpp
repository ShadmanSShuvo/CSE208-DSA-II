#include <bits/stdc++.h>

using namespace std;

int shortestPath(vector<vector<int>> &grid, pair<int, int> source,
                 pair<int, int> destination) {
    if (grid.empty() || grid[0].empty())
        return -1;

    const int n = grid.size();
    const int m = grid[0].size();

    if (!grid[source.first][source.second] ||
        !grid[destination.first][destination.second]) {
        return -1;
    }

    vector<vector<int>> dist(n, vector<int>(m, INT_MAX));
    queue<pair<int, int>> q;
    dist[source.first][source.second] = 0;
    q.push(source);

    const int dr[] = {-1, 0, 1, 0};
    const int dc[] = {0, 1, 0, -1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (r == destination.first && c == destination.second) {
            return dist[r][c];
        }

        for (int i = 0; i < 4; i++) {
            const int nr = r + dr[i];
            const int nc = c + dc[i];

            if (nr < 0 || nr >= n || nc < 0 || nc >= m)
                continue;
            if (!grid[nr][nc])
                continue;
            if (dist[nr][nc] <= dist[r][c] + 1)
                continue;

            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }

    return -1;
}

int main()
{
    vector<vector<int>> grid = {
        {1, 0, 1, 1, 1},
        {1, 0, 1, 0, 1},
        {1, 1, 1, 0, 1},
        {0, 0, 1, 1, 1},
        {1, 1, 1, 0, 1}
    };
    pair<int, int> source = {0, 0};
    pair<int, int> destination = {4, 4};
    int result = shortestPath(grid, source, destination);
    if (result != -1) {
        cout << "Shortest path length: " << result << endl;
    } else {
        cout << "No path exists." << endl;
    }
    return 0;
}
