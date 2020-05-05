/**
 *
 */

#ifndef INC_20S_PA03_RANIROGAN_SET_H
#define INC_20S_PA03_RANIROGAN_SET_H
#include "Tree.h"
#include <vector>
#include <unordered_map>

using namespace std;

template <typename T>
class Set{
public:
    Set();
    ~Set() = default;
    void makeSet(T data);
    int find(T data);
    void union_(T first, T second);
private:
    unordered_map<int, Tree<T>*> sets;
    bool remove(T data);
    bool remove_key(int key);
    int count;
};

// constructor
template <typename T>
Set<T>::Set(){
    count = 0;
}
/**
 * Creates a new disjoint set
 * @tparam T is the type of the set
 * @param data is the new item to add
 */
template <typename T>
void Set<T>::makeSet(T data) {
    auto* t = new Tree<T>();
    t->insert(data);
    sets.insert(pair<int, Tree<T>*>(count, t));
    count++;
}

/**
 * finds if an item is in a set, if so return the id of the set
 * @tparam T is the type of the set
 * @param data is the key to look for
 * @return the id of the tree the item is in, -1 if not found
 */
template <typename T>
int Set<T>::find(T data) {
    auto iter = sets.begin();
    while(iter != sets.end()){
        Tree<T>* t = iter->second;
        T* probe = t->find(data);
        if(probe != nullptr)
            return iter->first;
        iter++;
    }
    return -1;
}

template <typename T>
void Set<T>::union_(T first, T second) {
    int key_One = find(first);
    int key_Two = find(second);
    if(key_One  == -1 || key_Two == -1)
        return;
    Tree<T>* tree_One = sets[key_One];
    Tree<T>* tree_Two = sets[key_Two];
    vector<T> set_Two = tree_Two->getList();
    for(unsigned int i  = 0; i < set_Two.size(); i++)
        tree_One->insert(set_Two.at(i));
    remove_key(key_Two);
}

/**
 * removes a whole tree from the map
 * @tparam T is the type of the set
 * @param data is a key within on of the maps
 * @return true if removed
 */
template <typename T>
bool Set<T>::remove(T data) {
    int key = find(data);
    if(key == -1)
        return false;
    sets.erase(key);
    return true;
}

/**
 * removes a whole tree from the map
 * @tparam T is the type of the set
 * @param key is the key of the tree to remove
 * @return true if removed
 */
template <typename T>
bool Set<T>::remove_key(int key) {
    if(key == -1)
        return false;
    sets.erase(key);
    return true;
}
#endif //INC_20S_PA03_RANIROGAN_SET_H
