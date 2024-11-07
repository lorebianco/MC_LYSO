/**
 * @file construction.cc
 * @brief Definition of the class @ref MyDetectorConstruction
 */
#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    DefineCommands();
    
    // Default settings
    fIsGrease = true;
    fIsOpticalGreaseSurface = false;
    fIsLightGuide = false;
    nLightGuideMat = 1;    
    fIsPCB = true;
    fIsEndcap = true;
    fIsASiPM = false;
    fIsCosmicRaysDetectors = true;

    DefineMaterials();
}



void MyDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();
    
    // LYSO
    fLYSO = new G4Material("LYSO", 7.25*g/cm3, 5, kStateSolid);
    fLYSO->AddElement(nist->FindOrBuildElement("Lu"), 73.8579*perCent);
    fLYSO->AddElement(nist->FindOrBuildElement("Y"), 1.9747*perCent);
    fLYSO->AddElement(nist->FindOrBuildElement("Si"), 6.2418*perCent);
    fLYSO->AddElement(nist->FindOrBuildElement("O"), 17.77*perCent);
    fLYSO->AddElement(nist->FindOrBuildElement("Ce"), 0.1556*perCent);

    G4MaterialPropertiesTable *mptLYSO = new G4MaterialPropertiesTable();
    
    std::vector<G4double> Energies = {2.14*eV, 2.95*eV, 3.69*eV};
    const G4int nEntries = Energies.size();
    std::vector<G4double> emissionEnergies =
    {
        2.14*eV, 2.16*eV, 2.19*eV, 2.22*eV, 2.26*eV, 
        2.29*eV, 2.33*eV, 2.37*eV, 2.41*eV, 2.45*eV, 
        2.49*eV, 2.53*eV, 2.57*eV, 2.6*eV, 2.64*eV, 
        2.67*eV, 2.7*eV, 2.72*eV, 2.74*eV, 2.76*eV, 
        2.78*eV, 2.81*eV, 2.84*eV, 2.86*eV, 2.9*eV, 
        2.95*eV, 3.0*eV, 3.05*eV, 3.07*eV, 3.09*eV, 
        3.12*eV, 3.14*eV, 3.15*eV, 3.18*eV, 3.2*eV, 
        3.21*eV, 3.22*eV, 3.24*eV, 3.25*eV, 3.28*eV, 
        3.3*eV, 3.32*eV, 3.37*eV, 3.43*eV, 3.52*eV, 
        3.6*eV, 3.69*eV
    };
    
    std::vector<G4double> LYSO_RINDEX(nEntries, 1.82);
    std::vector<G4double> LYSO_ABSLENGTH(nEntries, 50.*cm);
    std::vector<G4double> LYSO_SCINTILLATIONCOMPONENT1 =
    {
        0.0, 0.0231, 0.153, 0.867, 
        1.44, 2.13, 2.99, 4.07, 
        5.17, 7.27, 10.3, 14.3, 
        19.0, 23.2, 28.0, 32.6, 
        36.9, 41.6, 46.4, 51.0, 
        55.5, 60.1, 65.0, 69.5, 
        73.8, 77.7, 78.1, 73.2, 
        68.1, 63.4, 58.9, 54.3, 
        49.6, 45.2, 40.5, 35.8, 
        31.3, 26.5, 21.5, 17.0, 
        12.2, 7.48, 3.39, 0.726, 
        0.705, 0.314, 0.0
    };

    mptLYSO->AddProperty("SCINTILLATIONCOMPONENT1", emissionEnergies, LYSO_SCINTILLATIONCOMPONENT1);
    mptLYSO->AddProperty("RINDEX", Energies, LYSO_RINDEX);
    mptLYSO->AddProperty("ABSLENGTH", Energies, LYSO_ABSLENGTH);
    mptLYSO->AddConstProperty("SCINTILLATIONYIELD", 29000./MeV);
    mptLYSO->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptLYSO->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 42.*ns);
    mptLYSO->AddConstProperty("SCINTILLATIONYIELD1", 1.);  
    
    fLYSO->SetMaterialPropertiesTable(mptLYSO);
    
    // AIR
    fAir = nist->FindOrBuildMaterial("G4_AIR");
    
    G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
    
    std::vector<G4double> AIR_RINDEX(nEntries, 1.000293);
    mptAir->AddProperty("RINDEX", Energies, AIR_RINDEX);

    fAir->SetMaterialPropertiesTable(mptAir);

    // VACUUM
    fVacuum = nist->FindOrBuildMaterial("G4_Galactic");

    G4MaterialPropertiesTable *mptVacuum = new G4MaterialPropertiesTable();
    
    std::vector<G4double> VACUUM_RINDEX(nEntries, 1.);
    mptVacuum->AddProperty("RINDEX", Energies, VACUUM_RINDEX);

    fVacuum->SetMaterialPropertiesTable(mptVacuum);

    // SILICON
    fSilicon = nist->FindOrBuildMaterial("G4_Si");
    G4MaterialPropertiesTable *mptDetector = new G4MaterialPropertiesTable();
    std::vector<G4double> DETECTOR_RINDEX(nEntries, 1.55); //should be 3.88, but it is included in the PDE, so it's fixed at the same value of the window (Total transmission)
    mptDetector->AddProperty("RINDEX", Energies, DETECTOR_RINDEX);

    fSilicon->SetMaterialPropertiesTable(mptDetector);

    // EPOXY
    fEpoxy = new G4Material("Epoxy", 1.2*g/cm3, 2);
    fEpoxy->AddElement(nist->FindOrBuildElement("C"), 2);
    fEpoxy->AddElement(nist->FindOrBuildElement("H"), 2);
    G4MaterialPropertiesTable *mptEpoxy = new G4MaterialPropertiesTable();
    std::vector<G4double> EPOXY_RINDEX(nEntries, 1.55);
    mptEpoxy->AddProperty("RINDEX", Energies, EPOXY_RINDEX);

    fEpoxy->SetMaterialPropertiesTable(mptEpoxy);

    // FR4
    fFR4 = new G4Material("FR4", 1.85*g/cm3, 2);
    fFR4->AddMaterial(nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"), 52.8*perCent);
    fFR4->AddMaterial(fEpoxy, 47.2*perCent);

    // CARBON FIBER
    fCarbonFiber = new G4Material("Carbon Fiber", 1.5*g/cm3, 1);
    fCarbonFiber->AddElement(nist->FindOrBuildElement("C"), 1);

    // PLEXIGLASS
    fPlexiglass = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
    
    G4MaterialPropertiesTable *mptPlexiglass = new G4MaterialPropertiesTable();
    
    std::vector<G4double> PLEXIGLASS_RINDEX(nEntries, 1.49);
    mptPlexiglass->AddProperty("RINDEX", Energies, PLEXIGLASS_RINDEX);
    
    fPlexiglass->SetMaterialPropertiesTable(mptPlexiglass);
    
    // SAPPHIRE
    fSapphire = new G4Material("Sapphire", 3.98*g/cm3, 2, kStateSolid);
    fSapphire->AddElement(nist->FindOrBuildElement("Al"), 2);
    fSapphire->AddElement(nist->FindOrBuildElement("O"), 3);

    G4MaterialPropertiesTable *mptSapphire = new G4MaterialPropertiesTable();

    std::vector<G4double> SAPPHIRE_RINDEX(nEntries, 1.77);
    mptSapphire->AddProperty("RINDEX", Energies, SAPPHIRE_RINDEX);

    fSapphire->SetMaterialPropertiesTable(mptSapphire);

    // OPTICAL GREASE
    fGrease = new G4Material("OpticalGrease", 1.06*g/cm3, 2);
    fGrease->AddElement(nist->FindOrBuildElement("C"), 2);
    fGrease->AddElement(nist->FindOrBuildElement("H"), 6);

    G4MaterialPropertiesTable *mptGrease = new G4MaterialPropertiesTable();

    std::vector<G4double> GREASE_RINDEX(nEntries, 1.46);
    mptGrease->AddProperty("RINDEX", Energies, GREASE_RINDEX);
    
    fGrease->SetMaterialPropertiesTable(mptGrease);

    // Optical Grease surface
    if(fIsOpticalGreaseSurface)
    {
        fOpGreaseSurface = new G4OpticalSurface("GreaseSurface");
        fOpGreaseSurface->SetType(dielectric_dielectric);
        fOpGreaseSurface->SetModel(unified);
        fOpGreaseSurface->SetFinish(ground);
        fOpGreaseSurface->SetSigmaAlpha(0.1); // Dummy value indicating the roughness, will be fixed according to experimental data (I hope...)
    }
}



