#include <iostream>
#include <string>
#include <exception>
#include "Anthill.h"

int main() {
    try {
        const std::string filename = "C:/Users/gravy/Desktop/PROJETS/FOURMIS/uneviedefourmi/Test/fourmilieres/fourmiliere_cinq.txt";

        // create an anthill
        Anthill anthill0(filename);

        // load other rooms
        anthill0.loadRooms(filename);

        // connect rooms with each other
        anthill0.loadConnections(filename);

        // display the structure of the anthill
        anthill0.displayAnthill();

        // Get start and end rooms
        Room* start = anthill0.findRoomById("Sv");
        Room* end = anthill0.findRoomById("Sd");
        if (!start || !end) {
            std::cout << "Error: Unable to find start or end rooms" << std::endl;
            return 1;
        }

        // Create set for visited rooms
        std::set<const Room*> visited;

        // Get all possible paths from start to end
        auto allPaths = start->findAllPaths(end, visited);

        if (allPaths.empty()) {
            std::cout << "Aucun chemin trouvé" << std::endl;
            return 1;
        }


        // Analyze paths to find the best ones
        auto bestPaths = anthill0.analyzePaths(allPaths);

        // Display results
        std::cout << "Optimal paths found : " << bestPaths.size() << std::endl;
        for (const auto& path : bestPaths) {
            std::cout << "Path (capacity " << path.capacityMinimum << ") : ";
            bool first = true;
            for (const auto* room : path.path) {
                if (!first) std::cout << " -> ";
                std::cout << room->getId();
                first = false;
            }
            std::cout << std::endl;
        }

        // If no valid paths found, exit
        if (bestPaths.empty()) {
            std::cout << "No valid paths found for ant movement" << std::endl;
            return 1;
        }

        int step = 1;
        bool someAntMoved;

        do {
            someAntMoved = false;
            std::cout << "\n+++ E" << step << " +++" << std::endl;

            // Pour chaque chemin optimal
            for (const auto& path : bestPaths) {
                // On parcourt toutes les salles du chemin de l'avant-dernière à la première
                for (int i = path.path.size() - 2; i >= 0; i--) {
                    Room* previousRoom = const_cast<Room*>(path.path[i]);
                    Room* currentRoom = const_cast<Room*>(path.path[i + 1]);

                    // On vérifie combien de fourmis peuvent passer
                    int antsInPrevious = previousRoom->getAntsInside();
                    int spaceInCurrent = currentRoom->getCapacity() - currentRoom->getAntsInside();

                    // On déplace le maximum possible de fourmis
                    int antsToMove = std::min(antsInPrevious, spaceInCurrent);

                    // S'il y a des fourmis à déplacer
                    if (antsToMove > 0) {
                        for (int j = 0; j < antsToMove; j++) {
                            anthill0.movesAnt(previousRoom, currentRoom);
                        }
                        someAntMoved = true;
                    }
                }
            }
            step++;
        } while (someAntMoved);


        std::cout << "\nToutes les fourmis ont atteint leur destination !" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}