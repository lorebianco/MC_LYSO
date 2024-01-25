/**
 * @file physics.cc
 * @brief Definition of the class @ref MyPhysicsList
 */
#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
    // EM physics
    RegisterPhysics (new G4EmStandardPhysics());
    // Optical physics
    RegisterPhysics (new G4OpticalPhysics());
}