#include <stdlib.h>
#include <stdio.h>

typedef struct AVLTree {
    int value;
    unsigned char height;
    struct AVLTree* left;
    struct AVLTree* right;
} Tree;

unsigned char Height(Tree* tree) {
    return tree ? tree->height:0;
}

void UpdateHeight(Tree* tree) {
    unsigned char left = Height(tree->left), right = Height(tree->right);
    tree->height = ((left > right) ? left:right) + 1;
}

int BalanceFactor(Tree* tree) {
    return Height(tree->right) - Height(tree->left);
}

Tree* RotateRight(Tree* root) {
    Tree* left = root->left;
    root->left = left->right;
    left->right = root;
    UpdateHeight(root);
    UpdateHeight(left);
    return left;
}

Tree* RotateLeft(Tree* root) {
    Tree* right = root->right;
    root->right = right->left;
    right->left = root;
    UpdateHeight(root);
    UpdateHeight(right);
    return right;
}

Tree* Balance(Tree* root) {
    UpdateHeight(root);
    if (BalanceFactor(root) == 2) {
        if (BalanceFactor(root->right) < 0) {
            root->right = RotateRight(root->right);
        }
        return RotateLeft(root);
    }
    if (BalanceFactor(root) == -2) {
        if (BalanceFactor(root->left) > 0) {
            root->left = RotateLeft(root->left);
        }
        return RotateRight(root);
    }
    return root;
}

Tree* Insert(Tree* tree, int value, Tree* originalStart) {
    if (!tree) {
        Tree* newNode = originalStart;
        newNode->value = value;
        newNode->height = 1;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }
    if (value < tree->value) {
        tree->left = Insert(tree->left, value, originalStart);
    }
    else {
        tree->right = Insert(tree->right, value, originalStart);
    }
    tree = Balance(tree);
    return tree;
}

int main(void) {
    int n, value;
    if (!scanf("%i", &n)) exit(0);
    Tree* originalStart = (Tree*) malloc(sizeof(Tree) * n);
    Tree* tree = NULL;

    for (int i = 0; i < n; ++i) {
        if (!scanf("%i", &value)) exit(0);
        tree = Insert(tree, value, &originalStart[i]);
    }
    printf("%u\n", Height(tree));
    free(originalStart);
}
