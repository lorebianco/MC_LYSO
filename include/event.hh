#ifndef EVENT_HH
#define EVENT_HH

#include <vector>

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

#include "hit.hh"


class MyEventAction : public G4UserEventAction
{
public:
    MyEventAction();
    ~MyEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    inline void FindEntryTime(G4double newtimein) { fTimeIn = newtimein; }
    inline void FindEntryPosition(G4ThreeVector newposin) { fPosIn = newposin; }
    inline void AddEdep(G4double edep) { fEdep += edep; }
    inline void FindMaxEdepPos(G4double edep, G4ThreeVector maxedeppos)
    {
        if(edep>fMaxEdep)
        {
            fMaxEdep = edep;
            fMaxEdepPos = maxedeppos;
        }
    };

    // Primary's data
    G4double fTimeIn;
    G4ThreeVector fPosIn;
    
    // Crystal's data
    G4double fEdep;
    G4double fMaxEdep;
    G4ThreeVector fMaxEdepPos;

    // Detectors' data
    G4int fHitsNum_F;
    G4int fHitsNum_B;
    G4int fHitsNumwGhosts_F;
    G4int fHitsNumwGhosts_B;
    std::vector<G4double> fT_F;
    std::vector<G4double> fX_F;
    std::vector<G4double> fY_F;
    std::vector<G4double> fZ_F;
    std::vector<G4int> fChannel_F;
    std::vector<G4double> fT_B;
    std::vector<G4double> fX_B;
    std::vector<G4double> fY_B;
    std::vector<G4double> fZ_B;
    std::vector<G4int> fChannel_B;
};

#endif  // EVENT_HH
