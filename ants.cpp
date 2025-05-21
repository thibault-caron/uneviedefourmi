#include "ants.hpp"
#include <fstream>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <algorithm> // for std::reverse
#include <limits>
#include <iostream>

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
    // Use BFS to find shortest path from Sv to Sd
    std::unordered_map<std::string, std::string> predecessor;
    std::unordered_map<std::string, int> distance;
    std::queue<std::string> queue;
    
    // Initialize all distances to infinity
    for (const auto& room : rooms) {
        distance[room.first] = std::numeric_limits<int>::max();
    }
    
    // Start from vestibule
    queue.push("Sv");
    distance["Sv"] = 0;
    
    // BFS to find shortest paths
    while (!queue.empty()) {
        std::string current = queue.front();
        queue.pop();
        
        // Found the dormitory, end the search
        if (current == "Sd") {
            break;
        }
        
        // Check all adjacent rooms
        for (const std::string& next : rooms[current].connections) {
            if (distance[next] == std::numeric_limits<int>::max()) {
                distance[next] = distance[current] + 1;
                predecessor[next] = current;
                queue.push(next);
            }
        }
    }
    
    // If no path to dormitory was found
    if (distance["Sd"] == std::numeric_limits<int>::max()) {
        throw std::runtime_error("No path found from vestibule to dormitory");
    }
    
    // Determine the shortest path for each ant
    for (Ant& ant : ants) {
        std::vector<std::string> path;
        std::string current = "Sd";
        
        // Reconstruct path from predecessor map (backwards)
        while (current != "Sv") {
            path.push_back(current);
            current = predecessor[current];
        }
        
        // Reverse the path to get it in the right order (Sv to Sd)
        std::reverse(path.begin(), path.end());
        
        // Store the path for this ant (without Sv which is the starting point)
        ant.path = path;
    }
    
    // Now we need to coordinate the ant movements based on room capacities
    // This will be handled in simulateAntMovement()
}

