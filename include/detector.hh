/**
 * @file detector.hh
 * @brief Declaration of the class @ref MySensitiveDetector
 */
#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"

#include "globalsettings.hh"
#include "hit.hh"

/**
 * @brief Concrete class of G4VSensitiveDetector, representing the detector
 * (i.e. the SiPM).
 */
class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    /**
     * @brief Constructor of the class.
     * @param name The name of the sensitive detector.
     * @param hitsCollectionName The name of the @ref MyHitsCollection
     * associated with the SD.
     */
    MySensitiveDetector(G4String name, G4String hitsCollectionName);
    ~MySensitiveDetector(); /**< @brief Destructor of the class.*/
    
    /**
     * @brief Associates a new @ref MyHitsCollection with a G4HCofThisEvent
     * object at the beginning of each event.
     * 
     * @param hce Pointer to the G4HCofThisEvent object.
     */
    virtual void Initialize(G4HCofThisEvent *hce);
    /**
     * @brief For every optical photon that hits the SD this method
     * instantiates a MyHit object, fills his data and store it in the
     * @ref MyHitsCollection of the event.
     *
     * This method is invoked by G4SteppingManager when a step is composed in
     * the G4LogicalVolume which has the pointer to this SD.
     *
     * @param aStep G4Step object of the current step.
     * @param ROhist G4TouchableHistory object. Obsolete, not used.
     */
    virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);
    //virtual void EndOfEvent(G4HCofThisEvent*);

private:
    MyHit *newHit; /**< @brief Pointer to the MyHit of the optical photon.*/
    MyHitsCollection *fHitsCollection; /**< @brief Pointer to the hits collection of the event.*/
};

#endif  // DETECTOR_HH
