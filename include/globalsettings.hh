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
    constexpr G4double zScintillator = 20*cm; /**< @brief z-position of the scintillator crystal.*/

    constexpr G4double zFrontFaceScintillator = zScintillator - halfheightScintillator; /**< @brief z-position of the front face of the scintillator crystal.*/
    constexpr G4double zBackFaceScintillator = zScintillator + halfheightScintillator; /**< @brief z-position of the back face of the scintillator crystal.*/


    // Light Guide
    constexpr G4double radiusLightGuide = 42.5*mm; /**< @brief Radius of the light guide.*/
    constexpr G4double halfheightLightGuide = 1*mm; /**< @brief Half the height of the light guide.*/
    constexpr G4double radiusHole = 0.4*mm; /**< @brief Radius of the holes drilled in the light guide.*/
    constexpr G4double depthHole = 0.5*cm; /**< @brief Depth of the holes drilled in the light guide.*/

    
    // LED
    constexpr G4double depthLED = 2.5*mm; /**< @brief Depth of the LED inside the hole.*/
    constexpr G4double energyLED = 2.77*eV; /**< @brief Energy of the optical photons emitted by the LED.*/


    // SiPM (will be improved more and more)
        // Package in FR4: it's the window's mother volume
    constexpr G4int nOfSiPMs = 115; /**< @brief The number of SiPMs on a detector face.*/

    constexpr G4double halfXsidePackageSiPM = 3.675*mm; /**< @brief Half the x-side length of the SiPM package.*/
    constexpr G4double halfYsidePackageSiPM = 3.425*mm; /**< @brief Half the y-side length of the SiPM package.*/
    constexpr G4double halfZsidePackageSiPM = 0.725*mm; /**< @brief Half the z-side length of the SiPM package.*/
        // Window in epoxy resin only: it's the detector's mother volume
    constexpr G4double halfXsideWindowSiPM = 3.425*mm; /**< @brief Half the x-side length of the SiPM window.*/
    constexpr G4double halfYsideWindowSiPM = 3.425*mm; /**< @brief Half the y-side length of the SiPM window.*/
    constexpr G4double halfZsideWindowSiPM = 0.15*mm; /**< @brief Half the z-side length of the SiPM window.*/

    constexpr G4double xWindowSiPM = halfXsidePackageSiPM-0.5*mm-halfXsideWindowSiPM; /**< @brief x-position of the SiPM window referring to package center.*/
    constexpr G4double yWindowSiPM = 0*mm; /**< @brief y-position of the SiPM window referring to package center. */
    constexpr G4double zWindowSiPM = halfZsidePackageSiPM-halfZsideWindowSiPM; /**< @brief z-position of the SiPM window referring to package center. */
        // Detector in Si: note that now it is inside the window
    constexpr G4double halfXsideDetector = 3*mm; /**< @brief Half the x-side length of the SiPM silicon layer.*/
    constexpr G4double halfYsideDetector = 3*mm; /**< @brief Half the y-side length of the SiPM silicon layer.*/
    constexpr G4double halfZsideDetector = 25*um; /**< @brief Half the z-side length of the SiPM silicon layer.*/
  
    constexpr G4double xDetector = 0*mm; /**< @brief x-position of the SiPM silicon layer referring to window center.*/
    constexpr G4double yDetector = 0*mm; /**< @brief y-position of the SiPM silicon layer referring to window center.*/
    constexpr G4double zDetector = halfZsideDetector - halfZsideWindowSiPM; /**< @brief z-position of the SiPM silicon layer referring to window center.*/

    constexpr G4double PDE_SiPM = 24*perCent; /**< @brief Photon detection efficiency at 420 nm for the SiPM.*/


    // PCB
    constexpr G4double radiusPCB = 5.25*cm; /**< @brief Radius of the Printed Circuit Board (PCB). */
    constexpr G4double halfheightPCB = 0.75*mm; /**< @brief Half the height of the PCB. */


    // Endcap
    constexpr G4double radiusEndcap = 5.8*cm; /**< @brief Radius of the endcap. */
    constexpr G4double halfheightEndcap = 0.25*mm; /**< @brief Half the height of the endcap. */


    // Cilindrical Coating
    /** @brief Space between the crystal and the coating around it.
     *
     * If = 0 the absorber coating touches the crystal, like a tape;
     * if > 0 it is only used as optical photons killer in the visualization.
     */
    constexpr G4double coating_space = 0.12*mm;
    
    constexpr G4double coating_thickness = 0.2*mm; /**< @brief Thickness of the cylindrical coating.*/


    // World Dimensions 
    constexpr G4double halfXsideWorld = 0.5*m; /**< @brief Half the x-side length of the world volume.*/
    constexpr G4double halfYsideWorld = 0.5*m; /**< @brief Half the y-side length of the world volume.*/
    constexpr G4double halfZsideWorld = 0.5*m; /**< @brief Half the z-side length of the world volume.*/
}

#endif  // GLOBALSETTINGS_HH
