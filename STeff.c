#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_N 100000

struct SegmentTreeNode {
    int max; // Maximum element in the range
    int sum; // Sum of elements in the range
};

struct SegmentTreeNode tree[4 * MAX_N]; // Max size of tree 

// Function to build the segment tree
void build(int arr[], int index, int start, int end) {
    if (start == end) {
        tree[index].max = arr[start];
        tree[index].sum = arr[start];
    } 
    else {
        int mid = start + ((end - start) >> 1);
        build(arr, (index << 1) + 1, start, mid);
        build(arr, (index << 1) + 2, mid + 1, end);
        tree[index].max = fmax(tree[(index << 1) + 1].max, tree[(index << 1) + 2].max);
        tree[index].sum = tree[(index << 1) + 1].sum + tree[(index << 1) + 2].sum;
    }
}

// Function to update a tree node
void updateTreeNode(int index, int start, int end, int idx, int newValue) {
    if (start == end) {
        tree[index].max = newValue;
        tree[index].sum = newValue;
    } else {
        int mid = start + ((end - start) >> 1);
        if (idx <= mid) {
            updateTreeNode((index << 1) + 1, start, mid, idx, newValue);
        } else {
            updateTreeNode((index << 1) + 2, mid + 1, end, idx, newValue);
        }
        tree[index].max = fmax(tree[(index << 1) + 1].max, tree[(index << 1) + 2].max);
        tree[index].sum = tree[(index << 1) + 1].sum + tree[(index << 1) + 2].sum;
    }
}

// Function to get sum or max on interval [l, r)
int query(int index, int start, int end, int qs, int qe, char type) {
    if (qs <= start && qe >= end) {
        if (type == 's') // Sum query
            return tree[index].sum;
        else // Max query
            return tree[index].max;
    }

    if (end < qs || start > qe) {
        if (type == 's') // Sum query
            return 0;
        else // Max query
            return INT_MIN;
    }

    int mid = start + ((end - start) >> 1);
    int left = query((index << 1) + 1, start, mid, qs, qe, type);
    int right = query((index << 1) + 2, mid + 1, end, qs, qe, type);
    if (type == 's') // Sum query
        return left + right;
    else // Max query
        return fmax(left, right);
}

int main() {
    int arr[] = {1, 3, 5, 7, 9, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    build(arr, 0, 0, n - 1);

    int max = query(0, 0, n - 1, 2, 4, 'm'); // Max query
    printf("Maximum element in range [2, 4] is: %d\n", max);

    int sum = query(0, 0, n - 1, 2, 4, 's'); // Sum query
    printf("Sum of elements in range [2, 4] is: %d\n", sum);

    updateTreeNode(0, 0, n - 1, 3, 6);

    max = query(0, 0, n - 1, 2, 4, 'm'); // Max query
    printf("Updated maximum element in range [2, 4] is: %d\n", max);

    sum = query(0, 0, n - 1, 2, 4, 's'); // Sum query
    printf("Updated sum of elements in range [2, 4] is: %d\n", sum);

    return 0;
}
