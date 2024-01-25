/**
 * @file event.cc
 * @brief Definition of the class @ref MyEventAction
 */
#include "event.hh"

MyEventAction::MyEventAction()
{}



MyEventAction::~MyEventAction()
{}



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
    fHitsNumwGhosts_F = 0.;
    fHitsNumwGhosts_B = 0.;
    fT_F.clear();
    fX_F.clear();
    fY_F.clear();
    fZ_F.clear();
    fChannel_F.clear();
    fT_B.clear();
    fX_B.clear();
    fY_B.clear();
    fZ_B.clear();
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
        if((*THC)[i]->GetDetectorPos()[2] < GS::zScintillator)
            {   
                fHitsNumwGhosts_F++;
                if((*THC)[i]->GetDetection())
                {
                    fHitsNum_F++;
                    fT_F.push_back((*THC)[i]->GetOpticalPhotonTime());
                    fX_F.push_back((*THC)[i]->GetDetectorPos().x());
                    fY_F.push_back((*THC)[i]->GetDetectorPos().y());
                    fZ_F.push_back((*THC)[i]->GetDetectorPos().z());
                    fChannel_F.push_back((*THC)[i]->GetDetectorChannel());
                }
            }

            if((*THC)[i]->GetDetectorPos()[2] > GS::zScintillator)
            {
                fHitsNumwGhosts_B++;
                if((*THC)[i]->GetDetection()) 
                {
                    fHitsNum_B++;
                    fT_B.push_back((*THC)[i]->GetOpticalPhotonTime());
                    fX_B.push_back((*THC)[i]->GetDetectorPos().x());
                    fY_B.push_back((*THC)[i]->GetDetectorPos().y());
                    fZ_B.push_back((*THC)[i]->GetDetectorPos().z());
                    fChannel_B.push_back((*THC)[i]->GetDetectorChannel());
                }
            }
    }

    // Access info about primary particle
    G4PrimaryVertex* primaryVertex = event->GetPrimaryVertex();
    G4PrimaryParticle *primaryParticle = primaryVertex->GetPrimary(); 

    // Store data
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
    // Fill the physics TTree
    man->FillNtupleIColumn(0, 0, evt);
    man->FillNtupleDColumn(0, 1, primaryParticle->GetTotalEnergy());
    man->FillNtupleDColumn(0, 2, primaryVertex->GetX0());
    man->FillNtupleDColumn(0, 3, primaryVertex->GetY0());
    man->FillNtupleDColumn(0, 4, primaryVertex->GetZ0());
    man->FillNtupleDColumn(0, 5, primaryParticle->GetMomentumDirection().x());
    man->FillNtupleDColumn(0, 6, primaryParticle->GetMomentumDirection().y());
    man->FillNtupleDColumn(0, 7, primaryParticle->GetMomentumDirection().z());
    man->FillNtupleDColumn(0, 8, fTimeIn);
    man->FillNtupleDColumn(0, 9, fPosIn.x());
    man->FillNtupleDColumn(0, 10, fPosIn.y());
    man->FillNtupleDColumn(0, 11, fPosIn.z());
    man->FillNtupleDColumn(0, 12, fEdep);
    man->FillNtupleDColumn(0, 13, fMaxEdep);
    man->FillNtupleDColumn(0, 14, fMaxEdepPos.x());
    man->FillNtupleDColumn(0, 15, fMaxEdepPos.y());
    man->FillNtupleDColumn(0, 16, fMaxEdepPos.z());
    man->AddNtupleRow(0);

    // Fill the detector TTree
    man->FillNtupleIColumn(1, 0, evt);
    man->FillNtupleIColumn(1, 1, fHitsNum_F);
    man->FillNtupleIColumn(1, 2, fHitsNum_B);
    man->FillNtupleIColumn(1, 3, fHitsNum_F + fHitsNum_B);
    man->FillNtupleIColumn(1, 4, fHitsNumwGhosts_F);
    man->FillNtupleIColumn(1, 5, fHitsNumwGhosts_B);
    man->FillNtupleIColumn(1, 6, fHitsNumwGhosts_F + fHitsNumwGhosts_B);
    man->AddNtupleRow(1);
}
