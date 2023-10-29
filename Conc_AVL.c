#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<pthread.h>

#define MAXTHREADS 200

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

//global declaration of tree!
avlTree* tree;

int max(int a, int b){
    return a>b?a:b;
}

int height(treeNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

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

int main() {
    int maxLines = 200; // Maximum number of lines you want to read
    char lines[maxLines][32]; // Assuming each line can have up to 31 characters (plus the null-terminator)

    //Initialization
    int commandArray[maxLines];
    int commandValue[maxLines];
    for(int i =0; i<maxLines; i++){
        commandArray[i] = -1; 
        commandValue[i] = -1;
    }

    /*
        -1 -> Empty after that
        1 -> insert command
        2 -> delete command
        3 -> contains command
        4 -> inorder command
    */

    int lineCount = 0;
    while (lineCount < maxLines && fgets(lines[lineCount], sizeof(lines[0]), stdin)) {
        // Remove the newline character at the end of the line
        lines[lineCount][strcspn(lines[lineCount], "\n")] = '\0';
        if(!strcmp(lines[lineCount],"exit"))break;
        lineCount++;
    }

    for (int i = 0; i < lineCount; i++) {
        int value;
        if (sscanf(lines[i], "insert %d", &value) == 1) {
            commandArray[i] = 1;
            commandValue[i] = value;
        } else if(sscanf(lines[i], "delete %d", &value) == 1){
            commandArray[i] = 2;
            commandValue[i] = value;
        } else if(sscanf(lines[i], "contains %d", &value) == 1){
            commandArray[i] = 3;
            commandValue[i] = value;
        } else if(!strcmp("in order", lines[i])){
            commandArray[i] = 4;
        }
        else{
            printf("Invalid Command\n");
        }
    }
    //Input taken  

    //Now make threads for all above commands!
    //We need to make function pointers for each of insert, 
    // pthread_t thread_id[MAXTHREADS]; // Thread identifier
    
    // for(int i =0; i<lineCount; i++){
    //     // Create a new thread and pass the argument
    //     //Chnage the arguments and function pointers after making them
    //     if(commandArray[i] == 1 ){
    //         pthread_create(&thread_id[i], NULL, insert, &commandValue[i]);
    //     }
    //     else if(commandArray[i] == 2 ){
    //         pthread_create(&thread_id[i], NULL, delete, &commandValue[i]);
    //     }
    //     else if(commandArray[i] == 3 ){
    //         pthread_create(&thread_id[i], NULL, contains, &commandValue[i]);
    //     }
    //     else if(commandArray[i] == 4 ){
    //         pthread_create(&thread_id[i], NULL, inOrder, NULL);
    //     }
    
    // }
        //Need to Wait for all the threads to finish
        //Use pthread_join to do this!
        
        //Maintain an output array to print output to console at the very end.

        //
        
    
    return 0;
}

