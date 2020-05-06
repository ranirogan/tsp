/**
 * Implements the Christofides algorithm
 * The solution is not dependent on the starting node and is guaranteed to be within
 * 3/2 length of the optimal path
 */

#ifndef TSP_CHRIS_H
#define TSP_CHRIS_H

#include "Graph.h"
#include "Set.h"
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <climits>
#include <stack>

using namespace std;

template <typename T>
class Chris : public Graph<T>{
public:
    vector<T> getPath();
    vector<T> calcOddEdges();
    vector<weightEdge<T>> perfectMatch(vector<T> odds, vector<weightEdge<T>> weights);
    void merge(vector<weightEdge<T>> edges);
    vector<T> euler();
    vector<T> hamilton(vector<T> ePath);
private:
    weightEdge<T> findInWeights(T from, T to, vector<weightEdge<T>> weights);
};

/**
 * finds a path between all the nodes in the graph
 * @tparam T is the type of the graph
 * @return a vector of T objects representing the path
 */
template <typename T>
vector<T> Chris<T>::getPath() {
    // find the minimum spanning tree
    vector<weightEdge<T>> vec = this->getMinSpan();
    auto* subGraph = new Chris<T>();
    // create a new graph out of the spanning tree
    for(int i = 0; i < vec.size(); i++){
        subGraph->addNode(vec[i].from);
        subGraph->addNode(vec[i].to);
        subGraph->addEdge(vec[i].from, vec[i].to, vec[i].weight);
    }
    // find the vertices with odd degrees
    vector<T> odds = subGraph->calcOddEdges();
    // perform minimum-weight perfect matching on the odd vertices
    vector<weightEdge<T>> perf = perfectMatch(odds, subGraph->weights);
    // merge the perfect matches into the minimum spanning tree
    subGraph->merge(perf);
    // find the eulerian path
    vector<T> ePath = subGraph->euler();
    // from the eulerian path, find the hamiltonian path and return it
    return subGraph->hamilton(ePath);
}

/**
 * Calculates which vertices have odd edges
 * @tparam T is the type of the graph
 * @return a vector containing all the vertices with odd edges
 */
template <typename T>
vector<T> Chris<T>::calcOddEdges() {
    vector<T> toReturn;
    for(auto iter = this->nodes.begin(); iter != this->nodes.end(); iter++){
        vector<T> edges = iter->second.edges;
        if(edges.size() % 2  == 1){
            toReturn.push_back(iter->first);
        }
    }
    return toReturn;
}

/**
 * matches each vertices with an odd degree to another that has the smallest weight possible
 * @tparam T is the type of the graph
 * @param odds is the vector that contains all the odd edges to be matched
 * @param weights is the edges in the minimum spanning tree
 * @return a vector of the weighted edges that represent the matches
 */
template <typename T>
vector<weightEdge<T>> Chris<T>::perfectMatch(vector<T> odds, vector<weightEdge<T>> weights) {
    vector<weightEdge<T>> toReturn;
    while(!odds.empty()){
        T cur = *odds.begin();
        //finds a vertex with an odd degree in the graph
        auto mapFind = this->nodes.find(cur);
        weightEdge<T> temp;
        if(mapFind != this->nodes.end()){
            vector<T> edges = mapFind->second.edges;
            int curWeight = INT_MAX;
            // searches through all the connections that the current vertex has
            for(int i = 0; i < edges.size(); i++){
                auto fnd = find(odds.begin(), odds.end(), edges[i]);
                if(fnd != odds.end()){
                    // finds the weighted edge representing the pair of nodes
                    weightEdge<T> wE = findInWeights(cur, edges[i], this->weights);
                    // attempts to find the edge in the minimum spanning tree
                    weightEdge<T> test = findInWeights(cur, edges[i], weights);
                    // if the edge is in the minimum spanning tree, don't add it
                    if(test.weight == -1) {
                        if (wE.weight < curWeight) {
                            curWeight = wE.weight;
                            temp = wE;
                        }
                    }
                }
            }
            if(curWeight < INT_MAX){ // adds the best match to the vector
                toReturn.push_back(temp);
                // erases both nodes in the odds vector so they aren't used again
                auto loc = find(odds.begin(), odds.end(), temp.to);
                odds.erase(loc);
                loc = find(odds.begin(), odds.end(), temp.from);
                odds.erase(loc);
            }
        }
    }
    return toReturn;
}

