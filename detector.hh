#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4AnalysisManager.hh"
#include "globalsettings.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    MySensitiveDetector(G4String);
    ~MySensitiveDetector();
    MyGlobalSettings GS;
    
    virtual void EndOfEvent(G4HCofThisEvent*);

private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    
    G4int fF, fB, fwGhosts_F, fwGhosts_B;
};

#endif