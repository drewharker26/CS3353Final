//
// Created by Drew Harker on 3/6/20.
//

#ifndef INC_20S_3353_PA02_GRAPH_H
#define INC_20S_3353_PA02_GRAPH_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <iomanip>

#include "vertex.h"

using namespace std;

template<class U, class T>
class Graph {
    friend class Vertex<T>;
public:
    Graph() = default;
    //~Graph();
    void addVertex(U vertex);
    void addConnection(U vertex, U connection);
    void bft(U general);
private:
    vector<vector<U>> graph;
};

template<class U, class T>
bool operator!=(const Vertex<T> &v1, const Vertex<T> &v2) { //overload != operator to test vertex equality
    if (v1.getData() != v2.getData())
        return true;
    return false;
}

template<class U, class T>
bool operator==(const Vertex<T> &v1, const Vertex<T> &v2) { //overload == operator to test vertex equality
    if (v1.getData() == v2.getData())
        return true;
    return false;
}

template<class U, class T>
void Graph<U, T>::addVertex(U vertex) {
    if (graph.size() == 0) { //if graph is empty we know vertex doesnt exist so we create vector and push it back
        vector<U> tempVec;
        tempVec.push_back(vertex);
        graph.push_back(tempVec);
    } else {
        bool found = false;
        for (unsigned int i = 0; i < graph.size(); i++) { //loop through existing vertexs to see if duplicate
            if (graph[i][0].getData() == vertex.getData()) {
                found = true;
                break;
            }
        }
        if (found == false) { //if not a duplicate, push back
            vector<U> tempVec2;
            tempVec2.push_back(vertex);
            graph.push_back(tempVec2);
        }
    }
}

template<class U, class T>
void Graph<U, T>::addConnection(U vertex, U connection) {
    bool foundVertex = false;
    bool foundConnection = false;
    for (unsigned int i = 0; i < graph.size(); i++) { //find the appropriate vertex to add connection
        if (graph[i][0].getData() == vertex.data && foundVertex == false) {
            foundVertex = true;
            for (unsigned int j = 1; j < graph[i].size(); j++) { //check if vertex already has a connection
                if (graph[i][j].getData() == connection.getData() && foundConnection == false) {
                    foundConnection = true;
                    break;
                }
                else if (foundConnection == true)
                    break;
            }
            if (foundConnection == false)
            {
                graph[i].push_back(connection); //add connection if it does not already exist
            }
        }
        else if (foundVertex == true)
            break;
    }
    if (foundVertex == false)
        cout << "Vertex not found" << endl;
}

#endif //INC_20S_3353_PA02_GRAPH_H