void AntColony::simulateAntMovement() {
    // Structure to track room occupancy at each step
    std::unordered_map<std::string, int> roomOccupancy;
    
    // Initialization: all ants are in the vestibule
    bool allAntsInDormitory = false;
    std::vector<bool> antReachedDormitory(ants.size(), false);
    
    // Continue until all ants reach the dormitory
    int stepCount = 0;
    
    while (!allAntsInDormitory) {
        stepCount++;
        
        // Movements for this step
        std::vector<std::pair<int, std::string>> currentStepMoves;
        
        // Reset room occupancy for this step
        roomOccupancy.clear();
        
        // Temporary data to simulate movements for this step
        std::vector<std::string> nextRooms(ants.size());
        std::vector<bool> antMoved(ants.size(), false);
        
        // Phase 1: determine where each ant wants to go
        for (size_t i = 0; i < ants.size(); i++) {
            // If ant is already in dormitory, skip
            if (antReachedDormitory[i]) {
                continue;
            }
            
            Ant& ant = ants[i];
            
            // If ant is in vestibule and has a path
            if (ant.currentRoom == "Sv" && !ant.path.empty()) {
                nextRooms[i] = ant.path[0];
            }
            // If ant is in an intermediate room
            else if (ant.currentRoom != "Sd" && ant.currentRoom != "Sv") {
                // Find current position in path
                auto currentPosInPath = std::find(ant.path.begin(), ant.path.end(), ant.currentRoom);
                
                if (currentPosInPath != ant.path.end() && std::next(currentPosInPath) != ant.path.end()) {
                    nextRooms[i] = *std::next(currentPosInPath);
                } else {
                    // Problem with path, stay in place
                    nextRooms[i] = ant.currentRoom;
                }
            }
        }
        
        // Phase 2: check capacities and resolve conflicts
        bool movementMade = false;
        
        // Prioritize ants that can reach dormitory directly
        for (size_t i = 0; i < ants.size(); i++) {
            if (antReachedDormitory[i] || antMoved[i]) continue;
            
            Ant& ant = ants[i];
            if (nextRooms[i] == "Sd") {
                // Move directly to dormitory
                ant.moveTo("Sd");
                antReachedDormitory[i] = true;
                antMoved[i] = true;
                movementMade = true;
                currentStepMoves.push_back({ant.id, "Sd"});
            }
        }
        
        // Then process other movements
        for (size_t i = 0; i < ants.size(); i++) {
            if (antReachedDormitory[i] || antMoved[i]) continue;
            
            Ant& ant = ants[i];
            std::string targetRoom = nextRooms[i];
            
            if (targetRoom.empty() || targetRoom == ant.currentRoom) {
                // Ant stays in place
                continue;
            }
            
            // Check if target room can accommodate one more ant
            int currentOccupancy = roomOccupancy[targetRoom];
            int capacity = rooms[targetRoom].capacity;
            
            if (targetRoom != "Sd" && targetRoom != "Sv" && currentOccupancy >= capacity) {
                // Room is full, ant stays in place
                continue;
            }
            
            // Move the ant
            ant.moveTo(targetRoom);
            antMoved[i] = true;
            movementMade = true;
            roomOccupancy[targetRoom]++;
            
            // If ant reaches dormitory
            if (targetRoom == "Sd") {
                antReachedDormitory[i] = true;
            }
            
            currentStepMoves.push_back({ant.id, targetRoom});
        }
        
        // If no movement was made, it might be a blockage
        if (!movementMade) {
            // Could implement blockage resolution logic here
            // For example, make some ants step back to let others pass
            
            // For now, consider it a problem if no ant could move
            throw std::runtime_error("Blockage detected: no ant could move at step " + std::to_string(stepCount));
        }
        
        // Check if all ants have reached the dormitory
        allAntsInDormitory = true;
        for (bool reached : antReachedDormitory) {
            if (!reached) {
                allAntsInDormitory = false;
                break;
            }
        }
        
        // Record movements for this step
        steps.push_back(currentStepMoves);
    }
}

void AntColony::printSolution() const {
    // Display ant movement steps
    for (size_t i = 0; i < steps.size(); i++) {
        std::cout << "+++ E" << (i + 1) << " +++" << std::endl;
        
        for (const auto& move : steps[i]) {
            int antId = move.first;
            std::string targetRoom = move.second;
            
            // Find previous room
            std::string previousRoom;
            if (i == 0 || targetRoom == "Sd") {
                // First step or final destination
                previousRoom = "Sv";
            } else {
                // Look in previous moves
                for (const auto& prevMove : steps[i-1]) {
                    if (prevMove.first == antId) {
                        previousRoom = prevMove.second;
                        break;
                    }
                }
            }
            
            // Display movement
            std::cout << "f" << antId << " - " << previousRoom << " - " << targetRoom << std::endl;
        }
    }
}

void AntColony::generateGraph() const {
    // Implementation of graph generation (visual or textual)
    std::cout << "Ant colony graph:" << std::endl;
    std::cout << "Number of ants: " << antCount << std::endl;
    std::cout << "Rooms:" << std::endl;
    
    for (const auto& room : rooms) {
        std::cout << "  " << room.first;
        if (room.second.capacity > 1) {
            std::cout << " {capacity: " << room.second.capacity << "}";
        }
        std::cout << std::endl;
    }
    
    std::cout << "Tunnels:" << std::endl;
    std::unordered_set<std::string> printedConnections;
    
    for (const auto& room : rooms) {
        for (const std::string& connection : room.second.connections) {
            // Avoid displaying the same connection twice
            std::string connectionKey = room.first < connection ? 
                                        room.first + "-" + connection : 
                                        connection + "-" + room.first;
            
            if (printedConnections.find(connectionKey) == printedConnections.end()) {
                std::cout << "  " << room.first << " - " << connection << std::endl;
                printedConnections.insert(connectionKey);
            }
        }
    }
}