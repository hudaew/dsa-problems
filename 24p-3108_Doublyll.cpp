#include <iostream>
using namespace std;

class node {
public:
    int value;
    node* next;
    node* prev;
    node(int v) {
        value = v;
        next  = NULL;
        prev  = NULL;
    }
};

class list {
public:
    node* head;
    node* tail;

    list() {
        head = tail = NULL;
    }

    void pushback(int v) {
        node* newnode = new node(v);
        if (head == NULL) {
            head = tail = newnode;
        } else {
            tail->next    = newnode;
            newnode->prev = tail;
            tail          = newnode;
        }
    }

   
    void display(node* h) {
        if (h == NULL) { cout << "empty\n"; return; }
        node* temp = h;
        while (temp != NULL) {
            cout << temp->value;
            if (temp->next != NULL) cout << " <-> ";
            temp = temp->next;
        }
        cout << endl;
    }

    void insertsorted(int v) {
        node* newnode = new node(v);

        // case 1 — empty list
        if (head == NULL) {
            head = tail = newnode;
            return;
        }

        // case 2 — insert before head
        if (v <= head->value) {
            newnode->next = head;
            head->prev    = newnode;
            head          = newnode;
            return;
        }

        // case 3 — insert after tail
        if (v >= tail->value) {
            tail->next    = newnode;
            newnode->prev = tail;
            tail          = newnode;
            return;
        }

        // case 4 — find the right spot in middle
        node* temp = head;
        while (temp->next != NULL && temp->next->value < v) {
            temp = temp->next;  // keep moving until next is bigger
        }

        // insert between temp and temp->next
        newnode->next       = temp->next;
        newnode->prev       = temp;
        temp->next->prev    = newnode;
        temp->next          = newnode;
    }

   
    void deletekey(int key) {
        node* temp = head;

        while (temp != NULL) {
            if (temp->value == key) {  // found a match

                node* todelete = temp;
                temp           = temp->next;  // move forward before deleting

                // fix the neighbour before it
                if (todelete->prev != NULL) {
                    todelete->prev->next = todelete->next;
                } else {
                    head = todelete->next;  // deleted node was head → update head
                }

                // fix the neighbour after it
                if (todelete->next != NULL) {
                    todelete->next->prev = todelete->prev;
                } else {
                    tail = todelete->prev;  // deleted node was tail → update tail
                }

                delete todelete;

            } else {
                temp = temp->next;  // no match → move forward
            }
        }
    }

    
    void pairswithsum(int sum) {
        node* left  = head;  // start pointer
        node* right = tail;  // end pointer

        bool found = false;

        while (left != NULL && right != NULL && left != right && right->next != left) {
            int current = left->value + right->value;

            if (current == sum) {       // found a pair
                cout << "(" << left->value << ", " << right->value << ")\n";
                left  = left->next;     // move both inward
                right = right->prev;
                found = true;
            } else if (current < sum) { // too small → move left forward
                left = left->next;
            } else {                    // too big → move right backward
                right = right->prev;
            }
        }

        if (!found) cout << "no pairs found\n";
    }

    
    void rotate(int n) {
        if (head == NULL || n == 0) return;

        // find nth node
        node* nth = head;
        for (int i = 1; i < n; i++) {
            if (nth->next == NULL) return;  // n is bigger than list size
            nth = nth->next;
        }

        // nth is now the nth node
        node* newhead = nth->next;  // node after nth is new head
        if (newhead == NULL) return;  // nth was last node, nothing to rotate

        // connect original tail to original head
        tail->next    = head;
        head->prev    = tail;

        // cut after nth node
        nth->next     = NULL;
        newhead->prev = NULL;

        // update head and tail
        head = newhead;
        tail = nth;
    }

   
    node* mergesorted(node* l1, node* l2) {
        if (l1 == NULL) return l2;
        if (l2 == NULL) return l1;

        node* mergedhead = NULL;
        node* mergedtail = NULL;

        while (l1 != NULL && l2 != NULL) {
            node* picked;

            if (l1->value <= l2->value) {  // l1 is smaller
                picked = l1;
                l1     = l1->next;
            } else {                        // l2 is smaller
                picked = l2;
                l2     = l2->next;
            }

            // disconnect picked node from its old list
            picked->next = NULL;
            picked->prev = NULL;

            // add to merged list
            if (mergedhead == NULL) {
                mergedhead = mergedtail = picked;
            } else {
                mergedtail->next = picked;
                picked->prev     = mergedtail;
                mergedtail       = picked;
            }
        }

        // attach remaining nodes
        node* remaining = (l1 != NULL) ? l1 : l2;
        if (remaining != NULL) {
            mergedtail->next  = remaining;
            remaining->prev   = mergedtail;
        }

        return mergedhead;
    }



