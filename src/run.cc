/**
 * @file run.cc
 * @brief Definition of the class @ref MyRunAction
 */
#include "run.hh"

MyRunAction::MyRunAction(MyEventAction *eventAction) : fEventAction(eventAction)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Data about primary gamma and physics inside the crystal
    man->CreateNtuple("Phys", "Primary Gamma and physics inside the Crystal");
    man->CreateNtupleIColumn("Event");
    man->CreateNtupleDColumn("E_gun");
    man->CreateNtupleDColumn("X_gun");
    man->CreateNtupleDColumn("Y_gun");
    man->CreateNtupleDColumn("Z_gun");
    man->CreateNtupleDColumn("MomX_gun");
    man->CreateNtupleDColumn("MomY_gun");
    man->CreateNtupleDColumn("MomZ_gun");
    man->CreateNtupleDColumn("ToA");
    man->CreateNtupleDColumn("XoA");
    man->CreateNtupleDColumn("YoA");
    man->CreateNtupleDColumn("ZoA");
    man->CreateNtupleDColumn("Edep");
    man->CreateNtupleDColumn("MaxEdep");
    man->CreateNtupleDColumn("MaxEdepPosX");
    man->CreateNtupleDColumn("MaxEdepPosY");
    man->CreateNtupleDColumn("MaxEdepPosZ");
    man->FinishNtuple(0);

    // Data of detectors
    man->CreateNtuple("Det", "Detectors");
    man->CreateNtupleIColumn("Event");
    man->CreateNtupleIColumn("HitsNum_F");
    man->CreateNtupleIColumn("HitsNum_B");
    man->CreateNtupleIColumn("HitsNum_Tot");
    man->CreateNtupleIColumn("HitsNumwGhosts_F");
    man->CreateNtupleIColumn("HitsNumwGhosts_B");
    man->CreateNtupleIColumn("HitsNumwGhosts_Tot");
    man->CreateNtupleDColumn("T_F", fEventAction->fT_F);
    man->CreateNtupleDColumn("X_F", fEventAction->fX_F);
    man->CreateNtupleDColumn("Y_F", fEventAction->fY_F);
    man->CreateNtupleDColumn("Z_F", fEventAction->fZ_F);
    man->CreateNtupleIColumn("Channel_F", fEventAction->fChannel_F);
    man->CreateNtupleDColumn("T_B", fEventAction->fT_B);
    man->CreateNtupleDColumn("X_B", fEventAction->fX_B);
    man->CreateNtupleDColumn("Y_B", fEventAction->fY_B);
    man->CreateNtupleDColumn("Z_B", fEventAction->fZ_B);
    man->CreateNtupleIColumn("Channel_B", fEventAction->fChannel_B);
    man->FinishNtuple(1);
}



MyRunAction::~MyRunAction()
{}



void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    // Create and open the file root
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;

    man->OpenFile("output_MCID_run" + strRunID.str() + ".root");
}



void MyRunAction::EndOfRunAction(const G4Run* run)
{
    // Write TTrees and close root file
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}