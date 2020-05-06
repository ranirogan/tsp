/**
 * Reads in files and calls the find path method in graph and outputs the path to a file
 */

#ifndef INC_20S_PA03_RANIROGAN_DRIVER_H
#define INC_20S_PA03_RANIROGAN_DRIVER_H

#include <string>
#include "Graph.h"
#include "Set.h"
#include "various.h"

using namespace std;

class Driver {
public:
    Driver() : gr(nullptr), type(UNSET), out(nullptr){}
    void readFile(const string& fileName);
    void setType(const string& type);
    void setOutput(const string& fileName);
    void printVec(vector<string> vec);
private:
    algo_Type type;
    ofstream * out;
    Graph<string>* gr;
    static int parseInt(string str);
    static string trim(string str);
};


#endif //INC_20S_PA03_RANIROGAN_DRIVER_H
