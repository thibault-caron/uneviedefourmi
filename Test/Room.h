//
// Created by gravy on 20/05/2025.
//

#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include "Ant.h"

class Ant;

class Room {
public:
    Room(std::string id, int size_max, int ants = 0);
    ~Room();

    std::string getId() const;
    void addChildNode(Room* child);
    bool canAcceptAnt() const;
    // void display(int depth = 0) const;
    void addAnt(Ant* ant);
    void removeAnt();


private:
    std::string const id_room;
    int const ANTS_MAX;
    int ants_inside = 0;
    // std::deque<Ant*> ants;
    // std::vector<Room*> children;
};

#endif //ROOM_H