G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // Construct the World
    G4Box *solidWorld = new G4Box("solidWorld", GS::halfXsideWorld, GS::halfYsideWorld, GS::halfZsideWorld);
    logicWorld = new G4LogicalVolume(solidWorld, fAir, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    // Option to draw only a SiPM, for debug
    if(fIsASiPM)
    {
        ConstructASiPM();
        return physWorld;
    }

    // Construct the scintillator crystal
    G4Tubs *solidScintillator = new G4Tubs("solidScintillator", 0.*cm, GS::radiusScintillator, GS::halfheightScintillator, 0.*deg, 360.*deg);
    logicScintillator = new G4LogicalVolume(solidScintillator, fLYSO, "logicScintillator");
    G4VPhysicalVolume *physScintillator = new G4PVPlacement(0, G4ThreeVector(GS::xScintillator, GS::yScintillator, GS::zScintillator), logicScintillator, "physScintillator", logicWorld, false, 0, true);

    // Assign the logic scoring volume to the crystal
    fScoringVolume = logicScintillator;
    
    // Construct a coating for the crystal and the lightguide (if present)
    G4Tubs *solidCoating = new G4Tubs("solidCoating", (GS::radiusScintillator + GS::coating_space), (GS::radiusScintillator + GS::coating_space + GS::coating_thickness), GS::halfheightScintillator+2*(GS::halfheightLightGuide*fIsLightGuide), 0.*deg, 360.*deg);
    logicCoating = new G4LogicalVolume(solidCoating, fCarbonFiber, "logicCoating");
    G4VPhysicalVolume *physCoating = new G4PVPlacement(0, G4ThreeVector(GS::xScintillator, GS::yScintillator, GS::zScintillator), logicCoating, "physCoating", logicWorld, false, 0, true);
    
    // Construct lightguides, PCBs and endcaps -if setted-
    if(fIsGrease) ConstructGrease();
    if(fIsLightGuide) ConstructLightGuide();
    if(fIsPCB) ConstructPCB();
    if(fIsEndcap) ConstructEndcap();

    // Construct the package of the SIPM
    G4Box *solidPackageSiPM = new G4Box("solidPackageSiPM", GS::halfXsidePackageSiPM, GS::halfYsidePackageSiPM, GS::halfZsidePackageSiPM);
    logicPackageSiPM = new G4LogicalVolume(solidPackageSiPM, fFR4, "logicPackageSiPM");

    // Construct the window of the SIPM and put it inside the package
    G4Box *solidWindowSiPM = new G4Box("solidWindowSiPM", GS::halfXsideWindowSiPM, GS::halfYsideWindowSiPM, GS::halfZsideWindowSiPM);
    logicWindowSiPM = new G4LogicalVolume(solidWindowSiPM, fEpoxy, "logicWindowSiPM");
    G4VPhysicalVolume *physWindowSiPM = new G4PVPlacement(0, G4ThreeVector(GS::xWindowSiPM, GS::yWindowSiPM, GS::zWindowSiPM), logicWindowSiPM, "physWindowSiPM", logicPackageSiPM, false, 0, true);

    // Construct the silicon layer and put it inside the window
    G4Box *solidDetector = new G4Box("solidDetector", GS::halfXsideDetector, GS::halfYsideDetector, GS::halfZsideDetector);
    logicDetector = new G4LogicalVolume(solidDetector, fSilicon, "logicDetector");
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(GS::xDetector, GS::yDetector, GS::zDetector), logicDetector, "physDetector", logicWindowSiPM, false, 0, true);

    // Define the arrangement of SiPMs
    G4VPhysicalVolume *physFrontPackageSiPM[GS::nOfSiPMs];
    G4VPhysicalVolume *physBackPackageSiPM[GS::nOfSiPMs];

    G4int indexDetector = 0;
    for(G4int i = 0; i < GS::nRowsSiPMs; i++)
    {
        for(G4int j = 0; j < GS::nColsSiPMs; j++)
        {
            if(GS::panelSiPMs[i][j])
            {
                PositionSiPMs(physFrontPackageSiPM[indexDetector], physBackPackageSiPM[indexDetector], i, j, indexDetector);
                indexDetector++;
            }
        }
    }

    // Assign the logic trigger volume to the Si layer
    fDecayTriggerVolume = logicDetector;

    // Construct dummy cosmic rays detectors
    if(fIsCosmicRaysDetectors)
        ConstructCosmicRaysDetectors();

    // Add visualization attributes
    DefineVisAttributes();

    // Always return physWorld
    return physWorld;
}



