#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>

const int64_t BIG_NUMBER = 1000 * 1000 * 1000 * 1LL;

// template<typename T>
class Treap {
    struct TreeNode {
        TreeNode* parent;
        TreeNode* left;
        TreeNode* right;
        int64_t key;
        int64_t result;
        int64_t priority;
        int64_t size;

        TreeNode(int64_t key_added, int64_t priority_added) {
            key = key_added;
            result = key_added;
            parent = left = right = nullptr;
            priority = priority_added;
            size = 1;
        }
    };
    TreeNode* root = nullptr;

    struct SplitTrees {
        TreeNode* less_x;
        TreeNode* more_x;
    };

    int64_t result(TreeNode* node) {
        if (node == nullptr) {
            return 1000000001;
        }
        return node->result;
    }

    int64_t resultSize(TreeNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->size;
    }

    void updateNode(TreeNode* node) {
        node->result = std::min(std::min(result(node->left), result(node->right)), node->key);
        node->size = resultSize(node->left) + resultSize(node->right) + 1;
    }

    TreeNode* merge(TreeNode* t1, TreeNode* t2) {
        if (t1 == nullptr) {
            return t2;
        }
        if (t2 == nullptr) {
            return t1;
        }
        if (t1->priority > t2->priority) {
            TreeNode* new_right = merge(t1->right, t2);
            t1->right = new_right;
            // new_right->parent = t1;
            TreeNode* ans = t1;
            updateNode(t1);
            return t1;
        } else {
            TreeNode* new_left = merge(t1, t2->left);
            t2->left = new_left;
            // new_left->parent = t2;
            TreeNode* ans = t2;
            updateNode(t2);
            return t2;
        }
    }

    SplitTrees split(TreeNode* t, int64_t k) {
        if (t == nullptr) {
            return {nullptr, nullptr};
        }
        SplitTrees ans;
        if (resultSize(t->left) < k) {
            SplitTrees new_trees;
            new_trees = split(t->right, k - resultSize(t->left) - 1);
            t->right = new_trees.less_x;
            ans = {t, new_trees.more_x};
        } else {
            SplitTrees new_trees;
            new_trees = split(t->left, k);
            t->left = new_trees.more_x;
            ans = {new_trees.less_x, t};
        }
        updateNode(t);
        return ans;
    }

    TreeNode* findPrivate(TreeNode* t, int64_t x) {
        while (t) {
            if (x < resultSize(t)) {
                t = t->left;
            } else if (x > resultSize(t)) {
                t = t->right;
            } else {
                return t;
            }
        }
        return t;
    }

    TreeNode* findSpecialPrivate(TreeNode* t, int pos) {
        SplitTrees one = split(t, pos);
        SplitTrees two = split(one.less_x, pos - 1);
        TreeNode* res = two.more_x;
        TreeNode* three = merge(two.less_x, two.more_x);
        t = merge(three, one.more_x);
        return res;
    }

    TreeNode* insertPrivate(TreeNode* t, int64_t x, int64_t priority) {
        if (!t) {
            return new TreeNode(x, priority);
        }
        SplitTrees new_tree = split(root, x);
        TreeNode* merged_tree = new TreeNode(x, priority);
        return merge(merge(new_tree.less_x, merged_tree), new_tree.more_x);
    }

    TreeNode* reversePartsPrivate(TreeNode* t, int64_t first_pos, int64_t result_pos) {
        SplitTrees left_tree = split(t, first_pos);
        // std::cout << "left_tree:\n\n";
        // printTreePrivate(left_tree.less_x);
        SplitTrees middle_and_right_tree = split(left_tree.more_x, result_pos - resultSize(left_tree.less_x) + 1);
        // printTreePrivate(middle_and_right_tree.more_x);
        TreeNode* merge_left_right = merge(left_tree.less_x, middle_and_right_tree.more_x);

        return merge(middle_and_right_tree.less_x, merge_left_right);
    }
    // 1 2 3 4 5 6 --> 2 3 4 1 5 6

    void printTreePrivate(TreeNode* t) {
        // TreeNode* p = root;
        if (t != nullptr) {
            if (t->left != nullptr) {
                printTreePrivate(t->left);
            }
            //fprintf(_out, fmt, p->key, p->data);
            std::cout << t->key << " ";

            if (t->right != nullptr) {
                printTreePrivate(t->right);
            }
        }
    }

    TreeNode* erasePrivate(TreeNode* t, int pos, int64_t& deleted) {
        SplitTrees first = split(t, pos);
        SplitTrees second = split(first.more_x, 1);
        // std::cout << "deleted: " << second.less_x->key.name << "\n";
        // deleted = {second.less_x->key.name, second.less_x->key.key};
        return merge(first.less_x, second.more_x);
    }

    TreeNode* movePointPrivate(TreeNode* t, int pos, int64_t adding) {
        SplitTrees new_tree = split(t, pos);
        TreeNode* merged_tree = new TreeNode(adding, rand() % (int64_t) BIG_NUMBER);
        //TreeNode* sec = merge(tmp, first.more_x);
        return merge(merge(new_tree.less_x, merged_tree), new_tree.more_x);
    }

    int64_t queryPrivate(TreeNode* t, int left_query_border, int right_query_border) {
        SplitTrees tree_left = split(t, right_query_border);
        SplitTrees tree_left_right = split(tree_left.less_x, left_query_border - 1);
        int64_t ans = 0;
        ans = result(tree_left_right.more_x);
        TreeNode* t1 = merge(tree_left_right.more_x, tree_left.more_x);
        t = merge(tree_left_right.less_x, t1);
        return ans;
    }
public:
    void reverseParts(int64_t first_pos, int64_t result_pos) {
        root = reversePartsPrivate(root, first_pos, result_pos);
    }

    void insert(int64_t x, int64_t priority) {
        if (!findPrivate(root, x)) {
            root = insertPrivate(root, x, priority);
        }
    }

    int64_t erase(int pos) {
        int64_t deleted;
        root = erasePrivate(root, pos, deleted);
        // std::cout << "deleted: " << deleted.name << " " << deleted.key << "\n";
        return deleted;
    }

    void movePoint(int pos, int64_t adding) {
        root = movePointPrivate(root, pos, adding);
    }

    void printTree() {
        printTreePrivate(root);
    }

    char find(int pos) {
        TreeNode* res = findPrivate(root, pos);
        return res->key;
    }

    char findSpecial(int pos) {
        TreeNode* res = findSpecialPrivate(root, pos);
        return res->key;
    }

    int64_t query(int left_query_border, int right_query_border) {
        return queryPrivate(root, left_query_border, right_query_border);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    Treap tree;
    int n = 0;
    std::cin >> n;

//    for (int i = 0; i < n; i++) {
//        Element tmp;
//        std::cin >> tmp.name;
//        tmp.key = i + 1;
//        tree.insert(tmp, rand() % (int64_t) BIG_NUMBER);
//    }

    // d.key;
    // tree.insert(d, rand() % (int64_t) BIG_NUMBER);

    for (int i = 0; i < n; i++) {
        char c;
        std::cin >> c;
        if (c == '+') {
            int index = 0, value = 0;
            std::cin >> index >> value;
            tree.movePoint(index, value);
        }
        if (c == '?') {
            int l = 0, r = 0;
            std::cin >> l >> r;
            int64_t res = tree.query(l, r);
            printf("%lld\n", res);
        }
    }
    //yxzx --> xzxy
    //tree.printTree();
}