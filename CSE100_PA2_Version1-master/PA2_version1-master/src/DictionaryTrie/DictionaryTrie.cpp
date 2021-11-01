/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file implement the class DictionaryTrie and build
 * a ternary search tree with its root node, left, right, middle children,
 * size, freqency, highest frequency and layer. And it also implements
 * useful methods which can be processed on a TST object.
 */
#include "DictionaryTrie.hpp"
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

// Construtor for a TrieNode object
DictionaryTrie::TrieNode::TrieNode(char c) : data(c) {
    left = middle = right = nullptr;  // child nodes
    freq = 0;
    highestFq = 0;    // highest frequency in the subtree whose root is the node
    visited = false;  // check whether a node is visited
    notmd = false;
    md = false;
    layer = -1;  // the position this node could only be placed
}

/**
 * A constructor that only sets root to nullptr
 */

DictionaryTrie::DictionaryTrie() { this->root = nullptr; }

/**
 * Given a string and an unsigned int
 * Insert a copy of the string in the TST
 * Return true if the item was successfully added to this TST,
 * false if the call does not input any word into this TST.
 */
bool DictionaryTrie::insert(string word, unsigned int freq) {
    if (word.length() == 0) {  // the word is invalid
        return false;
    }

    if (this->root == nullptr) {  // the TST is empty
        this->root = new TrieNode(word[0]);
        this->root->md = true;
        this->root->layer = 0;
    }

    // store the word in a vector
    vector<char> vec;
    for (int i = 0; i < word.length(); i++) {
        vec.push_back(word[i]);
    }

    // insert the word
    int index = 0;
    TrieNode* curr = this->root;     // traverse the TST through root
    while (index < word.length()) {  // has not been to the end of the word
        if (vec.at(index) < curr->data) {  // might be on the left
            if (curr->left == nullptr) {   // insert the character
                curr->left = new TrieNode(word[index]);
                curr->left->notmd = true;
                curr->left->layer = curr->layer;
            }
            curr = curr->left;
        } else if (vec.at(index) > curr->data) {  // might be on the right
            if (curr->right == nullptr) {         // insert the character
                curr->right = new TrieNode(word[index]);
                curr->right->notmd = true;
                curr->right->layer = curr->layer;
            }
            curr = curr->right;
        } else if (vec.at(index) == curr->data) {  // might be in the middle
            if (index == word.length() - 1 &&
                curr->freq != 0) {  // the word is existed
                return false;
            } else if (index == word.length() - 1) {  // insert successfully
                curr->freq = freq;
                return true;
            }

            if (curr->middle == nullptr) {  // insert the character
                curr->middle = new TrieNode(word[index + 1]);
                curr->middle->md = true;
                curr->middle->layer = curr->layer + 1;
            }

            curr = curr->middle;
            index++;  // only move to the next character
                      // if the character is in the middle
        }
    }

    if (curr->freq != 0) {
        return false;
    }

    curr->freq = freq;
    return true;
}

/**
 * Given a string word
 * Find the word in the TST.
 * Return true if the word is in the TST
 * Return false if the word is not in the TST
 */
bool DictionaryTrie::find(string word) const {
    if (this->root == nullptr) {  // the TST is empty
        return false;
    }

    // plug a word string into a vector
    vector<char> vec;
    for (int i = 0; i < word.length(); i++) {
        vec.push_back(word[i]);
    }

    TrieNode* curr = this->root;
    int index = 0;
    // There are still some characters in the string
    while (index < word.length()) {
        if (vec.at(index) < curr->data) {  // might be on the left
            if (curr->left == nullptr) {   // not found
                break;
            }
            curr = curr->left;
        } else if (vec.at(index) == curr->data) {  // might be in the middle
            // find the word
            if (index == word.length() - 1 && curr->freq > 0) {
                return true;
            }
            if (curr->middle == nullptr) {  // not found
                break;
            }
            curr = curr->middle;
            index++;
        } else if (vec.at(index) > curr->data) {  // might be on the right
            if (curr->right == nullptr) {         // not found
                break;
            }
            curr = curr->right;
        }
    }
    return false;
}

/**
 * overloaded operator() for sort use in "predictUnderscores"
 */
