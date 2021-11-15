/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This files implement the class ActorGraph.
 * It build a graph with vertices and edges.
 * The ActorGraph has a hash map of actors and a hash map of movies
 * Each actor or actress has a vector of movies,
 * and each movie has a vector of actors and actresses.
 */

#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>
#include "Actor.hpp"
#include "Movie.hpp"

using namespace std;

/* The constructor of ActorGraph*/
ActorGraph::ActorGraph() {}

/* Build the actor graph from dataset file.
 * Each line of the dataset file must be formatted as:
 * ActorName <tab> MovieName <tab> Year
 * Two actors are connected by an undirected edge if they have worked in a movie
 * before.
 */
bool ActorGraph::buildGraphFromFile(const char* filename) {
    ifstream infile(filename);
    bool readHeader = false;

    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        // skip the header of the file
        if (!readHeader) {
            readHeader = true;
            continue;
        }

        // read each line of the dataset to get the movie actor relation
        istringstream ss(s);
        vector<string> record;
        while (ss) {
            string str;
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        // if format is wrong, skip current line
        if (record.size() != 3) {
            continue;
        }

        // extract the information
        string actor(record[0]);
        string title(record[1]);
        int year = stoi(record[2]);
        string curMV = title + "#@" + to_string(year);

        // double cycle
        if (this->ActorSet.find(actor) !=
            this->ActorSet.end()) {  // the actor exists
            // find the actor pointer in the Actorset
            Actor* AC = this->ActorSet.find(actor)->second;
            // the movie exists
            if (this->MovieSet.find(curMV) != this->MovieSet.end()) {
                Movie* MV = this->MovieSet.find(curMV)->second;
                AC->mvVec.push_back(
                    MV);  // store the movie in actor's movie vector
                MV->acVec.push_back(
                    AC);  // store the actor in the movie's actor vector
            } else {      // the movie does not exist
                Movie* Mv =
                    new Movie(title, year);  // create a new movie pointer
                this->MovieSet.insert(make_pair(curMV, Mv));
                AC->mvVec.push_back(Mv);
                Mv->acVec.push_back(AC);
            }
        } else {  // the actor does not exist
            Actor* Act = new Actor(actor);
            this->ActorSet.insert(make_pair(actor, Act));
            if (this->MovieSet.find(curMV) != this->MovieSet.end()) {
                Movie* MV = this->MovieSet.find(curMV)->second;
                Act->mvVec.push_back(MV);
                MV->acVec.push_back(Act);
            } else {
                Movie* Mv = new Movie(title, year);
                this->MovieSet.insert(make_pair(curMV, Mv));
                Act->mvVec.push_back(Mv);
                Mv->acVec.push_back(Act);
            }
        }

        // TODO: we have an actor/movie relationship to build the graph
    }
    // if failed to read the file, clear the graph and return
    if (!infile.eof()) {
        cerr << "Failed to read " << filename << endl;
        return false;
    }
    infile.close();

    return true;
}

/**
 * The method to do a BFS, and find out one of the shortest
 * path between two actors or actresses.
 */
