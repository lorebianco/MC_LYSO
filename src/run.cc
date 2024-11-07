/**
 * @file run.cc
 * @brief Definition of the class @ref MyRunAction
 */
#include "run.hh"

MyRunAction::MyRunAction(G4int theMCID, MyEventAction *eventAction) : fMCID(theMCID), fEventAction(eventAction)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Create the TTree
    man->CreateNtuple("lyso", "Primary Gamma, energy deposition inside the crystal and detectors output");
    // Data of primary gamma
    man->CreateNtupleIColumn("Event"); // entry 0
    man->CreateNtupleIColumn("PID_gun");
    man->CreateNtupleDColumn("E_gun");
    man->CreateNtupleDColumn("X_gun");
    man->CreateNtupleDColumn("Y_gun");
    man->CreateNtupleDColumn("Z_gun"); // entry 5
    man->CreateNtupleDColumn("MomX_gun");
    man->CreateNtupleDColumn("MomY_gun");
    man->CreateNtupleDColumn("MomZ_gun");
    man->CreateNtupleDColumn("ToA");
    man->CreateNtupleDColumn("XoA"); // entry 10
    man->CreateNtupleDColumn("YoA");
    man->CreateNtupleDColumn("ZoA");
    man->CreateNtupleDColumn("ToFI");
    man->CreateNtupleDColumn("XoFI");
    man->CreateNtupleDColumn("YoFI"); // entry 15
    man->CreateNtupleDColumn("ZoFI"); 
    // Data of energy deposition inside the crystal
    man->CreateNtupleDColumn("Edep");
    man->CreateNtupleDColumn("MaxEdep");
    man->CreateNtupleDColumn("MaxEdepPosX");
    man->CreateNtupleDColumn("MaxEdepPosY"); // entry 20
    man->CreateNtupleDColumn("MaxEdepPosZ"); 
    // Data of detectors
    man->CreateNtupleIColumn("NHits_F");
    man->CreateNtupleIColumn("NHits_B");
    man->CreateNtupleIColumn("NHits_Tot");
    man->CreateNtupleIColumn("NHits_F_Ch", fEventAction->fHitsNum_F_Ch); // entry 25
    man->CreateNtupleDColumn("T_F", fEventAction->fT_F);
    man->CreateNtupleDColumn("X_F", fEventAction->fX_F);
    man->CreateNtupleDColumn("Y_F", fEventAction->fY_F);
    man->CreateNtupleIColumn("Ch_F", fEventAction->fChannel_F);
    man->CreateNtupleIColumn("NHits_B_Ch", fEventAction->fHitsNum_B_Ch);
    man->CreateNtupleDColumn("T_B", fEventAction->fT_B); // entry 30
    man->CreateNtupleDColumn("X_B", fEventAction->fX_B);
    man->CreateNtupleDColumn("Y_B", fEventAction->fY_B);
    man->CreateNtupleIColumn("Ch_B", fEventAction->fChannel_B);

    man->FinishNtuple(0);
}



void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    // Create and open the file root
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    std::stringstream strMCID;
    strMCID << fMCID;

    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;

    if(!runID)
        man->OpenFile("MCID_" + strMCID.str() + ".root");
    else
    {
        man->OpenFile("MCID_" + strMCID.str() + "_RunID_" + strRunID.str() + ".root");
    }
}



void MyRunAction::EndOfRunAction(const G4Run* run)
{
    // Write TTree and close root file
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}