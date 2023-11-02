#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    G4LogicalVolume *GetScoringVolume() const { return fScoringVolume;}

    virtual G4VPhysicalVolume *Construct();

private:
    virtual void ConstructSDandField();
    void DefineMaterialsAndSurfaces();

    //Solidi
    G4Box *solidWorld, *solidBackDetector, *solidFrontDetector;
    G4Tubs *solidScintillator, *solidCoating, *solidPCB, *solidEndcap;
    //Volumi logici
    G4LogicalVolume *logicWorld, *logicScintillator, *logicBackDetector, *logicFrontDetector, *logicCoating, *logicPCB, *logicEndcap;
    //Volumi fisici
    G4VPhysicalVolume *physWorld, *physScintillator, *physBackDetector, *physFrontDetector, *physCoating, *physFrontPCB, *physBackPCB, *physFrontEndcap, *physBackEndcap;

    //G4VisAttributes 


    //Materiali
    G4Material *fLYSO, *fAir, *fVacuum, *fAluminium, *detectorMat, *fEpoxy, *fFR4, *fCarbonFiber;

    //Superfici
    G4OpticalSurface *tapeSurface;
    G4LogicalSkinSurface *skin;

    //Scoring volume
    G4LogicalVolume *fScoringVolume;

};


#endif