    // tree node structure
    struct treenode {
        int value;
        treenode* left;
        treenode* right;
        treenode(int v) {
            value = v;
            left  = right = NULL;
        }
    };

    node* treehead = NULL;  // head of resulting DLL
    node* treetail = NULL;  // tail of resulting DLL

    void inorder(treenode* root) {
        if (root == NULL) return;

        inorder(root->left);   // go left first

        // visit current node → add to DLL
        node* newnode = new node(root->value);
        if (treehead == NULL) {
            treehead = treetail = newnode;
        } else {
            treetail->next = newnode;
            newnode->prev  = treetail;
            treetail       = newnode;
        }

        inorder(root->right);  // go right
    }

    node* treetodll(treenode* root) {
        treehead = treetail = NULL;  // reset
        inorder(root);
        return treehead;
    }

    
    void tripletswithsum(int sum) {
        node* first = head;
        bool  found = false;

        while (first != NULL && first->next != NULL) {
            node* left  = first->next;  // left starts just after first
            node* right = tail;         // right starts at end

            while (left != NULL && right != NULL && left != right && right->next != left) {
                int current = first->value + left->value + right->value;

                if (current == sum) {        // found a triplet
                    cout << "(" << first->value << ", " << left->value << ", " << right->value << ")\n";
                    left  = left->next;
                    right = right->prev;
                    found = true;
                } else if (current < sum) {  // too small → move left forward
                    left = left->next;
                } else {                     // too big → move right backward
                    right = right->prev;
                }
            }

            first = first->next;  // fix next node
        }

        if (!found) cout << "no triplets found\n";
    }

    
    node* mergek(node* lists[], int k) {
        if (k == 0) return NULL;

        node* result = lists[0];  // start with first list

        for (int i = 1; i < k; i++) {
            result = mergesorted(result, lists[i]);  // merge one by one
        }

        return result;
    }

    node* reversekgroup(node* h, int k) {
        if (h == NULL) return NULL;

        // check if k nodes exist
        node* check = h;
        for (int i = 0; i < k; i++) {
            if (check == NULL) return h;  // less than k → dont reverse
            check = check->next;
        }

        // reverse k nodes by swapping next and prev of each node
        node* current = h;
        node* temp    = NULL;
        node* newhead = NULL;
        int   count   = 0;

        while (current != NULL && count < k) {
            temp          = current->prev;   // swap next and prev
            current->prev = current->next;
            current->next = temp;

            newhead = current;               // keep updating newhead
            current = current->prev;         // move forward (was next before swap)
            count++;
        }
        // newhead = new head of this group
        // h       = tail of this group (original head)
        // current = start of next group

        // recursively reverse next group
        node* nextgrouphead = reversekgroup(current, k);

        // connect tail of this group to head of next group
        h->next = nextgrouphead;
        if (nextgrouphead != NULL) {
            nextgrouphead->prev = h;
        }

        return newhead;
    }

    void splitcircular(node* h, node*& head1, node*& head2) {
        if (h == NULL) { head1 = head2 = NULL; return; }

        // find tail first (the node that points back to head)
        node* tail2 = h;
        while (tail2->next != h) {
            tail2 = tail2->next;
        }

        // slow/fast pointer to find middle
        node* slow = h;
        node* fast = h;

        while (fast->next != h && fast->next->next != h) {
            slow = slow->next;
            fast = fast->next->next;
        }
        // slow is at middle

        // head of second half
        head2 = slow->next;

        // fix first half — make it circular
        head1       = h;
        slow->next  = head1;    // tail of first half points back to head1
        head1->prev = slow;

        // fix second half — make it circular
        tail2->next  = head2;   // tail of second half points back to head2
        head2->prev  = tail2;
    }
};

node* buildlist(int n) {
    list temp;
    cout << "enter " << n << " elements:\n";
    for (int i = 0; i < n; i++) {
        int v;
        cin >> v;
        temp.pushback(v);
    }
    return temp.head;
}

