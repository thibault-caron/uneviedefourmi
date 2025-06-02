
#include "../include/Ant.h"
#include "../include/Room.h"
#include "../include/Anthill.h"

Ant::Ant(std::string id, Room* current, Room* previous)
    : id_ant(id), current_room(current), previous_room(previous), canMove(true) {}



Ant::~Ant() {}



std::string Ant::getId() const {
    // Return a copy of the ID string
    return id_ant;
}



void Ant::moves(Room* new_room) {
    // update ant's current and previous room pointers
    previous_room = current_room;
    // Set the new room as current
    current_room = new_room;
    // Disable movement until the next turn
    canMove = false;
}



bool Ant::getCanMove() const {
    // Return the movement capability status of the ant
    return canMove;
}



void Ant::toggleCanMove() {
    // Toggle the movement capability between true and false
    canMove = !canMove;
}



void Ant::displayMovement() const {
    // Display ant ID
    std::cout << id_ant << " - ";
    // Display previous room ID or "None" if no previous room
    if (previous_room)
        std::cout << previous_room->getId();
    else
        std::cout << "None";
    std::cout << " - ";
    // Display current room ID or "None" if no current room
    if  (current_room)
        std::cout << current_room->getId();
    else
        std::cout << "None";
    std::cout << std::endl;
}