void ActorGraph::BFS(const string& fromActor, const string& toActor,
                     string& shortestPath) {
    // one or two of the two actors do not exist
    // there cannot be any path between the two actors
    if (this->ActorSet.find(fromActor) == this->ActorSet.end() ||
        this->ActorSet.find(toActor) == this->ActorSet.end()) {
        return;
    }

    // there are not two distinct actors
    if (fromActor == toActor) {
        return;
    }
    queue<pair<Actor*, Movie*>> path;
    clear();

    // get the pointer of the fromActor
    Actor* curAct = this->ActorSet.find(fromActor)->second;
    curAct->dis = 0;  // set the distance of starter actor 0
    pair<Actor*, Movie*> cur = make_pair(curAct, nullptr);
    path.push(cur);

    // BFS
    while (!path.empty()) {
        pair<Actor*, Movie*> cur = path.front();
        path.pop();

        if (cur.first != nullptr) {            // cur is an actor
            if (cur.first->name == toActor) {  // find the final actor
                stack<string> SP;  // store the path between the two actors
                SP.push('(' + cur.first->name + ')');  // push the last actor

                Actor* act;
                act = cur.first;
                Movie* movie;
                // traverse back
                while (act->prevMov != nullptr) {
                    movie = act->prevMov;
                    act = movie->prevAct;
                    SP.push('[' + movie->movie + "#@" + to_string(movie->year) +
                            "]-->");
                    SP.push('(' + act->name + ")--");
                }
                // get the first element of the stack one by one
                while (!SP.empty()) {
                    shortestPath = shortestPath + SP.top();
                    SP.pop();
                }
            }

            if (!cur.first->done) {
                cur.first->done = true;
                int Dis = cur.first->dis + 1;  // set the distance
                // check all movies the actor perform
                for (int i = 0; i < cur.first->mvVec.size(); i++) {
                    // the distance need to be renew
                    if (Dis < cur.first->mvVec[i]->dis) {
                        cur.first->mvVec[i]->dis = Dis;
                        cur.first->mvVec[i]->prevAct = cur.first;
                        path.push(make_pair(nullptr, cur.first->mvVec[i]));
                    }
                }
            }
        } else if (cur.second != nullptr) {  // cur is a movie
            if (!cur.second->done) {
                cur.second->done = true;
                int Dis = cur.second->dis + 1;  // set the distance
                // check all actors and actresses in the movie
                for (int i = 0; i < cur.second->acVec.size(); i++) {
                    if (Dis < cur.second->acVec[i]->dis) {
                        cur.second->acVec[i]->dis = Dis;
                        cur.second->acVec[i]->prevMov = cur.second;
                        path.push(make_pair(cur.second->acVec[i], nullptr));
                    }
                }
            }
        }
    }
}

/**
 * The method to predict the potential actors and actresses
 * the actor will cooperate with.
 */
void ActorGraph::predictLink(const string& queryActor,
                             vector<string>& predictionNames,
                             unsigned int numPrediction) {
    priority_queue<pair<string, int>, vector<pair<string, int>>, CandidateCP>
        pq;
    // Find the actor pointer from the hash set
    Actor* curAct = this->ActorSet.find(queryActor)->second;
    vector<Movie*> MVVec = curAct->mvVec;

    unordered_map<Actor*, int> ADJAct;
    // check all the movies which the actor performs
    for (int i = 0; i < curAct->mvVec.size(); i++) {
        Movie* curMovie = curAct->mvVec[i];
        // check all the actors and actresses in the movie
        for (int j = 0; j < curMovie->acVec.size(); j++) {
            // the actor has already been checked before
            if (ADJAct.find(curMovie->acVec[j]) != ADJAct.end()) {
                // the num of path between the two actors increases
                ADJAct.find(curMovie->acVec[j])->second =
                    ADJAct.find(curMovie->acVec[j])->second + 1;
            } else {
                // add the actor as an adjacent actor of query actor
                ADJAct.insert(make_pair(curMovie->acVec[j], 1));
            }
        }
    }

    unordered_set<Actor*> Candidate;
    // check all the adjacent neighbors of query actor
    for (auto i = ADJAct.begin(); i != ADJAct.end(); i++) {
        Actor* curA = i->first;
        // check all the movies the actor performs
        for (int j = 0; j < curA->mvVec.size(); j++) {
            Movie* cMov = curA->mvVec[j];
            // check all the actors and actresses in the movie
            for (int k = 0; k < cMov->acVec.size(); k++) {
                // all of these actors and actresses are candidate
                // of the query actor
                Candidate.insert(cMov->acVec[k]);
            }
        }
    }

    // go through all actors and actresses to find candidates
    for (auto i = Candidate.begin(); i != Candidate.end(); i++) {
        // i is not queryActor itself, and there is no edge between queryActor
        // and the current actor or actress
        if ((*i)->name != queryActor && NumOfEdge(curAct, (*i)) == 0) {
            // go through all the actors and actresses adjacent with query
            int sum = 0;
            for (auto k = ADJAct.begin(); k != ADJAct.end(); k++) {
                // Actor k is different from queryActor and Actor i
                if (k->first->name != queryActor &&
                    k->first->name != (*i)->name) {
                    int Equery = 0;
                    int Ecandi = 0;

                    Equery = k->second;

                    // k is connected with queryActor
                    if (Equery != 0) {
                        Ecandi = NumOfEdge(*i, k->first);
                    }
                    // k is connected with candidateActor
                    // k is a common neighbor
                    if (Ecandi != 0) {
                        // the number of edges between query and candidate
                        // through k
                        int OneScore = Equery * Ecandi;
                        sum = sum + OneScore;
                    }
                }
            }
            pair<string, int> pr = make_pair((*i)->name, sum);
            pq.push(pr);
        }
    }

    // there is not enough actors in the priority queue
    if (pq.size() < numPrediction) {
        int len = pq.size();
        // store all the actors and actresses in the priority queue
        for (int i = 0; i < len; i++) {
            predictionNames.push_back(pq.top().first);
            pq.pop();
        }
    } else {
        // store the top "numPrediction" of actors
        for (int i = 0; i < numPrediction; i++) {
            predictionNames.push_back(pq.top().first);
            pq.pop();
        }
    }
}

