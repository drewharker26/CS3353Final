//
// Created by Drew Harker on 3/6/20.
//

#ifndef INC_20S_3353_PA02_GRAPH_H
#define INC_20S_3353_PA02_GRAPH_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <queue>

#include "vertex.h"

using namespace std;

template<class U, class T>
class Graph {
    friend class Vertex<T>;
public:
    Graph() = default;
    //~Graph();
    void addVertex(string faulty, U vertex);
    void addConnection(U vertex, U connection);
    void bftNaive(ofstream &file, U general, int order);
    void bftComplex(ofstream &file, U general, int order);
    void lamports(ofstream &file, U general, int order);
private:
    int processResults(U general);
    vector<vector<U>> graph;
    queue<U> Q;
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
int Graph<U, T>::processResults(U general)
{
    vector<int> results;
    for(int y=0; y < graph.size(); y++) //loop through graph vertices excluding general and determine results
    {
        if(graph[y][0].getData() != general.data) {
            int result = graph[y][0].getMajority();
            results.push_back(result);
        }
    }
    int attackFreq = count(results.begin(), results.end(), 1);
    int retreatFreq = count(results.begin(), results.end(), 0);
    if(attackFreq > retreatFreq)
        return 1;
    else if(retreatFreq > attackFreq)
        return 0;
    else
        return 2; //this should only happen if the attack orders and retreat orders are equal
}

template<class U, class T>
void Graph<U, T>::addVertex(string faulty, U vertex) {
    if(faulty == "G")
        vertex.isFaulty = false;
    else
        vertex.isFaulty = true;
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

template<class U, class T>
void Graph<U, T>::bftNaive(ofstream &file, U general, int order)
{
    bool foundGeneral = false;
    bool connectionFound = false;
    Vertex<T> vertexV = general;
    for (unsigned int i = 0; i < graph.size(); i++) { //find the vertex matching the general in the graph
        if(graph[i][0].getData() == vertexV.data) {
            graph[i][0].visited = true;
            Q.push(vertexV); //enqueue
            while (Q.size() != 0) //while queue is not empty
            {
                vertexV = Q.front(); //v=dequeue
                Q.pop(); //remove "A" from queue
                foundGeneral = false;
                connectionFound = false;
                for (unsigned int k = 0; k < graph.size(); k++) //find general we are currently at
                {
                    if (graph[k][0].getData() == vertexV.data && foundGeneral == false && connectionFound == false) {
                        foundGeneral = true;
                        for (unsigned int j = 1;
                             j < graph[k].size(); j++) //loop through direct connections of the general
                        {
                            connectionFound = false;
                            for (unsigned int x = 0; x < graph.size(); x++) //find the generals connections in the graph
                            {
                                if (graph[k][j].getData() == graph[x][0].data && connectionFound == false) {
                                    if (graph[x][0].visited == true) {
                                        if (graph[x][0].getData() == general.data)
                                            break;
                                        else {
                                            if (graph[k][0].isFaulty == true && order == 1)
                                                graph[x][0].ordersRecieved.push_back(0);
                                            else if (graph[k][0].isFaulty == true && order == 0)
                                                graph[x][0].ordersRecieved.push_back(1);
                                            else
                                                graph[x][0].ordersRecieved.push_back(order); //communicate the order
                                        }
                                    } else {
                                        connectionFound = true;
                                        if (graph[k][0].isFaulty == true && order == 1)
                                            graph[x][0].ordersRecieved.push_back(0);
                                        else if (graph[k][0].isFaulty == true && order == 0)
                                            graph[x][0].ordersRecieved.push_back(1);
                                        else
                                            graph[x][0].ordersRecieved.push_back(order); //communicate the order
                                        Q.push(graph[k][j]); //enqueue u
                                        graph[x][0].visited = true;
                                    }
                                } else if (connectionFound == true)
                                    break;
                            }
                        }
                    } else if (foundGeneral == true || connectionFound == true)
                        break;
                }
            }
        }
    }
    int finalOrder = processResults(general);
    if(finalOrder == 1)
        file << "We have been ordered to attack!" << endl;
    else if(finalOrder == 0)
        file << "We have been ordered to retreat!" << endl;
    else
        file << "The number of attack orders exactly equals retreat orders!" << endl;
}

template<class U, class T>
void Graph<U, T>::bftComplex(ofstream &file, U general, int order) { //uses A as the general
    bool foundGeneral = false;
    bool connectionFound = false;
    Vertex<T> vertexV = general;
    for (unsigned int i = 0; i < graph.size(); i++) { //find the vertex matching the general in the graph
        if(graph[i][0].getData() == vertexV.data) {
            graph[i][0].visited = true;
            Q.push(vertexV); //enqueue
            while (Q.size() != 0) //while queue is not empty
            {
                vertexV = Q.front(); //v=dequeue
                Q.pop(); //remove "A" from queue
                foundGeneral = false;
                connectionFound = false;
                for (unsigned int k = 0; k < graph.size(); k++) //find general we are currently at
                {
                    if (graph[k][0].getData() == vertexV.data && foundGeneral == false && connectionFound == false)
                    {
                        foundGeneral = true;
                        for (unsigned int j = 1; j < graph[k].size(); j++) //loop through direct connections of the general
                        {
                            connectionFound = false;
                            for (unsigned int x = 0; x < graph.size(); x++) //find the generals connections in the graph
                            {
                                if (graph[k][j].getData() == graph[x][0].data && connectionFound == false)
                                {
                                    if(graph[x][0].visited == true)
                                    {
                                        if(graph[x][0].getData() == general.data)
                                            break;
                                        else
                                        {
                                            if (graph[k][0].isFaulty == true && order == 1)
                                                graph[x][0].ordersRecieved.push_back(0);
                                            else if (graph[k][0].isFaulty == true && order == 0)
                                                graph[x][0].ordersRecieved.push_back(1);
                                            else
                                                graph[x][0].ordersRecieved.push_back(order); //communicate the order
                                        }
                                    }
                                    else {
                                        connectionFound = true;
                                        if (graph[k][0].isFaulty == true && order == 1)
                                            graph[x][0].ordersRecieved.push_back(0);
                                        else if (graph[k][0].isFaulty == true && order == 0)
                                            graph[x][0].ordersRecieved.push_back(1);
                                        else
                                            graph[x][0].ordersRecieved.push_back(order); //communicate the order
                                        Q.push(graph[k][j]); //enqueue u
                                        graph[x][0].visited = true;
                                    }
                                }
                                else if (connectionFound == true)
                                    break;
                            }
                        }
                    }
                    else if(foundGeneral == true || connectionFound == true)
                        break;
                }
            }
        }
    }
}

#endif //INC_20S_3353_PA02_GRAPH_H
