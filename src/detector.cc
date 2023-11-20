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
    newHit->SetOpticalPhotonTime(preStepPoint->GetGlobalTime());
    newHit->SetOpticalPhotonPos(preStepPoint->GetPosition());
    newHit->SetDetectorPos(posDetector);
    newHit->SetDetectorChannel(touchable->GetCopyNumber(2));

    if(G4UniformRand()<=GS::PDE_SiPM) newHit->SetDetection(true);
    else newHit->SetDetection(false);

    fHitsCollection->insert(newHit);
    
    return true;
}


void MySensitiveDetector::EndOfEvent(G4HCofThisEvent *hce)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    MyHitsCollection *THC = NULL;

    if(hce)
    {
        THC = (MyHitsCollection *)(hce->GetHC(0));
    }
    if(THC)
    {
        G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

        G4int entriesF = 0;
        G4int entriesB = 0;
        G4int entrieswGhostsF = 0;
        G4int entrieswGhostsB = 0;

        G4int nHits = THC->entries();

        for(G4int i = 0; i < nHits; i++)
        {
            if((*THC)[i]->GetDetectorPos()[2]<200)
            {   
                entrieswGhostsF++;
                if((*THC)[i]->GetDetection())
                {
                    entriesF++;
                    man->FillNtupleIColumn(1, 0, evt);
                    man->FillNtupleDColumn(1, 1, (*THC)[i]->GetOpticalPhotonTime());
                    man->FillNtupleDColumn(1, 2, (*THC)[i]->GetDetectorPos().x());
                    man->FillNtupleDColumn(1, 3, (*THC)[i]->GetDetectorPos().y());
                    man->FillNtupleDColumn(1, 4, (*THC)[i]->GetDetectorPos().z());
                    man->FillNtupleIColumn(1, 5, (*THC)[i]->GetDetectorChannel());
                    man->AddNtupleRow(1);
                }
            }

            if((*THC)[i]->GetDetectorPos()[2]>200)
            {
                entrieswGhostsB++;
                if((*THC)[i]->GetDetection()) 
                {
                    entriesB++;
                    man->FillNtupleIColumn(2, 0, evt);
                    man->FillNtupleDColumn(2, 1, (*THC)[i]->GetOpticalPhotonTime());
                    man->FillNtupleDColumn(2, 2, (*THC)[i]->GetDetectorPos().x());
                    man->FillNtupleDColumn(2, 3, (*THC)[i]->GetDetectorPos().y());
                    man->FillNtupleDColumn(2, 4, (*THC)[i]->GetDetectorPos().z());
                    man->FillNtupleIColumn(2, 5, (*THC)[i]->GetDetectorChannel());
                    man->AddNtupleRow(2);
                }
            }
        }

        man->FillNtupleIColumn(0, 0, evt);
        man->FillNtupleIColumn(0, 1, entriesF);
        man->FillNtupleIColumn(0, 2, entriesB);
        man->FillNtupleIColumn(0, 3, entriesF+entriesB);
        man->FillNtupleIColumn(0, 4, entrieswGhostsF);
        man->FillNtupleIColumn(0, 5, entrieswGhostsB);
        man->FillNtupleIColumn(0, 6, entrieswGhostsF+entrieswGhostsB);
        man->AddNtupleRow(0);
    }
    
}
