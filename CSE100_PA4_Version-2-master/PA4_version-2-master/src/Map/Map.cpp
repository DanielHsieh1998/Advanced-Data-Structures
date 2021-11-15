/*
 * Authors: Yilan Jiang & Hongsheng Xie
 * Emails: yij007@ucsd.edu & hox012@ucsd.edu
 * This files implement the class Map and the sub class DisjointSet.
 * It build a graph with vertices and edges.
 * Each vertex has a vector of edges, and each edge has two vertices.
 * It also implements methods which will be used to find spanning tree,
 * shortest path, and crucial path.
 */
#include "Map.hpp"
#include <queue>
#include <stack>
#include <vector>
/*
 * Constructor for Map
 */
Map::Map() {}
/**
 * return the index of the sentinel of the
 * input index
 * */
int Map::DisjointSet::find(int index) {
    int curr = arr[index];
    int returnIndex = index;
    // Implement path_compression for optimization
    vector<int> path_compression;
    // find the index of the input's sentinel
    while (curr >= 0) {
        path_compression.push_back(returnIndex);
        returnIndex = curr;
        curr = arr[curr];
    }
    // connect all the pass-by element directly to the sentinel
    for (int i = 0; i < path_compression.size(); i++) {
        arr[path_compression[i]] = returnIndex;
    }

    return returnIndex;
}

/**
 * union method of DisjointSet Data Structure
 * if the input two element pointing to different
 * sentinel, then union the smaller one to the bigger
 * one. If they have already pointed to the same sentinel,
 * then it will do nothing.
 * Return true if two groups are merged together.
 * Return false if inputs are in the same group.
 * */
bool Map::DisjointSet::merge(int index1, int index2) {
    // get the sentinel of vertex1
    int sentinel1 = find(index1);
    // get the sentinel of vertex2
    int sentinel2 = find(index2);
    // check if they have the same sentinel
    if (sentinel1 != sentinel2) {
        // get the sentinel1's location
        int size1 = arr[sentinel1];
        // get the sentinel2's location
        int size2 = arr[sentinel2];
        // all up the number of vertices
        int sum = size1 + size2;
        // check which group contains larger
        // number of elements
        if (abs(size1) > abs(size2)) {
            arr[sentinel2] = sentinel1;
            arr[sentinel1] = sum;
        } else {
            arr[sentinel1] = sentinel2;
            arr[sentinel2] = sum;
        }
        return true;
        // if so, then no need to union
    } else {
        return false;
    }
}

/* Build the map graph from vertex and edge files */
bool Map::buildMapFromFile(const string& vertexFileName,
                           const string& edgeFileName) {
    // add vertices first
    ifstream vertexFile(vertexFileName);
    while (vertexFile) {
        string s;
        if (!getline(vertexFile, s)) break;

        // process data at each line
        istringstream ss(s);
        vector<string> data;
        while (ss) {
            string str;
            if (!getline(ss, str, ' ')) break;
            data.push_back(str);
        }
        if (data.size() != 3) continue;

        // add vertex defined in this line to the graph
        string name(data[0]);
        float x = stoi(data[1]);
        float y = stoi(data[2]);

        addVertex(name, x, y);
    }

    // then add edges
    ifstream edgeFile(edgeFileName);
    while (edgeFile) {
        string s;
        if (!getline(edgeFile, s)) break;

        // process data at each line
        istringstream ss(s);
        vector<string> data;
        while (ss) {
            string str;
            if (!getline(ss, str, ' ')) break;
            data.push_back(str);
        }
        if (data.size() != 2) continue;

        // add edge defined in this line to the graph
        string name1(data[0]);
        string name2(data[1]);

        addEdge(name1, name2);
    }

    return true;
}

/*
 * Add a vertex with name and x, y coordinates to the map graph. Returns
 * false if the name already existed in the map graph, and true otherwise
 */
bool Map::addVertex(const string& name, float x, float y) {
    if (vertexId.count(name) > 0) return false;
    vertexId[name] = vertices.size();
    vertices.push_back(new Vertex(name, x, y));
    return true;
}

/*
 * Add an undirected edge between vertices with names "name1" and "name2".
 * Returns false if either name is not in the map graph.
 */
bool Map::addEdge(const string& name1, const string& name2) {
    if (vertexId.count(name1) == 0 || vertexId.count(name2) == 0) {
        return false;
    }
    unsigned int id1 = vertexId[name1];
    unsigned int id2 = vertexId[name2];
    Vertex* v1 = vertices[id1];
    Vertex* v2 = vertices[id2];
    float weight = sqrt(pow(v1->x - v2->x, 2) + pow(v1->y - v2->y, 2));
    v1->outEdges.push_back(new Edge(v1, v2, weight));
    v2->outEdges.push_back(new Edge(v2, v1, weight));

    undirectedEdges.push_back(new Edge(v1, v2, weight));
    return true;
}

