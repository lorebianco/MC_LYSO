/**
 * @file action.hh
 * @brief Declaration of the class @ref MyActionInitialization
 */
#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"

#include "generator.hh"
#include "run.hh"
#include "event.hh"
#include "stepping.hh"

/** 
 * @brief Mandatory user initialization concrete class of
 * G4VUserActionInitialization. It configures all the necessary user
 * action classes.
 */ 
class MyActionInitialization : public G4VUserActionInitialization
{
public:
    MyActionInitialization(); /**< @brief Constructor of the class.*/
    ~MyActionInitialization(); /**< @brief Destructor of the class.*/
    
    /** 
     * @brief Configures user action classes for worker threads.
     *
     * In addition to @ref MyPrimaryGenerator, these include @ref MyRunAction(),
     * @ref MyEventAction(), and @ref MySteppingAction().
     */
    virtual void Build() const;
    /** 
     * @brief Configures the @ref MyRunAction class for the master thread
     * (in multithreading mode).
     */
    virtual void BuildForMaster() const;
};

#endif  // ACTION_HH
