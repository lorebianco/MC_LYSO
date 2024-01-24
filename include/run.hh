/**
 * @file run.hh
 * @brief Declaration of the class @ref MyRunAction
 */
#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

#include "event.hh"

/**
 * @brief User action concrete class of G4UserRunAction. It defines procedures
 * to be executed at the start and at the end of the run.
 */
class MyRunAction : public G4UserRunAction
{
public:
    /**
     * @brief Constructor of the class.
     *
     * It creates TTrees and their corresponding branches to store simulation
     * output.
     * 
     * @param eventAction Pointer to a MyEventAction object, necessary for
     * associating data saved in each event with the TTrees.
     */
    MyRunAction(MyEventAction* eventAction);
    ~MyRunAction(); /**< @brief Destructor of the class.*/

    /**
     * @brief Creates and accesses the output root file at the beginning of the
     * run.
     * 
     * @param run Pointer to the G4Run.
     */
    virtual void BeginOfRunAction(const G4Run* run);
    /**
     * @brief Writes TTrees to the output root file and closes it at the end of
     * the run.
     * 
     * @param run Pointer to the G4Run.
     */
    virtual void EndOfRunAction(const G4Run* run);

private:
    MyEventAction *fEventAction; /**< @brief Pointer to the MyEventAction object.*/
};

#endif  // RUN_HH
