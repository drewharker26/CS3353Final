#include <iostream>
//#include <fstream>
//#include <sstream>
#include <algorithm>
//#include <string>
#include "vertex.h"
#include "graph.h"
#include "handler.h"

int main(int argc, char *argv[]) {
    Graph<Vertex<string>, string> publicGraph;
    Handler handler;
    ofstream fout;
    fout.open("bftOutput.txt");
    cout << "Output file hardcoded: bftOutput.txt " << endl;
    cout << "Command File supplied: " << argv[1] << endl;
    handler.addData(fout, argv, publicGraph);
    return 0;
}