struct sortComp {
    bool operator()(pair<string, int> a, pair<string, int> b) const {
        if (a.second != b.second) {
            return (a.second > b.second);  // swap if true
        } else {
            return (a.first < b.first);  // swap if true
        }
    }
} sortComp;

/**
 * help method of "predictCompletions"
 * set the visited value of all nodes of the TST false
 */
void DictionaryTrie::cleanVisited() {
    TrieNode* curr = this->root;
    stack<TrieNode*> traverseStack;
    traverseStack.push(curr);
    TrieNode* currTrieNode = nullptr;
    // use DFS to traverse the TST
    while ((!traverseStack.empty())) {
        currTrieNode = traverseStack.top();
        traverseStack.pop();
        currTrieNode->visited = false;  // set the value false

        if (currTrieNode->right != nullptr) {  // push to the right child
            traverseStack.push(currTrieNode->right);
        }

        if (currTrieNode->left != nullptr) {  // push to the left child
            traverseStack.push(currTrieNode->left);
        }

        if (currTrieNode->middle != nullptr) {  // push the middle child
            traverseStack.push(currTrieNode->middle);
        }
    }
}

/**
 * helper method to control the size of the priority_queue
 */
void DictionaryTrie::pd_fixed(
    priority_queue<pair<string, int>*, vector<pair<string, int>*>, compare>& pq,
    int numCompletions, pair<string, int>* currPair) {
    pair<string, int>* ref;
    // there are no enough words in the vector
    if (pq.size() < numCompletions) {
        pq.push(currPair);
        // cout << pq.top()->first << endl;
    } else {  // the size is full
        ref = pq.top();
        // the current word has larger frequency than the word
        // with smallest frequency in the vector
        if (currPair->second > ref->second) {
            pq.pop();
            delete ref;
            pq.push(currPair);
        } else if (currPair->second == ref->second) {
            if (currPair->first < ref->first) {
                pq.pop();
                delete ref;
                pq.push(currPair);
            } else {
                delete currPair;
            }
        } else {  // don't need to insert the current word
            delete currPair;
        }
    }
}

/**
 * helper method of main to set the highestFq of each node
 */
void DictionaryTrie::highestFreq(TrieNode* node) {
    if (node->right != nullptr) {  // there is a subtree on the right
        highestFreq(node->right);  // recurse to the right child
        // the frequency of the node is not the highest frequency of
        // the subtree whose root is the node
        if (node->freq < node->right->highestFq) {
            // the current highest frequency of the node is not the
            // actual highest frequency
            if (node->highestFq < node->right->highestFq) {
                node->highestFq = node->right->highestFq;
            }
        } else {  // the frequency of the node is the highest frequency
            node->highestFq = node->freq;
        }
    }

    if (node->middle != nullptr) {  // there is a subtree in the middle
        highestFreq(node->middle);  // recurse to the middle child
        // the frequency of the node is not the highest frequency of
        // the subtree whose root is the node
        if (node->freq < node->middle->highestFq) {
            // the current highest frequency of the node is not the
            // actual highest frequency
            if (node->highestFq < node->middle->highestFq) {
                node->highestFq = node->middle->highestFq;
            }
        } else {  // the frequency of the node is the highest frequency
            node->highestFq = node->freq;
        }
    }
    if (node->left != nullptr) {  // there is a subtree on the left
        highestFreq(node->left);  // recurse to the left child
        // the frequency of the node is not the highest frequency of
        // the subtree whose root is the node
        if (node->freq < node->left->highestFq) {
            // the current highest frequency of the node is not the
            // actual highest frequency
            if (node->highestFq < node->left->highestFq) {
                node->highestFq = node->left->highestFq;
            }
        } else {  // the frequency of the node is the highest frequency
            node->highestFq = node->freq;
        }
    }
    if (node->left == nullptr && node->middle == nullptr &&
        node->right == nullptr) {  // the node is leaf node
        node->highestFq = node->freq;
    }
}

/**
 * Helper method for predictCompletion to find the last index of
 * the prefix in the DictionaryTrie and return the corresponding
 * TrieNode* to predictCompletion.
 * */
