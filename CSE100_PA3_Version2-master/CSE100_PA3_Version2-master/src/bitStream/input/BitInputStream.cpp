/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file includes implementations 
 * for BitInputStream object. 
 *
 */
#include "BitInputStream.hpp"
#include <cmath>
#include <vector>

/**
 * Fill the buffer of size bufSize from input stream
 * */
void BitInputStream::fill() {
    nbits = 0;//set nbits to zero
    numOfBits = 0;
    in.read(buf, bufSize);
}

/**
 * Return true when istream tried to read more bytes 
 * than the remaining byte in the file and the 
 * buffer was then also fully read.
 * Otherwise, return false
 * */
bool BitInputStream::atEndOfFile() {
    // check if istream has read all the words in the file
    if (!in) {
        readAll = true;
        int numBytesRead = in.gcount();
        // check if the buffer was fully read
        if (numBytesRead == 0 || numBytesRead * BYTE_LEN == nbits) {
            return true;
        }
    }
    return false;
}

/* Returns true when trying to read past the end of the file, 
false otherwise */
bool BitInputStream::eof() { return eofBit; }

/** Read the next bit from the buffer.
 * If the buffer is full, then call 
 * fill() to read more bits into 
 * the buffer. 
 * Return 0 if the bit read is 0 and return 1
 * if bit read is 1
 * */
unsigned int BitInputStream::readBit() {
    //check if the buffer is full
    if (nbits == BYTE_LEN * bufSize) {
        this->fill();
    }
    //check if it is at the end of the file
    if (atEndOfFile()) {
        eofBit = true;
        return 0;
    }
    //get the bit from the buffer
    unsigned int index = nbits / BYTE_LEN;
    byte b = buf[index];

    unsigned int n = nbits % BYTE_LEN;
    nbits++;
    numOfBits++;
    return (b & (1 << (BYTE_LEN - n - 1))) != 0;
}

/**
 * Helper method to read the next byte
 * from the bit buffer.
 * */
byte BitInputStream::readByte() {
    //call readBit() for 8 times
    //and store them into a vector
    vector<unsigned int> v;
    for (int i = 0; i < BYTE_LEN; i++) {
        unsigned int input = readBit();
        v.push_back(input);
    }

    //calculate the value of the byte
    unsigned int sum = 0;
    byte result;
    for (int i = BYTE_LEN - 1; i >= 0; i--) {
        sum = sum + v[BYTE_LEN - 1 - i] * pow(TWO, i);
    }
    result = byte(sum);
    return result;
}

/**
 * Helper method to read the next int, 
 * 32 bits, from the bit buffer.
 * */
unsigned int BitInputStream::readInt() {
    //call the readBit() for 32 times
    //and store it into a vector
    vector<unsigned int> v;
    for (int i = 0; i < 32; i++) {
        unsigned int input = readBit();
        v.push_back(input);
    }

    //calculate the value of the extracted bits
    unsigned int sum = 0;
    for (int i = 31; i >= 0; i--) {
        sum = sum + v[32 - 1 - i] * pow(TWO, i);
    }

    return sum;
}

//destrcutor to delete the BitInputStream object
BitInputStream::~BitInputStream() { delete[] buf; }