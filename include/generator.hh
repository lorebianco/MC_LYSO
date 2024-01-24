/**
 * @file generator.hh
 * @brief Declaration of the class @ref MyPrimaryGenerator
 */
#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4GenericMessenger.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

#include "globalsettings.hh"

/**
 * @brief Mandatory user action concrete class of
 * G4VUserPrimaryGeneratorAction. It defines the settings for the generation of
 * the primary gamma.
 */
class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    /**
     * @brief Constructor of the class.
     * 
     * It defines the UI commands through DefineCommands() and gives
     * default values to the settable variables. Then, it instantiates the
     * G4ParticleGun object, representing the source of one primary gamma.
     */
    MyPrimaryGenerator();
    ~MyPrimaryGenerator(); /**< @brief Destructor of the class.*/

    /**
     * @brief It generates the primary vertex of the event.
     *
     * In standard mode with spread disabled, a gamma particle is generated
     * with energy randomly sampled from a Gaussian distribution with mean
     * @ref fMeanEnergy and stdev @ref fSigmaEnergy, directed towards the
     * center of the front face of the crystal perpendicular to it.
     * If the spread is enabled, the gamma particle may be generated with
     * incident angles that fall within the surface of the radius @ref 
     * fRadiusSpread defined on the front face.
     * In calibration mode, the isotropic emission of an optical photon from a
     * monochromatic blue LED (@ref GS::energyLED) situated in one of the light
     * guide's cavities is simulated.
     *
     * @param anEvent Pointer to the G4Event.
     */
    virtual void GeneratePrimaries(G4Event *anEvent);

private:
    void DefineCommands(); /**< @brief Defines new user commands for primary particle generation.*/

    G4ParticleGun *fParticleGun; /**< @brief Pointer to the G4ParticleGun object.*/

    // Generic Messengers
    G4GenericMessenger *fMessenger_Gun; /**< @brief Generic messenger for the standard gamma mode.*/
    G4GenericMessenger *fMessenger_Calib; /**< @brief Generic messenger for the calibration mode.*/

    // Define variables that want to set as UI
    G4double fMeanEnergy, /**< @brief Mean of the gaussian distribution of the energy of the primary particle.*/
             fSigmaEnergy, /**< @brief Sigma of the gaussian distribution of the energy of the primary particle.*/
             fRadiusSpread; /**< @brief Radius of the area on the front face of the crystal that could be hit by primary gamma when spread is enabled.*/
    G4bool fIsSpreadEnabled, /**< @brief Flag indicating whether the spread is enabled or not.*/
           fIsCalibrationEnabled; /**< @brief Flag indicating whether the calibration mode is enabled or not.*/
    G4String fChooseFrontorBack, /**< @brief Flag indicating on which face of the crystal a LED has to be switched ON.*/
             fSwitchOnLED; /**< @brief Flag indicating which LED has to be switched ON.*/
};

#endif  // GENERATOR_HH