DictionaryTrie::TrieNode* DictionaryTrie::findPrefix(string prefix) {
    TrieNode* start = nullptr;
    TrieNode* curr = this->root;
    int index = 0;

    while (index < prefix.length()) {
        if (prefix[index] < curr->data) {  // might on the left
            if (curr->left == nullptr) {   // the tree is not deep enough
                // break;
                return start;
            }
            curr = curr->left;                     // move to the left
        } else if (prefix[index] == curr->data) {  // might in the middle
            if (index == prefix.length() - 1) {    // find the word
                start = curr;
                // break;
                return start;
            }
            if (curr->middle == nullptr) {  // the tree is not deep enough
                // break;
                return start;
            }
            curr = curr->middle;  // move to the middle and point to the next
                                  // character
            index++;
        } else if (prefix[index] > curr->data) {  // might on the right
            if (curr->right == nullptr) {         // the tree is not deep enough
                // break;
                return start;
            }
            curr = curr->right;  // move to the right
        }
    }
}

/**
 * Given a string "prefix" and an integer "numCompletions".
 * Return a vector that include the "numCompletions" most frequenct
 * words which start with string "prefix".
 */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    // Initialize the return vector
    vector<string> vecReturn;

    // Prefix is a string with length 0 or the numCompletions is 0.
    if (prefix.length() == 0 || numCompletions == 0) {
        return vecReturn;
    }

    // call the helper method to find the prefix
    // in the DictionaryTrie
    TrieNode* start = findPrefix(prefix);

    if (start == nullptr) {  // there is no word starts with the string "prefix"
        return vecReturn;
    }

    // Initialize a priority_queue with an overloaded comparater
    std::priority_queue<pair<string, int>*, vector<pair<string, int>*>, compare>
        pq;

    // Start traversing the selected subtree starting with
    // the last element of the prefix with DFS
    stack<TrieNode*> traverseStack;
    string currword = "";
    int currlen = 0;
    traverseStack.push(start);
    int diff = start->layer;
    TrieNode* currTrieNode;
    pair<string, int>* currPair;
    while ((!traverseStack.empty())) {  // there is something in the stack
        currTrieNode = traverseStack.top();

        int l = currTrieNode->layer - diff;  // find out the index in currword

        for (int i = l + 1; i < currlen; i++) {
            currword.pop_back();
        }

        if (l > currlen - 1) {
            currword = currword + currTrieNode->data;
        } else {
            currword[l] = currTrieNode->data;
        }

        currlen = l + 1;
        if (currTrieNode->freq != 0) {  // the word exists
            currPair = new pair<string, int>(currword, currTrieNode->freq);

            pd_fixed(pq, numCompletions, currPair);
        }

        traverseStack.pop();
        currTrieNode->visited = true;  // Mark the node as visited

        // check if the node has a right child, if so,
        // then push its right child into the stack
        if (currTrieNode->right != nullptr &&
            currTrieNode->layer > start->layer) {
            if (pq.size() == 0) {
                if (currTrieNode->right->visited == false) {
                    traverseStack.push(currTrieNode->right);
                }
                // check the highestFreq of its right child branch is greater
                // than the lowest frequency in the priority_queue or if the
                // size of the priority_queue is less than the number of
                // suggested completions
            } else if (currTrieNode->right->highestFq >= pq.top()->second ||
                       pq.size() < numCompletions) {
                if (currTrieNode->right->visited == false) {
                    traverseStack.push(currTrieNode->right);
                }
            }
        }

        // check if the node has a left child, if so,
        // then push its left child into the stack
        if (currTrieNode->left != nullptr &&
            currTrieNode->layer > start->layer) {
            if (pq.size() == 0) {
                if (currTrieNode->left->visited == false) {
                    traverseStack.push(currTrieNode->left);
                }
                // check the highestFreq of its left child branch is greater
                // than the lowest frequency in the priority_queue or if the
                // size of the priority_queue is less than the number of
                // suggested completions
            } else if (currTrieNode->left->highestFq >= pq.top()->second ||
                       pq.size() < numCompletions) {
                if (currTrieNode->left->visited == false) {
                    traverseStack.push(currTrieNode->left);
                }
            }
        }

        // check if the node has a middle child, if so,
        // then push its middle child into the stack
        if (currTrieNode->middle != nullptr) {
            if (pq.size() == 0) {
                if (currTrieNode->middle->visited == false) {
                    traverseStack.push(currTrieNode->middle);
                }
                // check the highestFreq of its middle child branch is greater
                // than the lowest frequency in the priority_queue or if the
                // size of the priority_queue is less than the number of
                // suggested completions
            } else if (currTrieNode->middle->highestFq >= pq.top()->second ||
                       pq.size() < numCompletions) {
                if (currTrieNode->middle->visited == false) {
                    traverseStack.push(currTrieNode->middle);
                }
            }
        }
    }

    string pushString;
    // Get the part of the prefix in which we will concatenate
    // to the string in the priority_queue
    string addPrefix = prefix.substr(0, prefix.length() - 1);

    // check if the sie of the completed priority_queue is
    // less than the numCompletions. If so, then set the
    // numCompletions equal to the size of the priority_queue
    if (pq.size() < numCompletions) {
        numCompletions = pq.size();
    }

    // copy each string into a vector string called vecReturn
    pair<string, int>* element;
    for (int i = 0; i < numCompletions; i++) {
        element = pq.top();
        pq.pop();
        pushString = element->first;
        delete element;
        pushString = addPrefix + pushString;
        vecReturn.push_back(pushString);
    }

    // Since the priority_queue is a min-heap, we need
    // to swap the resulting vecReturn
    string temp;
    for (int i = 0; i < vecReturn.size() / TWO; i++) {
        temp = vecReturn[i];
        vecReturn[i] = vecReturn[vecReturn.size() - 1 - i];
        vecReturn[vecReturn.size() - i - 1] = temp;
    }
    return vecReturn;
}

