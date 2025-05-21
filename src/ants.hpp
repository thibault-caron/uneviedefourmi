#pragma once
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <memory>

class Room {
public:
    std::string id;
    int capacity;
    std::vector<std::string> connections;
    
    Room(const std::string& id, int capacity = 1);
    
    void addConnection(const std::string& roomId);
};

class Ant {
public:
    int id;
    std::string currentRoom;
    std::vector<std::string> path;
    
    Ant(int id);
    
    void moveTo(const std::string& room);
};

class AntColony {
private:
    int antCount;
    std::unordered_map<std::string, Room> rooms;
    std::vector<Ant> ants;
    std::vector<std::vector<std::pair<int, std::string>>> steps;

public:
    AntColony(const std::string& filename);
    void parseFile(const std::string& filename);
    void findOptimalPaths();
    void simulateAntMovement();
    void printSolution() const;
    void generateGraph() const;
};
