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

    G4double timePhoton = preStepPoint->GetGlobalTime(); //tempo del fotone ottico
    G4ThreeVector posPhoton = preStepPoint->GetPosition(); //posizione del fotone ottico

    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

    G4int copyNo = touchable->GetCopyNumber(); //indice del detector

    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation(); //posizione del detector

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
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
    if(posDetector[2]>200)
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