/**
 * Finds the weighted edge representing a pair of nodes in the given vector of weighted edges
 * @tparam T is the type of the graph
 * @param from is the source city
 * @param to is the destination city
 * @param weights is the vector of weighted edges to find the pair of nodes in
 * @return the weighted edge between the two nodes
 */
template <typename T>
weightEdge<T> Chris<T>::findInWeights(T from, T to, vector<weightEdge<T>> weights) {
    for(int i = 0; i < weights.size(); i++){
        weightEdge<T> wE = weights[i];
        if(wE.to == from || wE.to == to){ //undirected
            if(wE.from == from || wE.from == to){
                return wE;
            }
        }
    }
    weightEdge<T> def;
    def.weight = -1;
    return def;
}


/**
 * Takes in a vector of weighted edges and adds them into the graph
 * @tparam T is the type of the graph
 * @param edges is a vector of weighted edges to be added into the graph
 */
template <typename T>
void Chris<T>::merge(vector<weightEdge<T>> edges) {
    for(int i = 0; i < edges.size(); i++){
        this->addEdge(edges[i].from, edges[i].to, edges[i].weight);
    }
}

/**
 * Finds the euler circuit in a graph
 * @tparam T is the type of the graph
 * @return a vector representing the path
 */
template <typename T>
vector<T> Chris<T>::euler() {
    vector<T> path;
    vector<weightEdge<T>> toIgnore; // contains a list of edges that have already been visited
    auto iter = this->nodes.begin();
    stack<T> stk;
    stk.push(iter->first);
    T cur = stk.top();
    while(!stk.empty()){
        iter = this->nodes.find(cur);
        vector<T> edges = iter->second.edges;
        bool hasConnect = false;
        int index = -1;
        // attempts to find a connection that has not been used yet
        for(int i = 0; i < edges.size(); i++) {
            T con = edges[i];
            bool isValid = true;
            for(int j = 0; j < toIgnore.size(); j++){
                if(con == toIgnore[j].to || con == toIgnore[j].from) { //undirected
                    if (cur == toIgnore[j].to || cur == toIgnore[j].from){
                        isValid = false;
                        break;
                    }
                }
            }
            hasConnect = hasConnect || isValid;
            if(hasConnect) { // uses the first available connection
                index = i;
                break;
            }
        }
        if(!hasConnect){ // if there are no more connections, pop it off the stack
            path.push_back(cur);
            cur = stk.top();
            stk.pop();
        }
        else{ // visit the edge and push the destination node onto the stack
            weightEdge<T> wE;
            wE.from = cur;
            wE.to = edges[index];
            toIgnore.push_back(wE);
            stk.push(cur);
            cur = edges[index];
        }
    }
    return path;
}

/**
 * Takes in a eulerian path and ignores repeated vertices to create a hamiltonian path
 * @tparam T is the type of the graph
 * @param ePath is a vector of T objects representing the eulerian path
 * @return a vector of T objects representing the hamiltonian path
 */
template <typename T>
vector<T> Chris<T>::hamilton(vector<T> ePath) {
    vector<T> toIgnore;
    vector<T> hamilton;
    auto iter = ePath.begin();
    T start = *iter;
    while(iter != ePath.end()){
        T cur = *iter;
        auto fnd = find(toIgnore.begin(), toIgnore.end(), cur);
        if(fnd == toIgnore.end()){ // only use vertices that haven't been visited
            hamilton.push_back(cur);
            toIgnore.push_back(cur);
            iter++;
        }
        else {
            iter = ePath.erase(iter); // ignores repeated vertices
        }
    }
    hamilton.push_back(start);
    return hamilton;
}
#endif //TSP_CHRIS_H
