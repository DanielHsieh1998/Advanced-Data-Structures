/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file includes implementations 
 * for BitOutputStream object. 
 *
 */
#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** BitOutputStream allows for writing
 * data bit by bit to the buffer. 
*/
class BitOutputStream {
  private:
    ostream& out;          // reference to the output stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits have been writen to buf

  public:
    /* Constructor to create a BitOutputStream */
    explicit BitOutputStream(ostream& os, unsigned int bufSize) : out(os) {
        this->bufSize = bufSize;
        this->buf = new char[bufSize];
        this->nbits = 0;
        char* last = buf + bufSize;
        //call fill for initial filling
        std::fill(buf, last, 0);
    };

    //getter to the nbits
    unsigned int getNBits() { return nbits; }
    //Define a constant
    unsigned int BYTE_LEN = 8;

    /* Write the part of the buffer
    that was written by the user to the 
    output stream, and then clear the buffer
    for other use. */
    void flush();

    /* Write the least significant of the given int
    into the buffer. */
    void writeBit(unsigned int i);

    //Helper method to write a byte into the buffer
    void writeByte(byte b);

    //Helper method to write an int into the buffer
    void writeInt(unsigned int i);

    //getter to get a specific bit
    byte getBit(byte b, int n);

    //setter to set a specific bit
    byte setBit(byte b, unsigned int bit, int n);
    
    //destructor to delete the object
    ~BitOutputStream();
};

#endif
