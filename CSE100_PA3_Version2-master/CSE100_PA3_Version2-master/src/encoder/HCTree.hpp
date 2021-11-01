/**
 * TODO: file header
 *
 * Author:
 */
#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <fstream>
#include <queue>
#include <vector>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode.hpp"

using namespace std;

/** A class, which build a HCTREE
 * the tree contains a root and a vector to store the leaf nodes
 */
class HCTree {
  private:
    HCNode* root;            // the root of HCTree
    vector<HCNode*> leaves;  // a vector storing pointers to all leaf HCNodes

  public:
    int n;
    /* Constructor that initialize an HCTree */
    HCTree() {
        root = nullptr;
        // n = 0;
        // do we need to initialize leaves?
    }

    HCTree(HCNode* r) {
        root = r;
        // n = 0;
    }
    // unsigned int addOne(unsigned int n);

    HCNode* getRoot() { return this->root; }

    /* destructor that delete an HCTree */
    ~HCTree();

    /* Take in a freuqncy vector to build the HCTree */
    void build(const vector<unsigned int>& freqs);

    /* encode a paragraph into BitOutputStream */
    void encode(byte symbol, BitOutputStream& out) const;

    /* encode the paragraphs to ostream */
    void encode(byte symbol, ostream& out) const;

    /* decode the paragraph from BitInputStream */
    byte decode(BitInputStream& in) const;

    /* decode the paragraph from istream */
    byte decode(istream& in) const;

    // helper method for destructor
    void deleteAll(HCNode* n);
};

#endif  // HCTREE_HPP
