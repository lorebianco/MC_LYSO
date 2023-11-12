#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

#include "run.hh"

class MyEventAction : public G4UserEventAction
{
public:
    MyEventAction(MyRunAction*);
    ~MyEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void FindEntryTime(G4double newtimein) {fTimeIn = newtimein; }
    void FindEntryPosition(G4ThreeVector newposin) {fPosIn = newposin; }
    void AddEdep(G4double edep) {fEdep += edep; }
    void FindMaxEdepPos(G4double edep, G4ThreeVector maxedeppos);

private:
    G4double fTimeIn, fEdep, fMaxEdep;
    G4ThreeVector fPosIn, fMaxEdepPos;
};


#endif
