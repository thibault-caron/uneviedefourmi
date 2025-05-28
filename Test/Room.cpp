//
// Created by gravy on 20/05/2025.
//

#include "Room.h"
#include "Ant.h"
#include "Anthill.h"

/**
 * @brief Constructs a Room object with the given parameters.
 *
 * Initializes the room ID, max capacity, and optionally, the number of ants inside.
 */
Room::Room(const std::string& id, int size_max, int ants)
    : id_room(id), ANTS_MAX(size_max), ants_inside(ants) {

}

/**
 * @brief Returns the room ID.
 */
std::string Room::getId() const {
    return id_room;
}

/**
 * @brief Retrieves the first ant in the room's deque.
 *
 * @return Pointer to the first Ant if present, otherwise nullptr.
 */
Ant* Room::getFirstAnt() const {
    if (!ants.empty()) {
        return ants.front();
    } else {
        return nullptr;
    }
}

/**
 * @brief Room destructor.
 *
 * Frees memory for all ants currently inside the room.
 */
Room::~Room() {
    for (Ant* ant : ants) {
        delete ant;
    }
    ants.clear();
}

/**
 * @brief Adds a child Room to this room's list of connections.
 *
 * @param child A pointer to the child Room.
 */
void Room::addChildNode(Room *child) {
    children.push_back(child);
}

/**
 * @brief Checks whether the room contains any ants.
 *
 * @return True if there is at least one ant, false otherwise.
 */
bool Room::hasAnts() const {
    return !ants.empty();
}

int Room::getAntsInside() const {
    return ants_inside;
}

int Room::getCapacity() {
    return ANTS_MAX;
}

/**
 * @brief Determines if the room has capacity to accept another ant.
 *
 * @return True if ants_inside is less than ANTS_MAX.
 */
bool Room::canAcceptAnt() const {
    return ants_inside < ANTS_MAX;
}

/**
 * @brief Adds an Ant to the room if there is available space.
 *
 * @param ant Pointer to the Ant to be added.
 */
void Room::addAnt(Ant* ant) {
    if (canAcceptAnt()) {
        ants_inside += 1;
        ants.push_back(ant);
    } else {
        std::cout << "Room " << id_room << " is full!" << std::endl;
    }

}

/**
 * @brief Removes the first Ant in the room.
 *
 * Does not delete the Ant, only removes the pointer.
 */
void Room::removeAnt() {
    if (!ants.empty()) {
        ants_inside -= 1;
        ants.pop_front();
    }
}

/**
 * @brief Recursively prints the structure of the room and its children.
 *
 * @param depth The indentation level used for pretty printing.
 * @param visited A set of already printed rooms to avoid infinite loops.
 */
void Room::display(int depth, std::set<const Room*>& visited) const {
    // Avoid infinite recursion by checking if the room was already visited.
    if (visited.count(this)) return;
    visited.insert(this);

    for (int i = 0; i<depth; i++) std::cout << " ";

    std::cout << "-Room \"" << id_room << "\""
    << " | Ants: " << ants_inside << "/" << ANTS_MAX;

    if (!children.empty()) {
        std::cout << " | Children: ";
        for (size_t i = 0; i < children.size(); i++) {
            std::cout << children[i]->getId();
            if (i != children.size()-1) std::cout << ", ";
        }
    }

    std::cout << std::endl;

    // Recursively display child rooms
    for (auto child : children) {
        child->display(depth + 2, visited);
    }
}

std::vector<Path> Room::findAllPaths(Room* targetRoom, std::set<const Room*>& visited, Path path) {

    std::vector<Path> allPaths;

    if (visited.count(this)) return allPaths;

    path.capacityMinimum = std::min(path.capacityMinimum, ANTS_MAX);
    path.path.push_back(this);

    visited.insert(this);

    if (this == targetRoom) {
        allPaths.push_back(path);
    } else {
        for (auto child : children) {
            std::set<const Room*> newVisited = visited;
            auto childPaths = child->findAllPaths(targetRoom, newVisited, path);
            allPaths.insert(allPaths.end(), childPaths.begin(), childPaths.end());
        }
    }

    return allPaths;
}



