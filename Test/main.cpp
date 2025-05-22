#include <iostream>
#include <string>
#include <exception>
#include "Anthill.h"

int main() {
    try {
        const std::string filename = "C:/Users/gravy/Desktop/PROJETS/FOURMIS/uneviedefourmi/Test/fourmilieres/fourmiliere_zero.txt";

        // create an anthill
        Anthill anthill0(filename);

        // load other rooms
        anthill0.loadRooms(filename);

        // connect rooms with each other
        anthill0.loadConnections(filename);

        // display the structure of the anthill
        anthill0.displayAnthill();
    }
    catch (const std::exception& e) {
        std::cerr << "Error : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}