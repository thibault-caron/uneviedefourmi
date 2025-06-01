#include <iostream>
#include <string>
#include <exception>
#include "Anthill.h"

int main() {
    try {
        const std::string filename = "C:/Users/gravy/Desktop/PROJETS/FOURMIS/uneviedefourmi/Test/fourmilieres/fourmiliere_cinq.txt";

        // Create an anthill
        Anthill anthill0(filename);
        std::cout << "Anthill created" << std::endl;
        anthill0.loadRooms(filename);
        std::cout << "Rooms loaded" << std::endl;
        anthill0.loadConnections(filename);
        std::cout << "Connections loaded" << std::endl;
        anthill0.displayAnthill();
        std::cout << "Anthill structure displayed" << std::endl;

        // Research and analyze paths
        anthill0.searchAllPaths();
        anthill0.sortAllPaths();
        anthill0.displayPaths(anthill0.getAllPaths(), "All paths");

        // Optimisation et results
        anthill0.findOptimalPaths();
        anthill0.displayPaths(anthill0.getOptimalPaths(), "Optimal paths");
        anthill0.displayBestSolution();

    } catch (const std::exception& e) {
        std::cerr << "Error : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}