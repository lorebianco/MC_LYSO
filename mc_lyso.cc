//****************************************************************************//
//                        Lorenzo Bianco 27/10/2023                           //
//                                                                            //
//        My Montecarlo Simulation for the LYSO calorimeter prototype         //
//                                                                            //
//****************************************************************************//

/**
 * @file mc_lyso.cc
 * @brief Definition of the @ref main function of MC_LYSO
 */
#include <iostream>
#include <chrono>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4VisManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include "summary.hh"

/** @brief Main of the application */
int main(int argc, char** argv)
{
    // Set randomly the seed of the simulation and store it
    G4Random::setTheSeed(time(NULL));
    G4int fSeed = G4Random::getTheSeed();

    // Construct the run manager
    #ifdef G4MULTITHREADED
        G4MTRunManager *runManager = new G4MTRunManager();
    #else
        G4RunManager *runManager = new G4RunManager();
    #endif
    
    // Set mandatory initialization classes
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());
    
    // Detect interactive mode (if no arguments) and define UI session
    G4UIExecutive *ui = 0;
    if(argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    // Initialize visualization
    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // Start UI session or process macro
    if(ui)
    {
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else
    {
        // Batch mode
        auto start = std::chrono::high_resolution_clock::now();
        
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        
        // Save a summary of the simulation
        MC_summary(fileName, fSeed, duration.count(), "MC_summaries.txt");
        G4cout << G4endl;
        G4cout << "If you have any custom settings to annotate in the summary, please edit the MC_summaries file at the corresponding MC-SerialNumber." << G4endl;
        G4cout << "Now you should name the hadded-rootfile as \" MCID_" << fSeed << ".root \"." << G4endl;
    }

    // Job termination
    delete visManager;
    delete runManager;
    return 0;
}