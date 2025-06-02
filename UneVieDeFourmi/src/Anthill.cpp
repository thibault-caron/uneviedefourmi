
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <thread>
#include "../include/Room.h"
#include "../include/Ant.h"
#include "../include/Anthill.h"



Anthill::Anthill(const std::string& filename) : room_count(0), ant_count(0) {
    // Open the configuration file
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + filename);
    }
    std::string line;

    // Read and parse the number of rooms (expected format: "r=X")
    if (std::getline(file, line)) {
        if (line.substr(0, 2) == "r=") {
            room_count = std::stoi(line.substr(2));
        } else {
            throw std::runtime_error("Invalid file format : missing room count");
        }
    }

    // Read and parse the number of ants (expected format: "f=X")
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



Anthill::~Anthill() {
    // Delete all dynamically allocated Room objects
    for (Room* room : rooms) {
        delete room;
    }
    // Clear the vector of pointers
    rooms.clear();
}



void Anthill::loadRooms(const std::string &filename) {
    // Open the configuration file
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file" + filename);
    }

    std::string line;

    // Process each line of the file
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        // Ignore lines with '-' or '=' which are connection lines or metadata
        if (line.find("-") == std::string::npos && line.find("=") == std::string::npos) {
            std::string identifier;
            int capacity;

            // Extract room identifier from a line
            std::istringstream linestream(line);
            linestream >> identifier;

            // Check for optional capacity specification {X}
            size_t open = line.find('{');
            size_t close = line.find('}');
            if (open != std::string::npos && close != std::string::npos && close > open) {
                // Extract and parse capacity value
                std::string value = line.substr(open + 1, close - open - 1);
                capacity = std::stoi(value);
            } else {
                capacity = 1; // default capacity
            }

            // Create a new room if the identifier is valid
            if (!identifier.empty()) {
                rooms.push_back(new Room(identifier, capacity));
            }
        }
    }
    file.close();

    // Add the destination room "Sd" with capacity equal to ant_count
    rooms.push_back(new Room("Sd", ant_count));
}



void Anthill::loadConnections(const std::string &filename) const {
    // Open the configuration file
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file" + filename);
    }

    std::string line;

    // Process each line of the file
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        // Look for lines containing the connection symbol '-'
        if (line.find("-") != std::string::npos) {
            std::istringstream linestream(line);
            std::string from, dash, to;
            linestream >> from >> dash >> to;

            // Validate connection format (RoomID1 - RoomID2)
            if (!from.empty() && !to.empty() && dash == "-") {
                // Find the rooms to connect
                Room* parent = findRoomById(from);
                Room* child = findRoomById(to);

                // Create a bidirectional connection between rooms
                if (parent && child) {
                    parent->addChildNode(child);
                    child->addChildNode(parent); // Make connection bidirectional
                }
            }
        }

    }
    file.close();
}



Room* Anthill::findRoomById(const std::string& id) const {
    // Search through all rooms in the anthill
    for (Room* room : rooms) {
        // Compare room ID with searched ID
        if (room->getId() == id) {
            return room; // Return room if found
        }
    }
    return nullptr; // Return null if room not found
}



void Anthill::displayAnthill() const {
    // Check if there are any rooms in the anthill
    if (rooms.empty()) {
        std::cout << "No rooms found" << std::endl;
        return;
    }

    // Create a set to track visited rooms during traversal and avoid infinite loops
    std::set<const Room*> visited;
    std::cout << "=== Map of the anthill ===" << std::endl;
    // Start a recursive display from the first room (index 0)
    rooms[0]->display(0, visited);
}



void Anthill::antMovementDisplay(Room* origin_room, Room* direction_room) {
    if (!origin_room || !direction_room) return;

    Ant* ant = origin_room->getFirstAnt();
    if (ant && ant->getCanMove() == true) {
        direction_room->addAnt(ant);
        ant->moves(direction_room);
        ant->displayMovement();
        origin_room->removeAnt();
    }
}



