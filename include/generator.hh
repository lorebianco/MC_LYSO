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

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
        MyPrimaryGenerator();
        ~MyPrimaryGenerator();

        virtual void GeneratePrimaries(G4Event*);

private:
    G4ParticleGun *fParticleGun;
    G4ParticleTable* fParticleTable;

    G4GenericMessenger *fMessenger_Run, *fMessenger_Spread, *fMessenger_Calib;

    //Define variables that want to set as UI
    G4double meanEnergy, sigmaEnergy, radiusSpread;
    G4bool isSpreadEnabled, isCalibrationEnabled;
    G4String chooseFrontorBack, switchOnLED;

};
#endif
