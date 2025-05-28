//
// Created by gravy on 20/05/2025.
//

#ifndef ANTHILL_H
#define ANTHILL_H

#include <iostream>
#include <string>
#include <vector>
#include "Room.h"


/**
 * @class Anthill
 * @brief Represents an anthill composed of rooms connected together containing ants.
 *
 * This class handles loading rooms and connections from files,
 * managing ants, and displaying the anthill's structure.
 */
class Anthill {
public:
    /**
     * @brief Constructs an Anthill by reading initial data from a file.
     *
     * The file must start with the number of rooms and ants:
     * - Line 1: "r=<number_of_rooms>"
     * - Line 2: "f=<number_of_ants>"
     *
     * Initializes the start room "Sv" and places all ants there.
     *
     * @param filename The path to the file containing initial anthill data.
     * @throws std::runtime_error if the file cannot be opened or the format is invalid.
     */
    explicit Anthill(const std::string& filename);

    /**
     * @brief Destructor frees dynamically allocated rooms.
     */
    ~Anthill();

    /**
     * @brief Loads rooms from a file.
     *
     * The file format expects each room on a separate line, optionally with a capacity:
     *   RoomID {capacity}
     * If no capacity is specified, the default capacity is 1.
     * Adds a destination room "Sd" after loading all rooms.
     *
     * @param filename The path to the file containing room definitions.
     * @throws std::runtime_error if the file cannot be opened.
     */
    void loadRooms(const std::string& filename);

    /**
     * @brief Loads connections (edges) between rooms from a file.
     *
     * The file format expects connections in the form:
     *   RoomID1 - RoomID2
     * Connections are bidirectional.
     *
     * @param filename The path to the file containing room connections.
     * @throws std::runtime_error if the file cannot be opened.
     */
    void loadConnections(const std::string& filename) const;

    /**
     * @brief Displays a map of the anthill, starting from the first room.
     *
     * Uses depth-first traversal to print connected rooms.
     */
    void displayAnthill() const;

    /**
     * @brief Finds a room by its identifier.
     *
     * @param id The identifier of the room to find.
     * @return Pointer to the Room if found, nullptr otherwise.
     */
    Room* findRoomById(const std::string& id) const;

    /**
     * @brief Moves the first ant from the origin room to the direction room.
     *
     * Checks if the origin room contains ants and if the destination can accept more ants.
     * Prints appropriate messages if the move is not possible.
     *
     * @param origin_room Pointer to the room where the ant currently is.
     * @param direction_room Pointer to the room where the ant should move.
     */
    void movesAnt(Room* origin_room, Room* direction_room);

    bool haveCommonRooms(const Path& path1, const Path& path2);

    std::vector<Path> analyzePaths(const std::vector<Path>& allPaths);



private:
    int room_count;                ///< Number of rooms in the anthill
    int ant_count;                 ///< Number of ants in the anthill
    std::vector<Room*> rooms;      ///< List of all rooms in the anthill
};

#endif //ANTHILL_H
