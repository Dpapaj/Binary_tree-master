/*!
 * \file       tree.h
 * \author     Ondřej Ševčík
 * \date       6/2019
 * \brief      Definition of function for binary tree
 * **********************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

/*! \defgroup tree Binary tree
 *  \brief The binary tree allows the storage of the data in the tree-like structure following one
 * simple rule. A tree node with a lower value goes to the left, otherwise, it goes right. With this
 * API this rule is implemented and it is valid even if some node is deleted from the structure.
 *
 *  \{
 */
#ifndef _TREE_H_
#define _TREE_H_

/* Includes --------------------------------------------------------------------------------------*/
#include "data.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/* Exported types --------------------------------------------------------------------------------*/
/*! The TreeNode structure describes one entry in the binary tree. */
typedef struct _TreeNode {
  /*! Node data. */
  Data_t data;

  /*! Pointer at left node. */
  struct _TreeNode *left;

  /*! Pointer at right node. */
  struct _TreeNode *right;
} TreeNode;

/*! The structure that acts as a container for the binary tree. */
typedef struct _Tree {
  /*! Pointer at first (root) node in tree. */
  TreeNode *root;

  /*! Number of nodes currently in tree. */
  unsigned int nodeCount;
} Tree;

/*! Enumeration of supported processing modes. */
typedef enum _TreeProcessMode {
  /*! Data from a node are processed before both of his nodes (L&R). First process node itself, then
     left node and then right node. */
  PRE_ORDER,

  /*! First process the left node, then data and then right node. First node processed is left, then
   * node itself and then right node */
  IN_ORDER,

  /*! Data are processed after processing of both nodes (L&R). First processed is left node, then
   * right node and then node itself. */
  POST_ORDER
} TreeProcessMode;

/*!
 * \brief Callback function for node processing
 * \param node Pointer at current node
 */
typedef void (*TreeNodeProc)(TreeNode *node);

/* Exported macros -------------------------------------------------------------------------------*/
/* Exported variables ----------------------------------------------------------------------------*/
/* Exported functions declarations ---------------------------------------------------------------*/
/*! Initialize the tree, sets \ref Tree.nodeCount to 0 and \ref Tree.root to NULL.
 *
 * \param[in] tree Pointer at tree to initialize.
 *
 * \return Returns true if tree was initialized.
 */
bool Tree_Init(Tree *const tree);

/*! Deletes every node from a tree. It traverses the tree, frees the allocated memory, and sets the
 * \ref Tree.nodeCount to 0.
 *
 * \param[in] tree Pointer at tree to clear.
 */
void Tree_Clear(Tree *const tree);

/*! Creates a new node in the heap with data and puts it in the tree at the correct place. Smaller
 * value goes to the left node, bigger value to the right node. If data are already in the tree, the
 * insertion is aborted.
 *
 * Example: After inserting these values: Petr, Jana and
 * Tom in given order, the resulting tree will look like this:
 *                  Petr
 *                 /    \
 *             Jana     Tom
 *
 * Use function \ref Data_Cmp to compare the data.
 *
 * \param[in] tree Pointer to the tree, where to store data.
 * \param[in] data Data to store.
 *
 * \return Returns true if data were inserted and increases the \ref Tree.nodeCount by 1. Returns
 * false if node insertion was unsuccessful or the tree already has a node with the same value.
 */
bool Tree_Insert(Tree *const tree, const Data_t data);

/*! Deletes a node from a tree that contains \a data. Deleting is done according to rules. This
 * means that if the node has two children, it finds a node with the highest value (the most right)
 * in the left subtree and places it instead of the deleted node. If the deleted node has one child,
 * simply redirect the pointers to the child, and if the node has no children, just delete the node
 * and update parent pointer.
 *
 * \param[in] tree Pointer to the tree, where to delete the node.
 * \param[in] data Data that we want to remove from a tree.
 */
void Tree_Delete(Tree *const tree, const Data_t data);

/*! Returns data from a \a node.
 *
 * \param[in] node Pointer to the a node that contains data.
 *
 * \return Returns pointer to the data contained by the \a node.
 */
const Data_t *Tree_Get_Data(const TreeNode *const node);

/*! Search the node in the \a tree the contains the \a data.
 *
 * \param[in] tree Instance of the tree where we want to look for a \a data.
 * \param[in] data The searched data.
 *
 * \return Returns pointer to the node with searched data, otherwise return NULL.
 */
TreeNode *Tree_Find_Node(Tree tree, const Data_t data);

/*! Gets count of the nodes in the tree.
 *
 * \param[in] tree Instance of the tree.
 *
 * \return Returns the number of items in a tree.
 */
size_t Tree_Get_Count(Tree tree);

/*! Process every node in a tree with a function specified by a pointer \a proc. The processing mode
 * is selected by \a mode.
 *
 * \param[in] tree Instance of the tree to process.
 * \param[in] proc Pointer to the callback function for node processing.
 * \param[in] mode Type of tree processing.
 *
 * \sa TreeProcessMode
 */
void Tree_Process(Tree tree, TreeNodeProc proc, TreeProcessMode mode);
#endif  //_TREE_H_

/*! \} */
