#include <iostream>
#include <string>
#include "Room.h"
#include "Ant.h"

int main() {
    // Create rooms
    Room* Sv = new Room("Sv", 2);
    Room* S1 = new Room("S1", 1);
    Room* S2 = new Room("S2", 1);
    Room* Sd = new Room("Sd", 2);

    // create anthill
    Sv->addChildNode(S1);
    Sv->addChildNode(S2);
    S1->addChildNode(Sd);
    S2->addChildNode(Sd);

    // display anthill
    std::cout << "Structure initiale :" << std::endl;
    Sv->display();

    // create ants in "Sv" room
    Ant* f1 = new Ant("f1", Sv);
    Ant* f2 = new Ant("f2", Sv);

    // add ants to Sv (start room)
    Sv->addAnt(f1);
    Sv->addAnt(f2);

    // display anthill with ants
    std::cout << "Structure initiale remplie:" << std::endl;
    Sv->display();

    // add one more ant in a room full
    Ant* f3 = new Ant("f3", Sv);
    Sv->addAnt(f3);

    // f1 moves to room S1
    if (S1->canAcceptAnt()) {
        Sv->removeAnt();
        S1->addAnt(f1);
        f1->moves(S1);
    }

    // f2 moves to room S2
    if (S2->canAcceptAnt()) {
        Sv->removeAnt();
        S2->addAnt(f2);
        f2->moves(S2);
    }

    // display anthill : step 1
    std::cout << "Step 1 :" << std::endl;
    Sv->display();

    // display ants movements
    f1->displayMovement();
    f2->displayMovement();

    // clean memory
    delete f1;
    delete f2;
    delete f3;
    delete Sv;
    delete S1;
    delete S2;
    delete Sd;

    return 0;
}