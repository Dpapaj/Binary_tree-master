/*!
 * \file       main.c
 * \author     Ondřej Ševčík
 * \date       6/2019
 * \brief      Main file for binary tree project.
 * **********************************************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

/* Private includes -------------------------------------------------------- */
#include "data.h"
#include "ioutils.h"
#include "tree.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

/* Private functions ------------------------------------------------------- */
/*! Prints menu to stdout */
void menu(void)
{
  printf("1 - tree_init()\n"
         "2 - tree_clear()\n"
         "3 - tree_insert()\n"
         "4 - tree_delete()\n"
         "5 - tree_find_node()\n"
         "6 - tree_get_node_count()\n"
         "7 - tree_proces()\n"
         "M - MENU\n"
         "E - END\n");
}

/*! Prints informations about node to the stdout.
 *
 * \param[in] node  Pointer at node from binary tree.
 */
void process_tree_node(TreeNode *node)
{
  Data_Print(&node->data);
  printf(" | L -> ");

  if (node->left) {
    Data_Print(&node->left->data);
  } else {
    printf("NULL");
  }

  printf(" | R -> ");

  if (node->right) {
    Data_Print(&node->right->data);
  } else {
    printf("NULL");
  }

  printf("\n");
}

/* Main function ----------------------------------------------------------- */
int main(int argc, char **argv)
{
  (void)argc;  // unsed
  (void)argv;  // unsed
  printf("Tree v1.1\n");
  printf("----------\n\n");
  char userInput;
  Tree tree;
  bool initialized = false;
  menu();
  bool running = true;

  while (running) {
    printf("Your choice: ");
    running = io_utils_get_char(&userInput);

    if (running) {
      printf("Your choice: %c\n", userInput);
    }

    if ((initialized || userInput > '9' || userInput == '1') && running) {
      switch (userInput) {
        case '1':
          if (initialized) {
            printf("Tree was already initialized.\n");
            break;
          }

          initialized = Tree_Init(&tree);

          if (initialized) {
            printf("Tree initialized successfuly.\n");
          } else {
            printf("Tree initialization failed.\n");
          }

          break;

        case '2':
          Tree_Clear(&tree);
          printf("The nodes from a tree were deleted.\n");
          break;

        case '3': {
          printf("Insert data to a node:\n");
          Data_t data;
          running = Data_Get(&data);

          if (!running) {
            continue;
          }

          if (!Tree_Insert(&tree, data)) {
            printf("The data are already in tree!\n");
          }

          break;
        }

        case '4':
          printf("Insert data to delete:\n");
          Data_t data;
          running = Data_Get(&data);

          if (!running) {
            continue;
          }

          Tree_Delete(&tree, data);
          break;

        case '5': {
          printf("Insert data to search:\n");
          Data_t data;
          running = Data_Get(&data);

          if (!running) {
            continue;
          }

          TreeNode *node = Tree_Find_Node(tree, data);

          if (node) {
            printf("Data were found in a tree.\n");
          } else {
            printf("Data were not found in a tree.\n");
          }

          break;
        }

        case '6':
          printf("The number of nodes in a tree: %zu\n", Tree_Get_Count(tree));
          break;

        case '7': {
          long mode = 0;
          printf("Process:\n%d - preorder\n%d - inorder\n%d - postorder\nYour choice",
                 PRE_ORDER,
                 IN_ORDER,
                 POST_ORDER);
          running = io_utils_get_long(&mode);

          if (!running) {
            continue;
          }

          printf("%ld\n\n", mode);

          if (mode >= PRE_ORDER && mode <= POST_ORDER) {
            Tree_Process(tree, (TreeNodeProc)process_tree_node, (TreeProcessMode)mode);
          } else {
            printf("Unknown option!\n");
          }

          break;
        }

        case 'm':
        case 'M':
          menu();
          break;

        case 'e':
        case 'E':
          running = false;
          printf("END.\n");
          break;

        default:
          printf("Unknown option!\n");
          break;
      }
    } else if (!initialized) {
      printf("Tree is NOT initialized!\n");
    }

    printf("\n");
  }

  if (initialized) {
    Tree_Clear(&tree);
  }
  return 0;
}
