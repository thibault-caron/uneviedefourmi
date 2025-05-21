//
// Created by porta on 20/05/2025.
//

#include "node.h"

bool Node::canAcceptAnt() const {
    return currentLoad < capacity;
}

void Node::enterAnt() {
    currentLoad++;
}

void Node::leaveAnt() {
    if (currentLoad > 0) {
        currentLoad--;
    }
}

void Node::addNeighbors(Node* neighbor, double cost) {
    neighbors[neighbor] = cost;

