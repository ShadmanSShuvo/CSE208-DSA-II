#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum Color { RED, BLACK };

template <typename T> class RBTNode {
  public:
    T data;
    Color color;
    RBTNode *left;
    RBTNode *right;
    RBTNode *parent;
    int size;

    RBTNode(T data): data(data), color(RED), left(nullptr), right(nullptr),
          parent(nullptr), size(1) {}
};

template <typename T> class RedBlackTree {
  private:
    RBTNode<T> *root;
    RBTNode<T> *NIL;

    void updateSize(RBTNode<T> *node) {
        if (node != NIL) {
            node->size = 1 + getSize(node->left) + getSize(node->right);
        }
    }

    int getSize(RBTNode<T> *node) { return (node == NIL) ? 0 : node->size; }

    void updateSizesUpward(RBTNode<T> *node) {
        while (node != NIL) {
            updateSize(node);
            node = node->parent;
        }
    }

    void leftRotate(RBTNode<T> *x) {
        RBTNode<T> *y = x->right;
        x->right = y->left;
        if (y->left != NIL)
            y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
        updateSize(x);
        updateSize(y);
    }

    void rightRotate(RBTNode<T> *y) {
        RBTNode<T> *x = y->left;
        y->left = x->right;
        if (x->right != NIL)
            x->right->parent = y;
        x->parent = y->parent;
        if (y->parent == NIL)
            root = x;
        else if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
        x->right = y;
        y->parent = x;
        updateSize(y);
        updateSize(x);
    }

    void insertFixup(RBTNode<T> *z) {
        while (z->parent->color == RED) {
            // case 1 from slide
            if (z->parent == z->parent->parent->left) {
                RBTNode<T> *y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                RBTNode<T> *y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(RBTNode<T> *u, RBTNode<T> *v) {
        if (u->parent == NIL)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        v->parent = u->parent;
    }

    RBTNode<T> *minimum(RBTNode<T> *node) {
        while (node->left != NIL)
            node = node->left;
        return node;
    }

    void deleteFixup(RBTNode<T> *x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                RBTNode<T> *w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                RBTNode<T> *w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    RBTNode<T> *searchNode(T data) {
        RBTNode<T> *current = root;
        while (current != NIL) {
            if (data == current->data)
                return current;
            else if (data < current->data)
                current = current->left;
            else
                current = current->right;
        }
        return NIL;
    }

    void destroyTree(RBTNode<T> *node) {
        if (node != NIL) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

  public:
    RedBlackTree() {
        NIL = new RBTNode<T>(T());
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = NIL;
        NIL->size = 0;
        root = NIL;
    }

    bool insert(T data) {
        if (searchNode(data) != NIL)
            return false;

        RBTNode<T> *z = new RBTNode<T>(data);
        z->left = NIL;
        z->right = NIL;
        z->size = 1;

        RBTNode<T> *y = NIL;
        RBTNode<T> *x = root;

        while (x != NIL) {
            y = x;
            if (z->data < x->data)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;
        if (y == NIL)
            root = z;
        else if (z->data < y->data)
            y->left = z;
        else
            y->right = z;

        updateSizesUpward(z);
        insertFixup(z);
        return true;
    }

    bool remove(T data) {
        RBTNode<T> *z = searchNode(data);
        if (z == NIL)
            return false;

        RBTNode<T> *y = z;
        RBTNode<T> *x;
        RBTNode<T> *xParent;
        Color yOriginalColor = y->color;

        if (z->left == NIL) {
            x = z->right;
            xParent = z->parent;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            xParent = z->parent;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == z) {
                xParent = y;
            } else {
                xParent = y->parent;
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        updateSizesUpward(xParent);

        delete z;
        if (yOriginalColor == BLACK)
            deleteFixup(x);
        return true;
    }

    bool search(T data) { return searchNode(data) != NIL; }

    int countLess(T data) {
        int count = 0;
        RBTNode<T> *current = root;
        while (current != NIL) {
            if (data <= current->data) {
                current = current->left;
            } else {
                count += getSize(current->left) + 1;
                current = current->right;
            }
        }
        return count;
    }

    ~RedBlackTree() {
        destroyTree(root);
        delete NIL;
    }
};

int main() {
    ifstream inFile("input-rbt.txt");
    ofstream outFile("output-rbt.txt");

    if (!inFile.is_open()) {
        return 1;
    }

    int n;
    inFile >> n;

    RedBlackTree<int> rbt;
    vector<string> outputs;

    for (int i = 0; i < n; i++) {
        int ei, xi;
        inFile >> ei >> xi;
        string output = to_string(ei) + " " + to_string(xi) + " ";

        if (ei == 0) {
            bool result = rbt.remove(xi);
            output += (result ? "1" : "0");
        } else if (ei == 1) {
            bool result = rbt.insert(xi);
            output += (result ? "1" : "0");
        } else if (ei == 2) {
            bool result = rbt.search(xi);
            output += (result ? "1" : "0");
        } else if (ei == 3) {
            int count = rbt.countLess(xi);
            output += to_string(count);
        }
        outputs.push_back(output);
    }

    outFile << outputs.size() << endl;
    for (const string &out : outputs) {
        outFile << out << endl;
    }

    inFile.close();
    outFile.close();

    return 0;
}