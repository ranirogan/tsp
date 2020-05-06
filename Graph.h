/**
 * Graph Class
 * Rani Rogan
 * PA02
 */

#ifndef INC_20S_PA02_RANIROGAN_GRAPH_H
#define INC_20S_PA02_RANIROGAN_GRAPH_H

#include <cstdlib>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <queue>
#include <stack>
#include <climits>
#include "various.h"
#include "Set.h"
#include "Set.h"

using namespace std;

template <typename T>
class Graph {
protected:
    int numEdges = 0;
    class Node{ // holds the data and the adjacent nodes
    public:
        T obj;
        vector<T> edges;
        bool visited;
        int val;
        double input;
        int level;
        int community;
    };
    struct Dijsktra{ // contains the node with the current distance and previous node
    public:
        Node nd;
        int dist;
        Node prev;
        bool hasPrev;
    };
    unordered_map<T, Node> nodes;
    int findSmallest(vector<Dijsktra>& vec);
    int findNode(vector<Dijsktra>& vec, T search);
    vector<edge<T>> GNBFS(T source, vector<edge<T>>& toIgnore);
    vector<edge<T>> sumEdges(vector<edge<T>>& vec);
    vector<edge<T>> sumVecs(vector<edge<T>>& tot, vector<edge<T>> vec);
    vector<edge<T>> findLargest(vector<edge<T>>& vec);
    bool check(vector<edge<T>> temp, vector<edge<T>> toIgnore);
    vector<vector<T>> makeCommunities(vector<edge<T>> toIgnore);
    vector<weightEdge<T>> weights;
    void printGraph();
public:
    Graph()= default;
    void addNode(T val);
    void addEdge(T from, T to);
    void unVisit();
    vector<edge<T>> BFS(T source);
    vector<edge<T>> DFS(T source);
    vector<T> connect(T from, T to);
    vector<vector<T>> discover();
    void clear();
    void addEdge(T from, T to, int weight);
    virtual vector<T> getPath() = 0;
    vector<weightEdge<T>> getMinSpan();
    int calcWeights(vector<T> vec);
};

/**
 * Clears the graph
 * @tparam T is the type of the graph
 */
template <typename T>
void Graph<T>::clear(){
    nodes.clear();
    numEdges = 0;
    weights.clear();
}

/**
 * Adds a node to the graph
 * @tparam T is the type of the graph
 * @param val is the data that is stored in the node
 */
template <typename T>
void Graph<T>::addNode(T val){
    auto iter = nodes.find(val);
    // if the node doesn't already exist
    if(iter == nodes.end()) {
        Node nd;
        vector<T> vec;
        nd.edges = vec;
        nd.obj = val;
        nd.visited = false;
        nodes.insert(pair<T, Node>(val, nd));
    } else
        ; //don't re-add
}

/**
 * Connects two nodes
 * @tparam T is the type of the graph
 * @param from is the source node
 * @param to is the destination node
 */
template <typename  T>
void Graph<T>::addEdge(T from, T to) {
    auto iter = nodes.find(from);
    if(iter == nodes.end()) {   //makes sure the node exists
        cout << "Not Found" << endl;
        return;
    }
    Node fromNode = iter->second;
    iter = nodes.find(to);
    if(iter == nodes.end()) {   //makes sure the node exists
        cout << "Not Found" << endl;
        return;
    }
    numEdges++;
    Node toNode = iter->second;
    fromNode.edges.push_back(to);
    nodes[from] = fromNode;
    toNode.edges.push_back(from);
    nodes[to] = toNode;

}

/**
 * Performs a breath first search starting at the specified node and returns a vector of edges
 * @tparam T is the graph type
 * @param source is the starting node
 */