void MyDetectorConstruction::ConstructSDandField()
{
    // Set logic silicon layer as sensitive volume. Use G4SDManager to do it
    MySensitiveDetector *sensDet = static_cast<MySensitiveDetector*>(G4SDManager::GetSDMpointer()->FindSensitiveDetector("SensitiveDetector"));
    
    if(!sensDet)
    {
        sensDet = new MySensitiveDetector("SensitiveDetector", "HitsCollection");
        G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
    }

    SetSensitiveDetector(logicDetector, sensDet);
}



void MyDetectorConstruction::ConstructGrease()
{
    // Grease as cilinder
    G4Tubs *solidGrease = new G4Tubs("solidGrease", 0, GS::radiusScintillator, GS::halfheightGrease, 0.*deg, 360.*deg);
    logicGrease = new G4LogicalVolume(solidGrease, fGrease, "logicGrease");
    G4VPhysicalVolume *physFrontGrease = new G4PVPlacement(0, G4ThreeVector(0, 0, GS::zFrontFaceScintillator-GS::halfheightGrease), logicGrease, "physFrontGrease", logicWorld, false, 0, true);
    G4VPhysicalVolume *physBackGrease = new G4PVPlacement(0, G4ThreeVector(0, 0, GS::zBackFaceScintillator+GS::halfheightGrease), logicGrease, "physBackGrease", logicWorld, false, 1, true);
    
    if(fIsLightGuide)
    {
        G4VPhysicalVolume *physSecondFrontGrease = new G4PVPlacement(0, G4ThreeVector(0, 0, GS::zFrontFaceScintillator-2*GS::halfheightLightGuide-3*GS::halfheightGrease), logicGrease, "physSecondFrontGrease", logicWorld, false, 0, true);
        G4VPhysicalVolume *physSecondBackGrease = new G4PVPlacement(0, G4ThreeVector(0, 0, GS::zBackFaceScintillator+2*GS::halfheightLightGuide+3*GS::halfheightGrease), logicGrease, "physSecondBackGrease", logicWorld, false, 1, true);
    }

    // Grease as surface
    if(fIsOpticalGreaseSurface)
        G4LogicalSkinSurface *logicGreaseSurface = new G4LogicalSkinSurface("GreaseSurface", logicGrease, fOpGreaseSurface);
}



