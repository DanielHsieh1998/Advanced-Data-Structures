/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This files implement header files for the class ActorGraph.
 * It build a graph with vertices and edges.
 * The ActorGraph has a hash map of actors and a hash map of movies
 * Each actor or actress has a vector of movies,
 * and each movie has a vector of actors and actresses.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Actor.hpp"
#include "Movie.hpp"

using namespace std;

/**
 * The class for ActorGraph to hold
 * both actors and movies
 */
class ActorGraph {

  public:
    //a hash_map used to record all Actors in the ActorGraph
    unordered_map<string, Actor*> ActorSet;
    //a hash_map used to record all Movies in the ActorGraph
    unordered_map<string, Movie*> MovieSet;

    /* Constructor of ActorGraph */
    ActorGraph();

    /* Build the actor graph from dataset file.
     * Each line of the dataset file must be formatted as:
     * ActorName <tab> MovieName <tab> Year
     * Two actors are connected by an undirected edge if they have worked in a movie
     * before.
     */ 
    bool buildGraphFromFile(const char* filename);

    /**
     * Method to clear the ActorSet and MovieSet
     * Set all done to false, distance to infinite and prev to nullptr.
     */
    void clear();

    /* The method to do a BFS, and find out one of the shortest 
    path between two actors or actresses. */
    void BFS(const string& fromActor, const string& toActor,
             string& shortestPath);

    /**
     * The method to predict the potential actors and actresses
     * the actor will cooperate with.
     */
    void predictLink(const string& queryActor, vector<string>& predictionNames,
                     unsigned int numPrediction);

    /**
     * The helper method for predictLink.
     * Find out the number of edge between two actors
     */
    int NumOfEdge(Actor* A, Actor* B);

    /* The destructor of ActorGraph */
    ~ActorGraph();

    /**
     * The method to build a graph for self testers
     */
    void buildGraphTest();

    /*Comparator overwritten for priority_queue*/
    struct CandidateCP {
        bool operator()(pair<string, int> a, pair<string, int> b) const {
            if (a.second != b.second) {
                //The larger int will be in higher priority
                return (a.second < b.second);
            } else {
                //The smaller string will be in higher priority
                return (a.first > b.first);
            }
        }
    };
};

#endif  // ACTORGRAPH_HPP
