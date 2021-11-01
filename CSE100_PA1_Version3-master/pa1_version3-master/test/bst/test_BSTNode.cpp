/**
 * NAMES: Yilan Jiang & Hongsheng Xie
 * EMAILS: yij007@ucsd.edu & hox012@ucsd.edu
 * This file test any methods and constructors
 * written in BSTNode.hpp.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "BSTNode.hpp"

using namespace std;
using namespace testing;

/* Test the constructor of BSTNode and
 * a case in successor method
 */
TEST(BST_NODE_TESTS, TEST_SUCCESSOR) {
    BSTNode<int> node(3);
    ASSERT_EQ(node.successor(), nullptr);
}
