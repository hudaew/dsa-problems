#include <iostream>
using namespace std;


class queuearray {
public:
    int arr[100];
    int front;
    int rear;
    int size;
    int count;

    queuearray() {
        front = 0;
        rear  = -1;
        size  = 100;
        count = 0;
    }

    bool isempty() {
        return count == 0;
    }

    bool isfull() {
        return count == size;
    }

    // enqueue — add element at rear
    void enqueue(int v) {
        if (isfull()) {
            cout << "queue overflow\n";
            return;
        }
        rear        = (rear + 1) % size;  // circular move
        arr[rear]   = v;
        count++;
    }

    // dequeue — remove element from front
    void dequeue() {
        if (isempty()) {
            cout << "queue underflow\n";
            return;
        }
        cout << "dequeued: " << arr[front] << endl;
        front = (front + 1) % size;  // circular move
        count--;
    }

    // front element — view without removing
    void getfront() {
        if (isempty()) {
            cout << "queue is empty\n";
            return;
        }
        cout << "front: " << arr[front] << endl;
    }

    // display — print front to rear
    void display() {
        if (isempty()) { cout << "queue is empty\n"; return; }
        cout << "queue (front to rear): ";
        for (int i = 0; i < count; i++) {
            cout << arr[(front + i) % size];
            if (i < count - 1) cout << " ";
        }
        cout << endl;
    }
};


class node {
public:
    int value;
    node* next;
    node(int v) {
        value = v;
        next  = NULL;
    }
};

class queuelist {
public:
    node* front;
    node* rear;

    queuelist() {
        front = rear = NULL;
    }

    bool isempty() {
        return front == NULL;
    }

    // enqueue — add new node at rear
    void enqueue(int v) {
        node* newnode = new node(v);
        if (rear == NULL) {
            front = rear = newnode;  // first node
        } else {
            rear->next = newnode;
            rear       = newnode;
        }
    }

    // dequeue — remove node from front
    void dequeue() {
        if (isempty()) {
            cout << "queue underflow\n";
            return;
        }
        cout << "dequeued: " << front->value << endl;
        node* todelete = front;
        front          = front->next;
        if (front == NULL) rear = NULL;  // list became empty
        delete todelete;
    }

    // front element — view without removing
    void getfront() {
        if (isempty()) {
            cout << "queue is empty\n";
            return;
        }
        cout << "front: " << front->value << endl;
    }

