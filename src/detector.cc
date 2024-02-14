/**
 * @file detector.cc
 * @brief Definition of the class @ref MySensitiveDetector
 */
#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name, G4String hitsCollectionName) : G4VSensitiveDetector(name)
{
    // Add the collection name to collectionName vector
    collectionName.insert(hitsCollectionName);
}



void MySensitiveDetector::Initialize(G4HCofThisEvent *hce)
{
    // Add fHitsCollection to the GHCofThisEvent container
    fHitsCollection = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    hce->AddHitsCollection(hcID, fHitsCollection);
}



G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // Here's implemented the PDE
    if(G4UniformRand() > GS::PDE_SiPM) return false;

    // Create a new MyHit object
    MyHit *newHit = new MyHit();

    // Access the useful objects
    G4Track *track = aStep->GetTrack();
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    const G4VTouchable *touchable = preStepPoint->GetTouchable();


    // Stop and kill only optical photons
    if(track->GetParticleDefinition()->GetPDGEncoding()==-22)
        track->SetTrackStatus(fStopAndKill);
    // Detect only optical photons
    else return false;


    // Time of the optical photon
    newHit->SetDetectionTime(preStepPoint->GetGlobalTime());
    
    // Position of detector (Note that the position of the package is taken)
    newHit->SetDetectorPosition(touchable->GetVolume(2)->GetTranslation());

    // Channel of detector
    newHit->SetDetectorChannel(touchable->GetCopyNumber(2));


    // Insert the hit
    fHitsCollection->insert(newHit);
    
    return true;
}
