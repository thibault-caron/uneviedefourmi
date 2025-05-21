//
// Created by gravy on 20/05/2025.
//

#include "Room.h"
#include "Ant.h"


Room::Room(std::string id, int size_max, int ants)
    : id_room(id), ANTS_MAX(size_max), ants_inside(ants) {}

std::string Room::getId() const {
    return id_room;
}

void Room::addChildNode(Room *child) {
    children.push_back(child);
}

bool Room::canAcceptAnt() const {
    return ants_inside < ANTS_MAX;
}

void Room::addAnt(Ant* ant) {
    if (canAcceptAnt()) {
        ants_inside += 1;
        ants.push_back(ant);
    } else {
        std::cout << "Room " << id_room << " is full!" << std::endl;
    }

}

void Room::removeAnt() {
    if (!ants.empty()) {
        ants_inside -= 1;
        ants.pop_front();
    }
}

void Room::display(int depth) const {
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

    for (auto child : children) {
        child->display(depth + 1);
    }
}

Room::~Room() {}

