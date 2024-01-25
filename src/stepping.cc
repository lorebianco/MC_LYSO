/**
 * @file stepping.cc
 * @brief Definition of the class @ref MySteppingAction
 */
 #include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction) : fEventAction(eventAction)
{}



void MySteppingAction::UserSteppingAction(const G4Step *step)
{
    // Get the logical volume containing the step
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    
    // Get the scoring volume (i.e. the scintillator logic volume)
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

    
    // Here I'm selecting that I want data ONLY in the crystal
    if(volume != fScoringVolume)
        return;
    
    // Do not register optical photons (PDGEncoding == -22), they DON'T respect energy conservation
    if(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==-22)
        return;

    // Store time and position of arrival of primary gamma
    if(step->GetTrack()->GetTrackID()==1 && step->IsFirstStepInVolume())
    {
        G4double newtimein = step->GetPreStepPoint()->GetGlobalTime();
        fEventAction->FindEntryTime(newtimein);

        G4ThreeVector newposin = step->GetPreStepPoint()->GetPosition();
        fEventAction->FindEntryPosition(newposin);
    }

    // Sum the energy deposited in the step
    G4double edep = step->GetTotalEnergyDeposit();
    fEventAction->AddEdep(edep);
    
    // Check if it is the maximum deposition of energy per unit length.
    // If yes it will be stored with its position.
    G4double dx = step->GetStepLength();
    if(dx == 0) return;

    G4ThreeVector maxedeppos = (step->GetPreStepPoint()->GetPosition() + step->GetPostStepPoint()->GetPosition())/2;
    fEventAction->FindMaxEdep(edep/dx, maxedeppos);
}
