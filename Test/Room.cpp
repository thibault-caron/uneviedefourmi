//
// Created by gravy on 20/05/2025.
//

#include "Room.h"
#include "Ant.h"
#include "Anthill.h"

Room::Room(const std::string& id, int size_max, int ants)
    : id_room(id), ANTS_MAX(size_max), ants_inside(ants) {}



Room::~Room() {
    // Free memory for each ant in the room
    for (Ant* ant : ants) {
        delete ant;
    }
    // Clear the vector of pointers
    ants.clear();
}



std::string Room::getId() const {
    // Return a copy of the ID room
    return id_room;
}



std::deque<Ant*> Room::getAnts() const {
    // Return a copy of the deque containing ant pointers
    return ants;
}



Ant* Room::getFirstAnt() const {
    // Return the first ant in the room, or nullptr if the room is empty
    if (!ants.empty()) {
        return ants.front();
    } else {
        return nullptr;
    }
}




void Room::addChildNode(Room *child) {
    // Add the given room as a connected neighbor
    children.push_back(child);
}



bool Room::hasAnts() const {
    // Return true if the room contains at least one ant, false otherwise
    return !ants.empty();
}



int Room::getAntsInside() const {
    // Return the current number of ants in the room
    return ants_inside;
}



int Room::getCapacity() {
    // Return the maximum number of ants that can be held in this room
    return ANTS_MAX;
}



bool Room::canAcceptAnt() const {
    // Checks if the room can accept a new ant by comparing
    // the current number of ants with the maximum capacity
    return ants_inside < ANTS_MAX;
}



void Room::addAnt(Ant* ant) {
    // Check if the room can accept another ant
    if (canAcceptAnt()) {
        ants_inside += 1;
        // Add the ant to the room's deque
        ants.push_back(ant);
    } else {
        std::cout << "Room " << id_room << " is full!" << std::endl;
    }

}



void Room::removeAnt() {
    // check if there are any ants to remove
    if (!ants.empty()) {
        ants_inside -= 1;
        // Remove the first ant from the deque
        ants.pop_front();
    }
}



void Room::display(int depth, std::set<const Room*>& visited) const {
    // Avoid infinite recursion by checking if the room was already visited.
    if (visited.count(this)) return;
    visited.insert(this);

    // Print indentation on the depth level
    for (int i = 0; i<depth; i++) std::cout << " ";

    // Display room information: ID and ant occupancy
    std::cout << "-Room \"" << id_room << "\""
    << " | Ants: " << ants_inside << "/" << ANTS_MAX;

    // If a room has child connections, display them
    if (!children.empty()) {
        std::cout << " | Children: ";
        for (size_t i = 0; i < children.size(); i++) {
            std::cout << children[i]->getId();
            // Add comma separator between children, except for the last one
            if (i != children.size()-1) std::cout << ", ";
        }
    }

    std::cout << std::endl;

    // Recursively display child rooms with increased indentation
    for (auto child : children) {
        child->display(depth + 2, visited);
    }
}



std::vector<Path> Room::findAllPaths(Room* targetRoom, std::set<const Room*>& visited, Path path) {
    // Initialize vector to store all possible paths
    std::vector<Path> allPaths;

    // If this room has already been visited, return an empty path list to avoid cycles
    if (visited.count(this)) return allPaths;

    // Update the path's minimum capacity considering this room's capacity
    path.capacityMinimum = std::min(path.capacityMinimum, ANTS_MAX);
    // Add the current room to the path
    path.path.push_back(this);

    // Mark this room as visited
    visited.insert(this);

    // If we reached the target room, add the current path to solutions
    if (this == targetRoom) {
        allPaths.push_back(path);
    } else {
        // Otherwise, recursively explore all child rooms
        for (auto child : children) {
            // Create a new visited set for each branch to allow different paths
            std::set<const Room*> newVisited = visited;
            // Recursively find paths from child to target
            auto childPaths = child->findAllPaths(targetRoom, newVisited, path);
            // Add all found paths to the result
            allPaths.insert(allPaths.end(), childPaths.begin(), childPaths.end());
        }
    }

    return allPaths;
}



