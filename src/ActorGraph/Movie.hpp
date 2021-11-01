/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This file implements a Movie class to be a part of
 * ActorGraph
 * */

#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <limits>
#include <string>
#include <vector>
#include "Actor.hpp"

using namespace std;

class Actor;
/**
 * This class defines a Movie class used to
 * construct an ActorGraph
 * */
class Movie {
  public:
    //name of the movie
    string movie;
    //year of production
    int year;
    //a vector of actors worked in this movie
    vector<Actor*> acVec;
    //variable used for Dijkstra Algorithm
    int dis = std::numeric_limits<int>::max();
    //record the previous Actor
    Actor* prevAct = nullptr;
    //variable used for Dijkstra Algorithm
    bool done = false;

    //Constructor for the Movie class
    Movie(string movie, int year) : movie(movie), year(year) {}
};
#endif