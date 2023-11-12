#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
    fTimeIn = 0.;
    fPosIn = G4ThreeVector(0., 0., 0.);
    fEdep = 0.;
    fMaxEdep = 0;
    fMaxEdepPos = G4ThreeVector(0., 0., 0.);
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
    fTimeIn = 0.;
    fPosIn = G4ThreeVector(0., 0., 0.);
    fEdep = 0.;
    fMaxEdep = 0.;
    fMaxEdepPos = G4ThreeVector(0., 0., 0.);
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
    man->FillNtupleIColumn(3, 0, evt);
    man->FillNtupleDColumn(3, 1, fTimeIn);
    man->FillNtupleDColumn(3, 2, fPosIn.x());
    man->FillNtupleDColumn(3, 3, fPosIn.y());
    man->FillNtupleDColumn(3, 4, fPosIn.z());
    man->FillNtupleDColumn(3, 5, fEdep);
    man->FillNtupleDColumn(3, 6, fMaxEdep);
    man->FillNtupleDColumn(3, 7, fMaxEdepPos.x());
    man->FillNtupleDColumn(3, 8, fMaxEdepPos.y());
    man->FillNtupleDColumn(3, 9, fMaxEdepPos.z());
    man->AddNtupleRow(3);

    if(evt%100==0) G4cout << "Processed event number " << evt << G4endl;
}

void MyEventAction::FindMaxEdepPos(G4double edep, G4ThreeVector maxedeppos)
{
    if(edep>fMaxEdep)
    {
        fMaxEdep = edep;
        fMaxEdepPos = maxedeppos;
    }
}
