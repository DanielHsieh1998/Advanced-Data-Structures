/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file contains testers to test HCTree files.
 * */

#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "HCTree.hpp"

using namespace std;
using namespace testing;

//create a simple HCTree to be tested
class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    SimpleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs['a'] = 2;
        freqs['b'] = 3;
        tree.build(freqs);
    }
};
//test encode method
TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('a', os);
    ASSERT_EQ(os.str(), "0");
}
//test decode method
TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("1");
    ASSERT_EQ(tree.decode(is), 'b');
}


//create another HCTree to be tested
class ComplexHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    ComplexHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs['a'] = 2;
        freqs['b'] = 3;
        freqs['A'] = 4;
        freqs['B'] = 4;
        freqs['z'] = 10;
        tree.build(freqs);
    }
};

//test build method
TEST_F(ComplexHCTreeFixture, TEST_BUILD) {
    ASSERT_EQ(tree.getRoot()->symbol, 'z');
    ASSERT_EQ(tree.getRoot()->count, 23);
    ASSERT_EQ(tree.getRoot()->c1->symbol, 'a');
    ASSERT_EQ(tree.getRoot()->c1->count, 13);
}

//test decode method
TEST_F(ComplexHCTreeFixture, TEST_DECODE) {
    istringstream is("111");
    ASSERT_EQ(tree.decode(is), 'A');
}

//test encode method
TEST_F(ComplexHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('B', os);
    ASSERT_EQ(os.str(), "110");
}

//build another tree with only newline symbol
class HCTreeFixture : public ::testing::Test {
  protected:
    HCTree oneNodeT;

  public:
    HCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs['\n'] = 100;
        oneNodeT.build(freqs);
    }
};

//test encode method
TEST_F(HCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    unsigned char newLine = 10;
    oneNodeT.encode(newLine, os);
    ASSERT_EQ(os.str(), "0");
}

//test empty HCTree
class EmptyHCTreeFixture : public ::testing::Test {
  protected:
    HCTree EmptyTree;

  public:
    EmptyHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        EmptyTree.build(freqs);
    }
};

//test encode method
TEST_F(EmptyHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    EmptyTree.encode('B', os);
}

//Build a tree with only one symbol
class OneHCTreeFixture : public ::testing::Test {
  protected:
    HCTree OneTree;

  public:
    OneHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs['A'] = 2;
        OneTree.build(freqs);
    }
};
//test encode method
TEST_F(OneHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    OneTree.encode('A', os);
    ASSERT_EQ(os.str(), "0");
}