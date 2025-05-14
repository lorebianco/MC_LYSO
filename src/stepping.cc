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

    // Get the detector construction
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    // Settings depending on run mode type
    G4int modeType = static_cast<const MyPrimaryGenerator*>(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction())->GetModeType();
    switch(modeType)
    {
        case 20: // 176Lu decay
        case 21:
            SetTimeOfDecay(step);
            break;
        case 22:    // with trigger
            SetTimeOfDecay(step);
            SteppingForLuDecayBulkSignature(step, volume, detectorConstruction);
            break;
        case 30: // Cosmic rays
        case 31:
            SteppingForCosmicRaysDetectors(step, volume, detectorConstruction);
            break;
    }

    // Get the scoring volume (i.e. the scintillator logic volume)
    G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();


    // Here I'm selecting that I want data ONLY in the crystal
    if(volume != fScoringVolume)
        return;
    
    // Do not register optical photons (PDGEncoding == -22), they DON'T respect energy conservation
    if(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==-22)
        return;

    // Store time and position of arrival of primary gamma (or daughters)
    if(step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary)
    {
        G4double newtimein = step->GetPreStepPoint()->GetGlobalTime();
        G4ThreeVector newposin = step->GetPreStepPoint()->GetPosition();

        G4double newtimeinter = step->GetPostStepPoint()->GetGlobalTime();
        G4ThreeVector newposinter = step->GetPostStepPoint()->GetPosition();

        fEventAction->SetArrivalandFirstInteraction(newtimein, newposin, newtimeinter, newposinter);
    }

    // Sum the energy deposited in the step
    G4double edep = step->GetTotalEnergyDeposit();
    fEventAction->AddEdep(edep);
    
    // Check if it is the maximum deposition of energy per unit length.
    // If yes it will be stored with its position.
    G4double dx = step->GetStepLength();
    if(dx == 0) return;

    G4ThreeVector maxedeppos = (step->GetPreStepPoint()->GetPosition() + step->GetPostStepPoint()->GetPosition())/2;
    fEventAction->SetMaxEdep(edep/dx, maxedeppos);
}



void MySteppingAction::SteppingForCosmicRaysDetectors(const G4Step *step, G4LogicalVolume *volume, const MyDetectorConstruction *detectorConstruction)
{
    // Get the scoring volume (i.e. the cosmic rays detector logic volume)
    G4LogicalVolume *fCosmicVolume = detectorConstruction->GetCosmicTriggerVolume();

    // Here I'm selecting that I want data ONLY for cosmic rays detectors
    if(!fCosmicVolume || volume != fCosmicVolume)
        return;

    // Should see this condition. Now I'm selecting only primary muon
    if(step->GetTrack()->GetTrackID() != 1)
        return;

    // Up (0) or Bottom (1) detector
    G4bool isUpOrBottom = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
    
    if(!isUpOrBottom) // up
        fEventAction->SetCosmicTriggerUp(true);
    else // bottom
    {    
        fEventAction->SetCosmicTriggerBottom(true);
        step->GetTrack()->SetTrackStatus(fStopAndKill); 
    } 
}



void MySteppingAction::SetTimeOfDecay(const G4Step *step)
{
    // Set time = 0 ns also for primary data. Instantaneous decay
    if(step->GetTrack()->GetParentID() == 0 && step->IsFirstStepInVolume())
    {
        G4double newtimein = step->GetPreStepPoint()->GetGlobalTime();
        G4ThreeVector newposin = step->GetPreStepPoint()->GetPosition();

        fEventAction->SetArrivalandFirstInteraction(newtimein, newposin, newtimein, newposin);
    }

    // Find if the primary is killed by radioactive decay
    if(step->GetTrack()->GetParentID() == 0 && step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType() == 6)
    {
        const auto* fDecayProducts = step->GetSecondaryInCurrentStep();
        
        for(auto i = 0; i < fDecayProducts->size(); i++)
        {
            G4Track* modifiableTrack = const_cast<G4Track*>((*fDecayProducts)[i]);            
            modifiableTrack->SetGlobalTime(0.);
        }
    }
}



void MySteppingAction::SteppingForLuDecayBulkSignature(const G4Step *step, G4LogicalVolume *volume, const MyDetectorConstruction *detectorConstruction)
{
    // Get the scoring volume (i.e. the cosmic rays detector logic volume)
    G4LogicalVolume *fSiVolume = detectorConstruction->GetDecayTriggerVolume();

    // Here I'm selecting that I want data ONLY for Si detectors
    if(volume != fSiVolume)
        return;

    // Save only electrons
    if(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding() != 11)
        return;
    
    // Save only detector Front-57, default for 176Lu decay Si-Trigger runs
    if(step->GetPreStepPoint()->GetTouchableHandle()->GetVolume(2)->GetTranslation().z() > GS::zScintillator || step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(2) != 57)
        return;

    fEventAction->SetDecayTriggerSi(true);
}
