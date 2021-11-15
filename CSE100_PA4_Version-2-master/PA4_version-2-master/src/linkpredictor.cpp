/**
 * CSE 100 PA4 Link Predictor in Actor Graph
 */
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.hpp"

using namespace std;

/* Print the usage of the program */
void usage(char* program_name) {
    cerr << program_name << " called with incorrect arguments." << endl;
    cerr << "Usage: " << program_name
         << " movie_cast_file num_prediction query_actor_file link_predictions"
         << endl;
}

/* Main program that drives the linkpredictor */
int main(int argc, char* argv[]) {
    const int NUM_ARG = 5, INPUT_IDX = 1, NUM_PRED = 2, QUERY_IDX = 3,
              LINK_IDX = 4;

    if (argc != NUM_ARG) {
        usage(argv[0]);
        return 1;
    }

    char* graphFileName = argv[INPUT_IDX];
    unsigned int numPrediction = stoi(argv[NUM_PRED]);
    char* queryActors = argv[QUERY_IDX];
    char* prediction = argv[LINK_IDX];

    // build the actor graph from the input file
    ActorGraph* graph = new ActorGraph();
    cout << "Reading " << graphFileName << " ..." << endl;
    if (!graph->buildGraphFromFile(graphFileName)) return 1;
    cout << "Done." << endl;

    ifstream infile(queryActors);
    ofstream outfile(prediction);
    bool haveHeader = false;

    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        if (!haveHeader) {
            outfile << "Link Predictions" << endl;
            haveHeader = true;
            continue;
        }

        // read the query actor from each line
        istringstream ss(s);
        vector<string> record;
        while (ss) {
            string str;
            if (!getline(ss, str, '\n')) break;
            record.push_back(str);
        }

        // skip the incorrectly formatted line
        if (record.size() != 1) {
            continue;
        }

        // write the link predictions
        string actor(record[0]);
        vector<string> predictActors;
        graph->predictLink(actor, predictActors, numPrediction);

        unsigned int i = 0;
        for (auto name : predictActors) {
            if (i != predictActors.size() - 1)
                outfile << name << ", ";
            else
                outfile << name;
            i++;
        }
        outfile << endl;
    }

    outfile.close();
    infile.close();
    delete graph;
    return 0;
}