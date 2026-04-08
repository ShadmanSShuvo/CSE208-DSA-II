#include <algorithm> // Required for max
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

// Node supports both Key and Value to meet the "reusable code" requirement
template <typename Key, typename Value> class AVLNode {
  public:
    Key key;
    Value value;
    int height;
    AVLNode *left;
    AVLNode *right;

    AVLNode(Key k, Value v)
        : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
};

template <typename Key, typename Value> class AVLTree {
  private:
    AVLNode<Key, Value> *root;

    int height(AVLNode<Key, Value> *node) { return node ? node->height : 0; }

    int getBalance(AVLNode<Key, Value> *node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateHeight(AVLNode<Key, Value> *node) {
        if (node) {
            node->height = 1 + max(height(node->left), height(node->right));
        }
    }

    AVLNode<Key, Value> *rightRotate(AVLNode<Key, Value> *y) {
        AVLNode<Key, Value> *x = y->left;
        AVLNode<Key, Value> *T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    AVLNode<Key, Value> *leftRotate(AVLNode<Key, Value> *x) {
        AVLNode<Key, Value> *y = x->right;
        AVLNode<Key, Value> *T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    AVLNode<Key, Value> *insertNode(AVLNode<Key, Value> *node, Key key,
                                    Value value, bool &success) {
        if (!node) {
            success = true;
            return new AVLNode<Key, Value>(key, value);
        }

        if (key < node->key) {
            node->left = insertNode(node->left, key, value, success);
        } else if (key > node->key) {
            node->right = insertNode(node->right, key, value, success);
        } else {
            // Duplicate key exists. Return false (0).
            success = false;
            return node;
        }

        updateHeight(node);
        int balance = getBalance(node);

        // LL Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // RR Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // LR Case
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // RL Case
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    AVLNode<Key, Value> *minValueNode(AVLNode<Key, Value> *node) {
        AVLNode<Key, Value> *current = node;
        while (current->left)
            current = current->left;
        return current;
    }

    AVLNode<Key, Value> *deleteNode(AVLNode<Key, Value> *node, Key key,
                                    bool &success) {
        if (!node) {
            success = false;
            return node;
        }

        if (key < node->key) {
            node->left = deleteNode(node->left, key, success);
        } else if (key > node->key) {
            node->right = deleteNode(node->right, key, success);
        } else {
            success = true;

            // Case 1 & 2: No child or One child
            if (!node->left || !node->right) {
                AVLNode<Key, Value> *temp =
                    node->left ? node->left : node->right;

                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                // Case 3: Two children
                AVLNode<Key, Value> *temp = minValueNode(node->right);

                node->key = temp->key;
                node->value = temp->value;

                node->right = deleteNode(node->right, temp->key, success);
            }
        }

        // Fix: Safety check after deletion
        if (!node)
            return node;

        updateHeight(node);
        int balance = getBalance(node);

        // Balancing Logic
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void destroyTree(AVLNode<Key, Value> *node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    // Traversal Helpers
    void preOrder(AVLNode<Key, Value> *node, vector<Key> &result) {
        if (node) {
            result.push_back(node->key);
            preOrder(node->left, result);
            preOrder(node->right, result);
        }
    }

    void inOrder(AVLNode<Key, Value> *node, vector<Key> &result) {
        if (node) {
            inOrder(node->left, result);
            result.push_back(node->key);
            inOrder(node->right, result);
        }
    }

    void postOrder(AVLNode<Key, Value> *node, vector<Key> &result) {
        if (node) {
            postOrder(node->left, result);
            postOrder(node->right, result);
            result.push_back(node->key);
        }
    }

    void levelOrder(AVLNode<Key, Value> *node, vector<Key> &result) {
        if (!node)
            return;
        queue<AVLNode<Key, Value> *> q;
        q.push(node);
        while (!q.empty()) {
            AVLNode<Key, Value> *current = q.front();
            q.pop();

            result.push_back(current->key);

            if (current->left)
                q.push(current->left);
            if (current->right)
                q.push(current->right);
        }
    }

  public:
    AVLTree() : root(nullptr) {}

    bool insert(Key key, Value value) {
        bool success = false;
        root = insertNode(root, key, value, success);
        return success;
    }

    bool remove(Key key) {
        bool success = false;
        root = deleteNode(root, key, success);
        return success;
    }

    vector<Key> traverse(int type) {
        vector<Key> result;
        if (type == 1)
            preOrder(root, result);
        else if (type == 2)
            inOrder(root, result);
        else if (type == 3)
            postOrder(root, result);
        else if (type == 4)
            levelOrder(root, result);
        return result;
    }

    ~AVLTree() { destroyTree(root); }
};

int main() {
    ifstream inFile("input-avl.txt");
    ofstream outFile("output-avl.txt");

    if (!inFile.is_open()) {
        // Fallback or error handling
        return 1;
    }

    int n;
    inFile >> n;

    // Use <int, int> to satisfy template requirement
    AVLTree<int, int> avl;
    vector<string> outputs;

    for (int i = 0; i < n; i++) {
        int ei, xi;
        inFile >> ei >> xi;

        if (ei == 0) {
            bool result = avl.remove(xi);
            outputs.push_back(to_string(ei) + " " + to_string(xi) + " " +
                              (result ? "1" : "0"));
        } else if (ei == 1) {
            // Pass xi as both key and value
            bool result = avl.insert(xi, xi);
            outputs.push_back(to_string(ei) + " " + to_string(xi) + " " +
                              (result ? "1" : "0"));
        } else if (ei == 2) {
            vector<int> result = avl.traverse(xi);
            string output = "";
            for (size_t j = 0; j < result.size(); j++) {
                if (j > 0)
                    output += " ";
                output += to_string(result[j]);
            }
            outputs.push_back(output);
        }
    }

    outFile << outputs.size() << endl;
    for (const string &out : outputs) {
        outFile << out << endl;
    }

    inFile.close();
    outFile.close();

    return 0;
}