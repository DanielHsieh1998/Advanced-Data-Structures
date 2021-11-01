/**
 * CSE 100 PA4 some graph algorithms in map graph
 */
#include "Map.hpp"
#include <cxxopts.hpp>
#include <fstream>

using namespace std;

/* Print the usage of the program */
void usage(char* program_name) {
    cerr << program_name << " called with incorrect arguments." << endl;
    cerr << "Usage: " << program_name << " movie_cast.txt outputfile" << endl;
}

int main(int argc, char* argv[]) {
    cxxopts::Options options("./map",
                             "Run different graph algorithms on a map");
    options.positional_help(
        "./vertex_file ./edge_file [./pairs_file] ./out_file");

    bool isShortestPath = false;
    bool isMST = false;
    bool isCrucialRoads = false;
    string arg1, arg2, arg3, arg4;
    options.allow_unrecognised_options().add_options()(
        "sp", "find the shortest path between pairs of locations",
        cxxopts::value<bool>(isShortestPath))(
        "mst", "find the minimum spanning tree in the map",
        cxxopts::value<bool>(isMST))(
        "cr", "find the roads that are not contained in any cycle",
        cxxopts::value<bool>(isCrucialRoads))("arg1", "",
                                              cxxopts::value<string>(arg1))(
        "arg2", "", cxxopts::value<string>(arg2))("arg3", "",
                                                  cxxopts::value<string>(arg3))(
        "arg4", "", cxxopts::value<string>(arg4))("h,help",
                                                  "Print help and exit");

    options.parse_positional({"arg1", "arg2", "arg3", "arg4"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help")) {
        cout << options.help({""}) << endl;
        return 0;
    }

    Map* map = new Map();
    map->buildMapFromFile(arg1, arg2);

    if (isShortestPath) {
        ifstream pairsFile(arg3);
        ofstream outFile(arg4);
        while (pairsFile) {
            string s;
            if (!getline(pairsFile, s)) break;

            istringstream ss(s);
            vector<string> data;
            while (ss) {
                string str;
                if (!getline(ss, str, ' ')) break;
                data.push_back(str);
            }
            if (data.size() != 2) continue;

            vector<Vertex*> shortestPath;
            map->Dijkstra(data[0], data[1], shortestPath);

            // if no shortest path is found, continue to next pair
            if (shortestPath.size() == 0) {
                outFile << endl;
                continue;
            }

            // output the shortest path
            unsigned int len = shortestPath.size();
            for (unsigned int i = 0; i < len - 1; i++) {
                outFile << shortestPath[i]->name << " - ";
            }
            outFile << shortestPath[len - 1]->name << endl;
        }
        outFile.close();
    } else if (isMST) {
        ofstream outFile(arg3);
        vector<Edge*> mst;
        map->findMST(mst);
        float totalWeight = 0;
        for (Edge* e : mst) {
            outFile << e->source->name << " - " << e->target->name << endl;
            totalWeight += e->weight;
        }
        cout << "Number of edges in MST: " << mst.size() << endl;
        cout << "Total weight of MST: " << totalWeight << endl;
        outFile.close();
    } else if (isCrucialRoads) {
        ofstream outFile(arg3);
        vector<Edge*> roads;
        map->crucialRoads(roads);
        vector<string> roadsStr;
        for (Edge* e : roads) {
            roadsStr.push_back(e->source->name + " - " + e->target->name);
        }

        // sort the roads to produce unique outputs
        sort(roadsStr.begin(), roadsStr.end());
        for (string& str : roadsStr) {
            outFile << str << endl;
        }
        outFile.close();
    }
    delete map;
    return 0;
}
