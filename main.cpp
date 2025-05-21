#include "ants.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <fichier_fourmiliere>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    AntColony colony(filename);
    
    // Generate graph representation
    colony.generateGraph();
    
    // Find optimal paths and simulate movement
    colony.findOptimalPaths();
    colony.simulateAntMovement();
    
    // Print solution
    colony.printSolution();
    
    return 0;
}