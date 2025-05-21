#include "ants.hpp"
#include <iostream>
#include <string>
#include <filesystem>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <fichier_fourmiliere>" << std::endl;
        std::cerr << "Example: " << argv[0] << " fourmiliere_zero.txt" << std::endl;
        std::cerr << "         " << argv[0] << " fourmilieres/fourmiliere_zero.txt" << std::endl;
        return 1;
    }

    std::string inputPath = argv[1];
    std::string filename;

    // First try the direct path
    if (std::filesystem::exists(inputPath)) {
        filename = inputPath;
    } 
    // Try with fourmilieres/ prefix
    else if (inputPath.find('/') == std::string::npos && 
             inputPath.find('\\') == std::string::npos) {
        // If running from /src folder, we need to look one directory up
        std::string withPrefix = "../fourmilieres/" + inputPath;
        
        if (std::filesystem::exists(withPrefix)) {
            filename = withPrefix;
        } else {
            std::cerr << "Error: File not found: " << inputPath << std::endl;
            return 1;
        }
    } else {
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