/**
 * @file event.cc
 * @brief Definition of the class @ref MyEventAction
 */
#include "event.hh"

void MyEventAction::BeginOfEventAction(const G4Event *event)
{
    // Reset all event data
    fTimeIn = 999999.;
    fPosXIn = 999999.;
    fPosYIn = 999999.;
    fPosZIn = 999999.;
    fTimeFirstInter = 999999.;
    fPosXFirstInter = 999999.;
    fPosYFirstInter = 999999.;
    fPosZFirstInter = 999999.;    
    fEdep = 0.;
    fMaxEdep = 0.;
    fMaxEdepPos = G4ThreeVector(0., 0., 0.);
    fHitsNum_F = 0.;
    fHitsNum_B = 0.;
    fHitsNum_F_Ch.clear();
    fHitsNum_F_Ch = std::vector<G4int>(GS::nOfSiPMs, 0);
    fT_F.clear();
    fX_F.clear();
    fY_F.clear();
    fChannel_F.clear();
    fHitsNum_B_Ch.clear();
    fHitsNum_B_Ch = std::vector<G4int>(GS::nOfSiPMs, 0);
    fT_B.clear();
    fX_B.clear();
    fY_B.clear();
    fChannel_B.clear();
    fDecayTriggerSi = false;
    fCosmicTriggerUp = false;
    fCosmicTriggerBottom = false;
}



void MyEventAction::EndOfEventAction(const G4Event *event)
{
    // Settings depending on run mode type
    G4int modeType = static_cast<const MyPrimaryGenerator*>(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction())->GetModeType();

    switch(modeType)
    {
        case 22: // 176Lu decay with trigger
            if(!fDecayTriggerSi)
                return;
        case 30: // Cosmic rays
        case 31:
            if(!(fCosmicTriggerUp*fCosmicTriggerBottom))
                return;
    }


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
                fHitsNum_F_Ch[(*THC)[i]->GetDetectorChannel()]++;
                fT_F.push_back((*THC)[i]->GetDetectionTime());
                fX_F.push_back((*THC)[i]->GetDetectorPosition().x());
                fY_F.push_back((*THC)[i]->GetDetectorPosition().y());
                fChannel_F.push_back((*THC)[i]->GetDetectorChannel());
            }

            if((*THC)[i]->GetDetectorPosition().z() > GS::zScintillator)
            {
                fHitsNum_B++;
                fHitsNum_B_Ch[(*THC)[i]->GetDetectorChannel()]++;
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
    G4int evt = event->GetEventID();
    
    // Fill the primary gamma branches
    man->FillNtupleIColumn(0, evt);
    man->FillNtupleIColumn(1, primaryParticle->GetParticleDefinition()->GetPDGEncoding());
    man->FillNtupleDColumn(2, primaryParticle->GetTotalEnergy());
    man->FillNtupleDColumn(3, primaryVertex->GetX0());
    man->FillNtupleDColumn(4, primaryVertex->GetY0());
    man->FillNtupleDColumn(5, primaryVertex->GetZ0());
    man->FillNtupleDColumn(6, primaryParticle->GetMomentumDirection().x());
    man->FillNtupleDColumn(7, primaryParticle->GetMomentumDirection().y());
    man->FillNtupleDColumn(8, primaryParticle->GetMomentumDirection().z());
    man->FillNtupleDColumn(9, fTimeIn);
    man->FillNtupleDColumn(10, fPosXIn);
    man->FillNtupleDColumn(11, fPosYIn);
    man->FillNtupleDColumn(12, fPosZIn);
    man->FillNtupleDColumn(13, fTimeFirstInter);
    man->FillNtupleDColumn(14, fPosXFirstInter);
    man->FillNtupleDColumn(15, fPosYFirstInter);
    man->FillNtupleDColumn(16, fPosZFirstInter);
    // Fill the energy deposition branches
    man->FillNtupleDColumn(17, fEdep);
    man->FillNtupleDColumn(18, fMaxEdep);
    man->FillNtupleDColumn(19, fMaxEdepPos.x());
    man->FillNtupleDColumn(20, fMaxEdepPos.y());
    man->FillNtupleDColumn(21, fMaxEdepPos.z());
    // Fill the detectors branches
    man->FillNtupleIColumn(22, fHitsNum_F);
    man->FillNtupleIColumn(23, fHitsNum_B);
    man->FillNtupleIColumn(24, fHitsNum_F + fHitsNum_B);
    // Close the row
    man->AddNtupleRow(0);
}