template <typename T>
vector<edge<T>> Graph<T>::BFS(T source) {
    unVisit();  //reset all nodes
    vector<edge<T>> vec;
    queue<T> que;
    auto iter = nodes.find(source);
    // make sure that the source exists
    if(iter == nodes.end()){
        cout << "Source not found" << endl;
        return vec;
    }
    que.push(source);
    // exhaustive search
    while(!que.empty()){
        T orig_T = que.front();
        Node origin = nodes[orig_T];
        que.pop();
        if(origin.visited) //ignore if duplicate
            continue;
        origin.visited = true;
        nodes[orig_T] = origin; //reset obj in map
        // search through all connections
        for(unsigned int i = 0; i < origin.edges.size(); i++) {
            T dest = origin.edges[i];
            Node dest_Node = nodes[dest];
            if(!dest_Node.visited) {
                que.push(dest);
                edge<T> ed;
                ed.from = origin.obj;
                ed.to = dest;
                vec.push_back(ed);
            }
        }
    }
    return vec;
}

/**
 * Performs a depth first search starting at the specified node and returns a vector of edges
 * @tparam T
 * @param source
 * @return
 */
template <typename T>
vector<edge<T>> Graph<T>::DFS(T source) {
    unVisit();  //reset all nodes
    vector<edge<T>> vec;
    stack<T> stk;
    auto iter = nodes.find(source);
    // make sure that the source exists
    if(iter == nodes.end()){
        cout << "Source not found" << endl;
        return vec;
    }
    stk.push(source);
    // exhaustive search
    while(!stk.empty()){
        T orig_T = stk.top();
        Node origin = nodes[orig_T];
        origin.visited = true;
        nodes[orig_T] = origin; //reset obj in map
        bool isFound = false;
        // search until an available connection is found
        for(unsigned int i = 0; i < origin.edges.size(); i++) {
            T dest = origin.edges[i];
            Node dest_Node = nodes[dest];
            if(!dest_Node.visited) {
                stk.push(dest);
                edge<T> ed;
                ed.from = origin.obj;
                ed.to = dest;
                vec.push_back(ed);
                isFound = true;
                break;
            }
        }
        if(!isFound)
            stk.pop();
    }
    return vec;
}

/**
 * Sets all node objects' visit to false
 * @tparam T is the graph type
 */
template <typename T>
void Graph<T>::unVisit() {
    for(auto iter = nodes.begin(); iter != nodes.end(); iter++) {
        Node n = iter->second;
        n.visited = false;
        n.val = 0;
        n.level = INT_MAX;
        n.input = 0;
        nodes[n.obj] = n;
    }
}

/**
 * Connects two nodes to each other based off of Dijkstra's algorithm
 * @param from is the start node
 * @param to is the end node
 * @return a vector of the nodes
 */
template <typename T>
vector<T> Graph<T>::connect(T from, T to) {
    vector<Dijsktra> removed;
    vector<Dijsktra> vec;
    for(auto mapIter = nodes.begin(); mapIter != nodes.end(); mapIter++){ //puts all vertexes into a set
        Dijsktra d;
        d.nd = mapIter->second;
        d.hasPrev = false; // sets all nodes to automatically have no previous node
        if(d.nd.obj == from) {
            d.dist = 0;
        }     //if start, set distance to 0
        else {
            d.dist = INT_MAX;
        }   //set distance to 'infinity'
        vec.push_back(d);
    }

    bool isFound = false;
    auto vecIter = vec.begin();
    while(vec.size() != 0){ // as long as there are still nodes to look at
        int index = findSmallest(vec);  // find the smallest current distance
        if(index == -1)
            break;
        Dijsktra d = vec.at(index);
        removed.push_back(d); // keep track of previously looked at nodes
        vec.erase(vecIter + index);
        if(d.nd.obj == to) { // if this is the end, break
            isFound = true;
            break;
        }
        vector<T> edges = d.nd.edges;
        for(unsigned int j = 0; j < edges.size(); j++) { //look at all edges
            int alt = d.dist + 1;
            int pos = findNode(vec, edges.at(j));
            if (pos != -1) { //if the node hasn't already been looked at
                Dijsktra other = vec.at(pos);
                if (alt < other.dist) { // if the distance between these two nodes is smaller than its current distance
                    other.dist = alt;
                    other.prev = d.nd;
                    other.hasPrev = true;
                    vec.at(pos) = other; //reset in vector
                }
            }
        }
    }
    vector<T> toReturn;
    if(!isFound){
        return toReturn;
    }
    //starting at the end, find each node's previous and add to the stack to create a path
    stack<T> stk;
    int num = findNode(removed, to);
    Dijsktra reverse = removed.at(num);
    stk.push(reverse.nd.obj); //adds start node
    while(reverse.hasPrev){ //so it doesn't try to access a node that doesn't exist
        Node pre = reverse.prev;
        num = findNode(removed, pre.obj);
        reverse = removed.at(num);
        stk.push(reverse.nd.obj);
    }

    // reverses the stack by pushing the top into a vector
    while(!stk.empty()){
        toReturn.push_back(stk.top());
        stk.pop();
    }
    return toReturn;
}

