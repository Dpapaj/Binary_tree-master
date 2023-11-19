/*!
 * \file       tree.c
 * \author     Ondřej Ševčík
 * \date       6/2019
 * \brief      Source file for binary tree
 * **********************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

/* Includes --------------------------------------------------------------------------------------*/
#include "tree.h"


/* Private types ---------------------------------------------------------------------------------*/
/* Private macros --------------------------------------------------------------------------------*/
#define UNUSED(x) (void)x

/* Private variables -----------------------------------------------------------------------------*/
/* Private function declarations -----------------------------------------------------------------*/
/* Exported functions definitions ----------------------------------------------------------------*/

bool Tree_Init(Tree *const tree)
{
    if(tree == NULL)
    {
        return false;
    }

    tree->root = NULL;
    tree->nodeCount = 0;

    return true;
}

void Clear(TreeNode* root){
    if(root == NULL)
    {
        return;
    }
    Clear(root->left);
    Clear(root->right);
    free(root);
}

void Tree_Clear(Tree *const tree)
{
    if(tree == NULL)
    {
        return;
    }
    Clear(tree->root);
    tree->root = NULL;
    tree->nodeCount = 0;
}

bool Tree_Insert(Tree *const tree, const Data_t data)
{
    if(tree == NULL)
    {
        return false;
    }

    TreeNode *ukazatel = tree->root;
    TreeNode *predUkazatel = NULL;

    int dataCmp;
    while (ukazatel != NULL)
    {
        predUkazatel = ukazatel;
        dataCmp = Data_Cmp( &data, &ukazatel->data);
        if(dataCmp < 0)
        {
            ukazatel = ukazatel->left;
        }
        else if( dataCmp > 0)
        {
            ukazatel = ukazatel->right;
        }
        else return false;
    }

    ukazatel = (TreeNode*) malloc(sizeof (TreeNode));
    if (ukazatel == NULL) {
        return false;
    }

    ukazatel->data = data;
    ukazatel->left = ukazatel->right = NULL;
    if(predUkazatel == NULL)
    {
        tree->root = ukazatel;
    }
    else if( dataCmp < 0)
    {
        predUkazatel->left = ukazatel;
    }
    else
    {
        predUkazatel->right = ukazatel;
    }
    tree->nodeCount++;

    return true;
}

void findMin(TreeNode** found, TreeNode* node)
{
    if(node->right == NULL)
    {
        *found = node;
        return;
    }
    findMin(found, node->right);
}

void setPointer(TreeNode** setPtr, TreeNode* node, const Data_t data)
{
    if(node == NULL || *setPtr != NULL)
        return;

    if(Data_Cmp(&node->data,&data) == 0)
    {
        *setPtr = node;
        return;
    }
    setPointer(setPtr,node->left,data);
    setPointer(setPtr,node->right,data);
}

void findParent(TreeNode** setPtr, TreeNode* parent, TreeNode* child)
{
    if(parent == NULL)
        return;

    if(parent->left == child)
    {
        *setPtr = parent;
        return;
    }
    if(parent->right == child)
    {
        *setPtr = parent;
        return;
    }

    findParent(setPtr,parent->left,child);
    findParent(setPtr,parent->right,child);
}

void Tree_Delete(Tree *const tree, const Data_t data)
{

    if(tree == NULL || tree->nodeCount == 0)
    {
        return;
    }

    TreeNode* parentDeleteNode = tree->root;
    TreeNode* nodeDelete = Tree_Find_Node(*tree, data);

    if(nodeDelete == NULL)
    {
        return;
    }

    findParent(&parentDeleteNode,tree->root, nodeDelete);

    if(nodeDelete->left == NULL && nodeDelete->right == NULL)
    {

        if (parentDeleteNode->right == nodeDelete)
        {
            parentDeleteNode->right = NULL;
        }
        else if (parentDeleteNode->left == nodeDelete)
        {
            parentDeleteNode->left = NULL;
        }
        else if (parentDeleteNode == tree->root)
        {
            tree->root = NULL;
        }
    }
    else
    {
        if(nodeDelete->left->right != NULL)
        {
            TreeNode* parentNode = nodeDelete->left;
            TreeNode* node = NULL;

            findMin(&node, nodeDelete->left);
            if(node == NULL)
                return;

            while(parentNode->right != node)
            {
                parentNode = parentNode->right;
            }

            if (parentDeleteNode->right == nodeDelete)
            {
                parentDeleteNode->right = node;
            }
            else if (parentDeleteNode->left == nodeDelete)
            {
                parentDeleteNode->left = node;
            }
            else if (parentDeleteNode == tree->root)
            {
                tree->root = node;
            }

            parentNode->right = node->left;

            if(nodeDelete->left != node)
            {
                node->left = nodeDelete->left;
            }
            if(nodeDelete->right != node)
            {
                node->right = nodeDelete->right;
            }
        }
        else
        {
            nodeDelete->left->right = nodeDelete->right;
            if (parentDeleteNode->right == nodeDelete)
            {
                parentDeleteNode->right = nodeDelete->left;
            }
            else if (parentDeleteNode->left == nodeDelete)
            {
                parentDeleteNode->left = nodeDelete->left;
            }
            else if (parentDeleteNode == tree->root)
            {
                tree->root = nodeDelete->left;
            }
        }
    }
    free(nodeDelete);
    tree->nodeCount--;

}

const Data_t *Tree_Get_Data(const TreeNode *const node)
{
    if(node == NULL)
    {
        return NULL;
    }else return &node->data;
}

TreeNode *Tree_Find_Node(Tree tree, const Data_t data)
{
    TreeNode *ukazatel = tree.root;

    int dataCmp;
    while(ukazatel != NULL)
    {
        dataCmp = Data_Cmp( &data, &ukazatel->data);
        if(dataCmp < 0)
        {
            ukazatel = ukazatel->left;
        }
        else if( dataCmp > 0)
        {
            ukazatel = ukazatel->right;
        }
        else return ukazatel;
    }
    return NULL;
}

size_t Tree_Get_Count(Tree tree)
{
  return tree.nodeCount;
}

void print_preorder(TreeNode* node, TreeNodeProc proc) {
    if(node != NULL)
    {
        proc(node);
        print_preorder(node->left,proc);
        print_preorder(node->right,proc);
    }
}

void print_inorder(TreeNode* node, TreeNodeProc proc) {
    if(node != NULL)
    {
        print_inorder(node->left, proc);
        proc(node);
        print_inorder(node->right, proc);
    }
}

void print_postorder(TreeNode* node, TreeNodeProc proc) {
    if(node != NULL)
    {
        print_postorder(node->left,proc);
        print_postorder(node->right,proc);
        proc(node);
    }
}

void Tree_Process(Tree tree, TreeNodeProc proc, TreeProcessMode mode)
{
    switch(mode)
    {
        case PRE_ORDER:
            print_preorder(tree.root, proc);
            break;
        case IN_ORDER:
            print_inorder(tree.root, proc);
            break;
        case POST_ORDER:
            print_postorder(tree.root, proc);
            break;
    }
}

/* Private function definitions ------------------------------------------------------------------*/
