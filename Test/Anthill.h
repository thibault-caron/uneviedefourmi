//
// Created by gravy on 20/05/2025.
//

#ifndef ANTHILL_H
#define ANTHILL_H

#include <iostream>
#include <string>
#include <vector>
#include "Room.h"



class Anthill {
public:
    explicit Anthill(const std::string& filename);
    ~Anthill();

    void loadRooms(const std::string& filename);
    void loadConnections(const std::string& filename) const;
    void displayAnthill() const;
    Room* findRoomById(const std::string& id) const;

private:
    int room_count;
    int ant_count;
    std::vector<Room*> rooms;
};

#endif //ANTHILL_H
