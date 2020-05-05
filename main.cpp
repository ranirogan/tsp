#include <iostream>
#include "Graph.h"
#include "NN.h"
#include "Driver.h"

using namespace std;
int main() {
    Graph<string>* test = new NN<string>();
    Driver d;
    d.setType("optimal");
    d.setOutput("output.txt");
    d.readFile("inputFile03");
    return 0;
}
