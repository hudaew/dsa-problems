#include <iostream>
using namespace std;


class node {
public:
    int value;
    node* left;
    node* right;
    node(int v) {
        value = v;
        left  = right = NULL;
    }
};


class queue {
public:
    node* arr[100];
    int front;
    int rear;
    int count;

    queue() {
        front = 0;
        rear  = -1;
        count = 0;
    }

    bool isempty() { return count == 0; }

    void enqueue(node* n) {
        rear++;
        arr[rear] = n;
        count++;
    }

    node* dequeue() {
        node* n = arr[front];
        front++;
        count--;
        return n;
    }
};

node* insert(node* root, int v) {
    if (root == NULL) return new node(v);  

    if (v < root->value) {
        root->left  = insert(root->left,  v);  // go left
    } else if (v > root->value) {
        root->right = insert(root->right, v);  // go right
    }

    return root;
}

int height(node* root) {
    if (root == NULL) return 0;  

    int leftheight  = height(root->left);   // height of left subtree
    int rightheight = height(root->right);  // height of right subtree

    // return 1 (current node) + taller of two subtrees
    if (leftheight > rightheight) return 1 + leftheight;
    else                          return 1 + rightheight;
}

void countleaves(node* root, int& leaves, int& nonleaves) {
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL) {
        leaves++;      
    } else {
        nonleaves++;
    }

    countleaves(root->left,  leaves, nonleaves);  // check left subtree
    countleaves(root->right, leaves, nonleaves);  // check right subtree
}


void levelorder(node* root) {
    if (root == NULL) { cout << "empty tree\n"; return; }

    queue q;
    q.enqueue(root);  // start with root

    cout << "level order: ";

    while (!q.isempty()) {
        node* current = q.dequeue();  // take front
        cout << current->value << " ";

        if (current->left  != NULL) q.enqueue(current->left);   // add left child
        if (current->right != NULL) q.enqueue(current->right);  // add right child
    }
    cout << endl;
}

int diameter(node* root, int& maxdiameter) {
    if (root == NULL) return 0;

    int leftheight  = diameter(root->left,  maxdiameter);
    int rightheight = diameter(root->right, maxdiameter);

    int paththrough = leftheight + rightheight;  // path passing through this node

    if (paththrough > maxdiameter) {
        maxdiameter = paththrough;  // update if this is the longest so far
    }

    // return height of this node to parent
    if (leftheight > rightheight) return 1 + leftheight;
    else                          return 1 + rightheight;
}


bool isbst(node* root, int minval, int maxval) {
    if (root == NULL) return true;  // empty tree is a valid BST

    if (root->value <= minval || root->value >= maxval) {
        return false;  // value is out of allowed range
    }

    // left subtree values must be less than root value
    // right subtree values must be greater than root value
    return isbst(root->left,  minval,       root->value) &&
           isbst(root->right, root->value,  maxval);
}


bool findpath(node* root, int target, int path[], int& pathlen) {
    if (root == NULL) return false;  // reached end without finding

    path[pathlen] = root->value;  // add current node to path
    pathlen++;

    if (root->value == target) return true;  // found it

    // try left or right
    if (findpath(root->left,  target, path, pathlen)) return true;
    if (findpath(root->right, target, path, pathlen)) return true;

    
    pathlen--;
    return false;
}


int sumofleftleaves(node* root, bool isleft) {
    if (root == NULL) return 0;

    
    if (root->left == NULL && root->right == NULL && isleft) {
        return root->value;
    }
    return sumofleftleaves(root->left,  true)  +
           sumofleftleaves(root->right, false);
}


bool printancestors(node* root, int target) {
    if (root == NULL) return false;

    if (root->value == target) return true;  // found target

    // check if target is in left or right subtree
    if (printancestors(root->left,  target) ||
        printancestors(root->right, target)) {
        cout << root->value << " ";  
        return true;
    }

    return false;  // target not in this subtree
}


// ======================================================
void kthsmallest(node* root, int k, int& count, int& result) {
    if (root == NULL) return;

    kthsmallest(root->left, k, count, result);  // go left first

    count++;                                     // visited one more node
    if (count == k) {
        result = root->value;                    // found kth smallest
        return;
    }

    kthsmallest(root->right, k, count, result); // go right
}

/
void inorder(node* root) {
    if (root == NULL) return;
    inorder(root->left);
    cout << root->value << " ";
    inorder(root->right);
}


int main() {
    node* root = NULL;
    int choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1.  insert node\n";
        cout << "2.  display (inorder)\n";
        cout << "3.  height of tree\n";
        cout << "4.  count leaf and non-leaf nodes\n";
        cout << "5.  level order traversal\n";
        cout << "6.  diameter of tree\n";
        cout << "7.  check if BST\n";
        cout << "8.  path from root to node\n";
        cout << "9.  sum of left leaves\n";
        cout << "10. print ancestors of a node\n";
        cout << "11. kth smallest element\n";
        cout << "12. exit\n";
        cout << "enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1: {
            int v;
            cout << "enter value: ";
            cin >> v;
            root = insert(root, v);
            cout << "inorder: ";
            inorder(root);
            cout << endl;
            break;
        }

        case 2:
            cout << "inorder: ";
            inorder(root);
            cout << endl;
            break;

        case 3:
            cout << "height: " << height(root) << endl;
            break;

        case 4: {
            int leaves = 0, nonleaves = 0;
            countleaves(root, leaves, nonleaves);
            cout << "leaf nodes:     " << leaves    << endl;
            cout << "non-leaf nodes: " << nonleaves << endl;
            break;
        }

        case 5:
            levelorder(root);
            break;

        case 6: {
            int maxd = 0;
            diameter(root, maxd);
            cout << "diameter: " << maxd << endl;
            break;
        }

        case 7:
            cout << (isbst(root, -1000000, 1000000) ? "is a BST\n" : "not a BST\n");
            break;

        case 8: {
            int target;
            cout << "enter target value: ";
            cin >> target;
            int path[100];
            int pathlen = 0;
            if (findpath(root, target, path, pathlen)) {
                cout << "path: ";
                for (int i = 0; i < pathlen; i++) {
                    cout << path[i];
                    if (i < pathlen - 1) cout << " -> ";
                }
                cout << endl;
            } else {
                cout << "node not found\n";
            }
            break;
        }

        case 9:
            cout << "sum of left leaves: " << sumofleftleaves(root, false) << endl;
            break;

        case 10: {
            int target;
            cout << "enter target value: ";
            cin >> target;
            cout << "ancestors: ";
            if (!printancestors(root, target)) {
                cout << "node not found";
            }
            cout << endl;
            break;
        }

        case 11: {
            int k;
            cout << "enter k: ";
            cin >> k;
            int count = 0, result = -1;
            kthsmallest(root, k, count, result);
            if (result == -1) cout << "k is larger than tree size\n";
            else              cout << "kth smallest: " << result << endl;
            break;
        }

        case 12:
            cout << "exiting...\n";
            break;

        default:
            cout << "invalid choice\n";
        }

    } while (choice != 12);

    return 0;
}