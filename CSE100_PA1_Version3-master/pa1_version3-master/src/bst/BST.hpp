/**
 * NAMES: Yilan Jiang & Hongsheng Xie
 * EMAILS: yij007@ucsd.edu & hox012@ucsd.edu
 * This file defines a binary search tree class with its root node,
 * size, and height. And it also implements useful methods
 * which can be processed on a BST object.
 * Reference: Stepik and tutors.
 */

#ifndef BST_HPP
#define BST_HPP
#include <iostream>
#include <queue>
#include <vector>
#include "BSTIterator.hpp"
#include "BSTNode.hpp"
using namespace std;

/**
 * To build a BST with root, size and height.
 * It is able to build an empty
 * tree directly or delete all of the nodes in a BST.
 */
template <typename Data>
class BST {
  protected:
    // pointer to the root of this BST, or 0 if the BST is empty
    BSTNode<Data>* root;

    // number of Data items stored in this BST.
    unsigned int isize;

    // height of this BST.
    int iheight;

  public:
    /** Define iterator as an aliased typename for BSTIterator<Data>. */
    typedef BSTIterator<Data> iterator;

    /**
     * Default constructor.
     * Initialize an empty BST.
     */
    BST() : root(0), isize(0), iheight(-1) {}

    /** A copy constructor that creates a valid balanced BST with all
     * the data in the given binary search tree */
    BST(const BST<Data>& bst) : root(0), isize(0), iheight(-1) {
        vector<Data> inorder_vector = bst.inorder();
        // call the 'buildSubtree' helper function to
        // build subtress by recursion
        this->root =
            buildSubtree(inorder_vector, 0, inorder_vector.size(), iheight);
        this->isize = bst.size();
    }

    /** Default destructor.
     * Delete every node in this BST. */
    ~BST() {
        deleteAll(this->root);
        this->isize = 0;
        this->iheight = -1;
    }

    /** Given a reference to a Data item.
     * Insert a copy of in the BST.
     * Return true if the item was successfully added to this BST,
     * false if an item equal to this one was already in this BST.
     * Reference: Stepik
     */
    bool insert(const Data& item) {
        // empty tree
        if (this->empty() == true) {
            BSTNode<Data>* rt = new BSTNode<Data>(item);
            this->root = rt;
            isize++;
            iheight++;
            return true;
        }
        // Start at the root
        BSTNode<Data>* current = this->root;
        int ht = 0;  // count the new height
        while ((current->getData() < item) || (item < current->getData())) {
            ht++;
            if (item < current->getData()) {
                // If no left child, then insert the element
                // as its left child
                if ((current->left < nullptr) || (nullptr < current->left)) {
                    current = current->left;
                } else {
                    BSTNode<Data>* child = new BSTNode<Data>(item);
                    child->parent = current;
                    current->left = child;
                    isize++;
                    if (iheight < ht) {
                        iheight = ht;
                    }
                    return true;
                }
                // If no right child, then insert the element
                // as its right child
            } else if (current->getData() < item) {
                if ((current->right < nullptr) || (nullptr < current->right)) {
                    current = current->right;
                } else {
                    BSTNode<Data>* child = new BSTNode<Data>(item);
                    child->parent = current;
                    current->right = child;
                    isize++;
                    if (iheight < ht) {
                        iheight = ht;
                    }
                    return true;
                }
            }
        }
        return false;
    }

    /**Find a Data item in the BST
     * Return an iterator pointing to the given item or pointing
     * past the last node in the BST (an iterator containing null)
     * if not found
     * Reference: Stepik
     */
    iterator find(const Data& item) const {
        if (!(this->root < nullptr) && !(nullptr < this->root)) {
            iterator nullItr(nullptr);
            return nullItr;
        }
        // Start at the root
        BSTNode<Data>* current = this->root;
        while (current->getData() < item || item < current->getData()) {
            // if the data is less than the current data
            if (item < current->getData()) {
                current = current->left;
                // if the data is greater than the current data
            } else if (current->getData() < item) {
                current = current->right;
            }
            // After traversal, if there was no child
            if (!(current < nullptr) && !(nullptr < current)) {
                iterator nullItr(nullptr);
                return nullItr;
            }
        }
        iterator currentItr(current);
        return currentItr;
    }

