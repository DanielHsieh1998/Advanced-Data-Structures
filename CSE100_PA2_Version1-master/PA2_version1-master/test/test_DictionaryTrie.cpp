/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file contains some tests and test fixtures to
 * test all implements written in DictionaryTrie.cpp file.
 *
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std;
using namespace testing;

/* Empty test */
TEST(DictTrieTests, EMPTY_TEST) {
    DictionaryTrie dict;
    ASSERT_EQ(dict.find("abrakadabra"), false);
}

/**
 * Test the insert and find methods
 **/
TEST(DictTrieTests, INSERT_TEST) {
    DictionaryTrie dict;
    bool x = dict.insert("apple", 100);
    ASSERT_TRUE(x);
    bool y = dict.insert("apple", 100);
    ASSERT_FALSE(y);
    bool z = dict.insert("drake", 999);
    ASSERT_TRUE(z);
    bool m = dict.insert("appletree", 101);
    ASSERT_TRUE(m);
    bool a1 = dict.insert("ape", 1);
    bool a2 = dict.insert("apt", 2);
    bool a3 = dict.insert("apz", 1);
    ASSERT_TRUE(a3);
    bool a4 = dict.insert("apz", 1);
    ASSERT_FALSE(a4);
    bool a5 = dict.insert("apt", 2);
    ASSERT_FALSE(a5);
    bool a6 = dict.insert("ap", 1);
    ASSERT_TRUE(dict.find("ap"));

    ASSERT_FALSE(dict.find("apx"));
    ASSERT_TRUE(dict.find("apz"));
    ASSERT_FALSE(dict.find("appletrees"));
    ASSERT_FALSE(dict.find("a"));
}

/**
 * Test the predictCompletions method
 * */
TEST(DictTrieTests, PREDICTCOMPLETE_TEST) {
    DictionaryTrie dict;
    bool x = dict.insert("apple", 100);
    bool m = dict.insert("appletree", 101);
    bool a1 = dict.insert("ape", 1);
    bool a2 = dict.insert("apt", 2);
    bool a3 = dict.insert("apz", 1);
    bool a4 = dict.insert("ap", 1);
    string tester = "ap";
    unsigned int num = 5;
    unsigned int zeros = 0;
    vector<string> test0 = dict.predictCompletions(tester, zeros);
    ASSERT_EQ(test0.size(), 0);
    vector<string> test = dict.predictCompletions(tester, num);

    ASSERT_EQ(test.size(), num);
}

/**
 * Test the predictCompletions method with a different
 * DictionaryTrie from previous tester
 * */
TEST(DictTrieTest, PREDICTCOMPLETE_NUMBER_TEST) {
    DictionaryTrie dict;
    bool a1 = dict.insert("butter", 100);
    bool a2 = dict.insert("ape", 30);
    bool a3 = dict.insert("az", 500);
    bool a4 = dict.insert("apde", 50);
    bool a5 = dict.insert("apdb", 50);
    bool a6 = dict.insert("apdf", 50);
    bool a7 = dict.insert("apdeq", 105);
    string tester = "a";
    unsigned int num = 7;
    vector<string> test = dict.predictCompletions(tester, num);
}

/**
 * Test predictCompletions method
 * */
TEST(DictTrieTest, PREDICTCOMPLETE_SHORT_DICT_TEST) {
    DictionaryTrie dict;
    dict.insert("a", 100000);
    dict.insert("a a", 10);
    dict.insert("a all", 100);
    dict.insert("a and", 192);
    dict.insert("a at", 93);
    dict.insert("a ba in", 33);
    dict.insert("a babble", 41);
    dict.insert("a babe", 100);
    dict.insert("a baby", 2424);
    dict.insert("a bach", 100);
    dict.insert("a bachelor", 532);
    dict.insert("a back", 152);
    dict.insert("a backdrop", 103);
    dict.insert("a background", 1002);
    dict.insert("a backlash", 130);
    dict.insert("a bad", 100);
    dict.insert("a bad c", 16);
    dict.insert("a badge", 35);
    dict.insert("a baff", 10);
    dict.insert("a bag", 1303);
    dict.insert("a bake", 16);
    dict.insert("a balance", 63);
    dict.insert("a ball", 1035);
    dict.insert("ant", 2300);
    vector<string> test = dict.predictCompletions("a ", 100);
    ASSERT_EQ(test[0], "a baby");
    vector<string> test1 = dict.predictCompletions("a ", 5);
}

/**
 * Build a new DictionaryTrie in a test fixture
 * */
class TrieUnderscoreTest : public ::testing::Test {
  protected:
    DictionaryTrie dict;

  public:
    TrieUnderscoreTest() {
        dict.insert("butter", 100);
        dict.insert("ape", 30);
        dict.insert("az", 500);
        dict.insert("apde", 50);
        dict.insert("apdb", 50);
        dict.insert("apdf", 50);
        dict.insert("apdeq", 105);
    }
};

// Test the predictUnderscores method
TEST_F(TrieUnderscoreTest, Simple_underscore_TEST) {
    vector<string> vecReturn = dict.predictUnderscores("a_d_", 3);
    ASSERT_EQ(vecReturn[0], "apdb");
    ASSERT_EQ(vecReturn[1], "apde");
    ASSERT_EQ(vecReturn[2], "apdf");
}

// Test the edge cases of predictUnderscores method
TEST_F(TrieUnderscoreTest, edge_TEST) {
    vector<string> vecReturn = dict.predictUnderscores("", 4);
    ASSERT_EQ(vecReturn.size(), 0);
    vecReturn = dict.predictUnderscores("aa", 4);
    ASSERT_EQ(vecReturn.size(), 0);
    vecReturn = dict.predictUnderscores("a_", 0);
    ASSERT_EQ(vecReturn.size(), 0);
    vecReturn = dict.predictUnderscores("a_", 10);
    ASSERT_EQ(vecReturn[0], "az");
    vecReturn = dict.predictUnderscores("_____", 10);
    ASSERT_EQ(vecReturn[0], "apdeq");
}
