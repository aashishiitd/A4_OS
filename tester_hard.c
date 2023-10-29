#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct avlTree{
    struct treeNode* root;
} avlTree;

typedef struct treeNode{
    struct treeNode* parent;
    struct treeNode* leftChild;
    struct treeNode* rightChild;
    int height;
    int value;
} treeNode;

int max(int a, int b){
    return a>b?a:b;
}

// Function to get the height of a node
int height(treeNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

// Function to update the height of a node
void updateHeight( treeNode* node) {
    int leftHeight = height(node->leftChild);
    int rightHeight = height(node->rightChild);
    node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int balance_factor(treeNode* node){
    return height(node->leftChild) - height(node->rightChild);
}

treeNode* RotateLeft(treeNode* node){
    
    treeNode* grandParent = node->parent;
    treeNode* parent = node->rightChild;
    node->rightChild = parent->leftChild;
    if(node->rightChild != NULL) node->rightChild->parent = node;
    parent->leftChild = node;
    node->parent = parent;
    if(grandParent != NULL){
        parent->parent = grandParent;
        if(grandParent->leftChild == node){
            grandParent->leftChild = parent;
        }
        else{
            grandParent->rightChild = parent;
        }    
    }
    else parent->parent = NULL;
    treeNode* temp = parent->leftChild;
    updateHeight(temp);
    updateHeight(parent);
    return parent;
       
}

treeNode* RotateRight(treeNode* node){
    treeNode* grandParent = node->parent;
    treeNode* parent = node->leftChild;
    node->leftChild = parent->rightChild;
    if(node->leftChild != NULL) node->leftChild->parent = node;
    parent->rightChild = node;
    node->parent = parent;
    
    if(grandParent != NULL){
        parent->parent = grandParent;
        if(grandParent->leftChild == node){
            grandParent->leftChild = parent;
        }
        else{
            grandParent->rightChild = parent;
        }    
    }
    else parent->parent = NULL;
    treeNode* temp = parent->rightChild;
    updateHeight(temp);
    updateHeight(parent);
    return parent;    
    
}
//Declaring a new treeNode
treeNode* newTreeNode( int value ){
    struct treeNode* node = (treeNode*)malloc(sizeof(treeNode));
    node->value = value;
    node->parent = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;
    node->height = 1;

    return node;
}

bool contains( int value, treeNode* root ){
    if(root == NULL) return false;

    if(value == root->value) return true;
    else if( value > root->value) return contains(value, root->rightChild);
    return contains(value, root->leftChild);

}

treeNode* give_node( int value, treeNode* root ){
    
    if(value == root->value) return root;
    else if( value > root->value) return give_node(value, root->rightChild);
    return give_node(value, root->leftChild);

}
void swap(treeNode* node1, treeNode* node2){
    int temp = node1->value;
    node1->value = node2->value;
    node2->value = temp;
}

treeNode* swapForDeletion( treeNode* to_del){

    treeNode* swapper = to_del;
    //already covered that there is atleast one child!

    //Case 1: Only leftChild
    if(to_del->leftChild != NULL && to_del->rightChild == NULL){
        swapper = swapper->leftChild;
        return swapForDeletion(swapper);
    }
    //Case 2: Only RightChild
    if(to_del->rightChild != NULL && to_del->leftChild == NULL){
        swapper = swapper->rightChild;
        return swapForDeletion(swapper);
    }

    //Case 3: Both Children
    if(to_del->rightChild != NULL && to_del->leftChild != NULL){
        //go to right && then go as left as possible
        //then call swap for deletion!
        swapper = swapper->rightChild;
        while(swapper->leftChild != NULL){
            swapper = swapper->leftChild;
        }
        return swapForDeletion(swapper);
    }
    return swapper;

}

treeNode* delete( int value, treeNode* root){
    
    if(!contains(value, root)) return root; //value wasn't in tree. No deletion necessary!!
    
    //height also update
    //find the element to be deleted! using recursion.
    treeNode* to_del = give_node(value, root);
    
    //swap until element is a leaf node
    //need to find element to swap
    treeNode* to_swap = to_del; 
    
    while(to_swap-> leftChild != NULL || to_swap->rightChild != NULL){
        to_swap = swapForDeletion(to_swap );
        swap(to_del, to_swap);
    }

    to_del = to_swap;

    //now to_del is a leaf node. Delete it.
    if(to_del->parent->leftChild == to_del){
        to_del->parent->leftChild = NULL;
    }
    else{
        to_del->parent->rightChild = NULL;
    }
    
    //update height of all nodes in the path
    treeNode* temp = to_del->parent;
    while(temp != NULL){
        updateHeight(temp);
        temp = temp->parent;
    }

    //rotate if necessary
    treeNode* parent = to_del->parent;
    while(parent != NULL){
        if(balance_factor(parent) > 1 && balance_factor(parent->leftChild) >=0){
            parent = RotateRight(parent);
            break;
        }
        if(balance_factor(parent) < -1 && balance_factor(parent->rightChild) <=0){
            parent = RotateLeft(parent);
            break;
        }
        if(balance_factor(parent) > 1 && balance_factor(parent->leftChild) < 0){
            parent->leftChild = RotateLeft(parent->leftChild);
            parent = RotateRight(parent);
            break;
        }
        if(balance_factor(parent) < -1 && balance_factor(parent->rightChild) > 0){
            parent->rightChild = RotateRight(parent->rightChild);
            parent = RotateLeft(parent);
            break;
        }
        parent = parent->parent;
    }
    //free(to_del);
    return root;
}

treeNode* insert( int value, treeNode* root){
    //make a new node of this
    treeNode* node = newTreeNode(value);
    
    //now insert this node in the traditional way
    if(root == NULL){  //empty tree right now
        root = node;
    }
    else if(root->value > value){
        root->leftChild = insert(value, root->leftChild);   
        root->leftChild->parent = root;
    }
    else {
        root->rightChild = insert(value, root->rightChild);
        root->rightChild->parent = root;
    }

    updateHeight(root);

    if(balance_factor(root) > 1 && balance_factor(root->leftChild) >=0){
        return RotateRight(root);
    }
    else if(balance_factor(root) < -1 && balance_factor(root->rightChild) <=0){
        return RotateLeft(root);
    }
    else if(balance_factor(root) > 1 && balance_factor(root->leftChild) < 0){
         root->leftChild = RotateLeft(root->leftChild);
         return RotateRight(root);
    }
    else if(balance_factor(root) < -1 && balance_factor(root->rightChild) > 0){
         root->rightChild = RotateRight(root->rightChild);
         return RotateLeft(root);
    }   

    return root;
         
}


void inOrder( treeNode* root ){
    if(root != NULL){
        inOrder(root->leftChild);
        printf("%d ", root->value);
        inOrder(root->rightChild);
    }
}

void preOrder( treeNode* root ){
    if(root != NULL){
        printf("%d ", root->value);
        preOrder(root->leftChild);        
        preOrder(root->rightChild);
    }
}

bool isBalanced(treeNode* node) {
    if (node == NULL) {
        return true;
    }

    int balance = balance_factor(node);
    if (balance < -1 || balance > 1) {
        return false;
    }

    return isBalanced(node->leftChild) && isBalanced(node->rightChild);
}

// Function to find the minimum and maximum horizontal distance
void findMinMaxHD(struct treeNode *root, int *minHD, int *maxHD, int hd) {
    if (root == NULL)
        return;
    
    if (hd < *minHD)
        *minHD = hd;
    if (hd > *maxHD)
        *maxHD = hd;
    
    findMinMaxHD(root->leftChild, minHD, maxHD, hd - 1);
    findMinMaxHD(root->rightChild, minHD, maxHD, hd + 1);
}

// Function to print the AVL tree vertically
void printVertical(struct treeNode* root, int h, int hd) {
    if (root == NULL)
        return;
    
    if (h == hd) {
        printf("%d ", root->value);
    }
    else {
        printf("  ");
    }
    
    printVertical(root->leftChild, h, hd - 1);
    printVertical(root->rightChild, h, hd + 1);
}

// Function to print the AVL tree vertically (wrapper function)
void printVerticalTree(struct treeNode *root) {
    if (root == NULL)
        return;
    
    int minHD = 0, maxHD = 0;
    
    // Find the minimum and maximum horizontal distance (hd)
    findMinMaxHD(root, &minHD, &maxHD, 0);
    
    // Print the tree for each horizontal distance
    for (int hd = minHD; hd <= maxHD; hd++) {
        printVertical(root, hd, 0);
        printf("\n");
    }
}

int main() {
    // Create an empty AVL tree
    avlTree* avl = (avlTree*)malloc(sizeof(avlTree));

    // Insert 50 random elements into the AVL tree
    for (int i = 1; i <= 50; i++) {
        avl->root = insert(i, avl->root);
        //printf("For this insertion: %d\n", i);
        //inOrder(avl->root);
    }
    //printVerticalTree(avl->root);
    printf("After insertion: ");
    preOrder(avl->root);
    printf("\n");
    inOrder(avl->root);
    printf("\n");

    // Check if the tree is balanced
    if (isBalanced(avl->root)) {
        printf("Tree is balanced after insertions\n");
    } else {
        printf("Tree is not balanced after insertions\n");
    }

    // Check if the 'contains' method works for elements in the tree
    for (int i = 1; i <= 50; i++) {
        if (contains(i, avl->root)) {
            printf("Tree contains %d\n", i);
        } else {
            printf("Tree does not contain %d\n", i);
        }
    }

    // Check if the 'contains' method correctly returns false for elements not in the tree
    for (int i = 51; i <= 100; i++) {
        if (!contains(i, avl->root)) {
            printf("Tree does not contain %d\n", i);
        } else {
            printf("Tree incorrectly contains %d\n", i);
        }
    }

    // Delete a few elements and ensure the tree remains balanced
    int elementsToDelete[] = {7, 15, 20, 30};
    for (int i = 0; i < sizeof(elementsToDelete) / sizeof(elementsToDelete[0]); i++) {
        avl->root = delete(elementsToDelete[i], avl->root);
        if (isBalanced(avl->root)) {
            printf("Tree is balanced after deleting %d\n", elementsToDelete[i]);
        } else {
            printf("Tree is not balanced after deleting %d\n", elementsToDelete[i]);
        }
    }

    // Check that the deleted elements are no longer in the tree
    for (int i = 0; i < sizeof(elementsToDelete) / sizeof(elementsToDelete[0]); i++) {
        if (!contains( elementsToDelete[i], avl->root)) {
            printf("Tree does not contain %d after deletion\n", elementsToDelete[i]);
        } else {
            printf("Tree still contains %d after deletion\n", elementsToDelete[i]);
        }
    }

    // Check that the remaining elements are still in the tree
    for (int i = 1; i <= 50; i++) {
        if (i != 7 && i != 15 && i != 20 && i != 30) {
            if (contains(i, avl->root)) {
                printf("Tree contains %d\n", i);
            } else {
                printf("Tree does not contain %d\n", i);
            }
        }
    }
    printf("After deletion: ");
    preOrder(avl->root);
    printf("\n");
    inOrder(avl->root);
    printf("\n");

    free(avl);
    return 0;
}





