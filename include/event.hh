/**
 * @file event.hh
 * @brief Declaration of the class @ref MyEventAction
 */
#ifndef EVENT_HH
#define EVENT_HH

#include <vector>

#include "G4RunManager.hh"
#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"

#include "globalsettings.hh"
#include "hit.hh"
#include "generator.hh"

/** 
 * @brief User action concrete class of G4UserEventAction. In addition to
 * defining procedures executed at the start and end of the event, this class
 * also implements all the cointers for data to be saved in the output root
 * file, along with some methods for extracting them during the event.
 */
class MyEventAction : public G4UserEventAction
{
public:
    MyEventAction() = default; /**< @brief Constructor of the class.*/
    ~MyEventAction() override = default; /**< @brief Destructor of the class.*/

    /**
     * @brief Resets every data containers at the beginning of a new event.
     * 
     * @param event Pointer to the G4Event.
     */
    void BeginOfEventAction(const G4Event *event) override;
    /**
     * @brief Fills the TTree with the data of the event.
     *  
     * It accesses the MyHitsCollection of the event and fills the detector
     * branches with data stored in it. After that, it fills the other branches
     * with data concerning the primary particle and the energy deposit in the
     * crystal.
     *
     * @param event Pointer to the G4Event.
     */
    void EndOfEventAction(const G4Event *event) override;

    /**
     * @brief Stores the time and the position of arrival to the crystal of the primary gamma.
     * 
     * @param newtimein The time of arrival to be saved.
     * @param newposin The position of arrival to be saved.
     */
    inline void SetArrivalandFirstInteraction(G4double newtimein, G4ThreeVector newposin, G4double newtimeinter, G4ThreeVector newposinter)
    {
        if(newtimein < fTimeIn)
        {
            fTimeIn = newtimein;
            fPosXIn = newposin.x();
            fPosYIn = newposin.y();
            fPosZIn = newposin.z();

            fTimeFirstInter = newtimeinter;
            fPosXFirstInter = newposinter.x();
            fPosYFirstInter = newposinter.y();
            fPosZFirstInter = newposinter.z();
        }
    }
    /** 
     * @brief For every G4Step inside the crystal it sums the energy deposit.
     *
     * @param edep The total energy deposit in the step.
     */
    inline void AddEdep(G4double edep) { fEdep += edep; }
    /**
     * @brief Stores the maximum deposit of energy per unit length and its
     * position inside the crystal.
     *
     * @param edepondx The total energy deposit in the step divided by the step
     * length.
     * @param maxedeppos The position of the energy deposit in the step.
     */
    inline void SetMaxEdep(G4double edepondx, G4ThreeVector maxedeppos)
    {
        if(edepondx > fMaxEdep)
        {
            fMaxEdep = edepondx;
            fMaxEdepPos = maxedeppos;
        }
    };


    inline void SetDecayTriggerSi(G4bool trg) { fDecayTriggerSi = trg; }
    inline void SetCosmicTriggerUp(G4bool trg) { fCosmicTriggerUp = trg; }
    inline void SetCosmicTriggerBottom(G4bool trg) { fCosmicTriggerBottom = trg; }


    // Primary's data
    G4double fTimeIn, /**< @brief Time of arrival of primary gamma.*/
             fPosXIn, /**< @brief X position of arrival of primary gamma.*/
             fPosYIn, /**< @brief Y position of arrival of primary gamma.*/
             fPosZIn, /**< @brief Z position of arrival of primary gamma.*/
             fTimeFirstInter, /**< @brief Time of first interaction of primary gamma.*/
             fPosXFirstInter, /**< @brief X position of first interaction of primary gamma.*/
             fPosYFirstInter, /**< @brief Y position of first interaction of primary gamma.*/
             fPosZFirstInter; /**< @brief Z position of first interaction of primary gamma.*/
    
    // Crystal's data
    G4double      fEdep; /**< @brief Total energy deposited inside the crystal.*/
    G4double      fMaxEdep; /**< @brief Maximum deposit of energy per unit length (MeV/mm) in the crystal.*/
    G4ThreeVector fMaxEdepPos; /**< @brief Position of the maximum deposit of energy per unit length in the crystal.*/

    // Detectors' data
    G4int                 fHitsNum_F, /**< @brief Total number of optical photons detected by SiPMs on the front face.*/
                          fHitsNum_B; /**< @brief Total number of optical photons detected by SiPMs on the back face.*/
    std::vector<G4double> fT_F, /**< @brief Vector containing times of detection of optical photons on the front face.*/
                          fX_F, /**< @brief Vector containing x-positions of detection of optical photons on the front face.*/
                          fY_F, /**< @brief Vector containing y-positions of detection of optical photons on the front face.*/
                          fT_B, /**< @brief Vector containing times of detection of optical photons on the back face.*/
                          fX_B, /**< @brief Vector containing x-positions of detection of optical photons on the back face.*/
                          fY_B; /**< @brief Vector containing y-positions of detection of optical photons on the back face.*/
    std::vector<G4int>    fHitsNum_F_Ch,
                          fChannel_F, /**< @brief Vector containing SiPM channels of detection of optical photons on the front face.*/
                          fHitsNum_B_Ch,
                          fChannel_B; /**< @brief Vector containing SiPM channels of detection of optical photons on the back face.*/

    // Intercalibration triggers
    G4double fTimeOfDecay;
    G4bool   fDecayTriggerSi,
             fCosmicTriggerUp,
             fCosmicTriggerBottom;
};

#endif  // EVENT_HH
