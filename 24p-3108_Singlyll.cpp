#include <iostream>
using namespace std;

class node {
public:
    int value;
    node* next;
    node(int v) {
        value = v;
        next  = NULL;
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
            tail->next = newnode;
            tail       = newnode;
        }
    }

   
    void display(node* h) {
        if (h == NULL) { cout << "empty\n"; return; }
        node* temp = h;
        while (temp != NULL) {
            cout << temp->value;
            if (temp->next != NULL) cout << " -> ";
            temp = temp->next;
        }
        cout << endl;
    }

   
    node* reverse(node* h) {
        node* prev    = NULL;   // nothing is before head at start
        node* current = h;      // start at head
        node* next    = NULL;   // will save next before flipping

        while (current != NULL) {
            next          = current->next;  // save next
            current->next = prev;           // flip arrow
            prev          = current;        // move prev forward
            current       = next;           // move current forward
        }

        return prev;  // prev is the new head
    }

    
    node* mergesorted(node* l1, node* l2) {
        node* mergedhead = NULL;
        node* mergedtail = NULL;

        while (l1 != NULL && l2 != NULL) {

            int picked;

            if (l1->value <= l2->value) {  // l1 front is smaller
                picked = l1->value;
                l1     = l1->next;          // move l1 forward
            } else {                        // l2 front is smaller
                picked = l2->value;
                l2     = l2->next;          // move l2 forward
            }

            // add picked value to merged list
            node* newnode = new node(picked);
            if (mergedhead == NULL) {
                mergedhead = mergedtail = newnode;  // first node
            } else {
                mergedtail->next = newnode;
                mergedtail       = newnode;
            }
        }

        // one list ran out → attach the rest of the other
        if (l1 != NULL) mergedtail->next = l1;
        if (l2 != NULL) mergedtail->next = l2;

        return mergedhead;
    }

    node* middle(node* h) {
        node* slow = h;
        node* fast = h;

        while (fast != NULL && fast->next != NULL) {
            slow = slow->next;        // 1 step
            fast = fast->next->next;  // 2 steps
        }

        return slow;  // slow is at middle
    }

    node* removenth(node* h, int n) {
        node* dummy = new node(0);  // extra node before head
        dummy->next = h;

        node* slow = dummy;
        node* fast = dummy;

        // move fast n steps ahead
        for (int i = 0; i < n; i++) {
            fast = fast->next;
        }

        // move both until fast reaches last node
        while (fast->next != NULL) {
            slow = slow->next;
            fast = fast->next;
        }

        // slow->next is the node to delete
        node* todelete = slow->next;
        slow->next     = slow->next->next;  // skip it
        delete todelete;

        return dummy->next;  // return head (may have changed)
    }

    bool hascycle(node* h) {
        node* slow = h;
        node* fast = h;

        while (fast != NULL && fast->next != NULL) {
            slow = slow->next;        // 1 step
            fast = fast->next->next;  // 2 steps

            if (slow == fast) return true;  // they met → cycle exists
        }

        return false;  // fast hit NULL → no cycle
    }

    node* addtwonumbers(node* l1, node* l2) {
        node* resulthead = NULL;
        node* resulttail = NULL;
        int   carry      = 0;

        while (l1 != NULL || l2 != NULL || carry != 0) {
            int sum = carry;  // start with leftover carry

            if (l1 != NULL) { sum += l1->value; l1 = l1->next; }  // add l1 digit
            if (l2 != NULL) { sum += l2->value; l2 = l2->next; }  // add l2 digit

            carry = sum / 10;  // carry for next round
            sum   = sum % 10;  // digit to store

            node* newnode = new node(sum);
            if (resulthead == NULL) {
                resulthead = resulttail = newnode;
            } else {
                resulttail->next = newnode;
                resulttail       = newnode;
            }
        }

        return resulthead;
    }

    node* reorder(node* h) {
        if (h == NULL || h->next == NULL) return h;

        // step 1 — find middle (slow/fast pointer trick)
        node* slow = h;
        node* fast = h;
        while (fast->next != NULL && fast->next->next != NULL) {
            slow = slow->next;
            fast = fast->next->next;
        }
        // slow is at middle

        // step 2 — cut into two halves
        node* secondhalf = slow->next;
        slow->next       = NULL;        // end first half here

        // step 3 — reverse second half
        secondhalf = reverse(secondhalf);

        // step 4 — merge alternately
        node* first  = h;
        node* second = secondhalf;

        while (second != NULL) {
            node* tmp1 = first->next;   // save next of first
            node* tmp2 = second->next;  // save next of second

            first->next  = second;      // first points to second
            second->next = tmp1;        // second points to next first

            first  = tmp1;              // move first forward
            second = tmp2;              // move second forward
        }

        return h;
    }

    node* removeduplicates(node* h) {
        node* current = h;

        while (current != NULL && current->next != NULL) {
            if (current->value == current->next->value) {  // duplicate found?
                node* todelete = current->next;
                current->next  = current->next->next;       // skip it
                delete todelete;
                // dont move current → check again (could be triple duplicate)
            } else {
                current = current->next;  // no duplicate → move forward
            }
        }

        return h;
    }

    node* reversekgroup(node* h, int k) {

        // check if k nodes exist
        node* check = h;
        for (int i = 0; i < k; i++) {
            if (check == NULL) return h;  // less than k → dont reverse
            check = check->next;
        }

        // reverse k nodes
        node* prev    = NULL;
        node* current = h;
        node* next    = NULL;
        int   count   = 0;

        while (current != NULL && count < k) {
            next          = current->next;
            current->next = prev;
            prev          = current;
            current       = next;
            count++;
        }

        // connect tail of this group to result of next group
        h->next = reversekgroup(current, k);

        return prev;  // new head of this group
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

int main() {
    list  l;
    node* head = NULL;
    int   choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1.  build list\n";
        cout << "2.  display\n";
        cout << "3.  reverse\n";
        cout << "4.  merge two sorted lists\n";
        cout << "5.  middle of list\n";
        cout << "6.  remove nth from end\n";
        cout << "7.  cycle detection\n";
        cout << "8.  add two numbers\n";
        cout << "9.  reorder list\n";
        cout << "10. remove duplicates (sorted)\n";
        cout << "11. reverse in k groups\n";
        cout << "12. exit\n";
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

        case 3:
            head = l.reverse(head);
            cout << "reversed: ";
            l.display(head);
            break;

        case 4: {
            int n2;
            cout << "enter size of second list: ";
            cin >> n2;
            node* head2 = buildlist(n2);
            cout << "list 1: "; l.display(head);
            cout << "list 2: "; l.display(head2);
            head = l.mergesorted(head, head2);
            cout << "merged: ";
            l.display(head);
            break;
        }

        case 5: {
            node* mid = l.middle(head);
            cout << "middle: " << mid->value << endl;
            break;
        }

        case 6: {
            int n;
            cout << "enter n: ";
            cin >> n;
            head = l.removenth(head, n);
            cout << "after removal: ";
            l.display(head);
            break;
        }

        case 7:
            cout << (l.hascycle(head) ? "cycle detected: true" : "cycle detected: false") << endl;
            break;

        case 8: {
            int n2;
            cout << "enter size of second number list: ";
            cin >> n2;
            node* head2  = buildlist(n2);
            cout << "list 1: "; l.display(head);
            cout << "list 2: "; l.display(head2);
            node* result = l.addtwonumbers(head, head2);
            cout << "sum:    ";
            l.display(result);
            break;
        }

        case 9:
            head = l.reorder(head);
            cout << "reordered: ";
            l.display(head);
            break;

        case 10:
            head = l.removeduplicates(head);
            cout << "after removing duplicates: ";
            l.display(head);
            break;

        case 11: {
            int k;
            cout << "enter k: ";
            cin >> k;
            head = l.reversekgroup(head, k);
            cout << "after reverse in k groups: ";
            l.display(head);
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