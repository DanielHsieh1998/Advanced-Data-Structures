/**
 * NAMES: Yilan Jiang & Hongsheng Xie
 * EMAILS: yij007@ucsd.edu & hox012@ucsd.edu
 * This file contains google tests to test the
 * methods written in BSTIterator.hpp.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "BSTIterator.hpp"
#include "BSTNode.hpp"

using namespace std;
using namespace testing;

/**
 * Test the BSTIterator constructor and
 * the post-increment opeator
 * */
TEST(BST_ITERATOR_TEST, TEST_ITERATOR_INCREMENT) {
    BSTNode<int> node(3);
    BSTIterator<int> iter(&node);

    ASSERT_NE(iter, nullptr);

    iter++;

    ASSERT_EQ(iter, nullptr);
}

/**
 * Test the equality operator overwritten
 * in the BSTIterator file
 * */
TEST(BST_ITERATOR_TEST, TEST_ITERATOR_EQUAL) {
    BSTNode<int> n1(10);
    BSTNode<int> n2(11);
    BSTIterator<int> iter1(&n1);
    BSTIterator<int> iter2(&n2);
    bool bol = (iter1 == iter2);
    ASSERT_EQ(bol, false);
}

/**
 * Test the inequality operator overwritten
 * in the BSTIterator file
 * */
TEST(BST_ITERATOR_TEST, TEST_ITERATOR_NEQUAL) {
    BSTNode<int> n1(10);
    BSTNode<int> n2(11);
    BSTIterator<int> iter1(&n1);
    BSTIterator<int> iter2(&n2);
    bool bol = (iter1 != iter2);
    ASSERT_EQ(bol, true);
}