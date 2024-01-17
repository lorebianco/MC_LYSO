#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name, G4String hitsCollectionName) : G4VSensitiveDetector(name)
{
    collectionName.insert(hitsCollectionName);
}



MySensitiveDetector::~MySensitiveDetector()
{}



void MySensitiveDetector::Initialize(G4HCofThisEvent *hce)
{
    fHitsCollection = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    hce->AddHitsCollection(hcID, fHitsCollection);
}



G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    newHit = new MyHit();
    G4Track *track = aStep->GetTrack();
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    const G4VTouchable *touchable = preStepPoint->GetTouchable();


    // Stop and kill only optical photons
    if(track->GetParticleDefinition()->GetPDGEncoding()==-22)
        track->SetTrackStatus(fStopAndKill);


    // Time of the optical photon
    G4double timePhoton = preStepPoint->GetGlobalTime(); 

    // Position of the optical photon
    G4ThreeVector posPhoton = preStepPoint->GetPosition(); 
        //G4cout << posPhoton << G4endl;
    
    // Position of detector
    G4VPhysicalVolume *physVol = touchable->GetVolume(2);
    G4ThreeVector posDetector = physVol->GetTranslation();
        //G4cout << posDetector << G4endl;

    // Channel of detector
    G4int copyNo = touchable->GetCopyNumber(2); 
        //G4cout << copyNo << G4endl;

    newHit->SetOpticalPhotonTime(preStepPoint->GetGlobalTime());
    newHit->SetOpticalPhotonPos(preStepPoint->GetPosition());
    newHit->SetDetectorPos(posDetector);
    newHit->SetDetectorChannel(touchable->GetCopyNumber(2));

    // Here's implemented the PDE
    if(G4UniformRand()<=GS::PDE_SiPM) newHit->SetDetection(true);
    else newHit->SetDetection(false);

    fHitsCollection->insert(newHit);
    
    return true;
}