/* Given a pair of different location names,
find the weighted shortest path from one vertex to the other */
void Map::Dijkstra(const string& from, const string& to,
                   vector<Vertex*>& shortestPath) {
    // check if from or to vertex does not exist in the map
    if (vertexId.count(from) == 0 || vertexId.count(to) == 0) {
        return;
    }
    // create a priority_queue for Dijkstra algorithm
    priority_queue<Vertex*, vector<Vertex*>, PQcompare> pq;
    // locate the from vertex in the map
    unsigned int index = vertexId[from];
    Vertex* beginVertex = vertices[index];
    // push the from vertex into pq
    pq.push(beginVertex);
    // set the dis of the from vertex to zero
    beginVertex->dis = 0;

    while (!pq.empty()) {
        // dequeue the top element from pq
        Vertex* currVertex = pq.top();
        pq.pop();
        // check if the current Vertex is done
        if (currVertex->done == false) {
            // if not, set done to true
            currVertex->done = true;
            // get the edges connecting to the current vertex
            vector<Edge*> currEdgeVector = currVertex->outEdges;
            // loop through all edges
            for (int i = 0; i < currEdgeVector.size(); i++) {
                int currDist = currVertex->dis + currEdgeVector[i]->weight;
                Vertex* neighborVertex = nullptr;
                // check whether the currentVertex is the source
                // of the current Edge
                if (currVertex == currEdgeVector[i]->source) {
                    neighborVertex = currEdgeVector[i]->target;
                }
                // check whether the currentVertex is the target
                // of the current Edge
                if (currVertex == currEdgeVector[i]->target) {
                    neighborVertex = currEdgeVector[i]->source;
                }
                // check if the dist is less than the current distance
                if (currDist < neighborVertex->dis) {
                    neighborVertex->pre = currVertex;
                    neighborVertex->dis = currDist;
                    pq.push(neighborVertex);
                }
            }
        }
    }
    // get the to vertex
    unsigned int index_to = vertexId[to];
    Vertex* endVertex = vertices[index_to];
    stack<Vertex*> outputStack;

    Vertex* currentVertex = endVertex;
    // collect all pre vertices starting from end vertex
    while (currentVertex != beginVertex) {
        outputStack.push(currentVertex);
        currentVertex = currentVertex->pre;
    }
    outputStack.push(currentVertex);
    // collect all vertices starting from the begin vertex
    while (!outputStack.empty()) {
        shortestPath.push_back(outputStack.top());
        outputStack.pop();
    }
    clear();
}

/* helper method to clear all vertices in a map as unvisited*/
void Map::clear() {
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i]->dis = std::numeric_limits<float>::max();
        vertices[i]->done = false;
        vertices[i]->pre = nullptr;
    }
}
/* Find all the undirected edges in the minimum spanning tree
of the map graph. */
void Map::findMST(vector<Edge*>& MST) {
    // create a priority_queue to sort edges by their weights
    priority_queue<Edge*, vector<Edge*>, PQEdgeCP> pq;
    // push all edges into the pq
    for (int i = 0; i < undirectedEdges.size(); i++) {
        pq.push(undirectedEdges[i]);
    }
    // create a disjointSet object
    this->disjoint_set = new DisjointSet(vertices.size());
    Edge* currEdge = nullptr;
    int index1 = 0;
    int index2 = 0;
    // check if two vertices were in two different sets
    bool mergeSignal = false;
    while (!pq.empty()) {
        // dequeue the top edge from pq
        currEdge = pq.top();
        pq.pop();
        // get the index of the source vertex of the current edge
        index1 = vertexId[currEdge->source->name];
        // get the index of the targe vertex of the current edge
        index2 = vertexId[currEdge->target->name];
        // merge two vertices
        mergeSignal = this->disjoint_set->merge(index1, index2);
        // if merge successfully, then push the edge into the MST vector
        if (mergeSignal == true) {
            MST.push_back(currEdge);
        }
    }
    // delete the disjoint_set object
    delete this->disjoint_set;
}

/* Find all the crucial roads in the map graph */
void Map::crucialRoads(vector<Edge*>& roads) {
    // create a disjointSet object
    this->disjoint_set = new DisjointSet(vertices.size());
    // loop through all edges in the map
    for (int i = 0; i < undirectedEdges.size(); i++) {
        Edge* testEdge = undirectedEdges[i];
        // clear the arr in disjointSet each time after one loop through
        for (int k = 0; k < vertices.size(); k++) {
            this->disjoint_set->arr[k] = -1;
        }
        // initialize the number of sentinels
        int numSentinel = this->vertices.size();
        // loop through the edges in the map again
        for (int j = 0; j < undirectedEdges.size(); j++) {
            // rule out the current testEdge
            if (i != j) {
                // union all other edges
                Edge* curEdge = undirectedEdges[j];
                int index1 = vertexId[curEdge->source->name];
                int index2 = vertexId[curEdge->target->name];
                bool mergeSignal = this->disjoint_set->merge(index1, index2);
                // update the number of sentinels
                if (mergeSignal) {
                    numSentinel--;
                }
            }
        }
        // if the numSentinel is greater than 1, then the
        // testEdge is a crucial road as it is not in a cycle
        if (numSentinel > 1) {
            roads.push_back(testEdge);
        }
    }
    // delete the disjoint_set object
    delete this->disjoint_set;
}

