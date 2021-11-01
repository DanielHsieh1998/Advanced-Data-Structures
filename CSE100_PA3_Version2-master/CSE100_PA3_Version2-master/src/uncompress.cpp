/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file uncompresses a compressed file
 */
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

/*A Method to build the HCTree recursively*/
void buildTree(HCNode* n, BitInputStream& in, int& index) {
    //read the inputFile bit by bit to build a HCTree
    unsigned int readIn = in.readBit();
    index++;
    //if the bit is 0, then create a HCNode with
    //zero count and zero symbol
    if (readIn == 0) {
        HCNode* leftHCNode = new HCNode(0, 0);
        leftHCNode->p = n;
        n->c0 = leftHCNode;
        buildTree(leftHCNode, in, index);
        HCNode* rightHCNode = new HCNode(0, 0);
        rightHCNode->p = n;
        n->c1 = rightHCNode;
        buildTree(rightHCNode, in, index);
        //if the bit is 1, then create a HCNode 
        //with a symbol inside. 
    } else {
        index = index + 8;
        n->symbol = in.readByte();
    }
}

/* Pseudo decompression with ascii encoding and naive header (checkpoint)
 */
void pseudoDecompression(string inFileName, string outFileName) {
    ifstream inputFile;
    inputFile.open(inFileName);

    //read in the first 256 lines to get the frequency vector
    vector<unsigned int> frequencyV;
    unsigned int inputInt;
    unsigned int freqInt;
    for (int i = 0; i < 256; i++) {
        freqInt = 0;
        inputInt = inputFile.get();
        while (inputInt != 10) {
            freqInt = freqInt * 10 + (inputInt - 48);
            inputInt = inputFile.get();
        }
        frequencyV.push_back(freqInt);
    }
    HCTree* tree = new HCTree();
    //build the HCTree with frequency vector
    tree->build(frequencyV);

    //decode the file byte by byte
    unsigned char inputChar;
    ofstream outputFile;
    outputFile.open(outFileName);
    while (1) {
        if (inputFile.peek() == EOF) {
            break;
        }
        inputChar = tree->decode(inputFile);
        outputFile << inputChar;
    }
    outputFile.close();
    inputFile.close();
    delete tree;
}

/* True decompression with bitwise i/o and small header (final) */
void trueDecompression(string inFileName, string outFileName) {
    ifstream inputFile;
    inputFile.open(inFileName);

    unsigned char inputChar;
    ofstream outputFile;
    //create a BitInputStream object
    BitInputStream in(inputFile, 4000);

    //get the index used to avoid padding mistake
    //at the end
    unsigned int end = in.readInt();
    //get the length of the tree encoded
    //and then plus the initial 64 bits
    unsigned int length = in.readInt() + 64;
    //revise end to avoid padding mistake
    end = ((length - 1) / 8 + 1) * 8 + end;
    
    //start building the HCTree
    HCNode* root = new HCNode(0, 0);
    HCTree* tree = new HCTree(root);
    int index = 0;
    //call the helper method to build the HCTree
    buildTree(tree->getRoot(), in, index);

    //avoid padding mistake after building the tree
    unsigned int n = in.getNBits();
    for (int i = 0; i < 8 - index % 8; i++) {
        in.readBit();
    }
    //start decoding the actual file
    outputFile.open(outFileName);
    while (1) {
        if (in.eof()) {
            break;
        }
        //readAll is true if the istream has already
        //read all bits from the inputFile
        if (in.readAll == true) {
            //get the nbits of the buffer
            unsigned int n = in.getNBits();
            //stop to avoid padding mistake
            if (n >= end) {
                break;
            }
        }
        //call decode to uncompress the inputFile
        inputChar = tree->decode(in);
        outputFile << inputChar;
    }
    outputFile.close();
    inputFile.close();
    delete tree;
}

/* Main program that runs the uncompress */
int main(int argc, char* argv[]) {
    cxxopts::Options options("./uncompress",
                             "Uncompresses files using Huffman Encoding");
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
    //check which uncompress method to use
    if (isAsciiOutput == true) {
        ofstream outFile;
        outFile.open(outFileName);
        if (outFile.is_open()) {
            pseudoDecompression(inFileName, outFileName);
        } else {
            ofstream outFile(outFileName);
            pseudoDecompression(inFileName, outFileName);
        }
        outFile.close();
    } else {
        ofstream outFile;
        outFile.open(outFileName);
        if (outFile.is_open()) {
            trueDecompression(inFileName, outFileName);
        } else {
            ofstream outFile(outFileName);
            trueDecompression(inFileName, outFileName);
        }
        outFile.close();
    }
}
