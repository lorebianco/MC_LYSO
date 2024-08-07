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
    man->CreateNtupleDColumn("E_gun");
    man->CreateNtupleDColumn("X_gun");
    man->CreateNtupleDColumn("Y_gun");
    man->CreateNtupleDColumn("Z_gun");
    man->CreateNtupleDColumn("MomX_gun"); // entry 5
    man->CreateNtupleDColumn("MomY_gun");
    man->CreateNtupleDColumn("MomZ_gun");
    man->CreateNtupleDColumn("ToA");
    man->CreateNtupleDColumn("XoA");
    man->CreateNtupleDColumn("YoA"); // entry 10
    // Data of energy deposition inside the crystal
    man->CreateNtupleDColumn("Edep");
    man->CreateNtupleDColumn("MaxEdep");
    man->CreateNtupleDColumn("MaxEdepPosX");
    man->CreateNtupleDColumn("MaxEdepPosY"); 
    man->CreateNtupleDColumn("MaxEdepPosZ"); // entry 15
    // Data of detectors
    man->CreateNtupleIColumn("NHits_F");
    man->CreateNtupleIColumn("NHits_B");
    man->CreateNtupleIColumn("NHits_Tot");
    man->CreateNtupleDColumn("T_F", fEventAction->fT_F); 
    man->CreateNtupleDColumn("X_F", fEventAction->fX_F); // entry 20
    man->CreateNtupleDColumn("Y_F", fEventAction->fY_F);
    man->CreateNtupleIColumn("Ch_F", fEventAction->fChannel_F);
    man->CreateNtupleDColumn("T_B", fEventAction->fT_B);
    man->CreateNtupleDColumn("X_B", fEventAction->fX_B); 
    man->CreateNtupleDColumn("Y_B", fEventAction->fY_B); // entry 25
    man->CreateNtupleIColumn("Ch_B", fEventAction->fChannel_B);

    man->FinishNtuple(0);
}



void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    // Create and open the file root
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    std::stringstream strMCID;
    strMCID << fMCID;

    man->OpenFile("MCID_" + strMCID.str() + ".root");
}



void MyRunAction::EndOfRunAction(const G4Run* run)
{
    // Write TTree and close root file
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}