/**
 * NAMES: Yilan Jiang & Hongsheng Xie
 * EMAILS: yij007@ucsd.edu & hox012@ucsd.edu
 * This file defines a node class used to contruct
 * a binary search tree. Each node contains data value,
 * and its left, right and parent node pointers. It also
 * includes a few implementations to be used in BST.hpp.
 */
#ifndef BSTNODE_HPP
#define BSTNODE_HPP
#include <iomanip>
#include <iostream>
using namespace std;

/**
 * This class defines a BSTNode class as components to
 * build a binary search tree. It also contains getter
 * and setter for its private instance variable.
 * And it contains a method to return the successor
 * of the calling BSTNode.
 */
template <typename Data>
class BSTNode {
  private:
    Data data;

  public:
    BSTNode<Data>* left;
    BSTNode<Data>* right;
    BSTNode<Data>* parent;

    /** Constructor to initialize a BSTNode with
     * the given data reference as parameter and
     * set its parent and children as nullptr.
     */
    BSTNode(const Data& d) : data(d) { left = right = parent = nullptr; }

    /** Set the value of data */
    void setData(const Data& d) { data = d; }

    /** Get the value of data */
    Data getData() { return data; }

    /** Return the successor of this BSTNode in a BST
     * Successor has the smallest element that is
     * larger than this BSTNode */
    BSTNode<Data>* successor() {
        // Check if the BSTNode has a right child, if so,
        // then the successor is on the right subtree of
        // the calling BSTNode.
        if (this->right != nullptr) {
            BSTNode<Data>* curr = this->right;
            // Traverse to find the BSTNode with the
            // smallest data on the right subtree.
            while (curr->left != nullptr) {
                curr = curr->left;
            }
            return curr;
        } else {
            // If the BSTNode does not have a right child,
            // then traverse up until the current node
            // is the left child of its parent.
            BSTNode<Data>* curr = this;
            while (curr->parent != nullptr) {
                if (curr == curr->parent->left) {
                    return curr->parent;
                } else {
                    curr = curr->parent;
                }
            }
            // Reach the root and no successor exists.
            return nullptr;
        }
    }
};

/**
 * DO NOT CHANGE THIS METHOD
 * Overload operator<< to print a BSTNode's fields to an ostream.
 */
template <typename Data>
ostream& operator<<(ostream& stm, const BSTNode<Data>& n) {
    stm << '[';
    stm << setw(10) << &n;                  // address of the BSTNode
    stm << "; p:" << setw(10) << n.parent;  // address of its parent
    stm << "; l:" << setw(10) << n.left;    // address of its left child
    stm << "; r:" << setw(10) << n.right;   // address of its right child
    stm << "; d:" << n.data;                // its data field
    stm << ']';
    return stm;
}

#endif  // BSTNODE_HPP
