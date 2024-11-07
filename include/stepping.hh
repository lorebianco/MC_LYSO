/**
 * @file stepping.hh
 * @brief Declaration of the class @ref MySteppingAction
 */
#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

#include "construction.hh"
#include "event.hh"

/**
 * @brief User action concrete class of G4UserSteppingAction. It defines
 * procedures to be executed for every step.
 */
class MySteppingAction : public G4UserSteppingAction
{
public:
    /**
     * @brief Constructor of the class.
     *
     * @param eventAction Pointer to a MyEventAction object, necessary for
     * updating and storing data of the event.
     */
    MySteppingAction(MyEventAction *eventAction);
    ~MySteppingAction() override = default; /**< @brief Destructor of the class.*/

    /**
     * @brief For every step updates and stores quantities about the physics
     * inside the crystal by calling the methods of MyEventAction.
     * 
     * @param step Pointer to the G4Step object.
     */
    void UserSteppingAction(const G4Step *step) override;

private:
    void SteppingForLuDecayBulkSignature(const G4Step *step, G4LogicalVolume *volume, const MyDetectorConstruction *detectorConstruction);
    void SteppingForCosmicRaysDetectors(const G4Step *step, G4LogicalVolume *volume, const MyDetectorConstruction *detectorConstruction);
    void SetTimeOfDecay(const G4Step *step);

    MyEventAction *fEventAction; /**< @brief Pointer to the MyEventAction object.*/
};

#endif  // STEPPING_HH
