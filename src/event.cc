/**
 * @file event.cc
 * @brief Definition of the class @ref MyEventAction
 */
#include "event.hh"

void MyEventAction::BeginOfEventAction(const G4Event *event)
{
    // Reset all event data
    fTimeIn = 0.;
    fPosIn = G4ThreeVector(0., 0., 0.);
    fEdep = 0.;
    fMaxEdep = 0.;
    fMaxEdepPos = G4ThreeVector(0., 0., 0.);
    fHitsNum_F = 0.;
    fHitsNum_B = 0.;
    fT_F.clear();
    fX_F.clear();
    fY_F.clear();
    fChannel_F.clear();
    fT_B.clear();
    fX_B.clear();
    fY_B.clear();
    fChannel_B.clear();
}



void MyEventAction::EndOfEventAction(const G4Event *event)
{
    // Access the hit collection
    G4HCofThisEvent *hce = event->GetHCofThisEvent();
    MyHitsCollection *THC = (MyHitsCollection *)(hce->GetHC(0));
    
    if(!THC)
        return;

    // Fill vectors of data about hits
    G4int nHits = THC->entries();
    for(G4int i = 0; i < nHits; i++)
    {
        if((*THC)[i]->GetDetectorPosition().z() < GS::zScintillator)
            {   
                fHitsNum_F++;
                fT_F.push_back((*THC)[i]->GetDetectionTime());
                fX_F.push_back((*THC)[i]->GetDetectorPosition().x());
                fY_F.push_back((*THC)[i]->GetDetectorPosition().y());
                fChannel_F.push_back((*THC)[i]->GetDetectorChannel());    
            }

            if((*THC)[i]->GetDetectorPosition().z() > GS::zScintillator)
            {
                fHitsNum_B++;
                fT_B.push_back((*THC)[i]->GetDetectionTime());
                fX_B.push_back((*THC)[i]->GetDetectorPosition().x());
                fY_B.push_back((*THC)[i]->GetDetectorPosition().y());
                fChannel_B.push_back((*THC)[i]->GetDetectorChannel());    
            }
    }

    // Access info about primary particle
    G4PrimaryVertex* primaryVertex = event->GetPrimaryVertex();
    G4PrimaryParticle *primaryParticle = primaryVertex->GetPrimary(); 

    // Store data
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
    // Fill the primary gamma branches
    man->FillNtupleIColumn(0, evt);
    man->FillNtupleDColumn(1, primaryParticle->GetTotalEnergy());
    man->FillNtupleDColumn(2, primaryVertex->GetX0());
    man->FillNtupleDColumn(3, primaryVertex->GetY0());
    man->FillNtupleDColumn(4, primaryVertex->GetZ0());
    man->FillNtupleDColumn(5, primaryParticle->GetMomentumDirection().x());
    man->FillNtupleDColumn(6, primaryParticle->GetMomentumDirection().y());
    man->FillNtupleDColumn(7, primaryParticle->GetMomentumDirection().z());
    man->FillNtupleDColumn(8, fTimeIn);
    man->FillNtupleDColumn(9, fPosIn.x());
    man->FillNtupleDColumn(10, fPosIn.y());
    man->FillNtupleDColumn(11, fPosIn.z());
    // Fill the energy deposition branches
    man->FillNtupleDColumn(12, fEdep);
    man->FillNtupleDColumn(13, fMaxEdep);
    man->FillNtupleDColumn(14, fMaxEdepPos.x());
    man->FillNtupleDColumn(15, fMaxEdepPos.y());
    man->FillNtupleDColumn(16, fMaxEdepPos.z());
    // Fill the detectors branches
    man->FillNtupleIColumn(17, fHitsNum_F);
    man->FillNtupleIColumn(18, fHitsNum_B);
    man->FillNtupleIColumn(19, fHitsNum_F + fHitsNum_B);
    // Close the row
    man->AddNtupleRow(0);
}
