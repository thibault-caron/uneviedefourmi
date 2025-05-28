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
#include "Path.h"


class Ant;

/**
 * @class Room
 * @brief Represents a room in the anthill that can contain ants and link to child rooms.
 *
 * Each room has a unique identifier, a maximum number of ants that can be stored, a current number of ants, a list of ants, and a list of child rooms.
 */
class Room {
public:
    /**
     * @brief Constructs a Room.
     * @param id Identifier for the room.
     * @param size_max Maximum number of ants the room can hold.
     * @param ants Initial number of ants in the room (default is 0).
     */
    Room(const std::string& id, int size_max, int ants = 0);

    /**
     * @brief Destroys the Room.
     * Deletes all ants currently in the room
     */
    ~Room();

    /**
     * @brief Returns the ID of the room.
     * @return Room ID as a string.
     */
    std::string getId() const;

    /**
     * @brief Gets the first ant in the room.
     * @return Pointer to the first ant, or nullptr if the room is empty.
     */
    Ant* getFirstAnt() const;

    /**
     * @brief Adds a child room as a connection.
     * @param child Pointer to the child room.
     */
    void addChildNode(Room* child);

    /**
     * @brief Checks if the room contains any ants.
     * @return True if least one ant is present, false otherwise.
     */
    bool hasAnts() const;

    /**
     * @brief Checks if the room can accept a new ant.
     * @return True if the number of ants is lower than the maximum capacity.
     */
    bool canAcceptAnt() const;

    /**
     * @brief Displays the room's information and its child rooms recursively.
     * @param depth Indentation level for pretty-printing the hierarchy.
     * @param visited A set to avoid printing the same room multiple times.
     */
    void display(int depth, std::set<const Room*>& visited) const;

    /**
     * @brief Adds an ant to tthe room.
     * @param ant Pointer to the ant to be added.
     */
    void addAnt(Ant* ant);

    /**
     * @brief Removes the first ant from the room.
     */
    void removeAnt();

    std::vector<Path> findAllPaths(Room* targetRoom, std::set<const Room*>& visited, Path path);

    std::vector<Path> findAllPaths(Room* targetRoom, std::set<const Room*>& visited) {
        return findAllPaths(targetRoom, visited, Path(ANTS_MAX));
    };



private:
    std::string const id_room;         ///< Unique identifier for the room.
    int const ANTS_MAX;                ///< Maximum number of ants the room can hold.
    int ants_inside = 0;               ///< Current number of ants in the room.
    std::deque<Ant*> ants;             ///< Queue of pointers to ants in the room.
    std::vector<Room*> children;       ///< List of connected child rooms.
};

#endif //ROOM_H
