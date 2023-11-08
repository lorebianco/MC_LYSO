#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    G4Track *track = aStep->GetTrack(); //Ora ho info sulla traccia che entra nel sensitive volume

    //Stop and kill only optical photons
    if(track->GetParticleDefinition()->GetPDGEncoding()==-22)
        track->SetTrackStatus(fStopAndKill);

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    G4double timePhoton = preStepPoint->GetGlobalTime(); //time of the optical photon
    G4ThreeVector posPhoton = preStepPoint->GetPosition(); //position of the optical photon
    //G4cout << posPhoton << G4endl;

    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

    G4int copyNo = touchable->GetCopyNumber(2); //detector index
    //G4cout << copyNo << G4endl;

    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = -touchable->GetHistory()->GetTopTransform().TransformPoint(G4ThreeVector(0,0,0)); //detector position
    //G4cout << posDetector << G4endl;

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    if(G4UniformRand()<=GS.PDE_SiPM)
    {
        if(posDetector[2]<200)
        {
            man->FillNtupleIColumn(0, 0, evt);
            man->FillNtupleDColumn(0, 1, timePhoton);
            man->FillNtupleDColumn(0, 2, posDetector[0]);
            man->FillNtupleDColumn(0, 3, posDetector[1]);
            man->FillNtupleDColumn(0, 4, posDetector[2]);
            man->FillNtupleIColumn(0, 5, copyNo);
            man->AddNtupleRow(0);
        }
        if( posDetector[2]>200)
        {
            man->FillNtupleIColumn(1, 0, evt);
            man->FillNtupleDColumn(1, 1, timePhoton);
            man->FillNtupleDColumn(1, 2, posDetector[0]);
            man->FillNtupleDColumn(1, 3, posDetector[1]);
            man->FillNtupleDColumn(1, 4, posDetector[2]);
            man->FillNtupleIColumn(1, 5, copyNo);
            man->AddNtupleRow(1);
        }
    }

    return true;
}
