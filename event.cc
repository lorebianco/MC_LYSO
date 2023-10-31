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
    
    man->FillNtupleIColumn(2, 0, evt);
    man->FillNtupleDColumn(2, 1, fTimeIn);
    man->FillNtupleDColumn(2, 2, fPosIn.x());
    man->FillNtupleDColumn(2, 3, fPosIn.y());
    man->FillNtupleDColumn(2, 4, fPosIn.z());
    man->FillNtupleDColumn(2, 5, fEdep);
    man->FillNtupleDColumn(2, 6, fMaxEdep);
    man->FillNtupleDColumn(2, 7, fMaxEdepPos.x());
    man->FillNtupleDColumn(2, 8, fMaxEdepPos.y());
    man->FillNtupleDColumn(2, 9, fMaxEdepPos.z());
    man->AddNtupleRow(2);

    if(evt%10==0) G4cout << "Processed event number " << evt << G4endl;
}

void MyEventAction::FindMaxEdepPos(G4double edep, G4ThreeVector maxedeppos)
{
    if(edep>fMaxEdep)
    {
        fMaxEdep = edep;
        fMaxEdepPos = maxedeppos;
    }
}
