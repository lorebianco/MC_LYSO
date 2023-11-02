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


int main(int argc, char** argv)
{
    G4Random::setTheSeed(time(NULL));
    G4cout << "The seed of this MC is " << G4Random::getTheSeed() << G4endl;

    #ifdef G4MULTITHREADED
        G4MTRunManager *runManager = new G4MTRunManager();
    #else
        G4RunManager *runManager = new G4RunManager();
    #endif
    
    runManager->SetUserInitialization(new MyDetectorConstruction());
    //FTFP_BERT* physlist = new FTFP_BERT(1);
    //runManager->SetUserInitialization(physlist);
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

    //Qui decido se aprire l'interfaccia grafica: se c'è una macro da eseguire non la apro
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
    }

    delete runManager;
    return 0;
}