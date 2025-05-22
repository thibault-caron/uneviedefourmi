//
// Created by gravy on 20/05/2025.
//

#ifndef ANT_H
#define ANT_H

#include <string>
#include "Room.h"

class Room;

class Ant {
public:
    Ant(std::string id, Room* current, Room* previous = nullptr);
    ~Ant();

    void moves(Room* new_room);
    void displayMovement() const;

private:
    std::string const id_ant;
    Room* current_room;
    Room* previous_room;

};

#endif //ANT_H
