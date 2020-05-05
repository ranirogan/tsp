/**
 * Implements the Christofides algorithm
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
    vector<weightEdge<T>> perfectMatch(vector<T> odds);
    void merge(vector<weightEdge<T>> edges);
    vector<T> euler();
    vector<T> hamilton(vector<T> ePath);
private:
    weightEdge<T> findInWeights(T from, T to);
};

template <typename T>
vector<T> Chris<T>::getPath() {
    vector<weightEdge<T>> vec = this->getMinSpan();
    auto* subGraph = new Chris<T>();
    for(int i = 0; i < vec.size(); i++){
        subGraph->addNode(vec[i].from);
        subGraph->addNode(vec[i].to);
        subGraph->addEdge(vec[i].from, vec[i].to, vec[i].weight);
    }
    vector<T> odds = subGraph->calcOddEdges();
    vector<weightEdge<T>> perf = perfectMatch(odds);
    subGraph->merge(perf);
    vector<T> ePath = subGraph->euler();
    for(int i = 0; i < ePath.size(); i++)
        cout << ePath[i] << endl;
    return subGraph->hamilton(ePath);
}

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

template <typename T>
vector<weightEdge<T>> Chris<T>::perfectMatch(vector<T> odds) {
    vector<weightEdge<T>> toReturn;
    while(!odds.empty()){
        auto loop = odds.begin();
        T cur = *loop;
        auto mapFind = this->nodes.find(cur);
        weightEdge<T> temp;
        if(mapFind != this->nodes.end()){
            vector<T> edges = mapFind->second.edges;
            int curWeight = INT_MAX;
            for(int i = 0; i < edges.size(); i++){
                auto fnd = find(odds.begin(), odds.end(), edges[i]);
                if(fnd != odds.end()){
                    weightEdge<T> wE = findInWeights(cur, edges[i]);
                    if(wE.weight < curWeight){
                        curWeight = wE.weight;
                        temp = wE;
                    }
                }
            }
            if(curWeight < INT_MAX){
                toReturn.push_back(temp);
                auto loc = find(odds.begin(), odds.end(), temp.to);
                odds.erase(loc);
                loc = find(odds.begin(), odds.end(), temp.from);
                odds.erase(loc);
            }
        }
    }
    return toReturn;
}

template <typename T>
weightEdge<T> Chris<T>::findInWeights(T from, T to) {
    for(int i = 0; i < this->weights.size(); i++){
        weightEdge<T> wE = this->weights[i];
        if(wE.to == from || wE.to == to){
            if(wE.from == from || wE.from == to){
                return wE;
            }
        }
    }
}

template <typename T>
void Chris<T>::merge(vector<weightEdge<T>> edges) {
    for(int i = 0; i < edges.size(); i++){
        T from = edges[i].from;
        auto iter = this->nodes.find(from);
        vector<T> connections = iter->second.edges;
        this->addEdge(edges[i].from, edges[i].to, edges[i].weight);
    }
}

template <typename T>
vector<T> Chris<T>::euler() {
    vector<T> path;
    vector<weightEdge<T>> toIgnore;
    auto iter = this->nodes.begin();
    stack<T> stk;
    stk.push(iter->first);
    T cur = stk.top();
    path.push_back(cur);
    while(!stk.empty()){
        iter = this->nodes.find(cur);
        vector<T> edges = iter->second.edges;
        bool hasConnect = false;
        int index = -1;
        for(int i = 0; i < edges.size(); i++) {
            T con = edges[i];
            bool isValid = true;
            for(int j = 0; j < toIgnore.size(); j++){
                if(con == toIgnore[j].to || con == toIgnore[j].from) {
                    if (cur == toIgnore[j].to || cur == toIgnore[j].from){
                        isValid = false;
                        break;
                    }
                }
            }
            hasConnect = hasConnect || isValid;
            if(hasConnect) {
                index = i;
                break;
            }
        }
        if(!hasConnect){
            path.push_back(cur);
            cur = stk.top();
            stk.pop();
        }
        else{
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

template <typename T>
vector<T> Chris<T>::hamilton(vector<T> ePath) {
    vector<T> toIgnore;
    vector<T> hamilton;
    auto iter = ePath.begin();
    T start = *iter;
    while(iter != ePath.end()){
        T cur = *iter;
        auto fnd = find(toIgnore.begin(), toIgnore.end(), cur);
        if(fnd == toIgnore.end()){
            hamilton.push_back(cur);
            toIgnore.push_back(cur);
        }
        iter++;
    }
    hamilton.push_back(start);
    return hamilton;
}
#endif //TSP_CHRIS_H