/**
 * Helper method used in backtrack() to check if the input string
 * does exist in the DictionaryTrie. It will return a pointer
 * to a pair with a TrieNode pointer as the first part and a
 * boolean as the second part. If the string does exist, the
 * boolean part will be true. Otherwise, it will be false.
 * */
pair<DictionaryTrie::TrieNode*, bool> DictionaryTrie::existed(
    string curString) {
    bool check = false;
    // Initialize a TrieNode pointer pointing to the root
    TrieNode* curr = this->root;
    int index = 0;
    // while loop until the index is greater or equal to
    // the length of the input string
    while (index < curString.length()) {
        // if the current char in curString is less than
        // the corresponding char in the DictionaryTrie
        if (curString[index] < curr->data) {
            // if the left of the current TrieNode does not
            // exist, then the curString does not exist
            // in the DictionaryTrie
            if (curr->left == nullptr) {
                check = false;
                break;
            }
            curr = curr->left;
            // if the current char in curString is the same
            // as the corresponding char in the DictionaryTrie
        } else if (curString[index] == curr->data) {
            // if the curString exist in the DictionaryTrie
            if (index == curString.length() - 1) {
                check = true;
                break;
            }
            // if the middle of the current TrieNode does not
            // exist, then the curString does not exist
            // in the DictionaryTrie
            if (curr->middle == nullptr) {
                check = false;
                break;
            }
            curr = curr->middle;
            index++;
            // if the current char in curString is greater
            // than the corresponding char in the DictionaryTrie
        } else if (curString[index] > curr->data) {
            // if the right of the current TrieNode does not
            // exist, then the curString does not exist
            // in the DictionaryTrie
            if (curr->right == nullptr) {
                check = false;
                break;
            }
            curr = curr->right;
        }
    }
    pair<TrieNode*, bool> Rtpair = make_pair(curr, check);
    return Rtpair;
}

/**
 * Helper method for predictUnderscore method.
 * This method find valid strings to the input PatternVP
 * by trying all possible combinations recursively.
 * It will populate the result with all valid strings.
 * */
