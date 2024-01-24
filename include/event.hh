/**
 * @file event.hh
 * @brief Declaration of the class @ref MyEventAction
 */
#ifndef EVENT_HH
#define EVENT_HH

#include <vector>

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

#include "globalsettings.hh"
#include "hit.hh"

/** 
 * @brief User action concrete class of G4UserEventAction. In addition to
 * defining procedures executed at the start and end of the event, this class
 * also implements all the cointers for data to be saved in the output root
 * file, along with some methods for extracting them during the event.
 */
class MyEventAction : public G4UserEventAction
{
public:
    MyEventAction(); /**< @brief Constructor of the class.*/
    ~MyEventAction(); /**< @brief Destructor of the class.*/

    /**
     * @brief Resets every data containers at the beginning of a new event.
     * 
     * @param event Pointer to the G4Event.
     */
    virtual void BeginOfEventAction(const G4Event *event);
    /**
     * @brief Fills the TTrees with the data of the event.
     *  
     * It accesses the MyHitsCollection of the event and fills the detector
     * TTree with data stored in it. After that, it fills the physics TTree
     * with data concerning the primary particle and the energy deposit in the
     * crystal.
     *
     * @param event Pointer to the G4Event.
     */
    virtual void EndOfEventAction(const G4Event *event);

    /**
     * @brief Stores the time of arrival to the crystal of the primary gamma.
     * 
     * @param newtimein The time of arrival to be saved.
     */
    inline void FindEntryTime(G4double newtimein) { fTimeIn = newtimein; }
    /**
     * @brief Stores the position of arrival to the crystal of the primary
     * gamma.
     * 
     * @param newposin The time of arrival to be saved.
     */
    inline void FindEntryPosition(G4ThreeVector newposin) { fPosIn = newposin; }
    /** 
     * @brief For every G4Step inside the crystal it sums the energy deposit.
     *
     * @param edep The total energy deposit in the step.
     */
    inline void AddEdep(G4double edep) { fEdep += edep; }
    /**
     * @brief Stores the maximum deposit of energy in a G4Step and its position
     * inside the crystal.
     *
     * @param edep The total energy deposit in the step.
     * @param maxedeppos The position of the energy deposit in the step.
     */
    inline void FindMaxEdep(G4double edep, G4ThreeVector maxedeppos)
    {
        if(edep>fMaxEdep)
        {
            fMaxEdep = edep;
            fMaxEdepPos = maxedeppos;
        }
    };

    // Primary's data
    G4double      fTimeIn; /**< @brief Time of arrival of primary gamma.*/
    G4ThreeVector fPosIn; /**< @brief Position of arrival of primary gamma.*/
    
    // Crystal's data
    G4double      fEdep; /**< @brief Total energy deposited inside the crystal.*/
    G4double      fMaxEdep; /**< @brief Maximum deposit of energy (in a step) in the crystal.*/
    G4ThreeVector fMaxEdepPos; /**< @brief Position of the maximum deposit of energy (in a step) in the crystal.*/

    // Detectors' data
    G4int                 fHitsNum_F, /**< @brief Total number of optical photons detected by SiPMs on the front face.*/
                          fHitsNum_B, /**< @brief Total number of optical photons detected by SiPMs on the back face.*/
                          fHitsNumwGhosts_F, /**< @brief Total number of optical photons that reached the SiPMs on the front face.*/
                          fHitsNumwGhosts_B; /**< @brief Total number of optical photons that reached the SiPMs on the back face.*/
    std::vector<G4double> fT_F, /**< @brief Vector containing times of detection of optical photons on the front face.*/
                          fX_F, /**< @brief Vector containing x-positions of detection of optical photons on the front face.*/
                          fY_F, /**< @brief Vector containing y-positions of detection of optical photons on the front face.*/
                          fZ_F, /**< @brief Vector containing z-positions of detection of optical photons on the front face.*/
                          fT_B, /**< @brief Vector containing times of detection of optical photons on the back face.*/
                          fX_B, /**< @brief Vector containing x-positions of detection of optical photons on the back face.*/
                          fY_B, /**< @brief Vector containing y-positions of detection of optical photons on the back face.*/
                          fZ_B; /**< @brief Vector containing z-positions of detection of optical photons on the back face.*/
    std::vector<G4int>    fChannel_F, /**< @brief Vector containing SiPM channels of detection of optical photons on the front face.*/
                          fChannel_B; /**< @brief Vector containing SiPM channels of detection of optical photons on the back face.*/
};

#endif  // EVENT_HH
