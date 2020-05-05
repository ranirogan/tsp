/**
 * Performs the nearest neighbor algorithm as the trivial solution
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

template <typename T>
vector<T> NN<T>::getPath() {
    vector<T> path;
    auto iter = this->nodes.begin();
    if(iter == this->nodes.end()) {
        cout << "error in graph";
    }
    vector<T> toLookAt;
    for(iter = this->nodes.begin(); iter != this->nodes.end(); iter++){
        toLookAt.push_back(iter->first);
    }
    bool isDone = false;
    T cur = this->nodes.begin()->first;
    T start = cur;
    path.push_back(cur);
    isDone = !eraseInVec(cur, toLookAt);
    while(!isDone){
        T nextN = findNextNeighbor(cur, toLookAt);
        if(nextN == cur)
            break;
        path.push_back(nextN);
        cur = nextN;
        eraseInVec(cur, toLookAt);
        isDone = toLookAt.size() == 0;
    }
    path.push_back(start);
    return path;
}

template <typename T>
bool NN<T>::eraseInVec(T toErase, vector<T>& vec) {
    auto iter = find(vec.begin(), vec.end(), toErase);
    if(iter == vec.end())
        return false;
    vec.erase(iter);
    return true;
}

template <typename T>
T NN<T>::findNextNeighbor(T cur, vector<T> toLookAt) {
    vector<weightEdge<T>> wgts = this->weights;
    auto iter = this->nodes.find(cur);
    vector<T> edges = iter->second.edges;
    vector<weightEdge<T>> options;
    for(int i = 0; i < edges.size(); i++){
        auto fnd = find(toLookAt.begin(), toLookAt.end(), edges[i]);
        if(fnd != toLookAt.end()) {
            weightEdge<T> wE = findWeights(cur, edges[i], wgts);
            options.push_back(wE);
        }
    }
    std::sort(options.begin(), options.end(), customFunc<T>());
    if(options.size() != 0){
        if(cur == options[0].from)
            return options[0].to;
        return options[0].from;
    }
}


template <typename T>
weightEdge<T> NN<T>::findWeights(T from, T to, vector<weightEdge<T>> vec) {
    for(int i = 0; i < vec.size(); i++){
        weightEdge<T> wE = vec[i];
        if(wE.to == from || wE.to == to){
            if(wE.from == from || wE.from == to){
                return wE;
            }
        }
    }
}
#endif //TSP_NN_H
