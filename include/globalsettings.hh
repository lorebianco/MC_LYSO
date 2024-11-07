/**
 * @file globalsettings.hh
 * @brief Definition of the namespace @ref GS
 */
#ifndef GLOBALSETTINGS_HH
#define GLOBALSETTINGS_HH

#include "G4SystemOfUnits.hh"

/**
 * @brief Encapsulates global constant values for the simulation.
 */
namespace GS
{
    // Scintillator
    constexpr G4double radiusScintillator = 42.5*mm; /**< @brief Radius of the scintillator crystal.*/
    constexpr G4double halfheightScintillator = 5.*cm; /**< @brief Half the height of the scintillator crystal.*/

    constexpr G4double xScintillator = 0*cm; /**< @brief x-position of the scintillator crystal.*/
    constexpr G4double yScintillator = 0*cm; /**< @brief y-position of the scintillator crystal.*/
    constexpr G4double zScintillator = 65*cm; /**< @brief z-position of the scintillator crystal.*/

    constexpr G4double zFrontFaceScintillator = zScintillator - halfheightScintillator; /**< @brief z-position of the front face of the scintillator crystal.*/
    constexpr G4double zBackFaceScintillator = zScintillator + halfheightScintillator; /**< @brief z-position of the back face of the scintillator crystal.*/


    // Grease
    constexpr G4double radiusGrease = radiusScintillator; /**< @brief Radius of the optical grease layer.*/
    constexpr G4double halfheightGrease = 0.05*mm; /**< @brief Half the height of the optical grease layer.*/



    // Light Guide
    constexpr G4double radiusLightGuide = radiusScintillator; /**< @brief Radius of the light guide.*/
    constexpr G4double halfheightLightGuide = 1*mm; /**< @brief Half the height of the light guide.*/
    constexpr G4double radiusHole = 0.4*mm; /**< @brief Radius of the holes drilled in the light guide.*/
    constexpr G4double depthHole = 0.5*cm; /**< @brief Depth of the holes drilled in the light guide.*/

    
    // LED
    constexpr G4double depthLED = 2.5*mm; /**< @brief Depth of the LED inside the hole.*/
    constexpr G4double energyLED = 2.77*eV; /**< @brief Energy of the optical photons emitted by the LED.*/


    // SiPM
        // Package in FR4: it's the window's mother volume
    constexpr G4int nOfSiPMs = 115; /**< @brief The number of SiPMs on a detector face.*/
    constexpr G4int nRowsSiPMs = 13;
    constexpr G4int nColsSiPMs = 11;
    constexpr G4bool panelSiPMs[nRowsSiPMs][nColsSiPMs] = 
    {
        {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},        
    };

    constexpr G4double halfXsidePackageSiPM = 3.675*mm; /**< @brief Half the x-side length of the SiPM package.*/
    constexpr G4double halfYsidePackageSiPM = 3.425*mm; /**< @brief Half the y-side length of the SiPM package.*/
    constexpr G4double halfZsidePackageSiPM = 0.725*mm; /**< @brief Half the z-side length of the SiPM package.*/
        // Window in epoxy resin only: it's the detector's mother volume
    constexpr G4double halfXsideWindowSiPM = 3.425*mm; /**< @brief Half the x-side length of the SiPM window.*/
    constexpr G4double halfYsideWindowSiPM = 3.425*mm; /**< @brief Half the y-side length of the SiPM window.*/
    constexpr G4double halfZsideWindowSiPM = 0.300*mm; /**< @brief Half the z-side length of the SiPM window.*/

    constexpr G4double xWindowSiPM = halfXsidePackageSiPM-0.5*mm-halfXsideWindowSiPM; /**< @brief x-position of the SiPM window referring to package center.*/
    constexpr G4double yWindowSiPM = 0*mm; /**< @brief y-position of the SiPM window referring to package center. */
    constexpr G4double zWindowSiPM = halfZsidePackageSiPM-halfZsideWindowSiPM; /**< @brief z-position of the SiPM window referring to package center. */
        // Detector in Si: note that now it is inside the window
    constexpr G4double halfXsideDetector = 3*mm; /**< @brief Half the x-side length of the SiPM silicon layer.*/
    constexpr G4double halfYsideDetector = 3*mm; /**< @brief Half the y-side length of the SiPM silicon layer.*/
    constexpr G4double halfZsideDetector = 100*um; /**< @brief Half the z-side length of the SiPM silicon layer.*/
  
