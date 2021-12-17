#include <iostream>
#include <vector>

void BuildTree(std::vector<int>& tree, std::vector<int>& data, int current_vertex, int l, int r) {
	if (l == r) {
		tree[current_vertex] = data[l];
		return;
	}
	int m = (l + r) / 2;
	BuildTree(tree, data, 2 * current_vertex, l, m);
	BuildTree(tree, data, 2 * current_vertex + 1, m + 1, r);

	tree[current_vertex] = tree[current_vertex * 2] + tree[current_vertex * 2 + 1];
}

int Sum(std::vector<int>& tree, int current_vertex, int l, int r, int lq, int rq) {
	if (lq > rq) {
		return 0;
	}
	if (l == lq && r == rq) {
		return tree[current_vertex];
	}
	int m = (l + r) / 2;
	return Sum(tree, current_vertex * 2, l, m, lq, std::min(rq, m)) + 
	Sum(tree, current_vertex * 2 + 1, m + 1, r, std::max(m + 1, lq), rq);
}

void Update(std::vector<int>& tree, int new_val, int current_vertex, int l, int r, int pos) {
	if (l == r) {
		tree[current_vertex] = new_val;
		return;
	}

	int m = (l + r) / 2;

	if (pos <= m) {
		Update(tree, new_val, 2 * current_vertex, l, m, pos);
	} else {
		Update(tree, new_val, 2 * current_vertex + 1, m + 1, r, pos);
	}
	tree[current_vertex] = tree[current_vertex * 2] + tree[current_vertex * 2 + 1];
}

int main() {
	int n = 0;
	std::cin >> n;
	std::vector<int> v(n);
	std::vector<int> tree(n * 4);

	for (int i = 0; i < n; i++) {
		std::cin >> v[i];
	}

	BuildTree(tree, v, 1, 0, n - 1);

	// for (int i = 0; i < 4 * n; i++) {
	// 	std::cout << tree[i] << " ";
	// }

	for (int i = 0; i < 2; i++) {
		int l = 0, r = 0;
		std::cin >> l >> r;
		std::cout << Sum(tree, 1, 0, n - 1, l - 1, r - 1) << "\n";
	}
	Update(tree, 5, 1, 0, n - 1, 1);
	int l = 0, r = 0;
	std::cin >> l >> r;
	std::cout << Sum(tree, 1, 0, n - 1, l - 1, r - 1) << "\n";
}