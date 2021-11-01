/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file includes implementations 
 * for BitOutputStream object. 
 *
 */
#include "BitOutputStream.hpp"
#include <stack>

/* Write the part of the buffer
that was written by the user to the 
output stream, and then clear the buffer
for other use. */
void BitOutputStream::flush() {
    int index = nbits / BYTE_LEN;
    int n = nbits % BYTE_LEN;
    if (n == 0) {
        for (int i = 0; i < index; i++) {
            out << buf[i];
        }
    } else if (n > 0) {
        for (int i = 0; i < index + 1; i++) {
            out << buf[i];
        }
    }

    // zero fill the buffer
    int fillEnd = nbits / BYTE_LEN + 1;
    char* end = buf + fillEnd;
    std::fill(buf, end, 0);
    nbits = 0;
}

/* setter to set a bit of a byte */
byte BitOutputStream::setBit(byte b, unsigned int bit, int n) {
    return ((bit << (BYTE_LEN - n - 1)) | b);
}

/* Write the least significant of the given int
into the buffer. */
void BitOutputStream::writeBit(unsigned int i) {
    //flush the buffer if the buffer is full
    if (nbits == BYTE_LEN * bufSize) {
        flush();
    }
    //write the bit by calling setBit
    int index = nbits / BYTE_LEN;
    byte b = buf[index];
    int n = nbits % BYTE_LEN;
    byte newByte = setBit(b, i, n);
    buf[index] = newByte;
    nbits++;
}

//Helper method to write a byte into the buffer
void BitOutputStream::writeByte(byte b) {
    //conver the byte into unsigned int
    unsigned int intb = (unsigned int)b;
    unsigned int one_zero = 0;
    stack<unsigned int> stac;
    for (int i = 0; i < BYTE_LEN; i++) {
        one_zero = intb - (intb / 2) * 2;
        intb = intb - (intb + 1) / 2;
        stac.push(one_zero);
    }
    //call writeBit and get the byte in
    //a correct order
    for (int i = 0; i < BYTE_LEN; i++) {
        writeBit(stac.top());
        stac.pop();
    }
}

//Helper method to write an int into the buffer
void BitOutputStream::writeInt(unsigned int i) {
    unsigned int one_zero = 0;
    stack<unsigned int> stac;
    for (int k = 0; k < 32; k++) {
        one_zero = i - (i / 2) * 2;
        i = i - (i + 1) / 2;
        stac.push(one_zero);
    }
    //call writeBit and get the 32 bit
    //in a correct order
    for (int k = 0; k < 32; k++) {
        writeBit(stac.top());
        stac.pop();
    }
}

//Destructor to delete this object
BitOutputStream::~BitOutputStream() { delete[] buf; }
