#ifndef GLOBALSETTINGS_HH
#define GLOBALSETTINGS_HH

#include "G4SystemOfUnits.hh"

class MyGlobalSettings
{
public:
    //*****     Run settings     *****//
    
    

    //*****     Construction settings     *****//
    //Scintillator
    const G4double radiusScintillator = 3.5*cm;
    const G4double halfheightScintillator = 5.*cm;

    const G4double xScintillator = 0*cm;
    const G4double yScintillator = 0*cm;
    const G4double zScintillator = 20*cm;

    const G4double zFrontFaceScintillator = zScintillator - halfheightScintillator;
    const G4double zBackFaceScintillator = zScintillator + halfheightScintillator;


    //Light Guide
    const G4double radiusLightGuide = 3.5*cm;
    const G4double halfheightLightGuide = 0.5*cm;
    const G4double radiusHole = 0.4*mm;
    const G4double depthHole = 0.5*cm;


    //SiPM (andrà modificato sempre di più)
    const G4double halfXsideDetector = 3*mm;
    const G4double halfYsideDetector = 3*mm;
    const G4double halfZsideDetector = 0.05*mm;


    //PCB
    const G4double radiusPCB = 4.*cm;
    const G4double halfheightPCB = 0.75*mm;


    //Endcap
    const G4double radiusEndcap = 5*cm;
    const G4double halfheightEndcap = 0.25*mm;


    //Cilindrical Coating
    const G4double coating_space = 0.12*mm; //se = 0 il coating assorbitore da 2 micron è a contatto con il cristallo, come un tape; se > 0 serve solo nella grafica per uccidere i fotoni ottici
    const G4double coating_thickness = 0.2*mm;


    //World Dimensions 
    const G4double halfXsideWorld = 0.5*m;
    const G4double halfYsideWorld = 0.5*m;
    const G4double halfZsideWorld = 0.5*m;
};


#endif