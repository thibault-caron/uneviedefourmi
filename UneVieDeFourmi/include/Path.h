
/**
 * @file path.h
 * @brief Header file defining the Path structure representing a path between rooms with capacity constraints
 */

#ifndef PATH_H
#define PATH_H

#include <vector>

class Room;

/**
 * @brief Structure representing a path between rooms with a minimum capacity requirement
 *
 * This structure stores a sequence of connected rooms (path) and the minimum capacity
 * that must be maintained throughout the entire path.
 */
struct Path {
    /** @brief Vector storing pointers to Room objects that form the path */
    std::vector<const Room*> path;

    /** @brief The minimum capacity requirement for the entire path */
    int capacityMinimum;

    /**
     * @brief Constructs a Path with a specified minimum capacity
     * @param capacity The minimum capacity requirement for the path
     */
    explicit Path(int capacity) :  capacityMinimum(capacity) {}
};

#endif //PATH_H
