#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

struct SegmentTreeNode {
    int start;
    int end;
    int max; // Maximum element in the range
    int sum; // Sum of elements in the range
    struct SegmentTreeNode* left;
    struct SegmentTreeNode* right;
};
typedef struct SegmentTreeNode* NODE;

NODE buildSegmentTree(int arr[], int start, int end) {
    NODE node = (NODE)malloc(sizeof(struct SegmentTreeNode));
    node->start = start;
    node->end = end;

    // LEAF NODES
    if (start == end) {
        node->max = arr[start];
        node->sum = arr[start];
    } 
    
    // INTERNAL NODES
    else {
        int mid = start + (end - start) / 2;
        node->left = buildSegmentTree(arr, start, mid);
        node->right = buildSegmentTree(arr, mid + 1, end);
        node->max = fmax(node->left->max, node->right->max);
        node->sum = node->left->sum + node->right->sum;
    }

    return node;
}

int getMax(NODE root, int qs, int qe) {
    // COMPLETELY OVERLAP CONDITION
    if (qs <= root->start && qe >= root->end) {
        return root->max;
    }

    // INVALID RANGE
    if (root->end < qs || root->start > qe) {
        return INT_MIN;
    }

    // RETURN MAX FROM LEFT AND RIGHT SEGMENTS
    return fmax(getMax(root->left, qs, qe), getMax(root->right, qs, qe));
}

int getSum(NODE root, int qs, int qe) {
    // COMPLETELY OVERLAP CONDITION 
    if (qs <= root->start && qe >= root->end) {
        return root->sum;
    }

    // INVALID RANGE
    if (root->end < qs || root->start > qe) {
        return 0;
    }

    // RETURN SUM FROM LEFT AND RIGHT SEGMENTS   
    return getSum(root->left, qs, qe) + getSum(root->right, qs, qe);
}

void updateValue(int arr[], NODE root, int index, int newValue) {
    if (index < root->start || index > root->end) {
        printf("\nInvalid Index\n");
        return;
    }

    // IF THE TARGET IS A LEAF
    if (root->start == root->end) {
        root->max = newValue;
        root->sum = newValue;
        arr[index] = newValue;
    } 
    

    else {
        int mid = root->start + (root->end - root->start) / 2;
        if (index <= mid) {
            updateValue(arr, root->left, index, newValue);
        } 
        else {
            updateValue(arr, root->right, index, newValue);
        }
        root->max = fmax(root->left->max, root->right->max);
        root->sum = root->left->sum + root->right->sum;
    }
}

// OPTIONAL
void freeSegmentTree(NODE root) {
    if (root == NULL) {
        return;
    }
    freeSegmentTree(root->left);
    freeSegmentTree(root->right);
    free(root);
}

int main() {
    int arr[] = {1, 3, 5, 7, 9, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    NODE root = buildSegmentTree(arr, 0, n - 1);

    int max = getMax(root, 2, 4);
    printf("Maximum element in range [2, 4] is: %d\n", max);

    int sum = getSum(root, 2, 4);
    printf("Sum of elements in range [2, 4] is: %d\n", sum);

    updateValue(arr, root, 3, 6);

    max = getMax(root, 2, 4);
    printf("Updated maximum element in range [2, 4] is: %d\n", max);

    sum = getSum(root, 2, 4);
    printf("Updated sum of elements in range [2, 4] is: %d\n", sum);

    freeSegmentTree(root);

    return 0;
}
