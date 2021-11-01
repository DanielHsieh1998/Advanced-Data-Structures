/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file includes implementations 
 * for BitInputStream object. 
 *
 */
#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** BitInputStream allows for reading
 * data bit by bit. It mainly contains an 
 * istream to read data into the butter,
 * which is another instance varialbe 
 * in the BitInputStream object. 
*/
class BitInputStream {
  private:
    istream& in;           // reference to the input stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits that have been read from buf
    bool eofBit;           // true when trying to read past the end of the file
    unsigned int numOfBits;

    /* Return true when istream tried to read more bytes 
     * than the remaining byte in the file and the 
     * buffer was then also fully read.
     * Otherwise, return false */
    bool atEndOfFile();

  public:
    /* Construct a BitInputStream object */
    explicit BitInputStream(istream& is, unsigned int bufSize) : in(is) {
        this->bufSize = bufSize;
        this->nbits = 0;
        this->buf = new char[bufSize];
        eofBit = false;
        this->fill();
    };
    
    //getter to get the nbits
    unsigned int getNBits() { return nbits; }

    bool readAll = false;//check if the file is entirely read

    /**
    * Fill the buffer of size bufSize from input stream
    * */
    void fill();

    /* Returns true when trying to read past the end of the file, 
    false otherwise */
    bool eof();

    /** Read the next bit from the buffer.
    * If the buffer is full, then call 
    * fill() to read more bits into 
    * the buffer. 
    * Return 0 if the bit read is 0 and return 1
    * if bit read is 1
    * */
    unsigned int readBit();

    //set constants
    unsigned int BYTE_LEN = 8;
    unsigned int TWO = 2;

    //destrcutor to delete the BitInputStream object
    ~BitInputStream();

    /**
    * Helper method to read the next byte
    * from the bit buffer.
    * */
    byte readByte();

    /**
    * Helper method to read the next int, 
    * 32 bits, from the bit buffer.
    * */
    unsigned int readInt();
};

#endif