// display circular DLL
void displaycircular(node* h) {
    if (h == NULL) { cout << "empty\n"; return; }
    node* temp = h;
    do {
        cout << temp->value;
        if (temp->next != h) cout << " <-> ";
        temp = temp->next;
    } while (temp != h);
    cout << endl;
}

int main() {
    list  l;
    node* head = NULL;
    int   choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1.  build list\n";
        cout << "2.  display\n";
        cout << "3.  insert in sorted list\n";
        cout << "4.  delete all occurrences of key\n";
        cout << "5.  find pairs with given sum\n";
        cout << "6.  rotate by n nodes\n";
        cout << "7.  merge two sorted lists\n";
        cout << "8.  convert binary tree to DLL\n";
        cout << "9.  find triplets with given sum\n";
        cout << "10. merge k sorted lists\n";
        cout << "11. reverse in k groups\n";
        cout << "12. split circular DLL into two halves\n";
        cout << "13. exit\n";
        cout << "enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1: {
            int n;
            cout << "enter size: ";
            cin >> n;
            head = buildlist(n);
            cout << "list built: ";
            l.display(head);
            break;
        }

        case 2:
            l.display(head);
            break;

        case 3: {
            int v;
            cout << "enter value to insert: ";
            cin >> v;
            l.insertsorted(v);
            head = l.head;
            cout << "after insert: ";
            l.display(head);
            break;
        }

        case 4: {
            int key;
            cout << "enter key to delete: ";
            cin >> key;
            l.deletekey(key);
            head = l.head;
            cout << "after deletion: ";
            l.display(head);
            break;
        }

        case 5: {
            int sum;
            cout << "enter sum: ";
            cin >> sum;
            l.pairswithsum(sum);
            break;
        }

        case 6: {
            int n;
            cout << "enter n: ";
            cin >> n;
            l.rotate(n);
            head = l.head;
            cout << "after rotation: ";
            l.display(head);
            break;
        }

        case 7: {
            int n2;
            cout << "enter size of second list: ";
            cin >> n2;
            node* head2 = buildlist(n2);
            cout << "list 1: "; l.display(head);
            cout << "list 2: "; l.display(head2);
            head = l.mergesorted(head, head2);
            l.head = head;
            cout << "merged: ";
            l.display(head);
            break;
        }

        case 8: {
            // build a small hardcoded tree for demo
            //      10
            //     /  \
            //    5   20
            list::treenode* root  = new list::treenode(10);
            root->left            = new list::treenode(5);
            root->right           = new list::treenode(20);
            node* dllhead         = l.treetodll(root);
            cout << "tree to DLL: ";
            l.display(dllhead);
            break;
        }

        case 9: {
            int sum;
            cout << "enter sum: ";
            cin >> sum;
            l.tripletswithsum(sum);
            break;
        }

        case 10: {
            int k;
            cout << "enter number of lists: ";
            cin >> k;
            node** lists = new node*[k];
            for (int i = 0; i < k; i++) {
                int n;
                cout << "enter size of list " << i + 1 << ": ";
                cin >> n;
                lists[i] = buildlist(n);
                cout << "list " << i + 1 << ": ";
                l.display(lists[i]);
            }
            node* merged = l.mergek(lists, k);
            cout << "merged: ";
            l.display(merged);
            delete[] lists;
            break;
        }

        case 11: {
            int k;
            cout << "enter k: ";
            cin >> k;
            head = l.reversekgroup(head, k);
            l.head = head;
            cout << "after reverse in k groups: ";
            l.display(head);
            break;
        }

        case 12: {
            // make list circular first
            if (head == NULL) { cout << "list is empty\n"; break; }
            node* temp = head;
            while (temp->next != NULL) temp = temp->next;
            temp->next = head;   // tail points to head (circular)
            head->prev = temp;   // head prev points to tail

            cout << "circular list: ";
            displaycircular(head);

            node* head1 = NULL;
            node* head2 = NULL;
            l.splitcircular(head, head1, head2);

            cout << "first half:  ";
            displaycircular(head1);
            cout << "second half: ";
            displaycircular(head2);

            // restore head to first half for further operations
            head   = head1;
            l.head = head1;
            break;
        }

        case 13:
            cout << "exiting...\n";
            break;

        default:
            cout << "invalid choice\n";
        }

    } while (choice != 13);

    return 0;
}