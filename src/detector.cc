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



MySensitiveDetector::~MySensitiveDetector()
{}



void MySensitiveDetector::Initialize(G4HCofThisEvent *hce)
{
    // Add fHitsCollection to the GHCofThisEvent container
    fHitsCollection = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    hce->AddHitsCollection(hcID, fHitsCollection);
}



G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    newHit = new MyHit();

    // Access the useful objects
    G4Track *track = aStep->GetTrack();
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    const G4VTouchable *touchable = preStepPoint->GetTouchable();


    // Stop and kill only optical photons
    if(track->GetParticleDefinition()->GetPDGEncoding()==-22)
        track->SetTrackStatus(fStopAndKill);


    // Time of the optical photon
    newHit->SetOpticalPhotonTime(preStepPoint->GetGlobalTime());

    // Position of the optical photon
    newHit->SetOpticalPhotonPos(preStepPoint->GetPosition());
    
    // Position of detector (Note that the position of the package is taken)
    newHit->SetDetectorPos(touchable->GetVolume(2)->GetTranslation());

    // Channel of detector
    newHit->SetDetectorChannel(touchable->GetCopyNumber(2));

    // Here's implemented the PDE
    if(G4UniformRand()<=GS::PDE_SiPM) newHit->SetDetection(true);
    else newHit->SetDetection(false);

    // Insert the hit
    fHitsCollection->insert(newHit);
    
    return true;
}
