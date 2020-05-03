//
// Created by Drew Harker on 3/6/20.
//

#ifndef INC_20S_3353_PA02_VERTEX_H
#define INC_20S_3353_PA02_VERTEX_H

#include <iostream>

using namespace std;

template<class T>
class Vertex {
public:
    Vertex() = default;
    Vertex(T data);
    T getData() const;


    T data;
    int distance = 0;
    bool visited = false;
};

template<class T>
Vertex<T>::Vertex(T data) {
    this->data = data;
}

template <class T>
T Vertex<T>::getData() const {
    return data;
}

#endif //INC_20S_3353_PA02_VERTEX_H
