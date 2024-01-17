#include "action.hh"

MyActionInitialization::MyActionInitialization()
{}



MyActionInitialization::~MyActionInitialization()
{}



void MyActionInitialization::BuildForMaster() const
{
    MyEventAction *eventAction = new MyEventAction();
    
    MyRunAction *runAction = new MyRunAction(eventAction);
    SetUserAction(runAction);
}



void MyActionInitialization::Build() const
{
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    MyEventAction *eventAction = new MyEventAction();
    SetUserAction(eventAction);

    MyRunAction *runAction = new MyRunAction(eventAction);
    SetUserAction(runAction);

    MySteppingAction *steppingAction = new MySteppingAction(eventAction);
    SetUserAction(steppingAction);
}
