#include <iostream>
#include <vector>
#include <queue>

void BFS(std::vector<std::vector<int>>& adj_list, int start, int finish) {
	std::vector parent(adj_list.size(), 1000);
	std::vector dist(adj_list.size(), 1000);

	std::queue<int> q;
	dist[start] = 0;
	q.push(start);

	while (!q.empty()) {
		int current_vertex = q.front();
		q.pop();
		for (int i = 0; i < adj_list[current_vertex].size(); i++) {
			int to = adj_list[current_vertex][i];
			if (dist[to] == 1000) {
				dist[to] = dist[current_vertex] + 1;
				parent[to] = current_vertex;
				q.push(to);
			}
		}
	}

	std::cout << dist[finish] << "\n";

	// std::vector<int> path;

	for (int v = finish; v != start; v = parent[v]) {
		std::cout << v + 1 << "\n";
	}
	std::cout << start + 1 << "\n";
}


int main() {
	int v = 0;
	int e = 0;

	std::cin >> v >> e;
	std::vector<std::vector<int>> adj_list(v, std::vector<int>());

	int start = 0;
	int finish = 0;
	std::cin >> start >> finish;

	for (int i = 0; i < e; i++) {
		int a = 0;
		int b = 0;
		std::cin >> a >> b;
		adj_list[a - 1].push_back(b - 1);
		adj_list[b - 1].push_back(a - 1);
	}

	BFS(adj_list, start - 1, finish - 1);
	return 0;
}