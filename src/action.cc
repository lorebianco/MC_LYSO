/**
 * @file action.cc
 * @brief Definition of the class @ref MyActionInitialization
 */
#include "action.hh"

void MyActionInitialization::BuildForMaster() const
{
    // Here must be initialized only Run Action
    MyEventAction *eventAction = new MyEventAction();
    
    MyRunAction *runAction = new MyRunAction(eventAction);
    SetUserAction(runAction);
}



void MyActionInitialization::Build() const
{
    // Initialize all the user actions. Only PrimaryGenerator is mandatory in G4
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    MyEventAction *eventAction = new MyEventAction();
    SetUserAction(eventAction);

    MyRunAction *runAction = new MyRunAction(eventAction);
    SetUserAction(runAction);

    MySteppingAction *steppingAction = new MySteppingAction(eventAction);
    SetUserAction(steppingAction);
}
