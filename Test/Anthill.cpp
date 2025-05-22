//
// Created by gravy on 20/05/2025.
//
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include "Room.h"
#include "Ant.h"
#include "Anthill.h"

Anthill::Anthill(const std::string& filename) : room_count(0), ant_count(0) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + filename);
    }
    std::string line;
    // initialize room_count
    if (std::getline(file, line)) {
        if (line.substr(0, 2) == "r=") {
            room_count = std::stoi(line.substr(2));
        } else {
            throw std::runtime_error("Invalid file format : missing room count");
        }
    }
    // initialize ant_count
    if (std::getline(file, line)) {
        if (line.substr(0, 2) == "f=") {
            ant_count = std::stoi(line.substr(2));
        } else {
            throw std::runtime_error("Invalid file format : missing ant count");
        }
    }

    // create first room "Sv"
    rooms.push_back(new Room("Sv", ant_count));

    // create alls ants and add in the first room
    for (int i = 1; i <= ant_count; i++) {
        rooms[0]->addAnt(new Ant("f" + std::to_string(i),rooms[0]));
    }

    file.close();
}

Anthill::~Anthill() {
    for (Room* room : rooms) {
        delete room;
    }
    rooms.clear();
}


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
        if (line.find("-") == std::string::npos && line.find("=") == std::string::npos) {
            // find the id_room
            std::string identifier;
            int capacity;
            std::istringstream linestream(line);
            linestream >> identifier;
            // find the capacity of the room
            size_t open = line.find('{');
            size_t close = line.find('}');
            if (open != std::string::npos && close != std::string::npos && close > open) {
                std::string value = line.substr(open + 1, close - open - 1);
                capacity = std::stoi(value);
            } else {
                capacity = 1;
            }
            if (!identifier.empty()) {
                rooms.push_back(new Room(identifier, capacity));
            }
        }
    }
    file.close();
    rooms.push_back(new Room("Sd", ant_count));
}

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
                    child->addChildNode(parent);
                }
            }
        }

    }
    file.close();
}

Room* Anthill::findRoomById(const std::string& id) const {
    for (Room* room : rooms) {
        if (room->getId() == id) {
            return room;
        }
    }
    return nullptr;
}

void Anthill::displayAnthill() const {
    if (rooms.empty()) {
        std::cout << "No rooms found" << std::endl;
        return;
    }

    std::set<const Room*> visited;
    std::cout << "=== Map of the anthill ===" << std::endl;
    rooms[0]->display(0, visited);
}