void MyDetectorConstruction::ConstructLightGuide()
{
    G4Material *fLightGuideMaterial = 0;

    // Solid light guide as cilinder
    G4Tubs *solidLightGuide = new G4Tubs("solidLightGuide", 0, GS::radiusLightGuide, GS::halfheightLightGuide, 0.*deg, 360.*deg);

    // Drill 4 holes in the light guide for LEDs 
    G4Tubs *solidHoleUP = new G4Tubs("solidHoleLightGuide", 0, GS::radiusHole, GS::depthHole, 0.*deg, 360*deg);
    G4Tubs *solidHoleDOWN = new G4Tubs("solidHoleLightGuide", 0, GS::radiusHole, GS::depthHole, 0.*deg, 360*deg);
    G4Tubs *solidHoleRIGHT = new G4Tubs("solidHoleLightGuide", 0, GS::radiusHole, GS::depthHole, 0.*deg, 360*deg);
    G4Tubs *solidHoleLEFT = new G4Tubs("solidHoleLightGuide", 0, GS::radiusHole, GS::depthHole, 0.*deg, 360*deg);

    G4Rotate3D rotXholeUP(90*deg, G4ThreeVector(1, 0, 0));
    G4Translate3D transYholeUP(G4ThreeVector(0, GS::radiusLightGuide, 0));
    G4Transform3D transformHoleUP = (transYholeUP)*(rotXholeUP);

    G4Rotate3D rotXholeDOWN(90*deg, G4ThreeVector(1, 0, 0));
    G4Translate3D transYholeDOWN(G4ThreeVector(0, -GS::radiusLightGuide, 0));
    G4Transform3D transformHoleDOWN = (transYholeDOWN)*(rotXholeDOWN);

    G4Rotate3D rotYholeRIGHT(90*deg, G4ThreeVector(0, 1, 0));
    G4Translate3D transYholeRIGHT(G4ThreeVector(GS::radiusLightGuide, 0, 0));
    G4Transform3D transformHoleRIGHT = (transYholeRIGHT)*(rotYholeRIGHT);

    G4Rotate3D rotYholeLEFT(90*deg, G4ThreeVector(0, 1, 0));
    G4Translate3D transYholeLEFT(G4ThreeVector(-GS::radiusLightGuide, 0, 0));
    G4Transform3D transformHoleLEFT = (transYholeLEFT)*(rotYholeLEFT);

    G4MultiUnion *solidHoles = new G4MultiUnion("solidHoles");
    solidHoles->AddNode(*solidHoleUP, transformHoleUP);
    solidHoles->AddNode(*solidHoleDOWN, transformHoleDOWN);
    solidHoles->AddNode(*solidHoleRIGHT, transformHoleRIGHT);
    solidHoles->AddNode(*solidHoleLEFT, transformHoleLEFT);
    solidHoles->Voxelize();

    G4VSolid *solidDrilledLightGuide = new G4SubtractionSolid("solidDrilledLightGuide", solidLightGuide, solidHoles);
        
    // Options for material
    switch(nLightGuideMat)
    {
        case 1:    // Plexiglass
            fLightGuideMaterial = fPlexiglass;
            break;
        case 2:    // Sapphire
            fLightGuideMaterial = fSapphire;
            break;
        default:
            G4cerr << "Option for lightguide's material not valid. \"Plexiglass\" has been setted" << G4endl;
            fLightGuideMaterial = fPlexiglass;
    }

    // Logic and phys volumes for light guides
    logicLightGuide = new G4LogicalVolume(solidDrilledLightGuide, fLightGuideMaterial, "logicLightGuide");
    G4VPhysicalVolume *physFrontLightGuide = new G4PVPlacement(0, G4ThreeVector(0., 0., GS::zFrontFaceScintillator-(2*GS::halfheightGrease*fIsGrease)-GS::halfheightLightGuide), logicLightGuide, "physFrontLightGuide", logicWorld, false, 0, true);
    G4VPhysicalVolume *physBackLightGuide = new G4PVPlacement(0, G4ThreeVector(0., 0., GS::zBackFaceScintillator+(2*GS::halfheightGrease*fIsGrease)+GS::halfheightLightGuide), logicLightGuide, "physBackLightGuide", logicWorld, false, 1, true);
}



