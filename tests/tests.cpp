/*!
 * \file       tests.cpp
 * \author     Jurena
 * \date       9. 2. 2022
 * \brief      Unit tests for vector module.
 * ******************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

/* Includes --------------------------------------------------------------------------------------*/

#include "gmock/gmock.h"

#include "gtest/gtest.h"
#include <inttypes.h>
#include <limits>
#include <string.h>
#include <vector>

extern "C" {
#include "tree.h"
}

/* Private variables -----------------------------------------------------------------------------*/
static std::vector<std::string> mNames;

/* Private types ---------------------------------------------------------------------------------*/
class TreeTest : public ::testing::Test
{
protected:
  Tree mTree;

  void SetUp()
  {
    Tree_Init(&mTree);
    mNames.clear();
  }
  void Teardown() { Tree_Clear(&mTree); }
};

class TreeTestParam :
  public testing::TestWithParam<std::tuple<std::vector<Data_t>, std::vector<std::string>>>
{
protected:
  Tree mTree;

  void SetUp()
  {
    mNames.clear();
    Tree_Init(&mTree);
    Data_t data[] = {{"Franz", 167, 0, 0},
                     {"Bianca", 158, 0, 0},
                     {"Sally", 174, 0, 0},
                     {"Aaron", 179, 0, 0},
                     {"Daisy", 164, 0, 0},
                     {"Luke", 184, 0, 0},
                     {"Caroline", 169, 0, 0},
                     {"Ethan", 198, 0, 0}};

    for (Data_t *it = data, *end = data + sizeof(data) / sizeof(*data); it < end; ++it) {
      Tree_Insert(&mTree, *it);
    }
  }

  void Teardown() { Tree_Clear(&mTree); }
};

/* Private macros --------------------------------------------------------------------------------*/

/* Private function declarations -----------------------------------------------------------------*/
bool operator==(const Data_t &lhs, const Data_t &rhs);
bool operator!=(const Data_t &lhs, const Data_t &rhs);

/* Exported functions definitions ----------------------------------------------------------------*/
/* Private function definitions ------------------------------------------------------------------*/
bool operator==(const Data_t &lhs, const Data_t &rhs)
{
  return Data_Cmp(&lhs, &rhs) == 0;
}

bool operator!=(const Data_t &lhs, const Data_t &rhs)
{
  return !(lhs == rhs);
}

void ProcessNode(TreeNode *node)
{
  mNames.push_back(node->data.name);
}

TEST(Tree, init)
{
  TreeNode node;
  Tree tree = {&node, 10};
  ASSERT_TRUE(Tree_Init(&tree));
  ASSERT_EQ(tree.root, nullptr);
  ASSERT_EQ(tree.nodeCount, 0);
}

TEST(Tree, initNullTree)
{
  ASSERT_FALSE(Tree_Init(nullptr));
}

TEST_F(TreeTest, insertOneValue)
{
  Data_t testingData = {"John", 0, 0, 0};

  ASSERT_TRUE(Tree_Insert(&mTree, testingData));
  ASSERT_EQ(mTree.nodeCount, 1);
  ASSERT_NE(mTree.root, nullptr);
  ASSERT_EQ(mTree.root->data, testingData);
}

TEST_F(TreeTest, insertTwoValues)
{
  {
    Data_t testingData = {"John", 0, 0, 0};
    Tree_Insert(&mTree, testingData);
  }

  Data_t testingData = {"Anna", 0, 0, 0};
  ASSERT_TRUE(Tree_Insert(&mTree, testingData));
  ASSERT_EQ(mTree.nodeCount, 2);
  ASSERT_NE(mTree.root, nullptr);
  ASSERT_EQ(mTree.root->left->data, testingData);
}

TEST_F(TreeTest, insertThreeValues)
{
  {
    Data_t testingData = {"John", 0, 0, 0};
    Tree_Insert(&mTree, testingData);
  }
  {
    Data_t testingData = {"Anna", 0, 0, 0};
    Tree_Insert(&mTree, testingData);
  }

  Data_t testingData = {"Peter", 0, 0, 0};
  ASSERT_TRUE(Tree_Insert(&mTree, testingData));
  ASSERT_EQ(mTree.nodeCount, 3);
  ASSERT_NE(mTree.root, nullptr);
  ASSERT_EQ(mTree.root->right->data, testingData);
}

