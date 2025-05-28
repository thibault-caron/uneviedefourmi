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

        // Obtenir la Room de départ et d'arrivée
        Room* start = anthill0.findRoomById("Sv");
        Room* end = anthill0.findRoomById("Sd");
        if (!start || !end) {
            std::cout << "Erreur: Impossible de trouver les salles de début ou de fin" << std::endl;
            return 1;
        }


        // Créer le set pour les salles visitées
        std::set<const Room*> visited;

        // Obtenir tous les chemins possibles à partir de la première salle
        auto allPaths = start->findAllPaths(end, visited);

        if (allPaths.empty()) {
            std::cout << "Aucun chemin trouvé" << std::endl;
            return 1;
        }


        // Analyser les chemins pour trouver les meilleurs
        auto bestPaths = anthill0.analyzePaths(allPaths);

        // Afficher les résultats
        std::cout << "Chemins optimaux trouvés : " << bestPaths.size() << std::endl;
        for (const auto& path : bestPaths) {
            std::cout << "Chemin (capacité " << path.capacityMinimum << ") : ";
            for (const auto* room : path.path) {
                std::cout << room->getId() << " -> ";
            }
            std::cout << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}