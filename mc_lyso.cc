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
#include <cstdlib> 
#include <sstream>
#include <cstring>

#include "G4RunManagerFactory.hh"
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
    // Seed
    G4int fSeed = 0;

    // Parsing command line arguments
    for(G4int i = 1; i < argc; i++)
    {
        if(G4String(argv[i]) == "-s" || G4String(argv[i]) == "-S")
        {
            if(i + 1 < argc)
            {
                fSeed = std::stoi(argv[i + 1]);
                G4cout << "Seed set manually to: " << fSeed << G4endl;
                ++i; // Skip the next argument since it is the seed
            }
            else
            {
                G4cerr << "Error: Missing seed value after -s or -S." << G4endl;
                return 1;
            }
        }
    }

    // If no seed is passed, set the seed randomly
    if(fSeed == 0)
        G4Random::setTheSeed(time(NULL) + getpid());
    else
        G4Random::setTheSeed(fSeed);
    
    // Save the seed of the simulation
    fSeed = G4Random::getTheSeed();

    // Construct the run manager
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    
    // Set mandatory initialization classes
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization(fSeed));
    
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
        G4String fileName = argv[argc - 1];
        UImanager->ApplyCommand(command+fileName);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        
        // Save a summary of the simulation
        MC_summary(fileName, fSeed, duration.count(), "MC_summaries.txt");
        G4cout << G4endl;

        // Execute hadd shell command for MT Run Manager
        if(runManager->GetRunManagerType() == 1)
        {
            G4int nOfRuns = runManager->GetCurrentRun()->GetRunID();
            std::ostringstream shell_command;
            std::stringstream strMCID;
            strMCID << fSeed;
            std::stringstream strRunID;

            // First run hadd
            shell_command << "hadd -v 0 MCID_" + strMCID.str() + ".root MCID_" + strMCID.str() + "_t*";
            system(shell_command.str().c_str());

            // Loop for next runs
            for(G4int i = 1; i <= nOfRuns; i++)
            {
                shell_command.str("");
                shell_command.clear();
                strRunID.str("");
                strRunID.clear();

                if(i == 1)
                {
                    std::ostringstream shell_command_0;
                    shell_command_0 << "mv MCID_" + strMCID.str() + ".root MCID_" + strMCID.str() + "_RunID_0.root";
                    system(shell_command_0.str().c_str());
                }

                strRunID << i;

                shell_command << "hadd -v 0 MCID_" + strMCID.str() + "_RunID_" + strRunID.str() + ".root MCID_" + strMCID.str() + "_RunID_" + strRunID.str() + "_t*";
                system(shell_command.str().c_str());
            }

            // Move files in RootFiles directory
            std::ostringstream move_files_command;
            move_files_command << "rm *" + strMCID.str() + "_t* && mv MCID_" + strMCID.str() + ".root ./RootFiles/";
            system(move_files_command.str().c_str());
        }
    }

    // Finally print some useful messages
    G4cout << G4endl;
    G4cout << "If you have any custom settings to annotate in the summary, please edit the MC_summaries file at the corresponding MC-SerialNumber." << G4endl;
    G4cout << "The rootfiles of this Monte Carlo have serial number (MCID): " << fSeed << G4endl;
    G4cout << G4endl;    

    // Job termination
    delete visManager;
    delete runManager;
    return 0;
}