/**
 * Helper method to find smallest current distance in a vector of Dijkstra nodes
 * @tparam T is the type of the graph
 * @param vec is a vector of nodes
 * @return the index of the smallest distance
 */
template <typename T>
int Graph<T>::findSmallest(vector<Graph<T>::Dijsktra>& vec) {
    int smallest = INT_MAX;
    int pos = -1;
    for(unsigned int i = 0; i < vec.size(); i++){
        Dijsktra d = vec.at(i);
        if(d.dist < smallest){
            smallest = d.dist;
            pos = static_cast<int>(i);
        }
    }
    return pos;
}

/**
 * Searches for the position of a given node based off of the object
 * @tparam T is the type of the graph
 * @param vec is a vector of Dijkstra nodes
 * @param search is the object to look for
 * @return the position
 */
template <typename T>
int Graph<T>::findNode(vector<Graph<T>::Dijsktra> &vec, T search) {
    for(unsigned int i = 0; i < vec.size(); i++){
        Dijsktra d = vec.at(i);
        if(d.nd.obj == search)
            return static_cast<int>(i);
    }
    return -1;
}


/**
 * Discovers communities using the girvan-newman algorithm
 * @tparam T is the type of the graph
 * @return a vector of vectors representing the different communities
 */
template <typename T>
vector<vector<T>> Graph<T>::discover() {
    auto iter = nodes.begin();
    vector<edge<T>> toIgnore;
    bool totSet = false;
    bool ignoreSet = false;
    bool cont = true;
    int num = 0;
    while (cont) {
//        if(num % 10 == 0)
//            cout << num << " " << toIgnore.size() << endl;
        totSet = false;
        vector<edge<T>> tot;
        for (iter = nodes.begin(); iter != nodes.end(); iter++) { //Modified BFS every node
            vector<edge<T>> vec = GNBFS(iter->first, toIgnore);
            vec = sumEdges(vec);
            if (!totSet) {
                if(vec.size() > 0) {
                    totSet = true;
                    tot = vec;
                }
            } else
                tot = sumVecs(tot, vec);
        }
        vector<edge<T>> temp = findLargest(tot);
        if(temp.size() == 0)
            break;
        cont = check(temp, toIgnore);
        if(!cont)
            break;
        if (!ignoreSet) {
            ignoreSet = true;
            toIgnore = temp;
        } else{
            toIgnore.insert(toIgnore.end(), temp.begin(), temp.end());
        }
        num++;
    }
    return makeCommunities(toIgnore);
}

/**
 * Modified BFS that only adds in nodes of a lower level than the current node
 * @tparam T is the type of the graph
 * @param source is the starting node
 * @param toIgnore is the list of edges to ignore
 * @return a vector of all the edges
 */
