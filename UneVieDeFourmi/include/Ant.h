
#ifndef ANT_H
#define ANT_H

#include <string>
#include "Room.h"

class Room;

/**
 * @class Ant
 * @brief Represents an ant within the anthill system.
 *
 * Each ant has a unique identifier, a current room, and an optional previous room.
 * It can move between rooms and display its movement history.
 */
class Ant {
public:
    /**
     * @brief Constructs an Ant.
     * @param id Unique identifier for the ant.
     * @param current Pointer to the current room.
     * @param previous Pointer to the previous room (optional).
     */
    Ant(std::string id, Room* current, Room* previous = nullptr);

    /**
     * @brief Destroys the Ant.
     */
    ~Ant();

    std::string getId() const;

    /**
     * @brief Moves the ant to a new room.
     * Updates the current and previous room pointers.
     * @param new_room Pointer to the destination room.
     */
    void moves(Room* new_room);

    /**
     * @brief Gets whether movement is currently possible
     * @return true if movement is allowed, false otherwise
     */
    bool getCanMove() const;

    /**
     * @brief Toggles the movement capability of the ant
     *
     * Switches the ant's ability to move between enabled and disabled states.
     * If the ant can currently move, it will be disabled from moving.
     * If the ant cannot currently move, it will be enabled to move.
     */
    void toggleCanMove();

    /**
     * @brief Displays the ant's movement.
     * Shows: id - previous room - current room.
     */
    void displayMovement() const;

private:
    std::string const id_ant;   ///< Unique identifier for the ant.
    Room* current_room;         ///< Pointer to the current room.
    Room* previous_room;        ///< Pointer to the previous room (optional).
    bool canMove;               ///< Flag indicating if the ant can move in the current turn
};

#endif //ANT_H
