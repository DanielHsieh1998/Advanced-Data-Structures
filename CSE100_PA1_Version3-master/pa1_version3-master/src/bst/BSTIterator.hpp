/**
 * NAMES: Yilan Jiang & Hongsheng Xie
 * EMAILS: yij007@ucsd.edu & hox012@ucsd.edu
 * This file defines an iterator class for BST
 * with several methods implemented.
 */
#ifndef BSTITERATOR_HPP
#define BSTITERATOR_HPP
#include <iterator>
#include <list>
#include "BSTNode.hpp"

using namespace std;

/**
 * This class defines a BSTIterator class with a private
 * instance variable, a BSTNode pointer. This class allows us
 * to traverse through a BST by connecting algorithms with
 * BST data structure. It overwritten a few operators such as
 * post-increment and pre-increment.
 */
template <typename Data>
class BSTIterator : public iterator<input_iterator_tag, Data> {
  private:
    BSTNode<Data>* curr;

  public:
    /**
     * Constructor that initialize the current BSTNode
     * in this BSTIterator.
     */
    BSTIterator(BSTNode<Data>* curr) : curr(curr) {}

    /** Dereference operator. */
    Data operator*() const { return curr->getData(); }

    /** Pre-increment operator. */
    BSTIterator<Data>& operator++() {
        curr = curr->successor();
        return *this;
    }

    /** Post-increment operator. */
    BSTIterator<Data> operator++(int) {
        BSTIterator before = BSTIterator(curr);
        ++(*this);
        return before;
    }

    /** Equality test operator
     * Return true if two iterators contain the same BSTNode pointer */
    bool operator==(BSTIterator<Data> const& other) const {
        if (this->curr == other.curr) {
            return true;
        }
        return false;
    }

    /** Inequality test operator
     * Return true if two iterators contain different BSTNode pointers */
    bool operator!=(BSTIterator<Data> const& other) const {
        if (this->curr != other.curr) {
            return true;
        }
        return false;
    }
};

#endif  // BSTITERATOR_HPP
