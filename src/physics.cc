#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
    // EM physics
    RegisterPhysics (new G4EmStandardPhysics());
    // Optical physics
    RegisterPhysics (new G4OpticalPhysics());
}



MyPhysicsList::~MyPhysicsList()
{}
