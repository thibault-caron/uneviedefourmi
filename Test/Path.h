//
// Created by gravy on 27/05/2025.
//

#ifndef PATH_H
#define PATH_H

#include <vector>

class Room;

struct Path {
    std::vector<const Room*> path;
    int capacityMinimum;

    explicit Path(int capacity) :  capacityMinimum(capacity) {}
};

#endif //PATH_H
