//
// Created by gravy on 20/05/2025.
//

#include "Ant.h"
#include "Room.h"
#include "Anthill.h"

/**
 *@brief Ant constructor.
 */
Ant::Ant(std::string id, Room* current, Room* previous)
    : id_ant(id), current_room(current), previous_room(previous) {}

/**
 *@brief Ant destructor.
 */
Ant::~Ant() {}

/**
 * @brief Moves the ant to a new room.
 *
 * the current room becomes the previous room, and the new room is set as the current room.
 */
void Ant::moves(Room* new_room) {
    previous_room = current_room;
    current_room = new_room;
}

/**
 * @brief Displays the ant's movement.
 * Shows: id - previous room ID - current room ID.
 * If either room is null, "None" is displayed instead.
 */
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
