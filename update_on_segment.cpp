#include <iostream>
#include <cstring>
#include <vector>

template<typename T>
struct TreeNode {
    T value = 0;
    int promise = 0;
};

template<typename T>
class SegmentTree {
    std::vector<TreeNode<T>> tree_;
    int size_;

    void treeBuildPrivate(std::vector<long long>& forces, int index, int left_border, int right_border);
    long long queryPrivate(int index, int left_query_border, int right_query_border, int left_border, int right_border);
    void updatePrivate(int index, int left_border, int right_border, int left_query_border, int right_query_border, int plus);
    void push(int index);
public:
    SegmentTree(size_t size) {
        tree_.resize(size * 4, {0, 0});
        size_ = size;
    }

    void treeBuild(std::vector<long long>& forces, int size);
    void update(int left_query_border, int right_query_border, int plus);
    long long query(int left_query_border, int right_query_border);
};

template<typename T>
void SegmentTree<T>::treeBuildPrivate(std::vector<long long>& forces, int index, int left_border, int right_border) {
    if (right_border - left_border == 0) {
        tree_[index].value = forces[left_border];
    } else {
        int middle = (left_border + right_border) / 2;
        treeBuildPrivate(forces, 2 * index, left_border, middle);
        treeBuildPrivate(forces, 2 * index + 1, middle + 1, right_border);
        tree_[index].value = std::max(tree_[2 * index].value, tree_[2 * index + 1].value);
    }
}

template <typename T>
long long SegmentTree<T>::queryPrivate(int index, int left_query_border, int right_query_border, int left_border, int right_border) {
    if (right_query_border < left_query_border) {
        return 0;
    }
    if (left_border == left_query_border && right_border == right_query_border) {
        return tree_[index].value + tree_[index].promise;
    }
    push(index);
    int middle = (left_border + right_border) / 2;
    return std::max(queryPrivate(2 * index, left_query_border, std::min(right_query_border, middle), left_border, middle),
               queryPrivate(2 * index + 1, std::max(left_query_border, middle + 1), right_query_border, middle + 1, right_border));
}

template<typename T>
void SegmentTree<T>::updatePrivate(int index, int left_border, int right_border, int left_query_border, int right_query_border, int plus) {
    if (left_query_border - right_query_border > 0) {
        return;
    }
    if (left_border == left_query_border && right_border == right_query_border) {
        tree_[index].promise += plus;
    } else {
        push(index);
        int middle = (left_border + right_border) / 2;
        updatePrivate(2 * index, left_border, middle, left_query_border, std::min(right_query_border, middle), plus);
        updatePrivate(2 * index + 1, middle + 1, right_border, std::max(left_query_border, middle + 1), right_query_border,
                      plus);

        tree_[index].value = std::max(tree_[index * 2].value + tree_[index * 2].promise, tree_[index * 2 + 1].value +
                                                                                    tree_[index * 2 + 1].promise);
    }
}

template<typename T>
void SegmentTree<T>::push(int index) {
    tree_[index * 2].promise += tree_[index].promise;
    tree_[index * 2 + 1].promise += tree_[index].promise;
    tree_[index].value += tree_[index].promise;
    tree_[index].promise = 0;
}

template<typename T>
void SegmentTree<T>::treeBuild(std::vector<long long> &forces, int size) {
    treeBuildPrivate(forces, 1, 0, size - 1);
}

template<typename T>
void SegmentTree<T>::update(int left_query_border, int right_query_border, int plus) {
    updatePrivate(1, 0, size_ - 1, left_query_border - 1, right_query_border - 1, plus);
}

template<typename T>
long long SegmentTree<T>::query(int left_query_border, int right_query_border) {
    return queryPrivate(1, left_query_border - 1, right_query_border - 1, 0, size_ - 1);
}

int main() {
    int n;
    std::cin >> n;
    std::vector<long long> forces(n);
    SegmentTree<long long> segment_tree(n);

    for (int i = 0; i < n; i++) {
        std::cin >> forces[i];
    }

    segment_tree.treeBuild(forces, n);

    int m;
    std::cin >> m;
    for (int i = 0; i < m; i++) {
        char c;
        std::cin >> c;
        if (c == 'm') {
            int l = 0, r = 0;
            std::cin >> l >> r;
            std::cout << segment_tree.query(l, r) << " ";
        } else if (c == 'a') {
            int l = 0, r = 0;
            std::cin >> l >> r;
            int value = 0;
            std::cin >> value;
            segment_tree.update(l, r, value);
        }
    }

}