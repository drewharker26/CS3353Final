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
    void lsp(ofstream &file, U general, int order);
    void lsp2(ofstream &file, U general, int numTraitors, int order);
    void printLSP(ofstream &file, U general);
private:
    int processResults(ofstream &file, U general);
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
void Graph<U, T>::printLSP(ofstream &file, U general) {
    int finalOrder = processResults(file, general); //recieves ultimate decision
    if(finalOrder == 1)
        file << "We have been ordered to attack!" << endl;
    else if(finalOrder == 0)
        file << "We have been ordered to retreat!" << endl;
    else
        file << "The number of attack orders exactly equals retreat orders!" << endl;

}

template<class U, class T>
int Graph<U, T>::processResults(ofstream &file, U general)
{
    vector<int> results;
    for(int y=0; y < graph.size(); y++) //loop through graph vertices excluding general and determine majority for each
    {
        if(graph[y][0].getData() != general.data) { //we exclude the general as he should not be recieving any orders
            int result = graph[y][0].getMajority();
            results.push_back(result); //add each individual result to overall result vector
        }
    }
    file << "Majority Vector in Alphabetical Order: " << endl;
    for (const auto &p : results) {
        file << results[p]  << endl;
    }
    int attackFreq = count(results.begin(), results.end(), 1); //compute number of attacks and retreats
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
                                    if (graph[x][0].visited == true) { //check if repeat visit
                                        if (graph[x][0].getData() == general.data) //we dont want to send orders back to the general
                                            break;
                                        else {
                                            if (graph[k][0].isFaulty == true && order == 1) //if node is faulty, reverse order
                                                graph[x][0].ordersRecieved.push_back(0);
                                            else if (graph[k][0].isFaulty == true && order == 0)
                                                graph[x][0].ordersRecieved.push_back(1);
                                            else
                                                graph[x][0].ordersRecieved.push_back(order); //communicate the order
                                        }
                                    } else { //if node has nto been previously visited
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
    int finalOrder = processResults(file, general);
    if(finalOrder == 1)
        file << "We have been ordered to attack!" << endl;
    else if(finalOrder == 0)
        file << "We have been ordered to retreat!" << endl;
    else
        file << "The number of attack orders exactly equals retreat orders!" << endl;
}


template<class U, class T>
void Graph<U, T>::lsp(ofstream &file, U general, int order) { //this executes if there are no known traitors
    for (unsigned int i = 0; i < graph.size(); i++) { //reset all relevant values due to previous algo
        graph[i][0].ordersRecieved.clear();
        graph[i][0].generalOrder = 0;
    }
    bool foundGeneral = false;
    bool connectionFound = false;
    for (unsigned int i = 0; i < graph.size(); i++) { //find the vertex matching the general in the graph
        if (graph[i][0].getData() == general.data) {
            foundGeneral = false;
            connectionFound = false;
            for (unsigned int k = 0; k < graph.size(); k++) //find general we are currently at
            {
                if (graph[k][0].getData() == general.data && foundGeneral == false && connectionFound == false) {
                    foundGeneral = true;
                    for (unsigned int j = 1; j < graph[k].size(); j++) //loop through direct connections of the general
                    {
                        connectionFound = false;
                        for (unsigned int x = 0; x < graph.size(); x++) //find the generals connections in the graph
                        {
                            if (graph[k][j].getData() == graph[x][0].data && connectionFound == false) {
                                connectionFound = true;
                                graph[x][0].ordersRecieved.push_back(0);
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

template<class U, class T>
void Graph<U, T>::lsp2(ofstream &file, U general, int numTraitors, int order) { //this executes recursivly while traitors exist
    for (unsigned int i = 0; i < graph.size(); i++) { //reset all relevant values due to previous algo
        graph[i][0].ordersRecieved.clear();
        graph[i][0].generalOrder = 0;
    }
    bool foundGeneral = false;
    bool connectionFound = false;
    for (unsigned int i = 0; i < graph.size(); i++) { //find the vertex matching the general in the graph
        if (graph[i][0].getData() == general.data) {
            foundGeneral = false;
            connectionFound = false;
            for (unsigned int k = 0; k < graph.size(); k++) //find general we are currently at
            {
                if (graph[k][0].getData() == general.data && foundGeneral == false && connectionFound == false) {
                    foundGeneral = true;
                    for (unsigned int j = 1; j < graph[k].size(); j++) //loop through direct connections of the general
                    {
                        connectionFound = false;
                        for (unsigned int x = 0; x < graph.size(); x++) //find the generals connections in the graph
                        {
                            if (graph[k][j].getData() == graph[x][0].data && connectionFound == false) {
                                connectionFound = true;
                                graph[x][0].generalOrder = order;
                                if (graph[k][0].isFaulty == true && order == 1)
                                    graph[x][0].ordersRecieved.push_back(0);
                                else if (graph[k][0].isFaulty == true && order == 0)
                                    graph[x][0].ordersRecieved.push_back(1);
                                else
                                    graph[x][0].ordersRecieved.push_back(graph[x][0].generalOrder); //communicate the order
                            } else if (connectionFound == true)
                                break;
                        }
                    }
                } else if (foundGeneral == true || connectionFound == true)
                    break;
            }
        }
    }
    for (unsigned int i = 0; i < graph.size(); i++) { //loop through lieutenants, calling function recursively as long as there are traitors
        if (graph[i][0].getData() != general.data && numTraitors > 0) {
            lsp2(file, graph[i][0], numTraitors - 1, graph[i][0].generalOrder);
        }
    }
}

#endif //INC_20S_3353_PA02_GRAPH_H