void MyDetectorConstruction::ConstructPCB()
{
    // PCB as cilinder
    G4Tubs *solidPCB = new G4Tubs("solidPCB", 0, GS::radiusPCB, GS::halfheightPCB, 0.*deg, 360.*deg);
    logicPCB = new G4LogicalVolume(solidPCB, fFR4, "logicPCB");
    G4VPhysicalVolume *physFrontPCB = new G4PVPlacement(0, G4ThreeVector(0, 0, GS::zFrontFaceScintillator-(2*GS::halfheightGrease*fIsGrease)-2*(GS::halfheightLightGuide*fIsLightGuide)-(2*GS::halfheightGrease*fIsLightGuide*fIsGrease)-2*GS::halfZsidePackageSiPM-GS::halfheightPCB), logicPCB, "physFrontPCB", logicWorld, false, 0, true);
    G4VPhysicalVolume *physBackPCB = new G4PVPlacement(0, G4ThreeVector(0, 0, GS::zBackFaceScintillator+(2*GS::halfheightGrease*fIsGrease)+2*(GS::halfheightLightGuide*fIsLightGuide)+(2*GS::halfheightGrease*fIsLightGuide*fIsGrease)+2*GS::halfZsidePackageSiPM+GS::halfheightPCB), logicPCB, "physBackPCB", logicWorld, false, 1, true);
}



