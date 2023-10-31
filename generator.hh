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

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
        MyPrimaryGenerator();
        ~MyPrimaryGenerator();

        virtual void GeneratePrimaries(G4Event*);

private:
    G4ParticleGun *fParticleGun;

    G4GenericMessenger *fMessenger;

    //Definisco le variabili che voglio settare per la primary particle
    G4double meanEnergy, sigmaEnergy;

};
#endif