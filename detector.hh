#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"
#include <vector>

#include "globalsettings.hh"
#include "hit.hh"

//class G4Step;
//class G4HCofThisEvent;

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    MySensitiveDetector(G4String, G4String);
    ~MySensitiveDetector();
    MyGlobalSettings GS;
    
    virtual void Initialize(G4HCofThisEvent*);
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    virtual void EndOfEvent(G4HCofThisEvent*);

    MyHitsCollection *fHitsCollection;

private:
    MyHit *newHit;
};

#endif