    /** Given a reference to a Data item, delete the copy of this BST
     * After the deletion, the BST should still be valid.
     * Return true if the item was successfully deleted from this BST,
     * false if an item equal to this one does not exist in this BST.
     */
    bool deleteNode(const Data& item) {
        BSTNode<Data>* findNd = this->findNode(item);

        // findNd == nullptr
        if (!(findNd < nullptr) && !(nullptr < findNd)) {
            return false;
        }

        // leaf node
        // findNd->left == nullptr && findNd->right == nullptr
        if (!(findNd->left < nullptr) && !(nullptr < findNd->left) &&
            !(findNd->right < nullptr) && !(nullptr < findNd->right)) {
            // call the helper method to delete a node with
            // with no children
            leafNodeDelete(findNd);
            this->isize--;
            delete findNd;
            int height = this->getHt(this->root);
            if (height < iheight) {  // need to renew the hight
                iheight = height;
            }
            return true;
            // findNd->left != nullptr || findNd->right != nullptr
        } else if ((findNd->left < nullptr) || (nullptr < findNd->left) ||
                   (findNd->right < nullptr) || (nullptr < findNd->right)) {
            // two children
            // findNd->left != nullptr && findNd->right != nullptr
            if (((findNd->left < nullptr) || (nullptr < findNd->left)) &&
                ((findNd->right < nullptr) || (nullptr < findNd->right))) {
                BSTNode<Data>* successorNode = findNd->successor();
                findNd->setData(successorNode->getData());
                // call the helper method to delete a node with
                // two children
                twoChildrenDelete(findNd, successorNode);
                delete successorNode;
                this->isize--;
                int height = this->getHt(this->root);
                if (height < iheight) {  // need to renew the height
                    iheight = height;
                }
                return true;
                // one left child
                // findNd->left != nullptr && findNd->right == nullptr
            } else if (((findNd->left < nullptr) || (nullptr < findNd->left)) &&
                       !(findNd->right < nullptr) &&
                       !(nullptr < findNd->right)) {
                // call the helper method to delete a node with
                // one left child
                leftChildDelete(findNd);
                delete findNd;
                this->isize--;
                int height = this->getHt(this->root);
                if (height < iheight) {  // need to renew the height
                    iheight = height;
                }
                return true;
                // one right child
            } else {
                // call the helper method to delete a node with
                // one right child
                rightChildDelete(findNd);
                delete findNd;
                this->isize--;
                int height = this->getHt(this->root);
                if (height < iheight) {  // need to renew the height
                    iheight = height;
                }
                return true;
            }
        }
    }

    /** Return the number of items currently in the BST. */
    unsigned int size() const { return this->isize; }

    /** Return the height of the BST.
     * Empty BST has height -1 and BST with only one node has height 0.
     */
    int height() const { return this->iheight; }

    /** Return true if the BST is empty,
     * else return false.
     */
    bool empty() const {
        if (isize == 0 && iheight == -1) {
            return true;
        } else {
            return false;
        }
    }

    /** Return an iterator pointing to the first item
     * in the BST which is the smallest element
     */
    iterator begin() const {
        // Call helper method 'first()'
        BSTNode<Data>* beginNode = this->first(this->root);
        iterator beginItr(beginNode);
        return beginItr;
    }

    /** Return an iterator pointing past the last item in the BST. */
    iterator end() const { return typename BST<Data>::iterator(0); }

    /**Perform an inorder traversal of this BST to collect
     * the data of each node in ascending order to a vector.
     * Return a vector.*/
    vector<Data> inorder() const {
        vector<Data> returnVec;
        if (isize == 0) {
            return returnVec;
        }
        BSTNode<Data>* current = this->root;

        // call helper method to make a ascending vector
        ordertraverse(current, returnVec);
        return returnVec;
    }

