/**
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This files contains the method headers for
 * all implementations written in Map.cpp.
 */

#ifndef MAP_HPP
#define MAP_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include "Edge.hpp"

using namespace std;
/**
 * The class for a MAP graph, implemented with a vector of vertices, a vector of
 * edges, a hash map and several methods to find out some specific pathes.
 */
class Map {
  private:
    // vector storing vertices in the map: id of each vertex = index in vector
    vector<Vertex*> vertices;

    // Map: name of vertex -> id of vertex = index storing vertex ptr
    unordered_map<string, unsigned int> vertexId;

    // Directed edge in vector represents an undirected edge used in MST
    vector<Edge*> undirectedEdges;

    /*
     * Add a vertex with name and x, y coordinates to the map graph. Returns
     * false if the name already existed in the map graph, and true otherwise
     */
    bool addVertex(const string& name, float x, float y);

    void clear();
    /*
     * Add an undirected edge between vertices with names "name1" and "name2".
     * Returns false if either name is not in the map graph.
     */
    bool addEdge(const string& name1, const string& name2);

  public:
    /**
     * The class for a DisjointSet, implemented as an array
     */
    class DisjointSet {
      public:
        int* arr;

        /**
         * The constuctor for DisjointSet
         */
        DisjointSet(int size) {
            arr = new int[size];
            for (int i = 0; i < size; i++) {
                arr[i] = -1;
            }
        };

        /**
         * Destructor
         */
        ~DisjointSet();

        /**
         * The method to find an elemnt with index
         * Return the index of sentinal of the element with the index
         */
        int find(int index);

        /**
         * The method to merge to elements with the indexes
         * After merging, the two elements will have the same sentinal
         */
        bool merge(int index1, int index2);
    };

    DisjointSet* disjoint_set = nullptr;

    /* The constructor of Map */
    Map();

    /* Build the map graph from vertex and edge files */
    bool buildMapFromFile(const string& vertexFileName,
                          const string& edgeFileName);

    /* The method to find the weighted shortest path from one vertex to the
     * other */
    void Dijkstra(const string& from, const string& to,
                  vector<Vertex*>& shortestPath);

    /* The method to find the shortest spanning tree of a weighted graph */
    void findMST(vector<Edge*>& MST);

    /* The method to find all the crucial edges in a graph */
    void crucialRoads(vector<Edge*>& roads);

    /* The method to build a Map graph for self testers */
    void buildMapTest();

    /* The struct used for the compare operation in priority queue */
    struct PQcompare {
        bool operator()(Vertex* A, Vertex* B) const {
            if (A->dis != B->dis) {
                return (A->dis > B->dis);
            } else {
                return (A->name < B->name);
            }
        }
    };

    /* The struct used for the compare operation in priority queue */
    struct PQEdgeCP {
        bool operator()(Edge* A, Edge* B) const {
            return (A->weight > B->weight);
        }
    };
    /* Destructor of Map graph */
    ~Map();
};

#endif  // Map_HPP