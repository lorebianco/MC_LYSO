/**
 * @file construction.hh
 * @brief Declaration of the class @ref MyDetectorConstruction
 */
#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4MultiUnion.hh"
#include "G4SubtractionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "globalsettings.hh"
#include "detector.hh"

/**
 * @brief Mandatory user initialization concrete class of
 * G4VUserDetectorConstruction. It represents the whole geometry of the
 * physical apparatus.
 */
class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    /**
     * @brief Constructor of the class.
     * 
     * It defines the UI commands through DefineCommands() and gives
     * default values to these settable variables: @ref fIsLightGuide = false,
     * @ref nLightGuideMat = 1, @ref fIsPCB = true and @ref fIsEndcap = true.
     * It also calls DefineMaterialsAndSurfaces().
     */
    MyDetectorConstruction();
    ~MyDetectorConstruction(); /**< @brief Destructor of the class.*/

    inline G4LogicalVolume *GetScoringVolume() const { return fScoringVolume;} /**< @brief Get the scoring volume. It will be used by MySteppingAction::UserSteppingAction().*/
    /**
     * @brief Construct the detector geometry.
     *
     * Depending on settings, it could call ConstructLightGuide(),
     * ConstructPCB() and ConstructEndcap().
     * It uses the auxiliary function PositionSiPMs() for placing the SiPMs
     * volumes and, at the end, it calls DefineVisAttributes().
     *
     * @return The world physical volume @ref physWorld.
     */
    virtual G4VPhysicalVolume *Construct();

private:
    virtual void ConstructSDandField(); /**< @brief It sets all the SiPMs' silicon layers as sensitive detectors.*/
    void ConstructLightGuide(); /**< @brief Auxiliary function called by Construct() for building the lightguides.*/
    void ConstructPCB(); /**< @brief Auxiliary function called by Construct() for building the PCBs.*/
    void ConstructEndcap(); /**< @brief Auxiliary function called by Construct() for building the endcaps.*/
    /**
     * @brief Auxiliary function called by Construct() for putting in the right
     * position every SiPM.
     * @param row The row of the SiPMs panel where the detector has to be placed
     * @param col The column of the SiPMs panel where the detector has to be
     * placed
     * @param halfCols The integer half of the number of SiPMs in the row
     * @param index The index number of the SiPM
     */
    void PositionSiPMs(G4int row, G4int col, G4int halfCols, G4int index);
    void DefineMaterials(); /**< @brief Defines all materials.*/
    void DefineVisAttributes(); /**< @brief Defines the visualization attributes for every component of the apparatus.*/
    void DefineCommands(); /**< @brief Defines new user commands for detector construction.*/

    // Solids
    G4Box    *solidWorld, /**< @brief Pointer to world solid.*/
             *solidPackageSiPM, /**< @brief Pointer to SiPM package solid.*/
             *solidWindowSiPM, /**< @brief Pointer to SiPM window solid.*/
             *solidDetector; /**< @brief Pointer to silicon layer of SiPM solid.*/
    G4Tubs   *solidScintillator, /**< @brief Pointer to crystal solid.*/
             *solidCoating, /**< @brief Pointer to coating solid.*/
             *solidPCB, /**< @brief Pointer to PCB solid.*/
             *solidEndcap; /**< @brief Pointer to endcap solid.*/
    G4VSolid *solidDrilledLightGuide; /**< @brief Pointer to light guide solid.*/

    // Logical volumes
    G4LogicalVolume *logicWorld, /**< @brief Pointer to world logical volume.*/
                    *logicScintillator, /**< @brief Pointer to crystal logical volume.*/
                    *logicPackageSiPM, /**< @brief Pointer to SiPM package logical volume.*/
                    *logicWindowSiPM, /**< @brief Pointer to SiPM window logical volume.*/
                    *logicDetector, /**< @brief Pointer to silicon layer of SiPM logical volume.*/
                    *logicCoating, /**< @brief Pointer to coating logical volume.*/
                    *logicPCB, /**< @brief Pointer to PCB logical volume.*/
                    *logicEndcap, /**< @brief Pointer to endcap logical volume.*/
                    *logicLightGuide; /**< @brief Pointer to light guide logical volume.*/
    
    // Scoring volume
    G4LogicalVolume *fScoringVolume; /**< @brief Pointer used to define the logical volume of the scoring volume. In the application it is assigned to @ref logicScintillator.*/

    // Physical volumes
    G4VPhysicalVolume *physWorld, /**< @brief Pointer to world physical volume.*/
                      *physScintillator, /**< @brief Pointer to crystal physical volume.*/
                      *physFrontPackageSiPM, /**< @brief Pointer to front SiPM package physical volume.*/
                      *physBackPackageSiPM, /**< @brief Pointer to back SiPM package physical volume.*/
                      *physWindowSiPM, /**< @brief Pointer to SiPM window physical volume.*/
                      *physDetector, /**< @brief Pointer to silicon layer of SiPM physical volume.*/
                      *physCoating, /**< @brief Pointer to coating physical volume.*/
                      *physFrontPCB, /**< @brief Pointer to front PCB physical volume.*/
                      *physBackPCB, /**< @brief Pointer to back PCB physical volume.*/
                      *physFrontEndcap, /**< @brief Pointer to front endcap physical volume.*/
                      *physBackEndcap, /**< @brief Pointer to back endcap physical volume.*/
                      *physFrontLightGuide, /**< @brief Pointer to front light guide physical volume.*/
                      *physBackLightGuide; /**< @brief Pointer to back light guide physical volume.*/

    // Vis Attributes 
    G4VisAttributes *visScintillator, /**< @brief Visualization settings for the crystal.*/
                    *visLightGuide, /**< @brief Visualization settings for the light guides.*/
                    *visCoating, /**< @brief Visualization settings for the coating.*/
                    *visPackage, /**< @brief Visualization settings for the SiPM package.*/
                    *visWindow, /**< @brief Visualization settings for the SiPM window.*/
                    *visDetector, /**< @brief Visualization settings for the SiPM silicon layer.*/
                    *visPCB, /**< @brief Visualization settings for the PCBs.*/
                    *visEndcap; /**< @brief Visualization settings for the endcaps.*/

    // Materials
    G4Material *fLYSO, /**< @brief Pointer to the LYSO material.*/
               *fAir, /**< @brief Pointer to the air material.*/
               *fVacuum, /**< @brief Pointer to the vacuum material.*/
               *fAluminium, /**< @brief Pointer to the aluminium material.*/
               *fSilicon, /**< @brief Pointer to the silicon material.*/
               *fEpoxy, /**< @brief Pointer to the epoxy material.*/
               *fFR4, /**< @brief Pointer to the FR4 material.*/
               *fCarbonFiber, /**< @brief Pointer to the carbon fiber material.*/
               *fPlexiglass, /**< @brief Pointer to the plexiglass material.*/
               *fSapphire; /**< @brief Pointer to the sapphire material.*/

    // Generic Messenger and settable variables 
    G4GenericMessenger *fMessenger; /**< @brief Generic messenger of the class.*/
    G4bool fIsLightGuide, /**< @brief Flag indicating whether the light guides must be constructed.*/
           fIsPCB, /**< @brief Flag indicating whether the PCBs must be constructed.*/
           fIsEndcap; /**< @brief Flag indicating whether the endcaps must be constructed.*/
    G4int nLightGuideMat; /**< @brief Indicates which material has to be used for light guides; 1 for plexiglass, 2 for sapphire.*/
};

#endif  // CONSTRUCTION_HH