    /**
     * DO NOT CHANGE THIS METHOD
     * Prints a tree, including its structure to an
     * output stream to help with grading and debugging.
     */
    void print(ostream* out) {
        if (empty()) {
            *out << "(Empty tree)" << endl;
            return;
        }

        // Using a queue to traverse a tree is otherwise
        // known as a breadth first traversal of the tree.
        // It's useful here to show which nodes are on which
        // level.
        queue<BSTNode<Data>*> toVisit;
        toVisit.push(root);
        int nodesPerLevel = 1;
        int totalSpacing = 1 << iheight;

        for (int i = 0; i <= iheight; i++) {
            for (int j = 0; j < nodesPerLevel; j++) {
                BSTNode<Data>* curr = toVisit.front();
                toVisit.pop();
                if (curr == nullptr) {
                    *out << "X";
                    // If we have an entire missing subtree, we
                    // would like to print X's in place of all
                    // nodes because it avoids ambiguity
                    toVisit.push(nullptr);
                    toVisit.push(nullptr);
                } else {
                    *out << curr->getData();
                    toVisit.push(curr->left);
                    toVisit.push(curr->right);
                }
                for (int k = 0; k < totalSpacing / nodesPerLevel; k++) {
                    *out << "\t";
                }
            }
            *out << endl;
            nodesPerLevel *= 2;
        }
    }

  private:
    /** Helper function for begin()
     * Find the first or smallest element in the BST.
     */
    static BSTNode<Data>* first(BSTNode<Data>* root) {
        if (root == nullptr) {
            return nullptr;
        }
        BSTNode<Data>* current = root;
        while (current->left != nullptr) {  // has a left child
            current = current->left;
        }
        return current;
    }

    /** Helper function for the destructor
     * Delete all nodes in the BST
     */
    static void deleteAll(BSTNode<Data>* n) {
        /* Pseudocode:
           if current node is null: return;
           recursively delete left sub-tree
           recursively delete right sub-tree
           delete current node
        */
        if (n == nullptr) {
            return;
        }

        if (n->left != nullptr) {  // has a left child
            deleteAll(n->left);
        }
        if (n->right != nullptr) {  // has a right child
            deleteAll(n->right);
        }
        delete n;
        return;
    }

    /** The helper function
     * that recursively builds a balanced BST with the given data.
     * data: a vector of data used in building BST
     * start: the start index (inclusive) of the vector
     * used to build the subtre
     * end: the end index (exclusive) of the vector used to build the subtree
     * Depth: the depth of recursion, used to update height
     */
    BSTNode<Data>* buildSubtree(vector<Data>& data, int start, int end,
                                int depth) {
        // The subtree is empty
        if (start == end) {
            return nullptr;
        }

        // The depth of root
        if (depth == -1) {
            depth = 0;
        }

        // Only have one element
        if ((start + 1) == end) {
            BSTNode<Data>* current = new BSTNode<Data>(data.at(start));
            if (depth > iheight) {  // need to renew the height
                iheight = depth;
            }
            return current;
        }

        // look for the median
        int median = (end + start) / 2;
        BSTNode<Data>* current = new BSTNode<Data>(data.at(median));

        BSTNode<Data>* left_sub = buildSubtree(data, start, median, depth + 1);
        current->left = left_sub;

        // Link the left subtree to the current node
        if (left_sub != nullptr) {
            left_sub->parent = current;
        }

        // Build the right subtree
        BSTNode<Data>* right_sub =
            buildSubtree(data, median + 1, end, depth + 1);
        current->right = right_sub;

        // Link the right subtree to the current node
        if (right_sub != nullptr) {
            right_sub->parent = current;
        }

        return current;
    }

    /**
     * Perform an inorder traversal of this BST to
     * collect the data of each node in ascending order to a vector.
     */
    void ordertraverse(BSTNode<Data>* node, vector<Data>& v) const {
        if (node == nullptr) {
            return;
        }
        ordertraverse(node->left, v);   // add the left subtree
        v.push_back(node->getData());   // add the node itself
        ordertraverse(node->right, v);  // add the right subtree
    }

    /**
     * Helper method for 'deleteNode'.
     * Return the node pointer which contains the certain value
     */
    BSTNode<Data>* findNode(const Data& item) const {
        // this->root == nullptr
        if (!(this->root < nullptr) && !(nullptr < this->root)) {
            return nullptr;
        }

        // current node to look for the data
        BSTNode<Data>* current = this->root;

        // current->getData() != item
        while (current->getData() < item || item < current->getData()) {
            // item can only be in the left subtree
            if (item < current->getData()) {
                current = current->left;
                // item can only be in the right subtree
            } else if (current->getData() < item) {
                current = current->right;
            }
            // current == nullptr
            if (!(current < nullptr) && !(nullptr < current)) {
                return nullptr;
            }
        }
        return current;
    }

