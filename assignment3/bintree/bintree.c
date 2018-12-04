/*
Assignment 3
Aim : To create BST from a given array of positive integers and print the inorder,preorder
and postorder traversal according to the given output format.
Date: 9/09/2018
Author: Rohithram R | EE16B031
*/

#include <stdio.h>
#include<stdlib.h>

typedef int dtype;

// Structure for each node in the Binary Search Tree
struct node
{
    dtype data;
    struct node* left;
    struct node* right;
};

// Function to add a node with some value so that insertion can be done
struct node* createNode(dtype value)
{
    struct node* newNode = malloc(sizeof(struct node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}
  
// Function to insert a new node to the tree at appropriate location
// Using a Pointer to Pointer for root node
void insert(struct node** root, dtype data)
{   

    // create new node , once leaf node is reached or when tree is empty
    if (*root == NULL){
        *root = createNode(data); 
    }
            
    // Call the function again with the current node acting as the new root
    if (data < (*(root))->data)
        insert(&((*(root))->left), data);   
    else if (data > (*(root))->data)
        insert(&((*(root))->right), data);
 }

// Function to implement in-order traversal. The output is in sorted order.
void inorder(struct node* root)
{
    if(root == NULL) 
        return;
    inorder(root->left);
    printf("%d\n", root->data);
    inorder(root->right);
}

// Function to implement pre-order traversal
void preorder(struct node* root)
{
    if(root == NULL) 
        return;
    printf("%d\n", root->data);
    preorder(root->left);
    preorder(root->right);
}

// Function to implement post-order traversal
void postorder(struct node* root)
{
    if(root == NULL) 
        return;
    postorder(root->left);
    postorder(root->right);
    printf("%d\n", root->data);
}

int main()
{
    dtype *arr;
    int j,no_of_elements;
    struct node *root = NULL;                           // Root node
    scanf("%d",&no_of_elements);                        // Read number of elements
    
    // Dynamic Memory Allocation for the input array
    arr = (dtype*)malloc(no_of_elements * sizeof(dtype));    

    for(j=0;j<no_of_elements;j++)
    {
        scanf("%d",&arr[j]);

        /*insert values in the array and update the root,
        pass by reference method, passing address of the root pointer
        so that to avoid updating root node manually after each insert
        */
        insert(&root, arr[j]);   
    }

    // Function calls to print inorder, pre-order, post-order traversals
    inorder(root);
    preorder(root);
    postorder(root);
}