    // display — print front to rear
    void display() {
        if (isempty()) { cout << "queue is empty\n"; return; }
        cout << "queue (front to rear): ";
        node* temp = front;
        while (temp != NULL) {
            cout << temp->value;
            if (temp->next != NULL) cout << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};


class priorityqueue {
public:
    int values[100];
    int priorities[100];
    int count;

    priorityqueue() {
        count = 0;
    }

    bool isempty() {
        return count == 0;
    }

    // enqueue — add element with priority
    void enqueue(int v, int priority) {
        if (count == 100) {
            cout << "queue overflow\n";
            return;
        }
        values[count]     = v;
        priorities[count] = priority;
        count++;
    }

    // dequeue — remove highest priority element
    void dequeue() {
        if (isempty()) {
            cout << "queue underflow\n";
            return;
        }

        // find index of highest priority
        int maxindex = 0;
        for (int i = 1; i < count; i++) {
            if (priorities[i] > priorities[maxindex]) {
                maxindex = i;
            }
        }

        cout << "dequeued: " << values[maxindex]
             << " (priority " << priorities[maxindex] << ")\n";

        // shift everything left to fill the gap
        for (int i = maxindex; i < count - 1; i++) {
            values[i]     = values[i + 1];
            priorities[i] = priorities[i + 1];
        }
        count--;
    }

    // display — print all elements with priorities
    void display() {
        if (isempty()) { cout << "queue is empty\n"; return; }
        cout << "priority queue (value:priority):\n";
        for (int i = 0; i < count; i++) {
            cout << "  " << values[i] << " : priority " << priorities[i] << "\n";
        }
    }
};


void reversefirstk(queuearray& q, int k) {
    if (k > q.count || k <= 0) {
        cout << "invalid k\n";
        return;
    }

    // step 1 — push first k elements onto stack
    int stack[100];
    int stacktop = -1;

    for (int i = 0; i < k; i++) {
        stacktop++;
        stack[stacktop] = q.arr[q.front];
        q.front         = (q.front + 1) % q.size;
        q.count--;
    }

    // step 2 — pop from stack and enqueue back
    while (stacktop >= 0) {
        q.rear        = (q.rear + 1) % q.size;
        q.arr[q.rear] = stack[stacktop];
        stacktop--;
        q.count++;
    }

    // step 3 — move remaining (n-k) elements to back
    int remaining = q.count - k;
    for (int i = 0; i < remaining; i++) {
        int val       = q.arr[q.front];
        q.front       = (q.front + 1) % q.size;
        q.rear        = (q.rear + 1) % q.size;
        q.arr[q.rear] = val;
    }
}

void checkpalindrome(queuearray& q) {
    if (q.isempty()) { cout << "queue is empty\n"; return; }

    int arr[100];
    int size = q.count;

    // copy queue into array
    for (int i = 0; i < size; i++) {
        arr[i] = q.arr[(q.front + i) % q.size];
    }

    // check palindrome
    bool palindrome = true;
    for (int i = 0; i < size / 2; i++) {
        if (arr[i] != arr[size - 1 - i]) {
            palindrome = false;
            break;
        }
    }

    cout << (palindrome ? "palindrome\n" : "not palindrome\n");
}


int main() {
    queuearray  qa;   // array queue
    queuelist   ql;   // linked list queue
    priorityqueue pq; // priority queue
    int choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "--- array queue ---\n";
        cout << "1.  enqueue (array queue)\n";
        cout << "2.  dequeue (array queue)\n";
        cout << "3.  front   (array queue)\n";
        cout << "4.  display (array queue)\n";
        cout << "--- linked list queue ---\n";
        cout << "5.  enqueue (linked list queue)\n";
        cout << "6.  dequeue (linked list queue)\n";
        cout << "7.  front   (linked list queue)\n";
        cout << "8.  display (linked list queue)\n";
        cout << "--- priority queue ---\n";
        cout << "9.  enqueue (priority queue)\n";
        cout << "10. dequeue (priority queue)\n";
        cout << "11. display (priority queue)\n";
        cout << "--- operations ---\n";
        cout << "12. reverse first k elements (array queue)\n";
        cout << "13. palindrome check (array queue)\n";
        cout << "14. exit\n";
        cout << "enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1: {
            int v;
            cout << "enter value: ";
            cin >> v;
            qa.enqueue(v);
            qa.display();
            break;
        }

        case 2:
            qa.dequeue();
            qa.display();
            break;

        case 3:
            qa.getfront();
            break;

        case 4:
            qa.display();
            break;

        case 5: {
            int v;
            cout << "enter value: ";
            cin >> v;
            ql.enqueue(v);
            ql.display();
            break;
        }

        case 6:
            ql.dequeue();
            ql.display();
            break;

        case 7:
            ql.getfront();
            break;

        case 8:
            ql.display();
            break;

        case 9: {
            int v, p;
            cout << "enter value: ";
            cin >> v;
            cout << "enter priority: ";
            cin >> p;
            pq.enqueue(v, p);
            pq.display();
            break;
        }

        case 10:
            pq.dequeue();
            pq.display();
            break;

        case 11:
            pq.display();
            break;

        case 12: {
            int k;
            cout << "enter k: ";
            cin >> k;
            cout << "before: "; qa.display();
            reversefirstk(qa, k);
            cout << "after:  "; qa.display();
            break;
        }

        case 13:
            checkpalindrome(qa);
            break;

        case 14:
            cout << "exiting...\n";
            break;

        default:
            cout << "invalid choice\n";
        }

    } while (choice != 14);

    return 0;
}