template <typename T>
vector<edge<T>> Graph<T>::GNBFS(T source, vector<edge<T>>& toIgnore) {
    unVisit();  //reset all nodes
    vector<edge<T>> vec;
    queue<T> que;
    auto iter = nodes.find(source);
    // make sure that the source exists
    if(iter == nodes.end()){
        cout << "Source not found" << endl;
        return vec;
    }
    iter->second.val++;
    iter->second.level = 0;
    nodes[source] = iter->second;
    que.push(source);
    // exhaustive search
    while(!que.empty()){
        T orig_T = que.front();
        Node origin = nodes[orig_T];
        que.pop();
        if(origin.visited) //ignore if duplicate
            continue;
        origin.visited = true;
        nodes[orig_T] = origin; //reset obj in map
        // search through all connections
        for(unsigned int i = 0; i < origin.edges.size(); i++) {
            T dest = origin.edges[i];
            Node dest_Node = nodes[dest];
            if(!dest_Node.visited && origin.level < dest_Node.level) { //only add if on a lower level
                bool willIgnore = false;
                for(unsigned int j = 0; j < toIgnore.size(); j++){
                    edge<T> ignoreEdge = toIgnore.at(j);
                    if((orig_T == ignoreEdge.from)&&(dest == ignoreEdge.to)){
                        willIgnore = true;
                        break;
                    }  //if undirected, check the other way around
                    if((orig_T == ignoreEdge.to)&&(dest == ignoreEdge.from)){
                        willIgnore = true;
                        break;
                    }

                }
                if(!willIgnore) {
                    dest_Node.val += origin.val;
                    dest_Node.level = origin.level + 1;
                    nodes[dest] = dest_Node;
                    que.push(dest);
                    edge<T> ed;
                    ed.from = origin.obj;
                    ed.to = dest;
                    vec.push_back(ed);
                }
            }
        }
    }
    return vec;
}

/**
 * Finds each edge value based off of node values
 * @tparam T is the type of graph
 * @param vec is the vector of edges
 * @return a vector of the edges with the values set
 */
template <typename T>
vector<edge<T>> Graph<T>::sumEdges(vector<edge<T>>& vec) {
    for(int i = vec.size() - 1; i >= 0; i--){
        edge<T> edg = vec.at(i);
        Node toNode = nodes[edg.to];
        Node fromNode = nodes[edg.from];
        double base = 1;    //all nodes have a default value of 1
        base  += toNode.input;  //add in weights of incoming edges
        base *= fromNode.val;   //multiply by the value of the starting node
        base /= toNode.val; //split the value amongst the parent nodes
        base /= 2.0;
        edg.val = base; //assign the edge the value
        vec.at(i) = edg;
        fromNode.input += base; //set the destination node's value
        nodes[edg.from] = fromNode;
    }
    return vec;
}

/**
 * Sums a set of edges with the total of the edges
 * @tparam T is the type of the graph
 * @param tot is the edge's totals
 * @param vec is the new edges to sum in
 * @return a vector of the edges with the updated totals
 */
template <typename T>
vector<edge<T>> Graph<T>::sumVecs(vector<edge<T>> &tot, vector<edge<T>> vec) {
    for(unsigned int i = 0; i < vec.size(); i++){
        edge<T> vecE = vec.at(i);
        bool isFound = false;
        for(unsigned int j = 0; j < tot.size(); j++){
            edge<T> totE = tot.at(j);
            if((vecE.to == totE.to)&&(vecE.from == totE.from)){
                totE.val += vecE.val;
                tot.at(j) = totE;
                isFound = true;
                break;
            }    //if undirected, check the other way around
            if((vecE.to == totE.from)&&(vecE.from == totE.to)){
                totE.val += vecE.val;
                tot.at(j) = totE;
                isFound = true;
                break;
            }
        }
        if(!isFound)
            tot.push_back(vecE);
    }
    return tot;
}

/**
 * Finds the edge(s) with the largest value
 * @tparam T is the type of the graph
 * @param vec
 * @return
 */
