/**
 * Performs the nearest neighbor algorithm as the trivial solution
 * Similar to Prim's algorithm, it starts at a node and finds the shortest connection to an unvisted node
 * At the end, the path connects back to the start
 */

#ifndef TSP_NN_H
#define TSP_NN_H

#include "Graph.h"
#include "Set.h"
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;
template <typename T>
class NN : public Graph<T>{
public:
    vector<T> getPath();
private:
    bool eraseInVec(T toErase, vector<T>& vec);
    T findNextNeighbor(T cur, vector<T> toLookAt);
    weightEdge<T> findWeights(T from, T to, vector<weightEdge<T>> vec);
};

/**
 * Finds a path that connects all the nodes in a graph
 * @tparam T is the type of the graph
 * @return a vector of T objects representing the path
 */
template <typename T>
vector<T> NN<T>::getPath() {
    vector<T> path;
    auto iter = this->nodes.begin();
    if(iter == this->nodes.end()) {
        cout << "error in graph";
    }
    vector<T> toLookAt;
    // set all nodes to unvisited
    for(iter = this->nodes.begin(); iter != this->nodes.end(); iter++){
        toLookAt.push_back(iter->first);
    }
    bool isDone = false;
    T cur = this->nodes.begin()->first;
    // the path changes according to the starting node
//    T cur = this->nodes.find("15")->first;
    T start = cur;
    path.push_back(cur);
    isDone = !eraseInVec(cur, toLookAt);
    // while there are still unvisited nodes
    while(!isDone){
        T nextN = findNextNeighbor(cur, toLookAt);
        if(nextN == cur) // if there was a problem finding the next node, quit
            break;
        path.push_back(nextN);
        cur = nextN;
        eraseInVec(cur, toLookAt); // set this node to visited
        isDone = toLookAt.size() == 0;
    }
    path.push_back(start); // must connect back to start
    return path;
}


/**
 * Erases an element in a vector
 * @tparam T is the type of the graph
 * @param toErase is the element to erase
 * @param vec is the vector to erase an element from
 * @return false if could not erase the element
 */
template <typename T>
bool NN<T>::eraseInVec(T toErase, vector<T>& vec) {
    auto iter = find(vec.begin(), vec.end(), toErase);
    if(iter == vec.end())
        return false;
    vec.erase(iter);
    return true;
}

/**
 * Finds the the shortest connecting edge from the currect node to an unvisited node
 * @tparam T is the type of the graph
 * @param cur is the node being looked at
 * @param toLookAt is a vector of the unvisited nodes
 * @return the name of the node that has the shortest connecting path between it and the current node
 */
template <typename T>
T NN<T>::findNextNeighbor(T cur, vector<T> toLookAt) {
    vector<weightEdge<T>> wgts = this->weights;
    auto iter = this->nodes.find(cur);
    vector<T> edges = iter->second.edges;
    vector<weightEdge<T>> options;
    //adds all the edges between the current node and a node that's unvisited to a vector
    for(int i = 0; i < edges.size(); i++){
        auto fnd = find(toLookAt.begin(), toLookAt.end(), edges[i]);
        if(fnd != toLookAt.end()) {
            weightEdge<T> wE = findWeights(cur, edges[i], wgts);
            options.push_back(wE);
        }
    }
    // sorts the vector by the weight of the paths such that the shortest path is first
    std::sort(options.begin(), options.end(), customFunc<T>());
    if(options.size() != 0){
        if(cur == options[0].from) //returns the value that is not the current node
            return options[0].to;
        return options[0].from;
    }
}


/**
 * Finds the pair of nodes in a specified vector of weight objects
 * @tparam T is the type of the graph
 * @param from is the start node
 * @param to is the destination node
 * @param vec is a vector of weighted edges to find the connecting edge in
 * @return the weighted edge with the same pair of cities
 */
template <typename T>
weightEdge<T> NN<T>::findWeights(T from, T to, vector<weightEdge<T>> vec) {
    for(int i = 0; i < vec.size(); i++){
        weightEdge<T> wE = vec[i];
        if(wE.to == from || wE.to == to){ //undirected
            if(wE.from == from || wE.from == to){
                return wE;
            }
        }
    }
}
#endif //TSP_NN_H
