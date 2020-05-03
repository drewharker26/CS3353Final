//
// Created by Drew Harker on 3/19/20.
//

#ifndef INC_20S_3353_PA02_HANDLER_H
#define INC_20S_3353_PA02_HANDLER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "vertex.h"
#include "graph.h"

using namespace std;

class Handler {
public:
    Handler() = default;
    void removePucnt(string &str);
    void addData(char *argv[], Graph<Vertex<string>, string>& publicGraph);
};

#endif //INC_20S_3353_PA02_HANDLER_H