TEST_F(TreeTest, insertThreeValuesAllToLeft)
{
  {
    Data_t testingData = {"John", 0, 0, 0};
    Tree_Insert(&mTree, testingData);
  }
  {
    Data_t testingData = {"Dana", 0, 0, 0};
    Tree_Insert(&mTree, testingData);
  }

  Data_t testingData = {"Anna", 0, 0, 0};
  ASSERT_TRUE(Tree_Insert(&mTree, testingData));
  ASSERT_EQ(mTree.nodeCount, 3);
  ASSERT_NE(mTree.root, nullptr);
  ASSERT_EQ(mTree.root->left->left->data, testingData);
}

TEST_F(TreeTest, insertDuplicateData)
{
  Data_t testingData = {"John", 0, 0, 0};

  Tree_Insert(&mTree, testingData);
  ASSERT_FALSE(Tree_Insert(&mTree, testingData));
  ASSERT_EQ(mTree.nodeCount, 1);
}

TEST(Tree, insertNullTree)
{
  Data_t testData = {"Franz", 0, 0, 0};
  ASSERT_FALSE(Tree_Insert(nullptr, testData));
}

TEST_F(TreeTest, clear)
{
  Data_t data[] = {{"Petr", 10, 0, 0},
                   {"Tomas", 20, 0, 0},
                   {"Adam", 30, 0, 0},
                   {"Jiri", 40, 0, 0},
                   {"Roman", 50, 0, 0},
                   {"Michal", 60, 0, 0}};

  for (Data_t *it = data, *end = data + sizeof(data) / sizeof(*data); it < end; ++it) {
    Tree_Insert(&mTree, *it);
  }

  Tree_Clear(&mTree);

  ASSERT_EQ(mTree.nodeCount, 0);
  ASSERT_EQ(mTree.root, nullptr);
}

TEST(Tree, clearNullTree)
{
  Tree_Clear(nullptr);
}

TEST(Tree, deleteNullTree)
{
  Data_t testingData = {"John", 0, 0, 0};
  Tree_Delete(nullptr, testingData);
}

TEST_F(TreeTest, deleteExistingNode)
{
  Data_t testingData = {"Luke", 0, 0, 0};
  Tree_Insert(&mTree, testingData);

  Tree_Delete(&mTree, testingData);
  ASSERT_EQ(mTree.nodeCount, 0);
  ASSERT_EQ(mTree.root, nullptr);
}

TEST_F(TreeTest, deleteNonExistingNode)
{
  {
    Data_t testingData = {"Luke", 0, 0, 0};
    Tree_Insert(&mTree, testingData);
  }

  Data_t testingData = {"Peter", 0, 0, 0};
  Tree_Delete(&mTree, testingData);
  ASSERT_EQ(mTree.nodeCount, 1);
  ASSERT_NE(mTree.root->data, testingData);
}

TEST_F(TreeTest, deleteEmptyTree)
{
  Data_t testingData = {"Peter", 0, 0, 0};
  Tree_Delete(&mTree, testingData);
  ASSERT_EQ(mTree.nodeCount, 0);
}

TEST_F(TreeTest, process)
{
  Data_t data[] = {{"Petr", 10, 0, 0},
                   {"Tomas", 20, 0, 0},
                   {"Adam", 30, 0, 0},
                   {"Jiri", 40, 0, 0},
                   {"Roman", 50, 0, 0},
                   {"Michal", 60, 0, 0}};

  for (Data_t *it = data, *end = data + sizeof(data) / sizeof(*data); it < end; ++it) {
    Tree_Insert(&mTree, *it);
  }

  Tree_Process(mTree, ProcessNode, IN_ORDER);
  ASSERT_THAT(mNames,
              ::testing::ElementsAreArray({"Adam", "Jiri", "Michal", "Petr", "Roman", "Tomas"}));

  mNames.clear();
  Tree_Process(mTree, ProcessNode, POST_ORDER);
  ASSERT_THAT(mNames,
              ::testing::ElementsAreArray({"Michal", "Jiri", "Adam", "Roman", "Tomas", "Petr"}));

  mNames.clear();
  Tree_Process(mTree, ProcessNode, PRE_ORDER);
  ASSERT_THAT(mNames,
              ::testing::ElementsAreArray({"Petr", "Adam", "Jiri", "Michal", "Tomas", "Roman"}));
}

