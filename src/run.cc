#include "run.hh"


MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    //Some Total Data from both detectors
    man->CreateNtuple("Tot", "boTh Detectors");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleIColumn("fF");
    man->CreateNtupleIColumn("fB");
    man->CreateNtupleIColumn("fTot");
    man->CreateNtupleIColumn("fwGhosts_F");
    man->CreateNtupleIColumn("fwGhosts_B");
    man->CreateNtupleIColumn("fwGhosts_Tot");
    man->FinishNtuple(0);

    //Data front detector
    man->CreateNtuple("F", "Front Detector");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fT");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleIColumn("fChannel");
    man->FinishNtuple(1);

    //Data back detector
    man->CreateNtuple("B", "Back Detector");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fT");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleIColumn("fChannel");
    man->FinishNtuple(2);

    //Data physics inside crystal
    man->CreateNtuple("Cry", "Physics inside Crystal");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fTin");
    man->CreateNtupleDColumn("fXin");
    man->CreateNtupleDColumn("fYin");
    man->CreateNtupleDColumn("fZin");
    man->CreateNtupleDColumn("fEdep");
    man->CreateNtupleDColumn("fMaxEdep");
    man->CreateNtupleDColumn("fMaxEdepPosX");
    man->CreateNtupleDColumn("fMaxEdepPosY");
    man->CreateNtupleDColumn("fMaxEdepPosZ");
    man->FinishNtuple(3);

    //Data primary gamma
    man->CreateNtuple("Gamma", "Primary Gamma");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fE");
    man->CreateNtupleDColumn("fX_start");
    man->CreateNtupleDColumn("fY_start");
    man->CreateNtupleDColumn("fZ_start");
    man->CreateNtupleDColumn("fMomX_start");
    man->CreateNtupleDColumn("fMomY_start");
    man->CreateNtupleDColumn("fMomZ_start");
    man->FinishNtuple(4);


}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();
    
    std::stringstream strRunID;

    strRunID << runID;

    man->OpenFile("output_MCID_run" + strRunID.str() + ".root");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}
