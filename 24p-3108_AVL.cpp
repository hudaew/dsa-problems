#include <iostream>
using namespace std;

// ======================================================
// AVL TREE NODE
// ======================================================
class node {
public:
    int value;
    node* left;
    node* right;
    int   height;  // height of this node

    node(int v) {
        value  = v;
        left   = right = NULL;
        height = 1;  // new node has height 1
    }
};

// ======================================================
// SIMPLE QUEUE FOR LEVEL ORDER
// ======================================================
class queue {
public:
    node* arr[200];
    int front;
    int rear;
    int count;

    queue() { front = 0; rear = -1; count = 0; }

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

// ======================================================
// HELPER FUNCTIONS
// ======================================================

// get height of a node (NULL = 0)
int getheight(node* n) {
    if (n == NULL) return 0;
    return n->height;
}

// update height of a node based on its children
void updateheight(node* n) {
    if (n == NULL) return;
    int left  = getheight(n->left);
    int right = getheight(n->right);
    n->height = 1 + (left > right ? left : right);
}

// get balance factor = left height - right height
int getbalance(node* n) {
    if (n == NULL) return 0;
    return getheight(n->left) - getheight(n->right);
}

// ======================================================
// ROTATIONS
//
// LL ROTATION → right rotation
// use when left-left case (balance > 1 and new node in left of left)
//
//     z                y
//    /                / \
//   y        →       x   z
//  /
// x
// ======================================================
node* rotateright(node* z) {
    node* y  = z->left;   // y becomes new root
    node* t  = y->right;  // save y's right subtree

    y->right = z;         // z goes to right of y
    z->left  = t;         // y's old right becomes z's left

    updateheight(z);  // update z first (it's now lower)
    updateheight(y);  // then update y (it's now higher)

    return y;  // y is new root
}

// ======================================================
// RR ROTATION → left rotation
// use when right-right case (balance < -1 and new node in right of right)
//
//   z                  y
//    \                / \
//     y      →       z   x
//      \
//       x
// ======================================================
node* rotateleft(node* z) {
    node* y  = z->right;  // y becomes new root
    node* t  = y->left;   // save y's left subtree

    y->left  = z;         // z goes to left of y
    z->right = t;         // y's old left becomes z's right

    updateheight(z);  // update z first
    updateheight(y);  // then update y

    return y;  // y is new root
}

// ======================================================
// LR ROTATION → left rotation on left child, then right rotation
// use when left-right case (balance > 1 and new node in right of left)
//
//     z              z              x
//    /              /              / \
//   y       →      x       →     y   z
//    \            /
//     x          y
// ======================================================
node* rotateleftright(node* z) {
    z->left = rotateleft(z->left);  // left rotation on left child
    return rotateright(z);          // right rotation on z
}

// ======================================================
// RL ROTATION → right rotation on right child, then left rotation
// use when right-left case (balance < -1 and new node in left of right)
//
//   z              z                x
//    \              \              / \
//     y      →       x     →     z   y
//    /                \
//   x                  y
// ======================================================
node* rotaterightleft(node* z) {
    z->right = rotateright(z->right);  // right rotation on right child
    return rotateleft(z);              // left rotation on z
}

// ======================================================
// BALANCE A NODE
// check balance factor and apply correct rotation
// ======================================================
node* balance(node* root, int v) {
    int bf = getbalance(root);

    // LL case → right rotation
    if (bf > 1 && v < root->left->value) {
        return rotateright(root);
    }

    // RR case → left rotation
    if (bf < -1 && v > root->right->value) {
        return rotateleft(root);
    }

    // LR case → left-right rotation
    if (bf > 1 && v > root->left->value) {
        return rotateleftright(root);
    }

    // RL case → right-left rotation
    if (bf < -1 && v < root->right->value) {
        return rotaterightleft(root);
    }

    return root;  // already balanced
}

// ======================================================
// 1. INSERTION IN AVL TREE
//
// idea:
// insert like a normal BST first
// after inserting, update height of current node
// check balance factor
// if unbalanced → apply correct rotation
// ======================================================
node* insert(node* root, int v) {
    // step 1 — normal BST insert
    if (root == NULL) return new node(v);

    if (v < root->value) {
        root->left  = insert(root->left,  v);
    } else if (v > root->value) {
        root->right = insert(root->right, v);
    } else {
        return root;  // duplicate → ignore
    }

    // step 2 — update height
    updateheight(root);

    // step 3 — check and fix balance
    return balance(root, v);
}

// ======================================================
// 2. DELETION IN AVL TREE
//
// idea:
// delete like a normal BST first
// after deleting, update height
// check balance factor
// if unbalanced → apply correct rotation
//
// for deleting a node with two children:
// find inorder successor (smallest in right subtree)
// replace value with successor, delete successor
// ======================================================

// helper — find minimum node in a subtree
node* findmin(node* root) {
    while (root->left != NULL) root = root->left;
    return root;
}

// balance after deletion (slightly different from insert balance)
node* balancedelete(node* root) {
    int bf = getbalance(root);

    // left heavy
    if (bf > 1) {
        if (getbalance(root->left) >= 0) {
            return rotateright(root);       // LL case
        } else {
            return rotateleftright(root);   // LR case
        }
    }

    // right heavy
    if (bf < -1) {
        if (getbalance(root->right) <= 0) {
            return rotateleft(root);        // RR case
        } else {
            return rotaterightleft(root);   // RL case
        }
    }

    return root;  // already balanced
}

node* deletenode(node* root, int v) {
    if (root == NULL) { cout << "value not found\n"; return NULL; }

    // step 1 — find the node to delete
    if (v < root->value) {
        root->left  = deletenode(root->left,  v);
    } else if (v > root->value) {
        root->right = deletenode(root->right, v);
    } else {
        // found the node to delete

        // case 1 — no children
        if (root->left == NULL && root->right == NULL) {
            delete root;
            return NULL;
        }

        // case 2 — one child
        if (root->left == NULL) {
            node* temp = root->right;
            delete root;
            return temp;
        }
        if (root->right == NULL) {
            node* temp = root->left;
            delete root;
            return temp;
        }

        // case 3 — two children
        // find inorder successor (smallest in right subtree)
        node* successor  = findmin(root->right);
        root->value      = successor->value;             // copy successor value
        root->right      = deletenode(root->right, successor->value); // delete successor
    }

    // step 2 — update height
    updateheight(root);

    // step 3 — rebalance
    return balancedelete(root);
}

// ======================================================
// 3. PRINT BALANCE FACTOR OF EVERY NODE
//
// idea:
// traverse every node
// for each node print: value, left height, right height, balance factor
// ======================================================
void printbalancefactors(node* root) {
    if (root == NULL) return;

    printbalancefactors(root->left);   // go left

    int lh = getheight(root->left);
    int rh = getheight(root->right);
    int bf = lh - rh;
    cout << "node: " << root->value
         << "  left height: " << lh
         << "  right height: " << rh
         << "  balance factor: " << bf << "\n";

    printbalancefactors(root->right);  // go right
}

// ======================================================
// 4. CHECK IF TREE IS AVL BALANCED
//
// idea:
// for every node check:
//   abs(left height - right height) <= 1
// if any node violates this → not AVL
// ======================================================
bool isavl(node* root) {
    if (root == NULL) return true;

    int lh = getheight(root->left);
    int rh = getheight(root->right);
    int bf = lh - rh;

    if (bf > 1 || bf < -1) return false;  // imbalanced

    return isavl(root->left) && isavl(root->right);
}

// ======================================================
// 5. CONSTRUCT AVL TREE FROM SORTED ARRAY
//
// idea:
// sorted array → middle element becomes root
// this keeps the tree balanced
// left half → left subtree
// right half → right subtree
// repeat recursively
//
// example: [1,2,3,4,5,6,7]
// middle = 4 → root
// left  [1,2,3] → middle=2 → left subtree
// right [5,6,7] → middle=6 → right subtree
// ======================================================
node* buildfromsortedarray(int arr[], int start, int end) {
    if (start > end) return NULL;

    int mid    = (start + end) / 2;
    node* root = new node(arr[mid]);  // middle element is root

    root->left  = buildfromsortedarray(arr, start,   mid - 1);  // left half
    root->right = buildfromsortedarray(arr, mid + 1, end);      // right half

    updateheight(root);
    return root;
}

// ======================================================
// 6. LEVEL ORDER TRAVERSAL
//
// idea:
// use a queue
// add root → take front → print → add its children → repeat
// ======================================================
void levelorder(node* root) {
    if (root == NULL) { cout << "empty tree\n"; return; }

    queue q;
    q.enqueue(root);

    cout << "level order: ";
    while (!q.isempty()) {
        node* current = q.dequeue();
        cout << current->value << " ";

        if (current->left  != NULL) q.enqueue(current->left);
        if (current->right != NULL) q.enqueue(current->right);
    }
    cout << endl;
}

// ======================================================
// INORDER DISPLAY
// ======================================================
void inorder(node* root) {
    if (root == NULL) return;
    inorder(root->left);
    cout << root->value << " ";
    inorder(root->right);
}

// ======================================================
// MAIN
// ======================================================
int main() {
    node* root = NULL;
    int choice;

    do {
        cout << "\n===== AVL TREE MENU =====\n";
        cout << "1.  insert node\n";
        cout << "2.  delete node\n";
        cout << "3.  print balance factors\n";
        cout << "4.  demonstrate all rotations\n";
        cout << "5.  check if AVL balanced\n";
        cout << "6.  build from sorted array\n";
        cout << "7.  level order traversal\n";
        cout << "8.  inorder display\n";
        cout << "9.  exit\n";
        cout << "enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1: {
            int v;
            cout << "enter value: ";
            cin >> v;
            root = insert(root, v);
            cout << "inorder after insert: ";
            inorder(root);
            cout << endl;
            break;
        }

        case 2: {
            int v;
            cout << "enter value to delete: ";
            cin >> v;
            root = deletenode(root, v);
            cout << "inorder after delete: ";
            inorder(root);
            cout << endl;
            break;
        }

        case 3:
            if (root == NULL) { cout << "tree is empty\n"; break; }
            cout << "balance factors of all nodes:\n";
            printbalancefactors(root);
            break;

        case 4: {
            cout << "\n--- LL ROTATION (right rotation) ---\n";
            cout << "insert 30, 20, 10 → imbalance at 30 → right rotation\n";
            node* ll = NULL;
            ll = insert(ll, 30);
            ll = insert(ll, 20);
            ll = insert(ll, 10);
            cout << "result inorder: ";
            inorder(ll); cout << endl;
            levelorder(ll);

            cout << "\n--- RR ROTATION (left rotation) ---\n";
            cout << "insert 10, 20, 30 → imbalance at 10 → left rotation\n";
            node* rr = NULL;
            rr = insert(rr, 10);
            rr = insert(rr, 20);
            rr = insert(rr, 30);
            cout << "result inorder: ";
            inorder(rr); cout << endl;
            levelorder(rr);

            cout << "\n--- LR ROTATION (left then right rotation) ---\n";
            cout << "insert 30, 10, 20 → imbalance at 30 → left-right rotation\n";
            node* lr = NULL;
            lr = insert(lr, 30);
            lr = insert(lr, 10);
            lr = insert(lr, 20);
            cout << "result inorder: ";
            inorder(lr); cout << endl;
            levelorder(lr);

            cout << "\n--- RL ROTATION (right then left rotation) ---\n";
            cout << "insert 10, 30, 20 → imbalance at 10 → right-left rotation\n";
            node* rl = NULL;
            rl = insert(rl, 10);
            rl = insert(rl, 30);
            rl = insert(rl, 20);
            cout << "result inorder: ";
            inorder(rl); cout << endl;
            levelorder(rl);
            break;
        }

        case 5:
            cout << (isavl(root) ? "tree is AVL balanced\n" : "tree is NOT AVL balanced\n");
            break;

        case 6: {
            int n;
            cout << "enter size of sorted array: ";
            cin >> n;
            int arr[100];
            cout << "enter " << n << " sorted elements:\n";
            for (int i = 0; i < n; i++) cin >> arr[i];
            root = buildfromsortedarray(arr, 0, n - 1);
            cout << "tree built successfully\n";
            cout << "inorder: ";
            inorder(root);
            cout << endl;
            levelorder(root);
            break;
        }

        case 7:
            levelorder(root);
            break;

        case 8:
            cout << "inorder: ";
            inorder(root);
            cout << endl;
            break;

        case 9:
            cout << "exiting...\n";
            break;

        default:
            cout << "invalid choice\n";
        }

    } while (choice != 9);

    return 0;
}