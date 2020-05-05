/**
 * AVL Tree from data structures search engine
 */

#ifndef INC_20S_PA03_RANIROGAN_TREE_H
#define INC_20S_PA03_RANIROGAN_TREE_H

#include <vector>
using namespace std;
template <typename T>
class Tree{
private:
    class Node{
    public:
        T obj;
        Node* left;
        Node* right;
        int height;
        Node(T& o, Node* lt, Node* rt, int h = 0) :
                obj(o), left(lt), right(rt), height(h) {}
        ~Node()= default;
        T& getObj(){return obj;}
    };

    Node* root;

    /**Finds the leftmost value aka the minimum
     * @brief findMin
     * @param x is the avl node being looked at
     * @return the lowest avl node
     */
    Node* findMin(Node* x){
        if(x != nullptr){
            if(x->left == nullptr)
                return x;
            else
                return findMin(x->left);
        }
        return nullptr;
    }

    /**Finds the rightmost AVL node, aka the largest value
     * @brief findMax
     * @param x is the avl node being looked at
     * @return the largest avl node
     */
    Node* findMax(Node* x){
        if( x != nullptr){
            if(x->right == nullptr)
                return x;
            else
                return findMax(x->right);
        }
        return nullptr;
    }

    /**Empties the tree recursively
     * @brief makeEmpty
     * @param x is the node to look at
     */
    void makeEmpty(Node*& x){
        if(x != nullptr){
            makeEmpty(x->left);
            makeEmpty(x->right);
            delete x;
        }
        x = nullptr;
    }

    /**Return the height, -1 if node is null
     * @brief height
     * @param t is the node
     * @return the height, -1 if nullptr
     */
    int height(Node* t) const{
        return t == nullptr ? -1 : t->height;
    }

    /**returns the max of 2 values
     * @brief max
     * @param lhs is one of the values
     * @param rhs is the other value to compare to
     * @return the larger value
     */
    int max(int lhs, int rhs) const{
        return lhs > rhs ? lhs : rhs;
    }

    //insert functions
    void insert(T& key, Node*& x);
    void rotateLC(Node*& k2);
    void rotateRC(Node*& k2);
    void doubleRotateLC(Node*& k3);
    void doubleRotateRC(Node*& k1);

    /**Returns if a object is contained in the tree recursively
     * @brief contains
     * @param obj is the object to look for
     * @param x is the node to look at
     * @return true if found, else false
     */
    bool contains(const T& key, Node*& x){
        if(x != nullptr){
            if(key == x->getObj())
                return true;
            if(key < x->getObj())
                return contains(key, x->left);
            else
                return contains(key, x->right);
        }
        else
            return false;
    }

    /**Searches throught the AVL Tree to find a specific object
     * @brief find
     * @param obj is the object to look for
     * @param x is the avl node being looked at
     * @return a pointer to the avl node that contains the object
     */
    Node* find(T key, Node* x){
        if(x != nullptr){
            if(key < x->obj)
                return find(key, x->left);
            else if(x->obj < key)
                return find(key, x->right);
            else {
                return x;
            }
        }
        return nullptr;
    }

    void getList(vector<T>& vec, Node* x);
public:
    Tree() : root(nullptr), size(0) {}
    Tree(const Tree& other) : root(nullptr){
        *this = other;
        this->size = other.size;
    }
    ~Tree(){
        makeEmpty();
    }

    int size;
    int getSize(){return size;}
    /**Find the smallest item in the tree, excpetion if empty
     * @brief findMin
     * @return the element of the smallest node
     */
    const T& findMin() const{
        if(!isEmpty())
            return findMin(root)->obj;
    }

    /**Find the largest item in the tree
     * @brief findMax
     * @return the element in the largest node
     */
    const T& findMax() const{
        if(!isEmpty())
            return findMax(root)->obj;
    }

    /**Return true if obj in the tree
     * @brief contains
     * @param obj is the object to look for
     * @return true if found
     */
    bool contains(T& key){
        return contains(key, root);
    }

    /**Test if the tree is empty
     * @brief isEmpty
     * @return true if the tree is empty
     */
    bool isEmpty(){
        return root == nullptr;
    }

    /**Empties tree
     * @brief makeEmpty
     */
    void makeEmpty(){
        makeEmpty(root);
    }

    /**Insert object into the tree, no duplicates
     * @brief insert
     * @param obj is the object to insert
     */
    void insert(T& o){
        insert(o, root);
    }

    /**Finds a specified object
     * @brief find
     * @param obj is the object to look for
     * @return the object found
     */
    T* find(T& key){
        Node* temp = find(key, root);
        if(temp == nullptr)
            return nullptr;
        else{
            return &temp->obj;
        }
    }

    vector<T> getList();
};

/**inserts an object into the tree
 * @param obj is the object
 * @param x is the node to look at
 */
template <typename T>
void Tree<T>::insert(T& key, Node*& x){
    if(x == nullptr){
        x = new Node(key,nullptr, nullptr);
        size++;
    }
    else if(key < x->obj){
        insert(key, x->left);
        if(height(x->left) - height(x->right) == 2){
            if(key < x->left->obj)
                rotateLC(x);
            else
                doubleRotateLC(x);
        }
    }
    else if(x->obj < key){
        insert(key, x->right);
        if(height(x->right) - height(x->left) == 2){
            if(x->right->obj < key)
                rotateRC(x);
            else
                doubleRotateRC(x);
        }
    }
    else{
        ; //dont add
    }
    x->height = max(height(x->left), height(x->right)) + 1;
}

/**Rotates with the left child for Case 1, updates heights
 * @param k2 is the node to rotate
 */
template <typename T>
void Tree<T>::rotateLC(Node*& k2){
    Node* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1;
}

/**Rotates with the right child for Case 4, updates heights
 * @param k2 is the node to rotate
 */
template <typename T>
void Tree<T>::rotateRC(Node*& k2){
    Node* k1 = k2->right;
    k2->right = k1->left;
    k1->left = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1;
}

/**Double rotates the binary tree node
 * @param k3 is the node to rotate
 */
template <typename T>
void Tree<T>::doubleRotateLC(Node*& k3){
    rotateRC(k3->left);
    rotateLC(k3);
}

/**Double rotates the binary tree node
 * @param k1 is the node to rotate
 */
template <typename T>
void Tree<T>::doubleRotateRC(Node*& k1){
    rotateLC(k1->right);
    rotateRC(k1);
}

/**
 * Returns a vector of all the objects in the tree
 * @tparam T is the type of the tree
 * @return a vector containing all the objects
 */
template <typename T>
vector<T> Tree<T>::getList() {
    vector<T> vec;
    getList(vec, root);
    return vec;
}

/**
 * recursively adds all the objects in the tree into a vector
 * @tparam T is the type of the tree
 * @param vec is a vector containing the other objects
 * @param x is the node to be looked at
 */
template <typename T>
void Tree<T>::getList(vector<T> &vec, Node *x) {
    if(x == nullptr)
        return;
    T temp = x->obj;
    vec.push_back(temp);
    getList(vec, x->left);
    getList(vec, x->right);
}
#endif //INC_20S_PA03_RANIROGAN_TREE_H
