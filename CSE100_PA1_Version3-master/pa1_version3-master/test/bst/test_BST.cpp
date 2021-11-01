/**
 * NAMES: Yilan Jiang & Hongsheng Xie
 * EMAILS: yij007@ucsd.edu & hox012@ucsd.edu
 * This file contains a few test fixtures to test
 * all implementations written in BST.hpp file.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "BST.hpp"
#include "util.hpp"

using namespace std;
using namespace testing;

/* Empty BST test starts here */

TEST(BSTTests, EMPTY_TREE_HEIGHT_TEST) {
    BST<int> bst;
    ASSERT_EQ(bst.height(), -1);
}

/* Small BST test starts here */

/**
 * A simple test fixture from which multiple tests
 * can be created. All fixture tests (denoted by the
 * TEST_F macro) can access the protected members of this
 * fixture. Its data is reset after every test.
 *
 * Builds the following BST:
 *         3
 *        / \
 *       1   4
 *      /     \
 *    -33     100
 */
class SmallBSTFixture : public ::testing::Test {
  protected:
    BST<int> bst;
    BST<int> bstEmpty;

  public:
    SmallBSTFixture() {
        // initialization code here
        vector<int> input{3, 4, 1, 100, -33};
        insertIntoBST(input, bst);
    }
    // code in SetUp() will execute just before the test ensues
    // void SetUp() {}
};

/**
 * Test the find() method
 * */
TEST_F(SmallBSTFixture, DeleteNode_Test) {
    ASSERT_TRUE(bst.deleteNode(3));
    BSTIterator<int> node1 = bst.find(1);
    BSTIterator<int> node4 = bst.find(4);
    ASSERT_TRUE(++node1 == node4);
}

// TEST print() method by printing an empty BST
TEST_F(SmallBSTFixture, PRINT_EMPTY) {
    ostream* out;
    out = &cout;
    bstEmpty.print(out);
}

// Print out the bst by print()
TEST_F(SmallBSTFixture, PRINT_BST) {
    ostream* out;
    out = &cout;
    bst.print(out);
}

// Test the size() method
TEST_F(SmallBSTFixture, SMALL_SIZE_TEST) {
    // assert that the small BST has the correct size
    ASSERT_EQ(bst.size(), 5);
}

// Test the height() method
TEST_F(SmallBSTFixture, SMALL_HEIGHT_TEST) {
    // assert that the small BST has the correct size
    ASSERT_EQ(bst.height(), 2);
}

// Test the insert method in the case of duplication
TEST_F(SmallBSTFixture, SMALL_INSERT_DUPLICATES_TEST) {
    // assert failed duplicate insertion
    ASSERT_FALSE(bst.insert(3));
}

// Test the insert method in the case of height
TEST_F(SmallBSTFixture, SMALL_INSERT_SUCCESS) {
    ASSERT_EQ(bst.height(), 2);
    ASSERT_TRUE(bst.insert(7));
    ASSERT_EQ(bst.height(), 3);
}

// Test find() method
TEST_F(SmallBSTFixture, SMALL_FIND_TEST) {
    BSTIterator<int> Itr3 = bst.find(3);
    ASSERT_EQ(*Itr3, 3);
    BSTIterator<int> Itr100 = bst.find(100);
    ASSERT_EQ(*Itr100, 100);
}

// Test find() method
TEST_F(SmallBSTFixture, SMALL_SUCCESSOR_TEST) {
    BSTIterator<int> Itr3 = bst.find(3);
    BSTIterator<int> subs = ++Itr3;
    BSTIterator<int> Itr4 = bst.find(4);
    ASSERT_EQ(subs == Itr4, true);
}

// Test empty() method
TEST_F(SmallBSTFixture, SIMPLE_EMPTY_TEST) { ASSERT_TRUE(bstEmpty.empty()); }

