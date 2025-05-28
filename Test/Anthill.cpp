//
// Created by gravy on 20/05/2025.
//
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Room.h"
#include "Ant.h"
#include "Anthill.h"

#include <algorithm>
#include <thread>

/**
 * @brief Constructs an Anthill by reading initial room and ant counts from a file.
 *
 * The file format requires:
 * - First line: "r=<number_of_rooms>"
 * - Second line: "f=<number_of_ants>"
 *
 * Initializes the start room "Sv" and populates it with all ants.
 *
 * @param filename The file path to read initial data from.
 * @throws std::runtime_error If a file cannot be opened or the format is invalid.
 */
Anthill::Anthill(const std::string& filename) : room_count(0), ant_count(0) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + filename);
    }
    std::string line;

    // Read and parse the number of rooms
    if (std::getline(file, line)) {
        if (line.substr(0, 2) == "r=") {
            room_count = std::stoi(line.substr(2));
        } else {
            throw std::runtime_error("Invalid file format : missing room count");
        }
    }

    // Read and parse the number of ants
    if (std::getline(file, line)) {
        if (line.substr(0, 2) == "f=") {
            ant_count = std::stoi(line.substr(2));
        } else {
            throw std::runtime_error("Invalid file format : missing ant count");
        }
    }

    // Create the start room "Sv" with capacity equal to the number of ants
    rooms.push_back(new Room("Sv", ant_count));

    // Create ants and add them to the start room
    for (int i = 1; i <= ant_count; i++) {
        rooms[0]->addAnt(new Ant("f" + std::to_string(i),rooms[0]));
    }

    file.close();
}

/**
 * @brief Destructor that deallocates all dynamically allocated rooms.
 */
Anthill::~Anthill() {
    for (Room* room : rooms) {
        delete room;
    }
    rooms.clear();
}

/**
 * @brief Loads additional rooms from a specified file.
 *
 * The file format expects each line to define a room by its identifier,
 * optionally followed by a capacity in braces, e.g.:
 *   RoomID {capacity}
 * If no capacity is specified, defaults to 1.
 * Adds a destination room "Sd" with capacity equal to the number of ants.
 *
 * @param filename The path to the file containing room definitions.
 * @throws std::runtime_error If the file cannot be opened.
 */
void Anthill::loadRooms(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file" + filename);
    }

    std::string line;


    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        // Ignore lines with '-' or '=' which are connection lines or metadata
        if (line.find("-") == std::string::npos && line.find("=") == std::string::npos) {
            std::string identifier;
            int capacity;

            std::istringstream linestream(line);
            linestream >> identifier;

            // Parse capacity if present between { }
            size_t open = line.find('{');
            size_t close = line.find('}');
            if (open != std::string::npos && close != std::string::npos && close > open) {
                std::string value = line.substr(open + 1, close - open - 1);
                capacity = std::stoi(value);
            } else {
                capacity = 1; // default capacity
            }

            if (!identifier.empty()) {
                rooms.push_back(new Room(identifier, capacity));
            }
        }
    }
    file.close();

    // Add the destination room "Sd" with capacity equal to ant_count
    rooms.push_back(new Room("Sd", ant_count));
}

/**
 * @brief Loads bidirectional connections between rooms from a file.
 *
 * The file format expects lines in the form:
 *   RoomID1 - RoomID2
 *
 * Adds connections between the two rooms both ways.
 *
 * @param filename The path to the file containing connection definitions.
 * @throws std::runtime_error If the file cannot be opened.
 */
void Anthill::loadConnections(const std::string &filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file" + filename);
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        if (line.find("-") != std::string::npos) {
            std::istringstream linestream(line);
            std::string from, dash, to;
            linestream >> from >> dash >> to;

            if (!from.empty() && !to.empty() && dash == "-") {
                Room* parent = findRoomById(from);
                Room* child = findRoomById(to);

                if (parent && child) {
                    parent->addChildNode(child);
                    // child->addChildNode(parent);
                }
            }
        }

    }
    file.close();
}

/**
 * @brief Finds a room by its identifier.
 *
 * @param id The identifier string of the room to find.
 * @return Pointer to the Room if found, nullptr otherwise.
 */
Room* Anthill::findRoomById(const std::string& id) const {
    for (Room* room : rooms) {
        if (room->getId() == id) {
            return room;
        }
    }
    return nullptr;
}

/**
 * @brief Displays the anthill starting from the first room.
 *
 * Performs a depth-first traversal to show all connected rooms.
 */
void Anthill::displayAnthill() const {
    if (rooms.empty()) {
        std::cout << "No rooms found" << std::endl;
        return;
    }

    std::set<const Room*> visited;
    std::cout << "=== Map of the anthill ===" << std::endl;
    rooms[0]->display(0, visited);
}


/**
 * @brief Moves an ant from the origin room to the direction room.
 *
 * Checks if the origin room contains ants and if the destination room can accept more ants.
 * If successful, moves the first ant, updates its location, and displays the movement.
 * Otherwise, prints appropriate error messages.
 *
 * @param origin_room Pointer to the room where the ant currently is.
 * @param direction_room Pointer to the room where the ant should move.
 */
void Anthill::movesAnt(Room* origin_room, Room* direction_room) {
    // if (origin_room->hasAnts()) {
    //     if (direction_room->canAcceptAnt()) {
            Ant* ant = origin_room->getFirstAnt();
            direction_room->addAnt(ant);
            ant->moves(direction_room);
            ant->displayMovement();
            origin_room->removeAnt();
    //     } else {
    //         std::cout << "Room " << direction_room->getId() << " is full!" << std::endl;
    //     }
    // }
}

bool Anthill::haveCommonRooms(const Path& path1, const Path& path2) {
    for (const auto* room1 : path1.path) {
        for (const auto* room2 : path2.path) {
            if (room1 == room2 &&
                room1->getId() != "Sv" &&
                    room1->getId() != "Sd") {
                return true;
            }
        }
    }
    return false;
}

std::vector<Path> Anthill::analyzePaths(const std::vector<Path>& allPaths) {
    if (allPaths.empty()) {
        return {};
    }

    int maxCapacity = 0;
    for (const auto& path : allPaths) {
        maxCapacity = std::max(maxCapacity, path.capacityMinimum);
    }

    std::vector<Path> bestPaths;
    for (const auto& path : allPaths) {
        if (path.capacityMinimum == maxCapacity) {
            bestPaths.push_back(path);
        }
    }

    if (bestPaths.size() == 1) {
        return bestPaths;
    }

    std::vector<Path> disjointPaths;
    for (const auto& path : bestPaths) {
        bool isDisjoint = true;
        for (const auto& existingPath : disjointPaths) {
            if (haveCommonRooms(path, existingPath)) {
                isDisjoint = false;
                break;
            }
        }
        if (isDisjoint) {
            disjointPaths.push_back(path);
        }
    }

    if (disjointPaths.empty()) {
        auto shortestPath = std::min_element(bestPaths.begin(), bestPaths.end(), [](const Path& p1, const Path& p2) {
            return p1.path.size() < p2.path.size();
        });
        return {*shortestPath};
    }

    size_t firstLength = disjointPaths[0].path.size();
    bool sameLengths = std::all_of(disjointPaths.begin(), disjointPaths.end(),
        [firstLength](const Path& p) {
            return p.path.size() == firstLength;
        });

    if (sameLengths) {
        return disjointPaths;
    } else {
        auto shortestPath = std::min_element(disjointPaths.begin(), disjointPaths.end(),
            [](const Path& a, const Path& b) {
                return a.path.size() < b.path.size();
            });
        return {*shortestPath};
    }
}



