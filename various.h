/**
 *
 */

#ifndef INC_20S_PA02_RANIROGAN_VARIOUS_H
#define INC_20S_PA02_RANIROGAN_VARIOUS_H

#include <stack>
using namespace std;
template <typename T>
struct edge{
    T to;
    T from;
    double val;
    int weight;
};

template <typename T>
struct weightEdge{
    bool operator<(const weightEdge<T>& other){
        return this->weight < other.weight;
    }
    bool operator==(const weightEdge<T>& other){
        bool eq = this->weight == other.weight;
        eq = eq && (this->from == other.from);
        eq = eq && (this->to == other.to);
        return eq;
    }
    T from;
    T to;
    int weight;
};

enum set_Type{my, ll, DEFAULT};

enum algo_Type{trivial, optimal, UNSET};

template <typename T>
struct customFunc{
    inline bool operator() (const weightEdge<T>& one, const weightEdge<T>& two){
        return one.weight < two.weight;
    }
};
#endif //INC_20S_PA02_RANIROGAN_VARIOUS_H
