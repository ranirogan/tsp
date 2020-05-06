#include <iostream>
#include "Graph.h"
#include "NN.h"
#include "Driver.h"

using namespace std;
int main() {
    Graph<string>* test = new NN<string>();
    Driver d;
    d.setOutput("output.txt");
    d.setType("trivial");
    d.readFile("inputFile04");
    d.setType("optimal");
    d.readFile("inputFile04");
    return 0;
}