void Anthill::displayBestSolution() {
    // Check if we have any valid paths to use
    if (optimalPaths.empty()) {
        std::cout << "No valid paths found for ant movement" << std::endl;
        return;
    }

    // Get start and end rooms
    Room* start = findRoomById("Sv");
    Room* end = findRoomById("Sd");

    if (!start || !end) {
        throw std::runtime_error("Error: Unable to find start or end rooms");
    }

    int step = 1;
    bool someAntMoved;

    // Reset simulation by moving all ants back to the start room
    while (end->getAntsInside() > 0) {
        movesAnt(end, start);
    }

    do {
        // Display the current step number
        std::cout << "\n+++ E" << step << " +++" << std::endl;

        // Prepare for a new movement phase
        resetAllAntsCanMove();
        someAntMoved = false;

        // Process each optimal path
        for (const auto& path : optimalPaths) {
            // Move ants from end to start of each path
            for (int i = path.path.size() - 2; i >= 0; i--) {
                Room* previousRoom = const_cast<Room*>(path.path[i]);
                Room* currentRoom = const_cast<Room*>(path.path[i + 1]);

                // Calculate how many ants can move between these rooms
                int antsInPrevious = previousRoom->getAntsInside();
                int spaceInCurrent = currentRoom->getCapacity() - currentRoom->getAntsInside();
                int antsToMove = std::min(antsInPrevious, spaceInCurrent);

                // Move ants if possible
                if (antsToMove > 0) {
                    for (int j = 0; j < antsToMove; j++) {
                        antMovementDisplay(previousRoom, currentRoom);
                    }
                    someAntMoved = true;
                }
            }
        }
        step++;
    } while (someAntMoved); // Continue until no more movements are possible

    std::cout << "All ants have reached the dormitory!" << std::endl;
}



void Anthill::movesAnt(Room* origin_room, Room* direction_room) {
    // Verify that both room pointers are valid
    if (!origin_room || !direction_room) return;

    // Get the first available ant from the origin room
    Ant* ant = origin_room->getFirstAnt();

    // Check if there is an ant and if it's allowed to move
    if (ant && ant->getCanMove() == true) {
        // Perform the actual movement
        direction_room->addAnt(ant);   // Add ant to the new room
        origin_room->removeAnt();      // Remove ant from the old room
        ant->toggleCanMove();          // Mark ant as moved for this turn
    }
}



void Anthill::resetAllAntsCanMove () {
    // Iterate through all rooms in the anthill
    for (Room* room : rooms) {
        // For each ant in the current room
        for (Ant* ant : room->getAnts()) {
            if (ant->getCanMove() == false) {
                ant->toggleCanMove(); // Set canMove to true
            }
        }
    }
}



int Anthill::simulateAntsMovement(Room* start, Room* end) {
    int steps = 0;
    bool someAntMoved;

    // Reset simulation by moving all ants back to the start room
    while (end->getAntsInside() > 0) {
        this->movesAnt(end, start);
    }

    do {
        someAntMoved = false;
        this->resetAllAntsCanMove();

        // Check if all ants have reached the end room
        if (end->getAntsInside() ==  ant_count) {
            break;
        }

        // Try moving ants along each optimal path
        for (const auto& path : optimalPaths) {
            // Move ants from back to front of each path
            for (int i = path.path.size() - 2; i >= 0; i--) {
                Room* previousRoom = const_cast<Room*>(path.path[i]);
                Room* currentRoom = const_cast<Room*>(path.path[i + 1]);

                // Calculate how many ants can move
                int antsInPrevious = previousRoom->getAntsInside();
                int spaceInCurrent = currentRoom->getCapacity() - currentRoom->getAntsInside();

                int antsToMove = std::min(antsInPrevious, spaceInCurrent);

                // Move ants if possible
                if (antsToMove > 0) {
                    for (int j = 0; j < antsToMove; j++) {
                        this->movesAnt(previousRoom, currentRoom);
                        someAntMoved = true;
                    }
                }
            }
        }
        steps++;
    } while (someAntMoved); // Continue while ants are still moving

    return steps; // Return the total number of steps needed
}



