#include "ants.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <fichier_fourmiliere>" << std::endl;
        std::cerr << "Example: " << argv[0] << " fourmiliere_zero.txt" << std::endl;
        std::cerr << "         " << argv[0] << " fourmilieres/fourmiliere_zero.txt" << std::endl;
        return 1;
    }

    std::string inputPath = argv[1];
    std::string filename;

    // Try several possible locations for the file
    std::vector<std::string> possiblePaths = {
        inputPath,                          // Direct input
        "fourmilieres/" + inputPath,        // In fourmilieres subdirectory
        "../fourmilieres/" + inputPath,     // Up one level then fourmilieres
        "../../fourmilieres/" + inputPath,  // Up two levels then fourmilieres
        "../" + inputPath,                  // Just up one level
    };

    for (const auto& path : possiblePaths) {
        if (std::filesystem::exists(path)) {
            filename = path;
            break;
        }
    }

    if (filename.empty()) {
        std::cerr << "Error: File not found: " << inputPath << std::endl;
        return 1;
    }

    try {
        AntColony colony(filename);
        
        // Generate graph representation
        colony.generateGraph();
        
        // Find optimal paths and simulate movement
        colony.findOptimalPaths();
        colony.simulateAntMovement();
        
        // Print solution
        colony.printSolution();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}