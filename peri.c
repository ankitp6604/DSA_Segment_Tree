#include <stdio.h>
#include <stdlib.h>

struct Node {
    int count;
    int total;
};

struct SegmentTree {
    struct Node *tree;
    int *count;
    int size;
};

void update(struct SegmentTree *segmentTree, int node, int start, int end, int qStart, int qEnd, int val) {
    if (qEnd < start || qStart > end) return;
    if (qStart <= start && qEnd >= end) {
        segmentTree->count[node] += val;
    } 
    else {
        int mid = (start + end) / 2;
        update(segmentTree, 2 * node, start, mid, qStart, qEnd, val);
        update(segmentTree, 2 * node + 1, mid + 1, end, qStart, qEnd, val);
    }

    if (segmentTree->count[node] > 0) segmentTree->tree[node].total = end - start + 1;
    else segmentTree->tree[node].total = (start == end) ? 0 : segmentTree->tree[2 * node].total + segmentTree->tree[2 * node + 1].total;
}

struct SegmentTree *createSegmentTree(int size) {
    struct SegmentTree *segmentTree = malloc(sizeof(struct SegmentTree));
    segmentTree->size = size;
    segmentTree->tree = malloc(4 * size * sizeof(struct Node));
    segmentTree->count = malloc(4 * size * sizeof(int));
    return segmentTree;
}

void insert(struct SegmentTree *segmentTree, int x1, int y1, int x2, int y2) {
    update(segmentTree, 1, 0, 1000000, y1, y2 - 1, 1);
}

void removeRectangle(struct SegmentTree *segmentTree, int x1, int y1, int x2, int y2) {
    update(segmentTree, 1, 0, 1000000, y1, y2 - 1, -1);
}

int getTotalPerimeter(struct SegmentTree *segmentTree) {
    return segmentTree->tree[1].total;
}

int main() {
    struct SegmentTree *segmentTree = createSegmentTree(1000001);

    // Example rectangles
    int rectangles[][4] = {{1, 1, 3, 3}, {2, 2, 4, 4}, {5, 5, 7, 7}};
    int numRectangles = sizeof(rectangles) / sizeof(rectangles[0]);

    // Insert rectangles
    for (int i = 0; i < numRectangles; i++) {
        insert(segmentTree, rectangles[i][0], rectangles[i][1], rectangles[i][2], rectangles[i][3]);
    }

    // Calculate and output total perimeter
    printf("Total perimeter: %d\n", getTotalPerimeter(segmentTree));

    free(segmentTree->tree);
    free(segmentTree->count);
    free(segmentTree);

    return 0;
}