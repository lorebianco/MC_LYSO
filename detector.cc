#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name, G4String hitsCollectionName) : G4VSensitiveDetector(name)
{
    collectionName.insert(hitsCollectionName);
}

MySensitiveDetector::~MySensitiveDetector()
{}

void MySensitiveDetector::Initialize(G4HCofThisEvent *hce)
{
    fHitsCollectionFront = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);
    fHitsCollectionBack = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);
    fHitsCollectionwGhostsFront = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);
    fHitsCollectionwGhostsBack = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);

    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollectionFront);
    hce->AddHitsCollection(hcID, fHitsCollectionBack);
    hce->AddHitsCollection(hcID, fHitsCollectionwGhostsFront); 
    hce->AddHitsCollection(hcID, fHitsCollectionwGhostsBack);
}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    MyHit *newHit = new MyHit();
    G4Track *track = aStep->GetTrack();
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    const G4VTouchable *touchable = preStepPoint->GetTouchable();


    //Stop and kill only optical photons
    if(track->GetParticleDefinition()->GetPDGEncoding()==-22)
        track->SetTrackStatus(fStopAndKill);


    //time of the optical photon
    G4double timePhoton = preStepPoint->GetGlobalTime(); 

    //position of the optical photon
    G4ThreeVector posPhoton = preStepPoint->GetPosition(); 
        //G4cout << posPhoton << G4endl;
    
    //position of detector
    G4VPhysicalVolume *physVol = touchable->GetVolume(2);
    G4ThreeVector posDetector = physVol->GetTranslation();
        //G4cout << posDetector << G4endl;

    //channel of detector
    G4int copyNo = touchable->GetCopyNumber(2); 
        //G4cout << copyNo << G4endl;

    
    newHit->SetTrackID(track->GetTrackID());
    newHit->SetOpticalPhotonTime(timePhoton);
    newHit->SetOpticalPhotonPos(posPhoton);
    newHit->SetDetectorPos(posDetector);
    newHit->SetDetectorChannel(copyNo);
    

    if(posDetector[2]<200)
    {
        fHitsCollectionwGhostsFront->insert(newHit);
        if(G4UniformRand()<=GS.PDE_SiPM)
        {
            fHitsCollectionFront->insert(newHit);
        }
    }

    if(posDetector[2]>200)
    {
        fHitsCollectionwGhostsBack->insert(newHit);
        if(G4UniformRand()<=GS.PDE_SiPM)
        {
            fHitsCollectionBack->insert(newHit);
        }
    }

    return true;
}


void MySensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int entriesF = fHitsCollectionFront->entries();
    G4int entriesB = fHitsCollectionBack->entries();
    G4int entriesGhostsF = fHitsCollectionwGhostsFront->entries();
    G4int entriesGhostsB = fHitsCollectionwGhostsBack->entries();

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->FillNtupleIColumn(0, 0, evt);
    man->FillNtupleIColumn(0, 1, entriesF);
    man->FillNtupleIColumn(0, 2, entriesB);
    man->FillNtupleIColumn(0, 3, entriesF+entriesB);
    man->FillNtupleIColumn(0, 4, entriesGhostsF);
    man->FillNtupleIColumn(0, 5, entriesGhostsB);
    man->FillNtupleIColumn(0, 6, entriesGhostsF+entriesGhostsB);
    man->AddNtupleRow(0);

    for(G4int i = 0; i < entriesF; i++)
    {
        man->FillNtupleIColumn(1, 0, evt);
        man->FillNtupleDColumn(1, 1, (*fHitsCollectionFront)[i]->GetOpticalPhotonTime());
        man->FillNtupleDColumn(1, 2, (*fHitsCollectionFront)[i]->GetDetectorPos().x());
        man->FillNtupleDColumn(1, 3, (*fHitsCollectionFront)[i]->GetDetectorPos().y());
        man->FillNtupleDColumn(1, 4, (*fHitsCollectionFront)[i]->GetDetectorPos().z());
        man->FillNtupleIColumn(1, 5, (*fHitsCollectionFront)[i]->GetDetectorChannel());
        man->AddNtupleRow(1);
    }
    
    for(G4int i = 0; i < entriesB; i++)
    {
        man->FillNtupleIColumn(2, 0, evt);
        man->FillNtupleDColumn(2, 1, (*fHitsCollectionBack)[i]->GetOpticalPhotonTime());
        man->FillNtupleDColumn(2, 2, (*fHitsCollectionBack)[i]->GetDetectorPos().x());
        man->FillNtupleDColumn(2, 3, (*fHitsCollectionBack)[i]->GetDetectorPos().y());
        man->FillNtupleDColumn(2, 4, (*fHitsCollectionBack)[i]->GetDetectorPos().z());
        man->FillNtupleIColumn(2, 5, (*fHitsCollectionBack)[i]->GetDetectorChannel());
        man->AddNtupleRow(2);
    }

}