template <typename T>
vector<edge<T>> Graph<T>::findLargest(vector<edge<T>>& vec) {
    vector<edge<T>> edges;
    double largest = INT_MIN;
    for(unsigned int i = 0; i < vec.size(); i++){
        edge<T> edg = vec.at(i);
        if((edg.val - largest) > 0.1){
            edges.clear();
            largest = edg.val;
            edges.push_back(edg);
        }
        else if((largest - edg.val) < 0.1)
            edges.push_back(edg);
    }
    return edges;
}

/**
 * Checks to make sure that a node won't be orphaned by taking out an edge
 * @tparam T is the type of the graph
 * @param temp is the vector of edges that may be removed
 * @param toIgnore is the vector of edges that already have been removed
 * @return true if it should continue, false if it should stop
 */
template <typename T>
bool Graph<T>::check(vector<edge<T>> temp, vector<edge<T>> toIgnore) {
    if(toIgnore.size() == 0)
        return true;
    else{
        vector<T> vec;
        for(unsigned int i = 0; i < temp.size(); i++){   //check edges to be removed
            edge<T> edg = temp.at(i);
            int connections = 0;
            T from = edg.from;
            Node fromNode = nodes[from];
            vec = fromNode.edges;
            for(unsigned int j = 0; j < vec.size(); j++){    //for every connection starting at the from node
                T to_temp = vec.at(j);
                bool isFound = false;
                if(to_temp == edg.to || to_temp == edg.from) {
                    continue;
                }
                for(unsigned int k = 0; k < toIgnore.size();k++){    //check if toIgnore
                    edge<T> ignoreEdge = toIgnore.at(k);
                    if(from == ignoreEdge.from && to_temp == ignoreEdge.to){
                        isFound = true;
                        break;
                    }
                    else if(from == ignoreEdge.to && to_temp == ignoreEdge.from){
                        isFound = true;
                        break;
                    }
                }
                for(unsigned int l = 0; l < temp.size(); l++){    //check if toIgnore
                    edge<T> tempEdge = temp.at(l);
                    if(from == tempEdge.from && to_temp == tempEdge.to){
                        isFound = true;
                        break;
                    }
                    else if(from == tempEdge.to && to_temp == tempEdge.from){
                        isFound = true;
                        break;
                    }
                }
                if(!isFound)
                    connections++;
            }
            if(connections == 0)    //if it will be orphaned, don't remove these edges
                return false;
            //if graph is undirected, check in the opposite direction
            T to = edg.to;
            connections = 0;
            Node toNode = nodes[to];
            vec = toNode.edges;
            for(unsigned int j = 0; j < vec.size(); j++){    //for every connection starting at the to node
                T from_temp = vec.at(j);
                bool isFound = false;
                if(from_temp == edg.from || from_temp == edg.to) {
                    continue;
                }
                for(unsigned int k = 0; k < toIgnore.size();k++){    //check if toIgnore
                    edge<T> ignoreEdge = toIgnore.at(k);
                    if((from_temp == ignoreEdge.from && to == ignoreEdge.to) ||
                       (from_temp == ignoreEdge.to && to == ignoreEdge.from)){
                        isFound = true;
                        break;
                    }
                }
                if(!isFound)
                    connections++;
            }
            if(connections == 0) //if it will be orphaned, don't remove the edges
                return false;
        }
    }
    return true; //no nodes will be orphaned
}

/**
 * Categorizes the map into communities
 * @tparam T is the type of the graph
 * @param toIgnore is the vector of edges that are removed
 */
