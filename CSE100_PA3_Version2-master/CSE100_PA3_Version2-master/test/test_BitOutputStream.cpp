/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file contains testers to test BitOutputStream
 * */

#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include "BitOutputStream.hpp"

using namespace std;
using namespace testing;

//test writeBit and flush method
TEST(BitOutputStreamTests, SIMPLE_TEST) {
    stringstream ss;
    BitOutputStream bos(ss, 1);
    bos.writeBit(1);

    bos.flush();

    string bitsStr = "10000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    // cout << asciiVal << endl;
    ASSERT_EQ(ss.get(), asciiVal);
}

//test writeByte method
TEST(BitOutputStreamTests, writeByte_TEST) {
    stringstream ss;
    BitOutputStream bos(ss, 1);
    bos.writeByte(10000000);

    bos.flush();

    string bitsStr = "10000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    // cout << asciiVal << endl;
    ASSERT_EQ(ss.get(), asciiVal);
}