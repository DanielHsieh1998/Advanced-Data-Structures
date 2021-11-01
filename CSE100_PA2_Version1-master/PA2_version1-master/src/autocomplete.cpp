/*
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This files contains the user interaction program.
 * This program will take one or more files and read
 * every word and its corresponding frequency from
 * the file and build a Dictionary Trie. Then the
 * user will be able to enter the prefix and the number of
 * suggestions to complete, and the program will run the
 * corresponding predictCompletions or predictUnderscores
 * methods to respond and print the most frequent strings
 * in the terminal.
 *
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std;

/* Check if a given data file is valid */
bool fileValid(const char* fileName) {
    ifstream in;
    in.open(fileName, ios::binary);

    // Check if input file was actually opened
    if (!in.is_open()) {
        cout << "Invalid input file. No file was opened. Please try again.\n";
        return false;
    }

    // Check for empty file
    in.seekg(0, ios_base::end);
    unsigned int len = in.tellg();
    if (len == 0) {
        cout << "The file is empty. \n";
        return false;
    }
    in.close();
    return true;
}

/* IMPORTANT! You should use the following lines of code to match the correct
 * output:
 *
 * cout << "This program needs exactly one argument!" << endl;
 * cout << "Reading file: " << file << endl;
 * cout << "Enter a prefix/pattern to search for:" << endl;
 * cout << "Enter a number of completions:" << endl;
 * cout << completion << endl;
 * cout << "Continue? (y/n)" << endl;
 *
 * arg 1 - Input file name (in format like freq_dict.txt)
 */
int main(int argc, char** argv) {
    const int NUM_ARG = 2;
    if (argc != NUM_ARG) {
        cout << "Invalid number of arguments.\n"
             << "Usage: ./autocomplete <dictionary filename>" << endl;
        return -1;
    }
    if (!fileValid(argv[1])) return -1;

    DictionaryTrie* dt = new DictionaryTrie();

    // Read all the tokens of the file in order to get every word
    cout << "Reading file: " << argv[1] << endl;

    ifstream in;
    in.open(argv[1], ios::binary);
    string word;

    Utils::loadDict(*dt, in);
    in.close();

    char cont = 'y';
    unsigned int numberOfCompletions;
    vector<string> returnStr;
    // pass the highest frequency of the subtree of a node to the
    // node
    dt->highestFreq(dt->root);
    while (cont == 'y') {
        // Ask the user to enter a prefix/pattern
        cout << "Enter a prefix/pattern to search for:" << endl;
        getline(cin, word);
        // Ask the user to enter the number of suggested completions
        cout << "Enter a number of completions:" << endl;
        cin >> numberOfCompletions;

        // Call cleanVisited() to make sure all visited in
        // each node false to do DFS
        dt->cleanVisited();

        // traverse through the input word to see which
        // method to call
        for (int i = 0; i < word.length(); i++) {
            // If there exists any underscore, then
            // the program will call predictUnderscores
            if (word[i] == '_') {
                returnStr = dt->predictUnderscores(word, numberOfCompletions);
                break;
                // If there is no underscore, then call predictCompletions
            } else if (i == word.length() - 1 && word[i] != '_') {
                returnStr = dt->predictCompletions(word, numberOfCompletions);
            }
        }

        // Print out a list of most frequent words
        for (int i = 0; i < returnStr.size(); i++) {
            cout << returnStr.at(i) << endl;
        }

        // Ask if the user want to continue
        cout << "Continue? (y/n)" << endl;
        cin >> cont;
        cin.ignore();
    }
    delete dt;
    return 0;
}
