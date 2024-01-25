/**
 * @file physics.hh
 * @brief Declaration of the class @ref MyPhysicsList
 */
#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

/**
 * @brief Mandatory user initialization concrete class of G4VModularPhysicsList.
 * It defines physical processes and particles to be considered in the
 * simulation.
 */
class MyPhysicsList : public G4VModularPhysicsList
{
public:
    MyPhysicsList(); /**< @brief Constructor of the class.*/
    ~MyPhysicsList() override = default; /**< @brief Destructor of the class.*/
};

#endif  // PHYSICS_HH
