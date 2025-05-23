//
// Created by gravy on 20/05/2025.
//

#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <deque>
#include <vector>
#include <set>
#include "Ant.h"


class Ant;

class Room {
public:
    Room(const std::string& id, int size_max, int ants = 0);
    ~Room();

    std::string getId() const;
    Ant* getFirstAnt() const;
    void addChildNode(Room* child);
    bool hasAnts() const;
    bool canAcceptAnt() const;
    void display(int depth, std::set<const Room*>& visited) const;
    void addAnt(Ant* ant);
    void removeAnt();


private:
    std::string const id_room;
    int const ANTS_MAX;
    int ants_inside = 0;
    std::deque<Ant*> ants;
    std::vector<Room*> children;
};

#endif //ROOM_H
