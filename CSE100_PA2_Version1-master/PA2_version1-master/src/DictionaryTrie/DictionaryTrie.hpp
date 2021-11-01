/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This files contains the method headers for
 * all implementations written in DictionaryTrie.cpp.
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <queue>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/**
 * The class for a dictionary ADT, implemented as either
 * a mulit-way trie or a ternary search tree.
 */
class DictionaryTrie {
    // private:
    // TODO: add private members and helper methods here
  public:
    // create an inner class called TrieNode to be used
    // to construct a DictionaryTrie
    class TrieNode {
      public:
        char data;
        TrieNode* left;
        TrieNode* middle;
        TrieNode* right;
        unsigned int freq;
        unsigned int highestFq;
        bool visited;
        // check if the current TrieNode is a
        // middle child of its parent TrieNode
        bool notmd;
        bool md;
        int layer;  // to record the layer of each TrieNode in a DictionaryTrie
        TrieNode(char c);
    };

    /* A constructor that only sets root to nullptr */
    DictionaryTrie();

    /* Given a string and an unsigned int
     * Insert a copy of the string in the TST
     * Return true if the item was successfully added to this TST,
     * false if the call does not input any word into this TST. */
    bool insert(string word, unsigned int freq);

    /**
     * Given a string word
     * Find the word in the TST.
     * Return true if the word is in the TST
     * Return false if the word is not in the TST
     */
    bool find(string word) const;

    /**
     * Given a string "prefix" and an integer "numCompletions".
     * Return a vector that include the "numCompletions" most frequenct
     * words which start with string "prefix".
     */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /**
     * predictUnderscores() will return a vector holding up to
     * the most frequent numCompletions of valid completions of
     * pattern, which contains one or more '_'.
     */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /**
     * Destructor of DiectionaryTrie
     * Called deleteAll(TrieNode* n) helper method
     * and passed in the root
     */
    ~DictionaryTrie();

    // instrance variable to DictionaryTrie
    TrieNode* root;
    int A = 32;
    int DELETE = 127;
    int UNDERSCORE = 95;
    int BACKQUOTE = 96;
    int TWO = 2;

    /**
     * Helper method for the DictionaryTrie destructor
     * to delete all TrieNodes in the heap
     * */
    static void deleteAll(TrieNode* n);

    /**
     * help method of "predictCompletions"
     * set the visited value of all nodes of the TST false
     */
    void cleanVisited();

    /**
     * Helper method used in backtrack() to check if the input string
     * does exist in the DictionaryTrie. It will return a pointer
     * to a pair with a TrieNode pointer as the first part and a
     * boolean as the second part. If the string does exist, the
     * boolean part will be true. Otherwise, it will be false.
     * */
    pair<TrieNode*, bool> existed(string curString);

    /**
     * Helper method for predictUnderscore method.
     * This method find valid strings to the input PatternVP
     * by trying all possible combinations recursively.
     * It will populate the result with all valid strings.
     * */
    void backtrack(vector<pair<string, unsigned int>>& result, int curIndex,
                   int maxLen, string curString,
                   vector<pair<int, char>> PatternVP);

    /**
     * helper method of main to set the highestFq of each node
     */
    void highestFreq(TrieNode* node);
    struct compare {
        bool operator()(pair<string, int>* a, pair<string, int>* b) const {
            if (a->second != b->second) {
                return (a->second > b->second);
            } else {
                return (a->first < b->first);
            }
        }
    };

    /**
     * helper method to control the size of the priority_queue
     */
    void pd_fixed(priority_queue<pair<string, int>*, vector<pair<string, int>*>,
                                 compare>& pq,
                  int numCompletions, pair<string, int>* currPair);

    /**
     * Helper method for predictCompletion to find the last index of
     * the prefix in the DictionaryTrie and return the corresponding
     * TrieNode* to predictCompletion.
     * */
    TrieNode* findPrefix(string prefix);
};

#endif  // DICTIONARY_TRIE_HPP