    constexpr G4double xDetector = 0*mm; /**< @brief x-position of the SiPM silicon layer referring to window center.*/
    constexpr G4double yDetector = 0*mm; /**< @brief y-position of the SiPM silicon layer referring to window center.*/
    constexpr G4double zDetector = halfZsideDetector - halfZsideWindowSiPM; /**< @brief z-position of the SiPM silicon layer referring to window center.*/

    constexpr G4double meanPDE = 23.6*perCent;
    
    const std::vector<G4double> pdeEnergies =
    {
        2.09*eV, 2.12*eV, 2.15*eV, 2.16*eV, 2.19*eV,
        2.20*eV, 2.25*eV, 2.28*eV, 2.31*eV, 2.34*eV,
        2.37*eV, 2.39*eV, 2.42*eV, 2.45*eV, 2.49*eV,
        2.53*eV, 2.58*eV, 2.63*eV, 2.70*eV, 2.74*eV,
        2.81*eV, 2.87*eV, 2.94*eV, 3.01*eV, 3.08*eV,
        3.10*eV, 3.13*eV, 3.18*eV, 3.21*eV, 3.25*eV,
        3.29*eV, 3.30*eV, 3.32*eV, 3.34*eV, 3.38*eV,
        3.41*eV, 3.44*eV, 3.50*eV, 3.52*eV, 3.56*eV,
        3.58*eV, 3.60*eV, 3.61*eV, 3.64*eV, 3.65*eV,
        3.67*eV, 3.69*eV, 3.71*eV, 3.73*eV, 3.76*eV,
        3.78*eV, 3.80*eV, 3.81*eV, 3.86*eV
    };

    const std::vector<G4double> pdeValues =
    {
        17.3*perCent, 18.0*perCent, 18.5*perCent, 18.9*perCent, 19.5*perCent,
        19.9*perCent, 20.8*perCent, 21.3*perCent, 21.9*perCent, 22.4*perCent,
        22.9*perCent, 23.4*perCent, 23.7*perCent, 24.2*perCent, 24.5*perCent,
        24.9*perCent, 25.2*perCent, 25.4*perCent, 25.4*perCent, 25.4*perCent,
        25.2*perCent, 24.8*perCent, 24.3*perCent, 23.7*perCent, 23.3*perCent,
        22.8*perCent, 22.4*perCent, 21.7*perCent, 21.0*perCent, 20.3*perCent,
        19.5*perCent, 18.9*perCent, 18.4*perCent, 17.9*perCent, 17.3*perCent,
        16.5*perCent, 15.8*perCent, 14.9*perCent, 13.9*perCent, 13.0*perCent,
        12.2*perCent, 11.3*perCent, 10.6*perCent, 9.95*perCent, 9.43*perCent,
        8.73*perCent, 7.71*perCent, 6.81*perCent, 6.09*perCent, 5.18*perCent,
        4.47*perCent, 3.81*perCent, 3.03*perCent, 2.19*perCent
    };


    // PCB
    constexpr G4double radiusPCB = radiusScintillator + 10.*mm; /**< @brief Radius of the Printed Circuit Board (PCB). */
    constexpr G4double halfheightPCB = 0.75*mm; /**< @brief Half the height of the PCB. */


    // Endcap
    constexpr G4double radiusEndcap = radiusScintillator + 15.5*mm; /**< @brief Radius of the endcap. */
    constexpr G4double halfheightEndcap = 0.25*mm; /**< @brief Half the height of the endcap. */


    // Cilindrical Coating
    /** @brief Space between the crystal and the coating around it.
     *
     * If = 0 the absorber coating touches the crystal, like a tape;
     * if > 0 it is only used as optical photons killer in the visualization.
     */
    constexpr G4double coating_space = 0.12*mm;
    
    constexpr G4double coating_thickness = 0.2*mm; /**< @brief Thickness of the cylindrical coating.*/


    // Cosmic rays detector
    constexpr G4double halfZXsideCosmicRayDetector = 1.5*cm;
    constexpr G4double halfYsideCosmicRayDetector = 2.5*mm;

    constexpr G4double xCosmicRayDetector = xScintillator;
    constexpr G4double yCosmicRayDetector = yScintillator + radiusScintillator + 1.*cm;
    constexpr G4double zCosmicRayDetector = zScintillator;


    // World Dimensions 
    constexpr G4double halfXsideWorld = 0.5*m; /**< @brief Half the x-side length of the world volume.*/
    constexpr G4double halfYsideWorld = 0.5*m; /**< @brief Half the y-side length of the world volume.*/
    constexpr G4double halfZsideWorld = 1*m; /**< @brief Half the z-side length of the world volume.*/
}

#endif  // GLOBALSETTINGS_HH
