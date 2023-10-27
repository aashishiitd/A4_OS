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
    parent->leftChild->height--;
    parent->height = max(height(parent->rightChild),height(parent->leftChild))+1;
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
    parent->rightChild->height--;
    parent->height = max(height(parent->rightChild),height(parent->leftChild))+1;
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


// Function to update the height of a node
void updateHeight( treeNode* node) {
    int leftHeight = height(node->leftChild);
    int rightHeight = height(node->rightChild);
    node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int balance_factor(treeNode* node){
    return height(node->leftChild) - height(node->rightChild);
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
treeNode* delete( int value, treeNode* root){
    
    if(!contains(value, root)) return root; //value wasn't in tree. No deletion necessary!!
    
    //height also update
    treeNode* to_del = give_node(value, root);

    //find the element to be deleted! using recursion.
    //swap until element is a leaf node
    while(to_del->leftChild != NULL || to_del->rightChild != NULL){
        if(to_del->leftChild != NULL){
            swap(to_del, to_del->leftChild);
            to_del = to_del->leftChild;
        }
        else{
            swap(to_del, to_del->rightChild);
            to_del = to_del->rightChild;
        }
    }
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
    if(balance_factor(to_del->parent) > 1 && balance_factor(to_del->parent->leftChild) >=0){
        return RotateRight(to_del->parent);
    }
    if(balance_factor(to_del->parent) < -1 && balance_factor(to_del->parent->rightChild) <=0){
        return RotateLeft(to_del->parent);
    }
    if(balance_factor(to_del->parent) > 1 && balance_factor(to_del->parent->leftChild) < 0){
        to_del->parent->leftChild = RotateLeft(to_del->parent->leftChild);
        return RotateRight(to_del->parent);
    }
    if(balance_factor(to_del->parent) < -1 && balance_factor(to_del->parent->rightChild) > 0){
        to_del->parent->rightChild = RotateRight(to_del->parent->rightChild);
        return RotateLeft(to_del->parent);
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
    if(balance_factor(root) < -1 && balance_factor(root->rightChild) <=0){
        return RotateLeft(root);
    }
    if(balance_factor(root) > 1 && balance_factor(root->leftChild) < 0){
        root->leftChild = RotateLeft(root->leftChild);
        return RotateRight(root);
    }
    if(balance_factor(root) < -1 && balance_factor(root->rightChild) > 0){
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


bool isAVLbalanced(treeNode* node) {
    if (node == NULL) {
        return true;
    }

    int balance = balance_factor(node);
    if (balance < -1 || balance > 1) {
        return false;
    }

    return isAVLbalanced(node->leftChild) && isAVLbalanced(node->rightChild);
}


int main() {
    avlTree* tree = (avlTree*)malloc(sizeof(avlTree));
    bool all_tests_passed = true;

    // Test 1: Insertion and balancing test
    int insert_values[] = {5, 3, 8, 1, 4, 7, 9, 2, 6};
    for (int i = 0; i < sizeof(insert_values) / sizeof(insert_values[0]); i++) {
        tree->root = insert(insert_values[i], tree->root);
        printf("Inserting %d: In-order: ", insert_values[i]);
        inOrder(tree->root);
        printf("\n");

        // Check if the tree is balanced after each insertion
        if (!isAVLbalanced(tree->root)) {
            all_tests_passed = false;
            printf("Test 1: Balancing test failed after inserting %d.\n", insert_values[i]);
        }
    }

    // Test 2: Search test
    int search_values[] = {4, 6};
    for (int i = 0; i < sizeof(search_values) / sizeof(search_values[0]); i++) {
        bool is_present = contains(search_values[i], tree->root);
        if (is_present) {
            printf("%d is present in the tree.\n", search_values[i]);
        } else {
            all_tests_passed = false;
            printf("%d is not present in the tree.\n", search_values[i]);
        }
    }

    // Test 3: Deletion and balancing test
    int delete_values[] = {3, 8};
    for (int i = 0; i < sizeof(delete_values) / sizeof(delete_values[0]); i++) {
        tree->root = delete(delete_values[i], tree->root);
        printf("Deleting %d: In-order: ", delete_values[i]);
        inOrder(tree->root);
        printf("\n");

        // Check if the tree is balanced after each deletion
        if (!isAVLbalanced(tree->root)) {
            all_tests_passed = false;
            printf("Test 3: Balancing test failed after deleting %d.\n", delete_values[i]);
        }
    }

    // Test 4: Complex test - Delete the root
    int root_value = 5;
    tree->root = delete(root_value, tree->root);
    printf("Deleting the root (%d): In-order: ", root_value);
    inOrder(tree->root);
    printf("\n");

    // Check if the tree is balanced after the root deletion
    if (!isAVLbalanced(tree->root)) {
        all_tests_passed = false;
        printf("Test 4: Balancing test failed after deleting the root.\n");
    }

    if (all_tests_passed) {
        printf("All test cases passed.\n");
    } else {
        printf("Some test cases failed.\n");
    }

    return 0;
}






