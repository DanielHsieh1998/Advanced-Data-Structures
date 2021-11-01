/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file declare an Actor class with its constructor
 */

#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <limits>
#include <string>
#include <vector>

using namespace std;

class Movie;
/*This class defines a Actor used to construct an ActorGraph*/
class Actor {
  public:
    //name of the Actor
    string name;
    //a vector of Movies the Actor did
    vector<Movie*> mvVec;
    //variable used for Dijkstra Algorithm
    int dis = std::numeric_limits<int>::max();
    //record the previous movie
    Movie* prevMov = nullptr;
    //variable used for Dijkstra Algorithm
    bool done = false;
    //constructor for Actor object
    Actor(string name) : name(name) {}
};

#endif