/**
 * The helper method for predictLink.
 * Find out the number of edge between two actors
 */
int ActorGraph::NumOfEdge(Actor* A, Actor* B) {
    int sum = 0;  // record the number of edge between two actors or actresses
    vector<Movie*> MVVec = A->mvVec;
    // go through all movies A performs
    for (int j = 0; j < MVVec.size(); j++) {
        Movie* curM = MVVec[j];
        vector<Actor*> ACTVec = curM->acVec;
        // go through all actors and actresses in this movie
        for (int k = 0; k < ACTVec.size(); k++) {
            // work with the query actor before
            if (ACTVec[k]->name == B->name) {
                sum++;
            }
        }
    }
    return sum;
}

/**
 * Method to clear the ActorSet and MovieSet
 * Set all done to false, distance to infinite and prev to nullptr.
 */
void ActorGraph::clear() {
    for (auto i = ActorSet.begin(); i != ActorSet.end(); i++) {
        i->second->dis = std::numeric_limits<int>::max();
        i->second->done = false;
        i->second->prevMov = nullptr;
    }

    for (auto i = MovieSet.begin(); i != MovieSet.end(); i++) {
        i->second->dis = std::numeric_limits<int>::max();
        i->second->done = false;
        i->second->prevAct = nullptr;
    }
}
/* The destructor of ActorGraph */
ActorGraph::~ActorGraph() {
    // clear the ActorSet
    for (auto i = ActorSet.begin(); i != ActorSet.end(); i++) {
        delete i->second;
    }
    // clear the MovieSet
    for (auto i = MovieSet.begin(); i != MovieSet.end(); i++) {
        delete i->second;
    }
}

/**
 * The method to build a graph for self testers
 */
void ActorGraph::buildGraphTest() {
    ofstream outFile;
    outFile.open("testFile.txt");
    outFile << "Actor/Actress	Movie	Year\n";
    outFile << "Kevin Bacon	X-Men: First Class	2011\n";
    outFile << "James McAvoy	X-Men: First Class	2011\n";
    outFile << "James McAvoy	X-Men: Apocalypse	2016\n";
    outFile << "James McAvoy	Glass	2019\n";
    outFile << "Michael Fassbender	X-Men: First Class	2011\n";
    outFile << "Michael Fassbender	X-Men: Apocalypse	2016\n";
    outFile << "Michael Fassbender	Alien: Covenant	2017\n";
    outFile << "Samuel L. Jackson	Glass	2019\n";
    outFile << "Samuel L. Jackson	Avengers: Endgame	2019\n";
    outFile << "Robert Downey Jr.	Avengers: Endgame	2019\n";
    outFile << "Robert Downey Jr.	Spider-Man: Homecoming	2017\n";
    outFile << "Tom Holland	Spider-Man: Homecoming	2017\n";
    outFile << "Tom Holland	The Current War	2017\n";
    outFile << "Katherine Waterston	Alien: Covenant	2017\n";
    outFile << "Katherine Waterston	The Current War	2017";
    outFile.close();

    buildGraphFromFile("testFile.txt");
}
