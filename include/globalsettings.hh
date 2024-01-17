#ifndef GLOBALSETTINGS_HH
#define GLOBALSETTINGS_HH

#include "G4SystemOfUnits.hh"


namespace GS
{
    //Scintillator
    constexpr G4double radiusScintillator = 42.5*mm;
    constexpr G4double halfheightScintillator = 5.*cm;

    constexpr G4double xScintillator = 0*cm;
    constexpr G4double yScintillator = 0*cm;
    constexpr G4double zScintillator = 20*cm;

    constexpr G4double zFrontFaceScintillator = zScintillator - halfheightScintillator;
    constexpr G4double zBackFaceScintillator = zScintillator + halfheightScintillator;


    //Light Guide
    constexpr G4double radiusLightGuide = 42.5*mm;
    constexpr G4double halfheightLightGuide = 1*mm;
    constexpr G4double radiusHole = 0.4*mm;
    constexpr G4double depthHole = 0.5*cm;

    
    //LED
    constexpr G4double depthLED = 2.5*mm; 
    constexpr G4double energyLED = 2.77*eV;


    //SiPM (will be improved more and more)
        //Package in FR4: it's the window's mother volume
    constexpr G4double halfXsidePackageSiPM = 3.675*mm;
    constexpr G4double halfYsidePackageSiPM = 3.425*mm;
    constexpr G4double halfZsidePackageSiPM = 0.725*mm;
        //Window in epoxy resin only: it's the detector's mother volume
    constexpr G4double halfXsideWindowSiPM = 3.425*mm;
    constexpr G4double halfYsideWindowSiPM = 3.425*mm;
    constexpr G4double halfZsideWindowSiPM = 0.15*mm;

    constexpr G4double xWindowSiPM = halfXsidePackageSiPM-0.5*mm-halfXsideWindowSiPM;
    constexpr G4double yWindowSiPM = 0*mm;
    constexpr G4double zWindowSiPM = halfZsidePackageSiPM-halfZsideWindowSiPM;
        //detector in Si: note that now it is inside the window
    constexpr G4double halfXsideDetector = 3*mm;
    constexpr G4double halfYsideDetector = 3*mm;
    constexpr G4double halfZsideDetector = 25*um;
  
    constexpr G4double xDetector = 0*mm;
    constexpr G4double yDetector = 0*mm;
    constexpr G4double zDetector = halfZsideDetector-halfZsideWindowSiPM;

    constexpr G4double PDE_SiPM = 24*perCent; //PDE at 420 nm, more specific spectrum in PDE.txt


    //PCB
    constexpr G4double radiusPCB = 5.25*cm;
    constexpr G4double halfheightPCB = 0.75*mm;


    //Endcap
    constexpr G4double radiusEndcap = 5.8*cm;
    constexpr G4double halfheightEndcap = 0.25*mm;


    //Cilindrical Coating
    constexpr G4double coating_space = 0.12*mm; //if = 0 the absorber coating of 2 micron touches the crystal, like a tape; if > 0 it is only used as optical photons killer in the visualization
    constexpr G4double coating_thickness = 0.2*mm;


    //World Dimensions 
    constexpr G4double halfXsideWorld = 0.5*m;
    constexpr G4double halfYsideWorld = 0.5*m;
    constexpr G4double halfZsideWorld = 0.5*m;
}

#endif  // GLOBALSETTINGS_HH
