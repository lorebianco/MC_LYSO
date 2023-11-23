#ifndef GLOBALSETTINGS_HH
#define GLOBALSETTINGS_HH

#include "G4SystemOfUnits.hh"


namespace GS
{
    //*****     Run settings     *****//


    //*****     Construction settings     *****//
    //Scintillator
    const G4double radiusScintillator = 42.5*mm;
    const G4double halfheightScintillator = 5.*cm;

    const G4double xScintillator = 0*cm;
    const G4double yScintillator = 0*cm;
    const G4double zScintillator = 20*cm;

    const G4double zFrontFaceScintillator = zScintillator - halfheightScintillator;
    const G4double zBackFaceScintillator = zScintillator + halfheightScintillator;


    //Light Guide
    const G4double radiusLightGuide = 42.5*mm;
    const G4double halfheightLightGuide = 1*mm;
    const G4double radiusHole = 0.4*mm;
    const G4double depthHole = 0.5*cm;

    
    //LED
    const G4double depthLED = 2.5*mm; 
    const G4double energyLED = 2.77*eV;


    //SiPM (will be improved more and more)
        //Package in FR4: it's the window's mother volume
    const G4double halfXsidePackageSiPM = 3.675*mm;
    const G4double halfYsidePackageSiPM = 3.425*mm;
    const G4double halfZsidePackageSiPM = 0.725*mm;
        //Window in epoxy resin only: it's the detector's mother volume
    const G4double halfXsideWindowSiPM = 3.425*mm;
    const G4double halfYsideWindowSiPM = 3.425*mm;
    const G4double halfZsideWindowSiPM = 0.15*mm;

    const G4double xWindowSiPM = halfXsidePackageSiPM-0.5*mm-halfXsideWindowSiPM;
    const G4double yWindowSiPM = 0*mm;
    const G4double zWindowSiPM = halfZsidePackageSiPM-halfZsideWindowSiPM;
        //detector in Si: note that now it is inside the window
    const G4double halfXsideDetector = 3*mm;
    const G4double halfYsideDetector = 3*mm;
    const G4double halfZsideDetector = 25*um;
  
    const G4double xDetector = 0*mm;
    const G4double yDetector = 0*mm;
    const G4double zDetector = halfZsideDetector-halfZsideWindowSiPM;

    const G4double PDE_SiPM = 24*perCent; //PDE at 420 nm, more specific spectrum in PDE.txt


    //PCB
    const G4double radiusPCB = 5.25*cm;
    const G4double halfheightPCB = 0.75*mm;


    //Endcap
    const G4double radiusEndcap = 5.8*cm;
    const G4double halfheightEndcap = 0.25*mm;


    //Cilindrical Coating
    const G4double coating_space = 0.12*mm; //if = 0 the absorber coating of 2 micron touches the crystal, like a tape; if > 0 it is only used as optical photons killer in the visualization
    const G4double coating_thickness = 0.2*mm;


    //World Dimensions 
    const G4double halfXsideWorld = 0.5*m;
    const G4double halfYsideWorld = 0.5*m;
    const G4double halfZsideWorld = 0.5*m;

}


#endif