    /**
     * Helper method to find the height of the BST whose root is the node.
     * Return the height of the BST.
     */
    int getHt(BSTNode<Data>* node) {
        // node->left == nullptr && node->right == nullptr
        if (!(node->left < nullptr) && !(nullptr < node->left) &&
            !(node->right < nullptr) && !(nullptr < node->right)) {
            return 0;
        }
        int heightLeft = -1;
        int heightRight = -1;

        // node->left != nullptr
        if (node->left < nullptr || nullptr < node->left) {
            heightLeft = getHt(node->left) + 1;
        }

        // node->right != nullptr
        if (node->right < nullptr || nullptr < node->right) {
            heightRight = getHt(node->right) + 1;
        }

        if (heightLeft < heightRight) {
            return heightRight;
        } else {
            return heightLeft;
        }
    }

    /**
     * Helper method of 'deleteNode'
     * Delete the node if the node is a leaf node
     */
    void leafNodeDelete(BSTNode<Data>* findNd) {
        // findNd->parent->left == findNd
        if (!(findNd->parent->left < findNd) &&
            !(findNd < findNd->parent->left)) {
            findNd->parent->left = nullptr;
        } else {
            findNd->parent->right = nullptr;
        }
    }

    /**
     * Helper method of 'deleteNode'
     * Delete the node if the node has two children
     */
    void twoChildrenDelete(BSTNode<Data>* findNd,
                           BSTNode<Data>* successorNode) {
        // successorNode->left == nullptr &&
        // successorNode->right == nullptr
        if (!(successorNode->left < nullptr) &&
            !(nullptr < successorNode->left) &&
            !(successorNode->right < nullptr) &&
            !(nullptr < successorNode->right)) {
            // successorNode->parent->left == successorNode
            if (!(successorNode->parent->left < successorNode) &&
                !(successorNode < successorNode->parent->left)) {
                successorNode->parent->left = nullptr;
            } else {
                successorNode->parent->right = nullptr;
            }
            // successorNode->right != nullptr
        } else if ((successorNode->right < nullptr) ||
                   (nullptr < successorNode->right)) {
            // successorNode->parent->right == successorNode
            if (!(successorNode->parent->right < successorNode) &&
                !(successorNode < successorNode->parent->right)) {
                successorNode->parent->right = successorNode->right;
                successorNode->right->parent = successorNode->parent;
            } else {
                successorNode->parent->left = successorNode->right;
                successorNode->right->parent = successorNode->parent;
            }
        }
    }

    /**
     * Helper method of 'deleteNode'
     * Delete the left child if the node only has a left child
     */
    void leftChildDelete(BSTNode<Data>* findNd) {
        // findNd->parent == nullptr
        if (!(findNd->parent < nullptr) && !(nullptr < findNd->parent)) {
            this->root = findNd->left;
            findNd->left->parent = nullptr;
        } else {
            // findNd->parent->left == findNd
            if (!(findNd->parent->left < findNd) &&
                !(findNd < findNd->parent->left)) {
                findNd->parent->left = findNd->left;
                findNd->left->parent = findNd->parent;
            } else {
                findNd->parent->right = findNd->left;
                findNd->left->parent = findNd->parent;
            }
        }
    }

    /**
     * Helper method of 'deleteNode'
     * Delete the right child if the node only has a right child
     */
    void rightChildDelete(BSTNode<Data>* findNd) {
        // findNd->parent == nullptr
        if (!(findNd->parent < nullptr) && !(nullptr < findNd->parent)) {
            this->root = findNd->right;
            findNd->right->parent = nullptr;
        } else {
            // findNd->parent->right == findNd
            if (!(findNd->parent->right < findNd) &&
                !(findNd < findNd->parent->right)) {
                findNd->parent->right = findNd->right;
                findNd->right->parent = findNd->parent;
            } else {
                findNd->parent->left = findNd->right;
                findNd->right->parent = findNd->parent;
            }
        }
    }
};

#endif  // BST_HPP
