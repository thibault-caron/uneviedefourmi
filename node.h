//
// Created by porta on 20/05/2025.
//

#ifndef NODE_H
#define NODE_H

#include <string>
#include <map>

class Node {
public:
    //Constructor
    Node(std::string& name, int capacity);
    bool canAcceptAnt() const;
    void enterAnt();
    void leaveAnt();
    void addNeighbors(Node* neighbor, double cost);
    Node* chooseNext(); // linked to the cost
    int getCurrentLoad() const;
    const std::string& getName() const;

private:
    std::string name;
    int capacity;
    int currentLoad = 0;
    std::map<Node*, double> neighbors;
};

#endif //NODE_H