void Anthill::searchAllPaths() {
    // Check if there are any rooms in the anthill
    if (rooms.empty()) {
        std::cout << "No rooms found" << std::endl;
        return;
    }

    // Get start and end rooms
    Room* start = findRoomById("Sv");
    Room* end = findRoomById("Sd");

    if (!start || !end) {
        throw std::runtime_error("Error: Unable to find start or end rooms");
    }

    // Create a set to track visited rooms during traversal and avoid infinite loops
    std::set<const Room*> visited;
    // Find and store all possible paths from start to end
    allPaths = start->findAllPaths(end, visited);
    std::cout << "All paths found" << std::endl;
}



const std::vector<Path>& Anthill::getAllPaths() const {
    // Return a copy of the allPaths vector
    return allPaths;
}



const std::vector<Path>& Anthill::getOptimalPaths() const {
    // Return a copy of the optimal paths vector
    return optimalPaths;
}



void Anthill::findOptimalPaths() {
    // Check if there are any paths to optimaze
    if (allPaths.empty()) {
        std::cout << "No paths found" << std::endl;
        return;
    }

    // Get start and end rooms
    Room* start = findRoomById("Sv");
    Room* end = findRoomById("Sd");

    if (!start || !end) {
        throw std::runtime_error("Error: Unable to find start or end rooms");
    }

    // Reset simulation by moving all ants back to the start room
    while (end->getAntsInside() > 0) {
        movesAnt(end, start);
    }

    int minimumSteps = INT_MAX;
    int bestPathCount = 0;
    int n_paths = 1;
    bool firstTry = true;

    // Try different combinations of paths
    do {
        // Clear and fill optimalPaths with the current number of paths
        optimalPaths.clear();
        for (int i = 0; i < n_paths && i < allPaths.size(); i++) {
            optimalPaths.push_back(allPaths[i]);
        }

        // Simulate movement with the current path combination
        int currentSteps = simulateAntsMovement(start, end);

        // Update the best solution if the current is better
        if (firstTry || currentSteps < minimumSteps) {
            minimumSteps = currentSteps;
            bestPathCount = n_paths;
            firstTry = false;
        }

        std::cout << "Test with " << optimalPaths.size() << " paths : " << currentSteps << " steps" << std::endl;
        n_paths++;
    } while (n_paths <= allPaths.size());

    // Store the best combination found
    optimalPaths.clear();
    for (int i = 0; i < bestPathCount; i++) {
        optimalPaths.push_back(allPaths[i]);
    }
}



void Anthill::sortAllPaths() {
    // Check if there are any paths to sort
    if (allPaths.empty()) {
        std::cout << "No paths found" << std::endl;
        return;
    }

    // Sort paths using a custom comparison lambda function
    std::sort(allPaths.begin(), allPaths.end(),
        [](const Path& a, const Path& b) {
            // Primary sort by minimum capacity (descending order)
            if (a.capacityMinimum != b.capacityMinimum) {
                return a.capacityMinimum > b.capacityMinimum;
            }
            // Secondary sort by length (ascending order)
            return a.path.size() < b.path.size();
        });

    std::cout << "All paths sorted" << std::endl;
}



void Anthill::displayPaths(const std::vector<Path>& paths, const std::string& namePaths) const {
    // Display the name and number of paths
    std::cout << namePaths << " : " << paths.size() << std::endl;

    // Iterate through each path in the collection
    for (const auto& path : paths) {
        // Display the path header with its minimum capacity
        std::cout << "Path (capacity " << path.capacityMinimum << ") : ";

        // Flag to handle arrow separator formatting
        bool first = true;

        // Display each room in the path
        for (const auto& room : path.path) {
            // Add an arrow separator between rooms, except for the first room
            if (!first) std::cout << " -> ";
            // Display room ID
            std::cout << room->getId();
            first = false;
        }
        std::cout << std::endl;
    }
}



