/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file contains all implementations written
 * for HCTree class.
 */

#include "HCTree.hpp"
#include <stack>
#include <vector>

/* destructor to delete the HCtree */
HCTree::~HCTree() { deleteAll(root); }

/* Take in a freuqncy vector to build the HCTree */
void HCTree::build(const vector<unsigned int>& freqs) {
    //Filter out any char with 0 freqs.
    bool isEmpty = false;
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i] > 0) {
            isEmpty = true;
        }
    }
    //if all freqs are 0 then return
    if (isEmpty == false) {
        return;
    }

    // create a priority_queue to hold all characters
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    // Push all nonzero frequency char into pq
    HCNode* curr = nullptr;
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i] > 0) {
            curr = new HCNode(freqs[i], char(i));
            pq.push(curr);
        }
    }
    
    //check if there's only one element in pq
    if (pq.size() == 1) {
        HCNode* oneNode = pq.top();
        leaves.push_back(oneNode);
    }

    HCNode* n0 = nullptr;
    HCNode* n1 = nullptr;
    HCNode* parent = nullptr;
    //build the tree with pq
    while (pq.size() > 1) {
        n0 = pq.top(); // highest priority node
        pq.pop();
        n1 = pq.top(); // second highest priority node
        pq.pop();
        if (n0->c0 == nullptr && n0->c1 == nullptr) { // if n0 is leaf node
            leaves.push_back(n0);
        }
        if (n1->c0 == nullptr && n1->c1 == nullptr) { // if n1 is leaf node
            leaves.push_back(n1);
        }
        // the symbol of parent is always the symbol of n0
        parent = new HCNode((n0->count + n1->count), n0->symbol, n0, n1);
        n0->p = parent; 
        n1->p = parent; 
        pq.push(parent);
    }
    root = pq.top(); // get the last node pointer in the stack
    pq.pop();
}

/* encode a paragraph into BitOutputStream*/
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    // the tree is empty
    if (leaves.empty()) {
        return;
    }

    /**
     * find the symbol in the tree
     */
    HCNode* curr = nullptr;
    for (int i = 0; i < leaves.size(); i++) {
        // the symbol does not exist in the tree
        if ((i == (leaves.size() - 1)) && (leaves[i]->symbol != symbol)) { 
            return;
        }
        if (leaves[i]->symbol == symbol) { // find the symbol
            curr = leaves[i];
            break;
        }
    }

    // case if the tree only has one node
    if (curr == root) {
        out.writeBit(0);
    }

    stack<unsigned int> bit_CP;
    // go back to the root of the HCTree
    while (curr != root) {
        if (curr->p->c0 == curr) { // curr is the left child of its parent
            bit_CP.push(0);
        }
        if (curr->p->c1 == curr) { // curr is the right child of its parent
            bit_CP.push(1);
        }
        curr = curr->p; // go the to parent node pointer
    }

    // write the stack
    while (!bit_CP.empty()) {
        unsigned int cur = bit_CP.top();
        bit_CP.pop();
        out.writeBit(cur);
    }
}

/* encode the paragraphs to ostream */
void HCTree::encode(byte symbol, ostream& out) const {
    if (leaves.empty()) {
        return;
    }

    // check if the symbol exists in the leaves
    HCNode* curr = nullptr;
    for (int i = 0; i < leaves.size(); i++) {
        // the symbol does not exist in the tree
        if ((i == (leaves.size() - 1)) && (leaves[i]->symbol != symbol)) { 
            return;
        }
        if (leaves[i]->symbol == symbol) { // find the symbol
            curr = leaves[i];
            break;
        }
    }
    string outputStr = "";
    // case if the tree only has one node
    if (curr == root) {
        outputStr = "0";
    }

    // encode the letter to a binary string
    while (curr != root) {
        if (curr->p->c0 == curr) { // curr is the left child of parent
            outputStr = '0' + outputStr;
        }
        if (curr->p->c1 == curr) { // curr is the right child of parent
            outputStr = '1' + outputStr;
        }
        curr = curr->p;
    }

    out << outputStr;
}

/* decode the paragraph from BitInputStream */
byte HCTree::decode(BitInputStream& in) const {
    HCNode* curr = root;
    if (curr == nullptr) { // the tree is empty
        return ' ';
    }

    unsigned int currBit = -1;
    // the tree only has one node
    if (curr->c0 == nullptr && curr->c1 == nullptr) { 
        currBit = in.readBit();
        return curr->symbol;
    }
    vector<unsigned int> v;
    // go down the tree to find the symbol
    while (curr->c0 != nullptr && curr->c1 != nullptr) {
        currBit = in.readBit();
        if (currBit == 0) {
            curr = curr->c0; // go left
        } else if (currBit == 1) {
            curr = curr->c1; // go right
        }
        v.push_back(currBit);
    }
    
    return curr->symbol;
}

/* decode the paragraph from istream */
byte HCTree::decode(istream& in) const {
    HCNode* curr = root;
    // the tree is empty
    if (curr == nullptr) {
        return ' ';
    }

    unsigned int currBit = -1;
    // curr is the leaf of the tree
    if (curr->c0 == nullptr && curr->c1 == nullptr) {
        currBit = in.get() - 48;
        return curr->symbol;
    }

    // go down the tree
    while (curr->c0 != nullptr && curr->c1 != nullptr) {
        currBit = in.get() - 48;
        if (currBit == 0) { // go left
            curr = curr->c0;
        } else if (currBit == 1) { // go right
            curr = curr->c1;
        }
    }
    return curr->symbol;
}

/**
 * helper method for destructor
 * delete all nodes in the HCTree
 */
void HCTree::deleteAll(HCNode* n) {
    // base case
    if (n == nullptr) {
        return;
    }
    // if its left child is not a nullptr,then
    // recurse to the left
    if (n->c0 != nullptr) {
        deleteAll(n->c0);
    }
    // if its right child is not a nullptr, then
    // recurve to the right
    if (n->c1 != nullptr) {
        deleteAll(n->c1);
    }
    delete n;
    return;
}
