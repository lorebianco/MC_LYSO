#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

    
    //Qui sto selezionando che voglio SOLO ciò che accade nel cristallo
    if(volume != fScoringVolume)
        return;
    
    //Non registrare i fotoni ottici (PDGEncoding == -22), non rispettano la conservazione dell'energia
    if(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==-22)
        return;


    if(step->GetTrack()->GetTrackID()==1 && step->IsFirstStepInVolume())
    {
        G4double newtimein = step->GetPreStepPoint()->GetGlobalTime();
        fEventAction->FindEntryTime(newtimein);

        G4ThreeVector newposin = step->GetPreStepPoint()->GetPosition();
        fEventAction->FindEntryPosition(newposin);
    }

    G4double edep = step->GetTotalEnergyDeposit();
    fEventAction->AddEdep(edep);
    
    G4ThreeVector maxedeppos = (step->GetPreStepPoint()->GetPosition() + step->GetPostStepPoint()->GetPosition())/2;
    fEventAction->FindMaxEdepPos(edep, maxedeppos);

}