void MyDetectorConstruction::ConstructEndcap()
{   
    // Endcap as cilinder
    G4Tubs *solidEndcap = new G4Tubs("solidEndcap", 0, GS::radiusEndcap, GS::halfheightEndcap, 0.*deg, 360.*deg);
    logicEndcap = new G4LogicalVolume(solidEndcap, fCarbonFiber, "logicEndcap");
    G4VPhysicalVolume *physFrontEndcap = new G4PVPlacement(0, G4ThreeVector(0, 0, GS::zFrontFaceScintillator-(2*GS::halfheightGrease*fIsGrease)-2*(GS::halfheightLightGuide*fIsLightGuide)-(2*GS::halfheightGrease*fIsLightGuide*fIsGrease)-2*GS::halfZsidePackageSiPM-2*(GS::halfheightPCB*fIsPCB)-GS::halfheightEndcap), logicEndcap, "physFrontEndcap", logicWorld, false, 0, true);
    G4VPhysicalVolume *physBackEndcap = new G4PVPlacement(0, G4ThreeVector(0, 0, GS::zBackFaceScintillator+(2*GS::halfheightGrease*fIsGrease)+2*(GS::halfheightLightGuide*fIsLightGuide)+(2*GS::halfheightGrease*fIsLightGuide*fIsGrease)+2*GS::halfZsidePackageSiPM+2*(GS::halfheightPCB*fIsPCB)+GS::halfheightEndcap), logicEndcap, "physBackEndcap", logicWorld, false, 1, true);
}



void MyDetectorConstruction::PositionSiPMs(G4VPhysicalVolume *physFrontSiPM, G4VPhysicalVolume *physBackSiPM, G4int row, G4int col, G4int index)
{
    G4double startX = -GS::halfXsidePackageSiPM*(GS::nColsSiPMs -1);
    G4double startY = GS::halfYsidePackageSiPM*(GS::nRowsSiPMs - 1);

    // Place the packages. When in back face, need to rotate them of 180°
    physFrontSiPM = new G4PVPlacement(0, G4ThreeVector(startX + col*2*GS::halfXsidePackageSiPM, startY - row*2*GS::halfYsidePackageSiPM, GS::zFrontFaceScintillator-(2*GS::halfheightGrease*fIsGrease)-2*(GS::halfheightLightGuide*fIsLightGuide)-(2*GS::halfheightGrease*fIsLightGuide*fIsGrease)-GS::halfZsidePackageSiPM), logicPackageSiPM, "physFrontPackageSiPM", logicWorld, false, index, true);
    
    G4Rotate3D rotXBackDet(180*deg, G4ThreeVector(1, 0, 0));
    G4Translate3D transBackDet(G4ThreeVector(startX + col*2*GS::halfXsidePackageSiPM, startY - row*2*GS::halfYsidePackageSiPM, GS::zBackFaceScintillator+(2*GS::halfheightGrease*fIsGrease)+2*(GS::halfheightLightGuide*fIsLightGuide)+(2*GS::halfheightGrease*fIsLightGuide*fIsGrease)+GS::halfZsidePackageSiPM));
    G4Transform3D transformBackDet = (transBackDet)*(rotXBackDet);
                
    physBackSiPM = new G4PVPlacement(transformBackDet, logicPackageSiPM, "physBackPackageSiPM", logicWorld, false, index, true);
}



