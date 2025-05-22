//
// Created by gravy on 20/05/2025.
//

#include "Ant.h"
#include "Room.h"
#include "Anthill.h"

Ant::Ant(std::string id, Room* current, Room* previous)
    : id_ant(id), current_room(current), previous_room(previous) {}

Ant::~Ant() {}



void Ant::moves(Room* new_room) {
    previous_room = current_room;
    current_room = new_room;
}

void Ant::displayMovement() const {
    std::cout << id_ant << " - ";
    if (previous_room)
        std::cout << previous_room->getId();
    else
        std::cout << "None";
    std::cout << " - ";
    if  (current_room)
        std::cout << current_room->getId();
    else
        std::cout << "None";
    std::cout << std::endl;
}
