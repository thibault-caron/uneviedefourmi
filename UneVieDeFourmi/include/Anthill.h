
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
    void antMovementDisplay(Room* origin_room, Room* direction_room);

    /**
     * @brief Displays the best solution by showing ant movements step by step.
     *
     * Simulates and displays the movement of ants through the optimal paths,
     * showing each step of the solution with the format "E<step_number>".
     * Continues until all ants have reached the destination room.
     */
    void displayBestSolution();

    /**
     * @brief Moves an ant from one room to another without displaying the movement.
     *
     * @param origin_room Pointer to the room where the ant currently is
     * @param direction_room Pointer to the room where the ant should move
     */
    void movesAnt(Room* origin_room, Room* direction_room);

    /**
     * @brief Resets the movement flag for all ants in the anthill.
     *
     * Sets all ants' canMove flag to true, allowing them to move in the next step.
     */
    void resetAllAntsCanMove();

    /**
     * @brief Simulates ant movement through given paths to count required steps.
     *
     * @param start Pointer to the start room
     * @param end Pointer to the end room
     * @return Number of steps required to move all ants to the end room
     */
    int simulateAntsMovement(Room* start, Room* end);

    /**
     * @brief Searches and stores all possible paths from start to end room.
     *
     * Uses depth-first search to find all possible paths from "Sv" to "Sd".
     * Stores results in the allPaths member variable.
     */
    void searchAllPaths();

    /**
     * @brief Gets the vector of all found paths.
     *
     * @return Constant reference to the vector containing all paths
     */
    const std::vector<Path>& getAllPaths() const;

    /**
     * @brief Gets the vector of optimal paths.
     *
     * @return Constant reference to the vector containing optimal paths
     */
    const std::vector<Path>& getOptimalPaths() const;

    /**
     * @brief Sorts all paths by capacity (descending) and length (ascending).
     *
     * Orders paths primarily by their minimum capacity in descending order,
     * and secondarily by their length in ascending order.
     */
    void sortAllPaths();

    /**
     * @brief Finds the optimal combination of paths for ant movement.
     *
     * Tests different combinations of paths to find the one that requires
     * the minimum number of steps to move all ants to the destination.
     * Stores the result in the optimalPaths member variable.
     */
    void findOptimalPaths();

    /**
     * @brief Displays a list of paths with their properties.
     *
     * @param paths Vector of paths to display
     * @param namePaths Name/description of the path set being displayed
     */
    void displayPaths(const std::vector<Path>& paths, const std::string& namePaths) const;

private:
    int room_count;                  ///< Number of rooms in the anthill
    int ant_count;                   ///< Number of ants in the anthill
    std::vector<Room*> rooms;        ///< Vector containing all rooms in the anthill
    std::vector<Path> allPaths;      ///< Vector containing all possible paths from start to end
    std::vector<Path> optimalPaths;  ///< Vector containing the selected optimal paths for the solution
};

#endif //ANTHILL_H
