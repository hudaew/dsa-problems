#include <iostream>
using namespace std;

// ================= display =================
void display(int nums[], int n) {
    cout << "array: [";
    for (int i = 0; i < n; i++) {
        cout << nums[i];
        if (i < n - 1) cout << ", ";
    }
    cout << "]\n";
}

// ================= remove duplicates =================
int removeduplicates(int nums[], int n) {
    if (n == 0) return 0;   // fix

    int newlength = 1;

    for (int i = 1; i < n; i++) {
        if (nums[i] != nums[i - 1]) {
            nums[newlength] = nums[i];
            newlength++;
        }
    }

    return newlength;
}

// ================= longest consecutive =================
int longestconsecutive(int nums[], int n) {

    // copy array (so original not destroyed)
    int temp[100];
    for (int i = 0; i < n; i++) temp[i] = nums[i];

    // sort temp
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (temp[j] > temp[j + 1]) {
                int t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    int longest = 1, count = 1;

    for (int i = 1; i < n; i++) {
        if (temp[i] == temp[i - 1] + 1) count++;
        else if (temp[i] != temp[i - 1]) count = 1;

        if (count > longest) longest = count;
    }

    return longest;
}

// ================= 3sum =================
void threesum(int nums[], int n) {
    cout << "triplets:\n";

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {

                if (nums[i] + nums[j] + nums[k] == 0) {

                    // avoid duplicates (simple check)
                    if (i > 0 && nums[i] == nums[i - 1]) continue;

                    cout << "[" << nums[i] << ", " 
                         << nums[j] << ", " 
                         << nums[k] << "]\n";
                }
            }
        }
    }
}

// ================= move zeroes =================
void movezeroes(int nums[], int n) {
    int pos = 0;

    for (int i = 0; i < n; i++) {
        if (nums[i] != 0) {
            nums[pos] = nums[i];
            pos++;
        }
    }

    for (int i = pos; i < n; i++) {
        nums[i] = 0;
    }
}

// ================= majority =================
int majority(int nums[], int n) {
    int candidate = nums[0];
    int count = 1;

    for (int i = 1; i < n; i++) {
        if (nums[i] == candidate) count++;
        else count--;

        if (count == 0) {
            candidate = nums[i];
            count = 1;
        }
    }

    return candidate;
}

// ================= subarray sum = k =================
int subarraysumk(int nums[], int n, int k) {
    int count = 0;

    for (int i = 0; i < n; i++) {
        int sum = 0;

        for (int j = i; j < n; j++) {
            sum += nums[j];

            if (sum == k) count++;
        }
    }

    return count;
}

// ================= longest subarray <= k =================
int longestlessthank(int nums[], int n, int k) {
    int maxlen = 0;

    for (int i = 0; i < n; i++) {
        int sum = 0;

        for (int j = i; j < n; j++) {
            sum += nums[j];

            if (sum <= k) {
                int len = j - i + 1;
                if (len > maxlen) maxlen = len;
            }
        }
    }

    return maxlen;
}

// ================= main =================
int main() {

    int nums[100], n;

    cout << "enter size: ";
    cin >> n;

    cout << "enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }

    int choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. remove duplicates (sorted)\n";
        cout << "2. longest consecutive sequence\n";
        cout << "3. 3sum\n";
        cout << "4. move zeroes\n";
        cout << "5. majority element\n";
        cout << "6. subarray sum = k\n";
        cout << "7. longest subarray <= k\n";
        cout << "8. display\n";
        cout << "9. exit\n";

        cout << "enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            n = removeduplicates(nums, n);
            display(nums, n);
            break;

        case 2:
            cout << "length: " << longestconsecutive(nums, n) << endl;
            break;

        case 3:
            threesum(nums, n);
            break;

        case 4:
            movezeroes(nums, n);
            display(nums, n);
            break;

        case 5:
            cout << "majority: " << majority(nums, n) << endl;
            break;

        case 6: {
            int k;
            cout << "enter k: ";
            cin >> k;
            cout << "count: " << subarraysumk(nums, n, k) << endl;
            break;
        }

        case 7: {
            int k;
            cout << "enter k: ";
            cin >> k;
            cout << "length: " << longestlessthank(nums, n, k) << endl;
            break;
        }

        case 8:
            display(nums, n);
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