template <typename T>
vector<vector<T>> Graph<T>::makeCommunities(vector<edge<T>> toIgnore){
    auto iter = nodes.begin();
    for(iter = nodes.begin(); iter != nodes.end(); iter++) //reset communities
        iter->second.community = -1;
    int num = 0;
    for(iter = nodes.begin(); iter != nodes.end(); iter++){
        Node nd = iter->second;
        if(nd.community == -1){ //if already categorized, skip
            nd.community = num;
            vector<edge<T>> com = GNBFS(nd.obj, toIgnore);
            for(unsigned int i = 0; i < com.size(); i++){  // sets both sides to the same community
                edge<T> edg = com.at(i);
                Node fromNode = nodes[edg.from];
                Node toNode = nodes[edg.to];
                if(fromNode.community == -1)
                    fromNode.community = num;
                if(toNode.community == -1)
                    toNode.community = num;
                nodes[edg.from] = fromNode;
                nodes[edg.to] = toNode;
            }
            num++;
        }
        nodes[nd.obj] = nd;
    }
    vector<vector<T>> toReturn;
    toReturn.resize(num);
    for(iter = nodes.begin(); iter != nodes.end(); iter++){ //pushes them into vectors according to their community num
        Node nd = iter->second;
        toReturn.at(nd.community).push_back(nd.obj);
    }
    return toReturn;
}

/**
 * Adds a weighted edge
 * @tparam T is the type of the graph
 * @param from is the source node
 * @param to is the destination node
 * @param weight is the weight of the edge
 */
template <typename T>
void Graph<T>::addEdge(T from, T to, int weight) {
    auto iter = nodes.find(from);
    if(iter == nodes.end()) {   //makes sure the node exists
        cout << "Not Found" << endl;
        return;
    }
    Node fromNode = iter->second;
    iter = nodes.find(to);
    if(iter == nodes.end()) {   //makes sure the node exists
        cout << "Not Found" << endl;
        return;
    }
    numEdges++;
    Node toNode = iter->second;
    fromNode.edges.push_back(to);
    toNode.edges.push_back(from);
    nodes[from] = fromNode;
    nodes[to] = toNode;
    weightEdge<T> wEd;
    wEd.from = from;
    wEd.to = to;
    wEd.weight = weight;
    weights.push_back(wEd);
}

/**
 * Gets the minimum spanning tree for a graph based off of Kruskal's algorithm
 * @tparam T is the type of the graph
 * @return a vector of weighted edges that are used in the spanning tree
 */
template <typename T>
vector<weightEdge<T>> Graph<T>::getMinSpan() {
    vector<weightEdge<T>> vec;
    auto* s = new Set<T>();
    sort(weights.begin(), weights.end(), customFunc<T>());
    for(unsigned int i = 0; i < weights.size(); i++){
        T from = weights[i].from;
        T to = weights[i].to;
        int index_from = s->find(from);
        int index_to = s->find(to);
        int weight = weights[i].weight;
        if(index_from == -1 || index_to == -1 || (index_from != index_to)){
            vec.push_back(weights[i]);
            if(index_from == -1)
                s->makeSet(from);
            if(index_to == -1)
                s->makeSet(to);
            s->union_(from, to);
        }
    }
    return vec;
}

/**
 * prints out each node and their connections in the graph
 * @tparam T is the type of the graph
 */
template <typename T>
void Graph<T>::printGraph() {
    for(auto iter = nodes.begin(); iter != nodes.end(); iter++){
        Node nd = iter->second;
        cout << nd.obj << ":" << endl;
        for(int i = 0; i < nd.edges.size(); i++){
            cout << "\t" << nd.edges[i] << endl;
        }
    }
}

/**
 * Given a vector of T objects representing a path, finds the corresponding weighted edges
 * and calculates the cost of the path
 * @tparam T is the type of the graph
 * @param vec is a vector representing a path
 * @return an integer that is the cost of the path
 */
template <typename T>
int Graph<T>::calcWeights(vector<T> vec) {
    int sum = 0;
    for(int i = 0; i < vec.size() - 1; i ++){
        T cur = vec[i];
        T next = vec[i+1];
        weightEdge<T> wE;
        for(int j = 0; j < weights.size(); j++){
            wE = weights[j];
            if(wE.to == cur || wE.to == next){ //undirected
                if(wE.from == cur || wE.from == next){
                    break;
                }
            }
        }
        sum += wE.weight;
    }
    return sum;
}
#endif //INC_20S_PA02_RANIROGAN_GRAPH_H