// Test inorder() method in the case of a empty BST
TEST_F(SmallBSTFixture, INORDER_EMPTY_TEST) {
    vector<int> emptyVec = bstEmpty.inorder();
    ASSERT_TRUE(emptyVec.empty());
}

// Test insert method in the case of inserting a root
TEST_F(SmallBSTFixture, INSERT_ROOT_TEST) { ASSERT_TRUE(bstEmpty.insert(100)); }

/**
 * Build another BST in a new test fixture.
 * */
class BigBSTFixture : public ::testing::Test {
  protected:
    BST<int> bst;

  public:
    BigBSTFixture() {
        // initialization code here
        vector<int> input{8, 7, 2, 1, 20, -1, 100};
        insertIntoBST(input, bst);
    }
    // code in SetUp() will execute just before the test ensues
    // void SetUp() {}
};

// Test the copy constucture to build a balanced BST
TEST_F(BigBSTFixture, Big_balanced_TEST) {
    BST<int> bst_balanced(bst);
    ASSERT_EQ(bst_balanced.height(), 2);
    BSTIterator<int> root_test = bst_balanced.find(7);
    ostream* out;
    out = &cout;
    bst_balanced.print(out);
}

// Test the inorder() method
TEST_F(BigBSTFixture, SIMPLE_INORDER_TEST) {
    vector<int> returnvec = bst.inorder();
    ASSERT_EQ(returnvec.at(0), -1);
    ASSERT_EQ(returnvec.at(6), 100);
}

// Test the begin() method
TEST_F(BigBSTFixture, BEGIN_TEST) {
    BSTIterator<int> beginItr = bst.begin();
    BSTIterator<int> testItr = bst.find(-1);
    ASSERT_TRUE(beginItr == testItr);
}

// Test the end() method
TEST_F(BigBSTFixture, END_TEST) {
    BSTIterator<int> endItr = bst.end();
    BSTIterator<int> testItr(nullptr);
    ASSERT_TRUE(endItr == testItr);
}

// Print out the BST by print()
TEST_F(BigBSTFixture, PRINT_BST) {
    ostream* out;
    out = &cout;
    bst.print(out);
}

// Test find() and equal operator of a BSTIterator
TEST_F(BigBSTFixture, Iterator_Traverse) {
    BSTIterator<int> beginItr = bst.begin();
    BSTIterator<int> testbeginItr = bst.find(1);
    BSTIterator<int> secondItr = (++beginItr);
    ASSERT_TRUE(secondItr == testbeginItr);
    BSTIterator<int> thirdItr = (++secondItr);
    BSTIterator<int> testthirdItr = bst.find(2);
    ASSERT_TRUE(thirdItr == testthirdItr);
    BSTIterator<int> lastItr = (++thirdItr);
    ++lastItr;
    ++lastItr;
    ++lastItr;
    BSTIterator<int> testlastItr = bst.find(100);
    ASSERT_TRUE(lastItr == testlastItr);
    ++lastItr;
    ASSERT_TRUE(lastItr == nullptr);
}

// Test insert() by inserting a duplicate data
TEST_F(BigBSTFixture, INSERT_DUPLICATE) { ASSERT_FALSE(bst.insert(-1)); }

/**
 * Create another test fixture and build a small BST
 * */
class EdgeBSTFixture : public ::testing::Test {
  protected:
    BST<int> bst;

  public:
    EdgeBSTFixture() {
        // initialization code here
        vector<int> input{3, 1, 2};
        insertIntoBST(input, bst);
    }
    // code in SetUp() will execute just before the test ensues
    // void SetUp() {}
};

// Test successor() of a BSTNode
TEST_F(EdgeBSTFixture, SUCCESSOR_EDGE_TEST) {
    BSTIterator<int> twoItr = bst.find(2);
    BSTIterator<int> threeItr = bst.find(3);
    ASSERT_TRUE(++twoItr == threeItr);
}

/**
 * Create a new test fixture and build a skewed BST
 * */
class SkewBSTFixture : public ::testing::Test {
  protected:
    BST<int> bst;

