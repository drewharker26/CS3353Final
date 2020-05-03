//
// Created by Drew Harker on 3/19/20.
//

#include "handler.h"

void Handler::removePucnt(string &str)
{
    str.erase(remove(str.begin(), str.end(), '['), str.end()); //removes [
    str.erase(remove(str.begin(), str.end(), ']'), str.end()); //removes ]
    str.erase(remove(str.begin(), str.end(), ' '), str.end()); //removes
    str.erase(remove(str.begin(), str.end(), '-'), str.end()); //removes -
    str.erase(remove(str.begin(), str.end(), '\n'), str.end()); //removes new line
}

void Handler::addData(char *argv[], Graph<Vertex<string>, string>& publicGraph) {
    string line, vertex, connection, tempNumVertexs;
    int numVertexs;
    ifstream fileName;
    fileName.open(argv[1]);
    if(fileName.is_open())
    {
        getline(fileName, tempNumVertexs);
        removePucnt(tempNumVertexs);
        numVertexs = stoi(tempNumVertexs);
        while(numVertexs > 0)
        {
            numVertexs--;
            getline(fileName, vertex);
            removePucnt(vertex);
            publicGraph.addVertex(vertex);
        }
        getline(fileName, line);
        while(getline(fileName, vertex, '-'))
        {
            getline(fileName, connection);
            removePucnt(vertex);
            removePucnt(connection);
            publicGraph.addConnection(vertex, connection);
            publicGraph.addConnection(connection, vertex);
        }
        fileName.close();
    }
    else
        cout << "Graph input file could not be opened. This should not be happening." << endl;
}