TEST_P(TreeTestParam, treeDelete)
{
  auto [toDeleteVect, expectedValues] = GetParam();
  for (const auto &toDelete : toDeleteVect) {
    Tree_Delete(&mTree, toDelete);
  }

  ASSERT_EQ(mTree.nodeCount, expectedValues.size());
  Tree_Process(mTree, ProcessNode, IN_ORDER);
  ASSERT_THAT(mNames, ::testing::ElementsAreArray(expectedValues));
}

INSTANTIATE_TEST_SUITE_P(
  treeDelete,
  TreeTestParam,
  testing::Values(
    std::make_tuple(
      std::vector<Data_t>{Data_t{"Franz", 167, 0, 0}},
      std::vector<std::string>({"Aaron", "Bianca", "Caroline", "Daisy", "Ethan", "Luke", "Sally"})),
    std::make_tuple(
      std::vector<Data_t>{Data_t{"Franz", 167, 0, 0}, Data_t{"Ethan", 167, 0, 0}},
      std::vector<std::string>({"Aaron", "Bianca", "Caroline", "Daisy", "Luke", "Sally"})),
    std::make_tuple(
      std::vector<Data_t>{Data_t{"Sally", 174, 0, 0}},
      std::vector<std::string>({"Aaron", "Bianca", "Caroline", "Daisy", "Ethan", "Franz", "Luke"})),
    std::make_tuple(
      std::vector<Data_t>{Data_t{"Aaron", 179, 0, 0}},
      std::vector<std::string>({"Bianca", "Caroline", "Daisy", "Ethan", "Franz", "Luke", "Sally"})),
    std::make_tuple(
      std::vector<Data_t>{Data_t{"Bianca", 158, 0, 0}},
      std::vector<std::string>({"Aaron", "Caroline", "Daisy", "Ethan", "Franz", "Luke", "Sally"})),
    std::make_tuple(
      std::vector<Data_t>{Data_t{"Betty", 179, 0, 0}},
      std::vector<std::string>(
        {"Aaron", "Bianca", "Caroline", "Daisy", "Ethan", "Franz", "Luke", "Sally"}))));

TEST_F(TreeTest, deleteAll)
{
  Data_t testingData = {"Franz", 167, 0, 0};
  Tree_Insert(&mTree, testingData);
  Tree_Delete(&mTree, testingData);
  ASSERT_EQ(mTree.nodeCount, 0);
  ASSERT_EQ(mTree.root, nullptr);
}

TEST_F(TreeTestParam, findNode)
{
  Data_t testingData = {"Franz", 167, 0, 0};
  TreeNode *node = Tree_Find_Node(mTree, testingData);
  ASSERT_NE(node, nullptr);
}

TEST_F(TreeTestParam, findNonExistingNode)
{
  Data_t testingData = {"Kayle", 167, 0, 0};
  TreeNode *node = Tree_Find_Node(mTree, testingData);
  ASSERT_EQ(node, nullptr);
}

TEST_F(TreeTestParam, getData)
{
  Data_t testingData = {"Caroline", 169, 0, 0};
  const Data_t *elm = Tree_Get_Data(Tree_Find_Node(mTree, testingData));

  ASSERT_NE(elm, nullptr);
  ASSERT_EQ(*elm, testingData);
}

TEST(Tree, getDataNullTree)
{
  ASSERT_EQ(Tree_Get_Data(nullptr), nullptr);
}

TEST_F(TreeTestParam, getCount)
{
  ASSERT_EQ(Tree_Get_Count(mTree), 8);
}