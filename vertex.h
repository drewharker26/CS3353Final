//
// Created by Drew Harker on 3/6/20.
//

#ifndef INC_20S_3353_PA02_VERTEX_H
#define INC_20S_3353_PA02_VERTEX_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template<class T>
class Vertex {
public:
    Vertex() = default;
    Vertex(T data);
    T getData() const;
    int getMajority();

    T data;
    bool visited = false;
    bool isFaulty = false;
    int generalOrder = 0;
    vector<int> ordersRecieved; //attack = 1, retreat = 0
};

template<class T>
Vertex<T>::Vertex(T data) {
    this->data = data;
}

template <class T>
T Vertex<T>::getData() const {
    return data;
}

template<class T>
int Vertex<T>::getMajority()
{
    int attackFreq = count(ordersRecieved.begin(), ordersRecieved.end(), 1);
    int retreatFreq = count(ordersRecieved.begin(), ordersRecieved.end(), 0);
    if(attackFreq > retreatFreq)
        return 1;
    else if(retreatFreq > attackFreq)
        return 0;
    else
        return 2; //this should only happen if the attack orders and retreat orders are equal
}


#endif //INC_20S_3353_PA02_VERTEX_H