void MyDetectorConstruction::DefineVisAttributes()
{
    // World
    G4VisAttributes *visWorld = new G4VisAttributes();
    visWorld->SetVisibility(false);
    logicWorld->SetVisAttributes(visWorld);    

    // Endcaps
    if(fIsEndcap)
    {
        G4VisAttributes *visEndcap = new G4VisAttributes();
        visEndcap->SetColour(0.5, 0.5, 0.5, 0.5);
        logicEndcap->SetVisAttributes(visEndcap);
    }
    
    // PCBs
    if(fIsPCB)
    {
        G4VisAttributes *visPCB = new G4VisAttributes();
        visPCB->SetColour(0, 1, 0, 0.65);
        logicPCB->SetVisAttributes(visPCB);
    }

    // Light guides
    if(fIsLightGuide)
    {
        G4VisAttributes *visLightGuide = new G4VisAttributes();
        visLightGuide->SetColour(1, 1, 1, 0.5);
        logicLightGuide->SetVisAttributes(visLightGuide);
    }

    if(fIsGrease)
    {
        G4VisAttributes *visGrease = new G4VisAttributes();
        visGrease->SetColour(1, 0.5, 0, 0.5);
        logicGrease->SetVisAttributes(visGrease);
    }

    // SiPM packages
    G4VisAttributes *visPackage = new G4VisAttributes();
    visPackage->SetColour(1, 1, 0, 0.7);
    logicPackageSiPM->SetVisAttributes(visPackage);

    // SiPM windows
    G4VisAttributes *visWindow = new G4VisAttributes();
    visWindow->SetColour(1, 1, 1, 0.7);
    logicWindowSiPM->SetVisAttributes(visWindow);

    // SiPM silicon layers
    G4VisAttributes *visDetector = new G4VisAttributes();
    visDetector->SetColour(0.45, 0.25, 0, 0.7);
    logicDetector->SetVisAttributes(visDetector);

    // Scintillator
    G4VisAttributes *visScintillator = new G4VisAttributes();
    visScintillator->SetColour(0, 0, 1, 0.98);
    logicScintillator->SetVisAttributes(visScintillator);

    // Coating
    G4VisAttributes *visCoating = new G4VisAttributes();
    visCoating->SetVisibility(false);
    logicCoating->SetVisAttributes(visCoating);

    // Cosmic rays detectors
    if(fIsCosmicRaysDetectors)
    {
        G4VisAttributes *visCosmicDet = new G4VisAttributes();
        visCosmicDet->SetColour(0, 0.5, 0.6, 0.7);
        logicCosmicRaysDetector->SetVisAttributes(visCosmicDet);
    }
}



void MyDetectorConstruction::DefineCommands()
{
    // Define my UD-messenger for the detector construction
    fMessenger = new G4GenericMessenger(this, "/MC_LYSO/myConstruction/", "Construction settings");
    fMessenger->DeclareProperty("isOpticalGrease", fIsGrease, "Set if optical grease is present");
    fMessenger->DeclareProperty("isLightGuide", fIsLightGuide, "Set if the two light guides are present");
    fMessenger->DeclareProperty("isPCB", fIsPCB, "Set if the two PCBs are present");
    fMessenger->DeclareProperty("isEndcap", fIsEndcap, "Set if the two endcaps are present");
    fMessenger->DeclareProperty("MaterialOfLightGuide", nLightGuideMat, "Set the material of light guide: 1 = Plexiglass, 2 = Sapphire");
    fMessenger->DeclareProperty("isASiPM", fIsASiPM, "Set if construct only a SiPM");
    fMessenger->DeclareProperty("isCosmicRaysDetectors", fIsCosmicRaysDetectors, "Set if the two cosmic rays detector are present");
}



