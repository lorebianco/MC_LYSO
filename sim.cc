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


int main(int argc, char** argv)
{
    G4Random::setTheSeed(time(NULL));
    G4int seed = G4Random::getTheSeed();

    #ifdef G4MULTITHREADED
        G4MTRunManager *runManager = new G4MTRunManager();
    #else
        G4RunManager *runManager = new G4RunManager();
    #endif
    
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());

    
    #ifndef G4MULTITHREADED
        runManager->Initialize();
    #endif


    G4UIExecutive *ui = 0;
    if(argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }


    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();


    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    //If there is a macro do not open the visualization
    if(ui)
    {
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);

        MC_summary(seed, "MC_summaries.txt");
    }

    delete runManager;
    return 0;
}