void DictionaryTrie::backtrack(vector<pair<string, unsigned int>>& result,
                               int curIndex, int maxLen, string curString,
                               vector<pair<int, char>> PatternVP) {
    pair<string, unsigned int> makePair;
    // base case when we read the maxLen
    if (curIndex == maxLen) {
        // call the helper method existed to check if the
        // curString is a string in the DictionaryTrie
        pair<TrieNode*, bool> exist = existed(curString);
        // check if the curString is valid
        if (exist.first->freq > 0 && exist.second == true) {
            // if so, then push the curString and corresponding
            // frequency into the result vector
            makePair = make_pair(curString, exist.first->freq);
            result.push_back(makePair);
        }
        return;
    }
    // check if the curIndex in the second part of
    // PatternVP is not an underscore
    if (PatternVP[curIndex].second != '_') {
        // if so, then add the corresponding char into the
        // curString
        curString = curString + PatternVP[curIndex].second;
        // call the helper method existed()
        pair<TrieNode*, bool> exist = existed(curString);
        if (exist.second == true) {
            // call backtrack() with curIndex+1
            backtrack(result, curIndex + 1, maxLen, curString, PatternVP);
        }
        // if the curIndex in the second part of PatternVP
        // is an underscore, then we will try different char
        // from A to ~ through ASCII code except '_' itself
    } else {
        // check all chars from A up to '^' by ascii code
        for (int i = A; i < UNDERSCORE; i++) {
            curString = curString + char(i);
            pair<TrieNode*, bool> exist = existed(curString);
            if (exist.second == true) {
                // if the curString with the symbol does exist in the
                // DictionaryTrie, then call backtrack
                backtrack(result, curIndex + 1, maxLen, curString, PatternVP);
            }
            curString.pop_back();
        }

        // check all chars from '`' to '-' by ascii code
        for (int i = BACKQUOTE; i < DELETE; i++) {
            curString = curString + char(i);
            pair<TrieNode*, bool> exist = existed(curString);
            if (exist.second == true) {
                // if the curString with the symbol does exist in the
                // DictionaryTrie, then call backtrack
                backtrack(result, curIndex + 1, maxLen, curString, PatternVP);
            }
            curString.pop_back();
        }
    }
}
/**
 * predictUnderscores() will return a vector holding up to
 * the most frequent numCompletions of valid completions of
 * pattern, which contains one or more '_'.
 */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    // vector<string> to be returned
    vector<string> vecReturn;
    // a vector holding (string, unsigned int) pairs
    // to be sorted later
    vector<pair<string, unsigned int>> highFreqPair;
    // edge cases if the numCompletions are 0 or the
    // the input pattern is an empty string
    if (numCompletions == 0 || pattern == "") {
        return vecReturn;
    }

    // decompose the input pattern into a vector
    // with each char's corresponding position
    vector<pair<int, char>> PatternVP;
    pair<int, char> makePair;
    for (int i = 0; i < pattern.length(); i++) {
        makePair = make_pair(i, pattern[i]);
        PatternVP.push_back(makePair);
    }

    // call the helper method backtrack to push all valid
    // strings into highFreqPair vector
    backtrack(highFreqPair, 0, pattern.length(), "", PatternVP);

    // sort the highFreqPair with overloaded comparator
    // called sortComp
    sort(highFreqPair.begin(), highFreqPair.end(), sortComp);

    // check if the resulting highFreqPair has smaller length
    // than the expected numCompletions
    if (highFreqPair.size() < numCompletions) {
        numCompletions = highFreqPair.size();
    }

    // push all most frequenct strings into the vecReturn
    // string vector
    for (int i = 0; i < numCompletions; i++) {
        vecReturn.push_back(highFreqPair[i].first);
    }

    return vecReturn;
}

/**
 * Helper method for the DictionaryTrie destructor
 * to delete all TrieNodes in the heap
 * */
void DictionaryTrie::deleteAll(TrieNode* n) {
    // base case
    if (n == nullptr) {
        return;
    }
    // if its left child is not a nullptr,then
    // recurse to the left
    if (n->left != nullptr) {
        deleteAll(n->left);
    }
    // if its middle child is not a nullptr,then
    // recurse to the middle
    if (n->middle != nullptr) {
        deleteAll(n->middle);
    }
    // if its right child is not a nullptr, then
    // recurve to the right
    if (n->right != nullptr) {
        deleteAll(n->right);
    }
    delete n;
    return;
}
/**
 * Destructor of DiectionaryTrie
 * Called deleteAll(TrieNode* n) helper method
 * and passed in the root
 */
DictionaryTrie::~DictionaryTrie() { deleteAll(this->root); }