  public:
    SkewBSTFixture() {
        // initialization code here
        vector<int> input{40, 3, 2, -10};
        insertIntoBST(input, bst);
    }
    // code in SetUp() will execute just before the test ensues
    // void SetUp() {}
};

// Test inorder(), size(), and empty() methods
TEST_F(SkewBSTFixture, skewed_TEST) {
    ASSERT_EQ(bst.size(), 4);
    ASSERT_EQ(bst.height(), 3);
    ASSERT_FALSE(bst.empty());
    vector<int> inorderVec = bst.inorder();
    ASSERT_EQ(inorderVec.at(0), -10);
    ASSERT_EQ(inorderVec.at(3), 40);
    BSTIterator<int> findItr1 = bst.find(3);
    ASSERT_EQ(*findItr1, 3);
}

// Test copy constructor of a BST to build
// a balanced BST
TEST_F(SkewBSTFixture, balanced_TEST) {
    BST<int> bst_balanced(bst);
    ASSERT_EQ(bst_balanced.height(), 2);
    BSTIterator<int> root_test = bst_balanced.find(3);
    ostream* out;
    out = &cout;
    bst_balanced.print(out);
}

// Test deleteNode() method
TEST_F(SkewBSTFixture, deleteNode_TEST) {
    ASSERT_TRUE(bst.deleteNode(3));
    BSTIterator<int> Node2 = bst.find(2);
    BSTIterator<int> testNode = bst.find(40);
    ASSERT_TRUE(++Node2 == testNode);
}

/**
 * Create a new test fixture and build a skewed to right BST
 * */
class SkewRightBSTFixture : public ::testing::Test {
  protected:
    BST<int> bst;

  public:
    SkewRightBSTFixture() {
        // initialization code here
        vector<int> input{0, 9, 1, 8};
        insertIntoBST(input, bst);
    }
    // code in SetUp() will execute just before the test ensues
    // void SetUp() {}
};

// Test the deleteNode() method
TEST_F(SkewRightBSTFixture, deletNode_TEST) {
    ASSERT_TRUE(bst.deleteNode(1));
    ostream* out;
    out = &cout;
    bst.print(out);
}

/**
 * Create a new test fixture
 * */
class testBSTFixture : public ::testing::Test {
  protected:
    BST<int> bst;

  public:
    testBSTFixture() {
        // initialization code here
        vector<int> input{10, 15, 12, 7, 3, 9, 13};
        insertIntoBST(input, bst);
    }
    // code in SetUp() will execute just before the test ensues
    // void SetUp() {}
};

// Test deleteNode() method
TEST_F(testBSTFixture, deleteNode_TEST) {
    ASSERT_TRUE(bst.deleteNode(13));
    ASSERT_TRUE(bst.deleteNode(3));
    ostream* out;
    out = &cout;
    bst.print(out);
}

// Test deleteNode() method
TEST_F(testBSTFixture, deleteNode1_TEST) {
    ASSERT_TRUE(bst.deleteNode(7));
    ostream* out;
    out = &cout;
    bst.print(out);
}

// Test deleteNode() method
TEST_F(testBSTFixture, deleteNode2_TEST) { ASSERT_FALSE(bst.deleteNode(8)); }

/**
 * Create another test fixture to test deleteNode() method
 * */
class TwoChildrenBSTFixture : public ::testing::Test {
  protected:
    BST<int> bst;

  public:
    TwoChildrenBSTFixture() {
        // initialization code here
        vector<int> input{0, 2, 1, 7, 3, 4, 5};
        insertIntoBST(input, bst);
    }
    // code in SetUp() will execute just before the test ensues
    // void SetUp() {}
};

// Test deleteNode() method in the case of two children
TEST_F(TwoChildrenBSTFixture, deleteNode_TEST) {
    ASSERT_TRUE(bst.deleteNode(2));
    ostream* out;
    out = &cout;
    bst.print(out);
}
// TODO: add more BST tests here