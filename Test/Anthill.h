//
// Created by gravy on 20/05/2025.
//

#ifndef ANTHILL_H
#define ANTHILL_H

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include "Room.h"
#include "Ant.h"

class Anthill {
public:
    Anthill();
    ~Anthill();

    void buildAnthill();
    void buildAntColony();




private:
    int ant_count;
    int const room_number;
    std::vector<Room*> rooms;
    std::deque<Ant*> ants;

};

#endif //ANTHILL_H
