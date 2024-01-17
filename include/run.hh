#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

#include "event.hh"


class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction(MyEventAction* eventAction);
    ~MyRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

private:
    MyEventAction *fEventAction;
};

#endif  // RUN_HH
