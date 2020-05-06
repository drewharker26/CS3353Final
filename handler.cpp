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

void Handler::addData(ofstream &file, char *argv[], Graph<Vertex<string>, string>& publicGraph) {
    string line, vertex, connection, tempNumVertexs, tempNumConnections, faulty, general, sOrder;
    int numVertexs, numConnections, order;
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
            getline(fileName, vertex,',');
            getline(fileName, faulty);
            removePucnt(vertex);
            publicGraph.addVertex(faulty, vertex);
        }
        getline(fileName, tempNumConnections);
        removePucnt(tempNumConnections);
        numConnections= stoi(tempNumConnections);
        while(numConnections > 0)
        {
            getline(fileName, vertex, '-');
            getline(fileName, connection);
            removePucnt(vertex);
            removePucnt(connection);
            publicGraph.addConnection(vertex, connection);
            publicGraph.addConnection(connection, vertex);
            numConnections--;
        }
        getline(fileName, general,',');
        getline(fileName, sOrder);
        order = stoi(sOrder);
        publicGraph.bftComplex(file, general, order);
        fileName.close();
    }
    else
        cout << "Graph input file could not be opened. This should not be happening." << endl;
}