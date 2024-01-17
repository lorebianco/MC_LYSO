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


class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    inline G4LogicalVolume *GetScoringVolume() const { return fScoringVolume;}

    virtual G4VPhysicalVolume *Construct();

private:
    virtual void ConstructSDandField();
    void DefineMaterialsAndSurfaces();
    void DefineVisAttributes();
    void DefineCommands();

    // Solidi
    G4Box *solidWorld, *solidPackageSiPM, *solidWindowSiPM, *solidDetector;
    G4Tubs *solidScintillator, *solidCoating, *solidPCB, *solidEndcap, *solidLightGuide, *solidHoleUP, *solidHoleDOWN, *solidHoleRIGHT, *solidHoleLEFT;
    G4VSolid *realsolidLightGuide;

    // Volumi logici
    G4LogicalVolume *logicWorld, *logicPackageSiPM, *logicWindowSiPM, *logicDetector, *logicScintillator, *logicCoating, *logicPCB, *logicEndcap, *logicLightGuide;
    
    // Scoring volume
    G4LogicalVolume *fScoringVolume;

    // Volumi fisici
    G4VPhysicalVolume *physWorld, *physScintillator, *physFrontPackageSiPM, *physBackPackageSiPM, *physWindowSiPM, *physDetector, *physCoating, *physFrontPCB, *physBackPCB, *physFrontEndcap, *physBackEndcap, *physFrontLightGuide, *physBackLightGuide;

    // MultiUnion
    G4MultiUnion *solidHoles;

    // Vis Attributes 
    G4VisAttributes  *visScintillator, *visLightGuide, *visCoating, *visPackage, *visWindow, *visDetector, *visPCB, *visEndcap;

    // Materiali
    G4Material *fLYSO, *fAir, *fVacuum, *fAluminium, *fSilicon, *fMylar, *fEpoxy, *fFR4, *fCarbonFiber, *fPlexiglass, *fSapphire;

    // Superfici
    G4OpticalSurface *tapeSurface;
    G4LogicalSkinSurface *tapeSkin;

    // Generic Messenger and settable variables 
    G4GenericMessenger *fMessenger;
    G4bool fIsLightGuide, fIsPCB, fIsEndcap;
    G4int nLightGuideMat;
};


#endif
