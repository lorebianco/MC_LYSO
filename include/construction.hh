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
    ~MyDetectorConstruction() override = default; /**< @brief Destructor of the class.*/

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
    G4VPhysicalVolume *Construct() override;

private:
    void ConstructSDandField() override; /**< @brief It sets all the SiPMs' silicon layers as sensitive detectors.*/
    void ConstructGrease(); /**< @brief Auxiliary function called by Construct() for building the optical grease.*/
    void ConstructLightGuide(); /**< @brief Auxiliary function called by Construct() for building the lightguides.*/
    void ConstructPCB(); /**< @brief Auxiliary function called by Construct() for building the PCBs.*/
    void ConstructEndcap(); /**< @brief Auxiliary function called by Construct() for building the endcaps.*/
    void ConstructASiPM(); /**< @brief Auxiliary function called by Construct() for building only one SiPM.*/

    /**
     * @brief Auxiliary function called by Construct() for putting in the right
     * position every SiPM.
     * @param physFrontSiPM The pointer to the physical volume of the SiPM to
     * place on the front face of the scintillator
     * @param physBackSiPM The pointer to the physical volume of the SiPM to
     * place on the back face of the scintillator
     * @param row The row of the SiPMs panel where the detector has to be placed
     * @param col The column of the SiPMs panel where the detector has to be
     * placed
     * @param halfCols The integer half of the number of SiPMs in the row
     * @param index The index number of the SiPM
     */
    void PositionSiPMs(G4VPhysicalVolume *physFrontSiPM, G4VPhysicalVolume *physBackSiPM, G4int row, G4int col, G4int halfCols, G4int index);
    void DefineMaterials(); /**< @brief Defines all materials.*/
    void DefineVisAttributes(); /**< @brief Defines the visualization attributes for every component of the apparatus.*/
    void DefineCommands(); /**< @brief Defines new user commands for detector construction.*/

    // Logical volumes
    G4LogicalVolume *logicWorld, /**< @brief Pointer to world logical volume.*/
                    *logicScintillator, /**< @brief Pointer to crystal logical volume.*/
                    *logicPackageSiPM, /**< @brief Pointer to SiPM package logical volume.*/
                    *logicWindowSiPM, /**< @brief Pointer to SiPM window logical volume.*/
                    *logicDetector, /**< @brief Pointer to silicon layer of SiPM logical volume.*/
                    *logicCoating, /**< @brief Pointer to coating logical volume.*/
                    *logicPCB, /**< @brief Pointer to PCB logical volume.*/
                    *logicEndcap, /**< @brief Pointer to endcap logical volume.*/
                    *logicLightGuide, /**< @brief Pointer to light guide logical volume.*/
                    *logicGrease; /**< @brief Pointer to optical grease logical volume.*/
    
    // Scoring logical volume
    G4LogicalVolume *fScoringVolume; /**< @brief Pointer used to define the logical volume of the scoring volume. In the application it is assigned to @ref logicScintillator.*/

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
               *fSapphire, /**< @brief Pointer to the sapphire material.*/
               *fGrease; /**< @brief Pointer to the optical grease material.*/
    
    // Surfaces
    G4OpticalSurface *fOpGreaseSurface; /**< @brief Pointer to the optical grease surface.*/

    // Generic Messenger and settable variables 
    G4GenericMessenger *fMessenger; /**< @brief Generic messenger of the class.*/
    G4bool fIsGrease, /**< @brief Flag indicating whether the optical grease must be constructed.*/
           fIsOpticalGreaseSurface, /**< @brief Flag indicating whether the optical grease surface must be constructed.*/
           fIsLightGuide, /**< @brief Flag indicating whether the light guides must be constructed.*/
           fIsPCB, /**< @brief Flag indicating whether the PCBs must be constructed.*/
           fIsEndcap, /**< @brief Flag indicating whether the endcaps must be constructed.*/
           fIsASiPM; /**< @brief Flag indicating whether only one SiPM must be constructed.*/
    G4int nLightGuideMat; /**< @brief Indicates which material has to be used for light guides; 1 for plexiglass, 2 for sapphire.*/
};

#endif  // CONSTRUCTION_HH