void MyDetectorConstruction::ConstructASiPM()
{
    // Construct the package of the SIPM
    G4Box *solidPackageSiPM = new G4Box("solidPackageSiPM", GS::halfXsidePackageSiPM, GS::halfYsidePackageSiPM, GS::halfZsidePackageSiPM);
    logicPackageSiPM = new G4LogicalVolume(solidPackageSiPM, fFR4, "logicPackageSiPM");
    
    // Construct the window of the SIPM and put it inside the package
    G4Box *solidWindowSiPM = new G4Box("solidWindowSiPM", GS::halfXsideWindowSiPM, GS::halfYsideWindowSiPM, GS::halfZsideWindowSiPM);
    logicWindowSiPM = new G4LogicalVolume(solidWindowSiPM, fEpoxy, "logicWindowSiPM");
    G4VPhysicalVolume *physWindowSiPM = new G4PVPlacement(0, G4ThreeVector(GS::xWindowSiPM, GS::yWindowSiPM, GS::zWindowSiPM), logicWindowSiPM, "physWindowSiPM", logicPackageSiPM, false, 0, true);

    // Construct the silicon layer and put it inside the window
    G4Box *solidDetector = new G4Box("solidDetector", GS::halfXsideDetector, GS::halfYsideDetector, GS::halfZsideDetector);
    logicDetector = new G4LogicalVolume(solidDetector, fSilicon, "logicDetector");
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(GS::xDetector, GS::yDetector, GS::zDetector), logicDetector, "physDetector", logicWindowSiPM, false, 0, true);

    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicPackageSiPM, "aSiPM", logicWorld, false, 0, true);


    // Vis attributes
    // World
    G4VisAttributes *visWorld = new G4VisAttributes();
    visWorld->SetVisibility(false);
    logicWorld->SetVisAttributes(visWorld);

    // SiPM packages
    G4VisAttributes *visPackage = new G4VisAttributes();
    visPackage->SetColour(1, 1, 0, 0.7);
    logicPackageSiPM->SetVisAttributes(visPackage);

    // SiPM windows
    G4VisAttributes *visWindow = new G4VisAttributes();
    visWindow->SetColour(1, 1, 1, 0.7);
    logicWindowSiPM->SetVisAttributes(visWindow);

    // SiPM silicon layers
    G4VisAttributes *visDetector = new G4VisAttributes();
    visDetector->SetColour(0.45, 0.25, 0, 0.7);
    logicDetector->SetVisAttributes(visDetector);
}



void MyDetectorConstruction::ConstructCosmicRaysDetectors()
{
    G4Box *solidCosmicRaysDetector = new G4Box("solidCosmicRaysDetector", GS::halfZXsideCosmicRayDetector, GS::halfYsideCosmicRayDetector, GS::halfZXsideCosmicRayDetector);
    logicCosmicRaysDetector = new G4LogicalVolume(solidCosmicRaysDetector, fAir, "logicCosmicRaysDetector");
    G4VPhysicalVolume *physUpCosmicRaysDetector = new G4PVPlacement(0, G4ThreeVector(GS::xCosmicRayDetector, GS::yCosmicRayDetector, GS::zCosmicRayDetector), logicCosmicRaysDetector, "physUpCosmicRaysDetector", logicWorld, false, 0, true);
    G4VPhysicalVolume *physBottomCosmicRaysDetector = new G4PVPlacement(0, G4ThreeVector(GS::xCosmicRayDetector, -GS::yCosmicRayDetector, GS::zCosmicRayDetector), logicCosmicRaysDetector, "physBottomCosmicRaysDetector", logicWorld, false, 1, true);

    fCosmicTriggerVolume = logicCosmicRaysDetector;
}