/* Destructor of Map graph */
Map::~Map() {
    // delete all vertices
    for (Vertex* v : vertices) {
        for (Edge* e : v->outEdges) {
            delete e;
        }
        delete v;
    }
    // delete all edges
    for (Edge* e : undirectedEdges) {
        delete e;
    }
}

/*Destructor for DisjointSet*/
Map::DisjointSet::~DisjointSet() { delete[] this->arr; }

/* create files for testing purpose*/
void Map::buildMapTest() {
    ofstream outFile;
    outFile.open("mapTestVertex.txt");
    outFile << "GalbraithHall 600 4750\n";
    outFile << "OslerParking 1750 4700\n";
    outFile << "Revelle 250 4400\n";
    outFile << "YorkHall 800 4500\n";
    outFile << "BioMedLib 1750 4250\n";
    outFile << "MainGym 500 3550\n";
    outFile << "Mandeville 1000 3300\n";
    outFile << "CenterHall 1750 3250\n";
    outFile << "GilmanParking 2700 3400\n";
    outFile << "PepperCanyonHall 2600 2850\n";
    outFile << "UniversityCenter 2200 3300\n";
    outFile << "Sixth 3600 3100\n";
    outFile << "MAE 2900 2600\n";
    outFile << "Muir 200 3000\n";
    outFile << "APM 550 2950\n";
    outFile << "PetersonHall 750 2550\n";
    outFile << "PriceCenter 1750 2750\n";
    outFile << "SolisHall 950 2250\n";
    outFile << "Geisel 1600 2200\n";
    outFile << "WarrenLectureHall 2500 2350\n";
    outFile << "Jacobs 2250 2000\n";
    outFile << "Marshall 500 2000\n";
    outFile << "CSE 2700 2000\n";
    outFile << "OceanView 100 1500\n";
    outFile << "Warren 2900 1600\n";
    outFile << "PangeaParking 0 1100\n";
    outFile << "HopkinsParking 1100 1250\n";
    outFile << "ERC 0 750\n";
    outFile << "RIMAC 1000 750\n";
    outFile << "SupercomputerCenter 1000 1050\n";
    outFile << "Rady 250 250";
    outFile.close();

    ofstream outFile1;
    outFile1.open("mapTestEdge.txt");
    outFile1
        << "GalbraithHall Revelle\nGalbraithHall YorkHall\nGalbraithHall "
           "MainGym\nOslerParking BioMedLib\nRevelle YorkHall\nRevelle "
           "MainGym\nYorkHall BioMedLib\nBioMedLib CenterHall\nMainGym "
           "Mandeville\nMainGym Muir\nMainGym APM\nMainGym YorkHall\nMuir "
           "APM\nMandeville CenterHall\nMandeville APM\nMandeville "
           "Muir\nCenterHall PriceCenter\nCenterHall "
           "UniversityCenter\nGilmanParking PepperCanyonHall\nGilmanParking "
           "Sixth\nPepperCanyonHall WarrenLectureHall\nPepperCanyonHall "
           "PriceCenter\nPepperCanyonHall MAE\nUniversityCenter "
           "GilmanParking\nUniversityCenter PepperCanyonHall\nUniversityCenter "
           "BioMedLib\nSixth MAE\nMAE PriceCenter\nMAE WarrenLectureHall\nMAE "
           "CSE\nAPM PetersonHall\nAPM "
           "PriceCenter\nPetersonHallGeisel\nPetersonHall "
           "PriceCenter\nPetersonHall SolisHall\nPetersonHall "
           "Marshall\nPriceCenter Geisel\nPriceCenter "
           "WarrenLectureHall\nPriceCenter SolisHall\nPriceCenter "
           "UniversityCenter\nPriceCenter Mandeville\nSolisHall "
           "Marshall\nSolisHall Geisel\nSolisHall HopkinsParking\nGeisel "
           "HopkinsParking\nGeisel WarrenLectureHall\nGeisel Jacobs\nMarshall "
           "HopkinsParking\nWarrenLectureHall CSE\nJacobs CSE\nJacobs "
           "WarrenLectureHall\nJacobs PriceCenter\nJacobs "
           "HopkinsParking\nMarshall OceanView\nCSE Warren\nOceanView "
           "PangeaParking\nPangeaParking HopkinsParking\nPangeaParking "
           "ERC\nPangeaParking SupercomputerCenter\nHopkinsParking "
           "SupercomputerCenter\nSupercomputerCenter RIMAC\nERC RIMAC\nERC "
           "Rady\nRIMAC Rady";
    outFile1.close();
    // build the map graph based on these two files
    buildMapFromFile("mapTestVertex.txt", "mapTestEdge.txt");
}