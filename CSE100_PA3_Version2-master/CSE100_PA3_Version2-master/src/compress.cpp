/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file compresses an unencoded file
 */
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

/** 
 * pseudo compression with ascii encoding and naive header
 */
void pseudoCompression(string inFileName, string outFileName) {
    // open the file to be compressed
    ifstream theFile;
    theFile.open(inFileName);

    // check if the file is empty
    if (theFile.peek() == EOF) {
        return;
    }

    // create a vector to build a Huffman Tree
    vector<unsigned int> frequencyV(256, 0);
    unsigned char nextChar;
    int asciiInt;
    while (1) {
        nextChar = theFile.get();
        if (theFile.eof()) { // at end of file
            break;
        }
        asciiInt = (unsigned int)nextChar;
        frequencyV[asciiInt]++; // count the frequency of each symbol
    }
    theFile.close();
    HCTree* tree = new HCTree();
    tree->build(frequencyV); // build the tree with the frequency vector

    ofstream outFile;
    outFile.open(outFileName);

    for (int i = 0; i < 256; i++) {
        outFile << frequencyV[i] << endl; // write the frequency vector into ofstream
    }

    theFile.open(inFileName); 
    // encode the file
    while (1) {
        nextChar = (unsigned char)theFile.get();
        if (theFile.eof()) {
            break;
        }
        // encode the symol to binary string and input to the ofstream
        tree->encode(nextChar, outFile); 
    }
    theFile.close();
    outFile.close();
    delete tree;
}

// helper method using recursion to count the length of the encoded tree 
// including symbols and return an unsigned int
unsigned int preOrderTraverse(HCNode* n, vector<char>& v, unsigned int length) {
    unsigned int L_Of_Sym = 8;
    if (n->c0 != nullptr && n->c1 != nullptr) { // the node pointer is not a leaf
        v.push_back('0'); // '0' means not a leaf
        length++; //length increses
        // recurse to the left subtree
        length = preOrderTraverse(n->c0, v, length); 
        // recurse to the right subtree
        length = preOrderTraverse(n->c1, v, length);
    } else { // the node pointer is a leaf
        v.push_back('1'); // '1' means not a leaf
        length++; 
        v.push_back(n->symbol); // record the symbol following the leaf sign
        length = length + L_Of_Sym; // 
    }
    return length;
}

/* True compression with bitwise i/o and small header */
void trueCompression(string inFileName, string outFileName) {
    ifstream inFile;
    ofstream outFile;
    inFile.open(inFileName);
    outFile.open(outFileName);

    // check if the file is empty
    if (inFile.peek() == EOF) {
        return;
    }
    // build a Huffman Tree
    vector<unsigned int> frequencyV(256, 0);
    unsigned char nextChar;
    int asciiInt;
    while (1) {
        nextChar = inFile.get();
        if (inFile.eof()) {
            break;
        }
        asciiInt = (unsigned int)nextChar;
        frequencyV[asciiInt]++; // collect the frequency of each symbol
    }
    inFile.close();
    HCTree* tree = new HCTree();
    tree->build(frequencyV); // build the HCTree

    vector<char> vecTree;
    unsigned int l = 0;
    // get the length of the tree
    l = preOrderTraverse(tree->getRoot(), vecTree, 0); 
    BitOutputStream out(outFile, 4000);
    
    //write the tree into the outputFile
    char curr;
    for (int i = 0; i < vecTree.size(); i++) {
        curr = vecTree[i];
        if (curr == '0') {
            out.writeBit(0);
        } else if (curr == '1') {
            out.writeBit(1);
            i++;
            out.writeByte(vecTree[i]);
        }
    }
    out.flush();

    //encode the inputFile into the outputFile
    inFile.open(inFileName);
    while (1) {
        nextChar = (unsigned char)inFile.get();
        if (inFile.eof()) {
            break;
        }
        tree->encode(nextChar, out);
    }
    
    //get the current nbits of the bitOutputStream
    unsigned int endBits = out.getNBits();
    out.flush();

    inFile.close();
    outFile.close();
    //create another bitOutpushStream object
    BitOutputStream out1(outFile, 4000);
    outFile.open(outFileName);
    //write the nbits in last buffer into the outputFile
    out1.writeInt(endBits);

    out1.flush();
    //write the length of the tree into the outputFile
    out1.writeInt(l);
    out1.flush();
    //write the tree into the outputFile
    for (int i = 0; i < vecTree.size(); i++) {
        curr = vecTree[i];
        if (curr == '0') {
            out1.writeBit(0);
        } else if (curr == '1') {
            out1.writeBit(1);
            i++;
            out1.writeByte(vecTree[i]);
        }
    }
    out1.flush();

    //write the compressed data into the outputFile
    inFile.open(inFileName);
    while (1) {
        nextChar = (unsigned char)inFile.get();
        if (inFile.eof()) {
            break;
        }
        tree->encode(nextChar, out1);
    }

    out1.flush();
    inFile.close();
    outFile.close();
    //delete the HCTree
    delete tree;
}

/* Main program that runs the compress */
int main(int argc, char* argv[]) {
    cxxopts::Options options("./compress",
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        exit(0);
    }
    //check which compress method to use
    if (isAsciiOutput == true) {
        ofstream outFile;
        outFile.open(outFileName);
        if (outFile.is_open()) {
            pseudoCompression(inFileName, outFileName);
        } else {
            ofstream outFile(outFileName);
            pseudoCompression(inFileName, outFileName);
        }
        outFile.close();
    } else {
        ofstream outFile;
        outFile.open(outFileName);
        if (outFile.is_open()) {
            trueCompression(inFileName, outFileName);
        } else {
            ofstream outFile(outFileName);
            trueCompression(inFileName, outFileName);
        }
        outFile.close();
    }
}