/**
 * Reads in files and calls the find path method in graph and outputs the path to a file
 */

#include "Driver.h"
#include "NN.h"
#include "Chris.h"
#include "Set.h"
#include <iostream>
#include <fstream>
#include <sstream>

/**
 * sets the type of algo to be used
 * @param type
 */
void Driver::setType(const string& tp) {
    if(tp == "trivial")
        type = trivial;
    else
        type = optimal;
}

/**
 * Sets the output file
 * @param fileName is the name of the output file
 */
void Driver::setOutput(const string& fileName) {
    out = new ofstream(fileName);
    if(!out->is_open()){
        cout << "Error opening output file" << endl;
        out = nullptr;
    }
}

/**
 * Reads in a set input file and finds the path
 * @param fileName
 */
void Driver::readFile(const string& fileName) {
    if(out == nullptr){
        cout << "Set output first" << endl;
        return;
    }
    if(type == UNSET){
        cout << "Set type first" << endl;
        return;
    }
    if(type == trivial)
        gr = new NN<string>();
    else
        gr = new Chris<string>();
    ifstream inputFile(fileName);
    if(!inputFile.is_open()){
        cout << "Error in input file" << endl;
        return;
    }
    // reads in the graph
    string line;
    getline(inputFile, line);
    int numNodes = parseInt(line);  // num nodes
    string node;
    for(int i = 0; i < numNodes; i++){
        getline(inputFile, line);
        node = trim(line);
        gr->addNode(node);
    }
    string numConnections;
    getline(inputFile, numConnections);
    int numCon = parseInt(numConnections);
    for(int j = 0; j < numCon; j++){    //adds all the edges
        getline(inputFile, line);
        if(line.length() <= 0)
            break;

        vector <string> tokens;
        stringstream check(line);
        string temp;
        while(getline(check, temp, ','))
        {
            tokens.push_back(temp);
        }
        gr->addEdge(tokens[0], tokens[1], stoi(tokens[2]));
        if(inputFile.eof())
            break;
    }
    // gets the path
    vector<string> vec = gr->getPath();
    *out << "Ideal path for " << fileName <<  " using ";
    if(type == trivial) {
        *out << "NN";
    }
    else {
        *out << "Christofides";
    }
    *out << " implementation:" << endl;
    *out << "Cost: " << gr->calcWeights(vec) << endl;
    // prints the path
    printVec(vec);
    out->flush();
    inputFile.close();
}

/**
 * Parses out an int from a line
 * @param line is in the format "[x]" where x is an int
 * @return the integer
 */
int Driver::parseInt(string line) {
    if(line.size() < 3)
        cout << "invalid token" << endl;
    string token;
    for(unsigned int i = 1; i < line.size() - 1; i++)
        token += line[i];
    return stoi(token);
}

/**
 * Removes the return character from a string
 * @param str is the string to trim
 * @return the shortened string
 */
string Driver::trim(string str) {
    size_t pos = str.find('\r');
    if(pos != string::npos)
        return str.substr(0, str.length() - 1);
    else
        return str;
}

/**
 * prints out the vector representing the path
 * @param vec is the path
 */
void Driver::printVec(vector<string> vec) {
    *out << "\t{";
    for(int i = 0; i < vec.size(); i++){
        *out << vec[i] ;
        if(i % 10 == 0 && i > 0 && i < vec.size() - 1)
            *out << endl << "\t" << vec[i];
        if(i < vec.size() - 1)
            *out << " - ";
        else
            *out << "}";
    }
    *out << endl;
}