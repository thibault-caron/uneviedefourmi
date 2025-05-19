#include "ants.hpp"
#include <fstream>
#include <sstream>

// Room implementation
Room::Room(const std::string& id, int capacity) : id(id), capacity(capacity) {}

void Room::addConnection(const std::string& roomId) {
    connections.push_back(roomId);
}

// Ant implementation
Ant::Ant(int id) : id(id), currentRoom("Sv") {}

void Ant::moveTo(const std::string& room) {
    path.push_back(room);
    currentRoom = room;
}

// AntColony implementation
AntColony::AntColony(const std::string& filename) {
    parseFile(filename);
}

void AntColony::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::string line;
    
    // Read the first line to get ant count
    if (std::getline(file, line)) {
        // Parse the f=X format
        if (line.substr(0, 2) == "f=") {
            antCount = std::stoi(line.substr(2));
        } else {
            throw std::runtime_error("Invalid file format: missing ant count");
        }
    }
    
    // Create default vestibule and dormitory rooms
    rooms["Sv"] = Room("Sv", antCount); // Vestibule can hold all ants
    rooms["Sd"] = Room("Sd", antCount); // Dormitory can hold all ants
    
    // Create ants
    for (int i = 1; i <= antCount; ++i) {
        ants.push_back(Ant(i));
    }
    
    // Read room definitions and tunnel connections
    while (std::getline(file, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        // Check if this is a connection definition (contains " - ")
        if (line.find(" - ") != std::string::npos) {
            size_t separatorPos = line.find(" - ");
            std::string room1 = line.substr(0, separatorPos);
            std::string room2 = line.substr(separatorPos + 3);
            
            // Make sure both rooms exist
            if (rooms.find(room1) == rooms.end()) {
                rooms[room1] = Room(room1);
            }
            if (rooms.find(room2) == rooms.end()) {
                rooms[room2] = Room(room2);
            }
            
            // Add bidirectional connections
            rooms[room1].addConnection(room2);
            rooms[room2].addConnection(room1);
        } 
        // Room definition
        else {
            std::istringstream iss(line);
            std::string roomId;
            iss >> roomId;
            
            int capacity = 1; // Default capacity
            
            // Check for capacity definition { X }
            if (line.find("{") != std::string::npos && line.find("}") != std::string::npos) {
                size_t openBrace = line.find("{");
                size_t closeBrace = line.find("}");
                std::string capacityStr = line.substr(openBrace + 1, closeBrace - openBrace - 1);
                capacity = std::stoi(capacityStr);
            }
            
            // Create room if it doesn't exist already
            if (rooms.find(roomId) == rooms.end()) {
                rooms[roomId] = Room(roomId, capacity);
            } else {
                // Update capacity if room already exists
                rooms[roomId].capacity = capacity;
            }
        }
    }
}

void AntColony::findOptimalPaths() {
    // Implementation would go here
    // Use graph algorithms to find optimal paths for ants
}

void AntColony::simulateAntMovement() {
    // Implementation would go here
    // Simulate the movement of ants through the colony
}

void AntColony::printSolution() const {
    // Implementation would go here
    // Print the solution in the required format
}

void AntColony::generateGraph() const {
    // Implementation would go here
    // Generate a graph representation of the ant colony
}