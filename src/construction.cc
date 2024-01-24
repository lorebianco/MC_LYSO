#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    DefineCommands();
    
    // Default settings
    fIsLightGuide = false;
    nLightGuideMat = 1;    
    fIsPCB = true;
    fIsEndcap = true;

    DefineMaterials();
}



MyDetectorConstruction::~MyDetectorConstruction()
{}



void MyDetectorConstruction::DefineMaterials()
{
    //Entries for LYSO properties
    const int nEntries = 3;

    G4NistManager *nist = G4NistManager::Instance();


    //*******************************************************************//
    //**********************       MATERIALS       **********************//
    //*******************************************************************//
    
    // LYSO
    fLYSO = new G4Material("LYSO", 7.25*g/cm3, 5, kStateSolid);
    fLYSO->AddElement(nist->FindOrBuildElement("Lu"), 73.8579*perCent);
    fLYSO->AddElement(nist->FindOrBuildElement("Y"), 1.9747*perCent);
    fLYSO->AddElement(nist->FindOrBuildElement("Si"), 6.2418*perCent);
    fLYSO->AddElement(nist->FindOrBuildElement("O"), 17.77*perCent);
    fLYSO->AddElement(nist->FindOrBuildElement("Ce"), 0.1556*perCent);

    G4MaterialPropertiesTable *mptLYSO = new G4MaterialPropertiesTable();
    
    G4double Energies[nEntries] = {2.97*eV, 3*eV, 3.56*eV};
    G4double LYSO_RINDEX[nEntries] = {1.82, 1.82, 1.82};
    G4double LYSO_ABSLENGTH[nEntries] = {1*m, 1*m, 1*m};
    G4double LYSO_SCINTILLATIONCOMPONENT1[nEntries] = {0.1, 1, 0.1};

    mptLYSO->AddProperty("SCINTILLATIONCOMPONENT1", Energies, LYSO_SCINTILLATIONCOMPONENT1, nEntries);
    mptLYSO->AddProperty("RINDEX", Energies, LYSO_RINDEX, nEntries);
    mptLYSO->AddProperty("ABSLENGTH", Energies, LYSO_ABSLENGTH, nEntries);
    mptLYSO->AddConstProperty("SCINTILLATIONYIELD", 29000./MeV);
    mptLYSO->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptLYSO->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 42.*ns);
    mptLYSO->AddConstProperty("SCINTILLATIONYIELD1", 1.);  
    
    fLYSO->SetMaterialPropertiesTable(mptLYSO);
    
    // AIR
    fAir = nist->FindOrBuildMaterial("G4_AIR");
    
    G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
    
    G4double AIR_RINDEX[nEntries] = {1.000293, 1.000293, 1.000293};
    mptAir->AddProperty("RINDEX", Energies, AIR_RINDEX, nEntries);

    fAir->SetMaterialPropertiesTable(mptAir);

    // VACUUM
    fVacuum = nist->FindOrBuildMaterial("G4_Galactic");

    G4MaterialPropertiesTable *mptVacuum = new G4MaterialPropertiesTable();
    
    G4double VACUUM_RINDEX[nEntries] = {1., 1., 1.};
    mptVacuum->AddProperty("RINDEX", Energies, VACUUM_RINDEX, nEntries);

    fVacuum->SetMaterialPropertiesTable(mptVacuum);

    // ALUMINIUM
    fAluminium = nist->FindOrBuildMaterial("G4_Al");

    // SILICON
    fSilicon = nist->FindOrBuildMaterial("G4_Si");
    G4MaterialPropertiesTable *mptDetector = new G4MaterialPropertiesTable();
    G4double DETECTOR_RINDEX[nEntries] = {1.55, 1.55, 1.55}; //should be 3.88, but it is included in the PDE, so it's fixed at the same value of the window (Total transmission)
    mptDetector->AddProperty("RINDEX", Energies, DETECTOR_RINDEX, nEntries);

    fSilicon->SetMaterialPropertiesTable(mptDetector);

    // EPOXY
    fEpoxy = new G4Material("Epoxy", 1.2*g/cm3, 2);
    fEpoxy->AddElement(nist->FindOrBuildElement("H"), 2);
    fEpoxy->AddElement(nist->FindOrBuildElement("C"), 2);
    G4MaterialPropertiesTable *mptEpoxy = new G4MaterialPropertiesTable();
    G4double EPOXY_RINDEX[nEntries] = {1.55, 1.55, 1.55};
    mptEpoxy->AddProperty("RINDEX", Energies, EPOXY_RINDEX, nEntries);

    fEpoxy->SetMaterialPropertiesTable(mptEpoxy);

    // FR4
    fFR4 = new G4Material("FR4", 1.85*g/cm3, 2);
    fFR4->AddMaterial(nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"), 52.8*perCent);
    fFR4->AddMaterial(fEpoxy, 47.2*perCent);

    // CARBBON FIBER
    fCarbonFiber = new G4Material("Carbon Fiber", 1.5*g/cm3, 1);
    fCarbonFiber->AddElement(nist->FindOrBuildElement("C"), 1);

    // PLEXIGLASS
    fPlexiglass = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
    
    G4MaterialPropertiesTable *mptPlexiglass = new G4MaterialPropertiesTable();
    
    G4double PLEXIGLASS_RINDEX[nEntries] = {1.49, 1.49, 1.49};
    mptPlexiglass->AddProperty("RINDEX", Energies, PLEXIGLASS_RINDEX, nEntries);
    
    fPlexiglass->SetMaterialPropertiesTable(mptPlexiglass);
    
    // SAPPHIRE
    fSapphire = new G4Material("Sapphire", 3.98*g/cm3, 2, kStateSolid);
    fSapphire->AddElement(nist->FindOrBuildElement("Al"), 2);
    fSapphire->AddElement(nist->FindOrBuildElement("O"), 3);

    G4MaterialPropertiesTable *mptSapphire = new G4MaterialPropertiesTable();

    G4double SAPPHIRE_RINDEX[nEntries] = {1.77, 1.77, 1.77};
    mptSapphire->AddProperty("RINDEX", Energies, SAPPHIRE_RINDEX, nEntries);

    fSapphire->SetMaterialPropertiesTable(mptSapphire);
}



G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // Construct the World
    solidWorld = new G4Box("solidWorld", GS::halfXsideWorld, GS::halfYsideWorld, GS::halfZsideWorld);
    logicWorld = new G4LogicalVolume(solidWorld, fAir, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    // Construct the scintillator crystal
    solidScintillator = new G4Tubs("solidScintillator", 0.*cm, GS::radiusScintillator, GS::halfheightScintillator, 0.*deg, 360.*deg);
    logicScintillator = new G4LogicalVolume(solidScintillator, fLYSO, "logicScintillator");
    physScintillator = new G4PVPlacement(0, G4ThreeVector(GS::xScintillator, GS::yScintillator, GS::zScintillator), logicScintillator, "physScintillator", logicWorld, false, 0, true);

    // Assign the logic scoring volume to the crystal
    fScoringVolume = logicScintillator;
    
    // Construct a coating for the crystal and the lightguide (if present)
    solidCoating = new G4Tubs("solidCoating", (GS::radiusScintillator + GS::coating_space), (GS::radiusScintillator + GS::coating_space + GS::coating_thickness), GS::halfheightScintillator+2*(GS::halfheightLightGuide*fIsLightGuide), 0.*deg, 360.*deg);
    logicCoating = new G4LogicalVolume(solidCoating, fAluminium, "logicCoating");
    physCoating = new G4PVPlacement(0, G4ThreeVector(GS::xScintillator, GS::yScintillator, GS::zScintillator), logicCoating, "physCoating", logicWorld, false, 0, true);
    
    // Construct lightguides, PCBs and endcaps -if setted-
    if(fIsLightGuide) ConstructLightGuide();
    if(fIsPCB) ConstructPCB();
    if(fIsEndcap) ConstructEndcap();

    // Construct the package of the SIPM
    solidPackageSiPM = new G4Box("solidPackageSiPM", GS::halfXsidePackageSiPM, GS::halfYsidePackageSiPM, GS::halfZsidePackageSiPM);
    logicPackageSiPM = new G4LogicalVolume(solidPackageSiPM, fFR4, "logicPackageSiPM");

    // Construct the window of the SIPM and put it inside the package
    solidWindowSiPM = new G4Box("solidWindowSiPM", GS::halfXsideWindowSiPM, GS::halfYsideWindowSiPM, GS::halfZsideWindowSiPM);
    logicWindowSiPM = new G4LogicalVolume(solidWindowSiPM, fEpoxy, "logicWindowSiPM");
    physWindowSiPM = new G4PVPlacement(0, G4ThreeVector(GS::xWindowSiPM, GS::yWindowSiPM, GS::zWindowSiPM), logicWindowSiPM, "physWindowSiPM", logicPackageSiPM, false, 0, true);

    // Construct the silicon layer and put it inside the window
    solidDetector = new G4Box("solidDetector", GS::halfXsideDetector, GS::halfYsideDetector, GS::halfZsideDetector);
    logicDetector = new G4LogicalVolume(solidDetector, fSilicon, "logicDetector");
    physDetector = new G4PVPlacement(0, G4ThreeVector(GS::xDetector, GS::yDetector, GS::zDetector), logicDetector, "physDetector", logicWindowSiPM, false, 0, true);

    // Define the arrangement of SiPMs
    G4int indexDetector = 0;
    for(G4int i = 0; i<13; i++)
    {
        if(i==0 || i==12)
        {
            for(G4int j = 0; j<3; j++)
            {
                PositionSiPMs(i, j, 1, indexDetector);
                indexDetector++;
            }
        }
        if(i==1 || i==11)
        {
            for(G4int j = 0; j<7; j++)
            {
                PositionSiPMs(i, j, 3, indexDetector);
                indexDetector++;
            }
        }
        if(i==2 || i==10)
        {
            for(G4int j = 0; j<9; j++)
            {
                PositionSiPMs(i, j, 4, indexDetector);
                indexDetector++;
            }
        }
        if(i>=3 && i<=9)
        {
            for(G4int j = 0; j<11; j++)
            {
                PositionSiPMs(i, j, 5, indexDetector);
                indexDetector++;
            }
        }
    }

    // Add visualization attributes
    DefineVisAttributes();

    // Always return physWorld
    return physWorld;
}



void MyDetectorConstruction::ConstructSDandField()
{
    // Set logic silicon layer as sensitive volume. Use G4SDManager to do it
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector", "HitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
    SetSensitiveDetector(logicDetector, sensDet);
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

    solidDrilledLightGuide = new G4SubtractionSolid("solidDrilledLightGuide", solidLightGuide, solidHoles);
        
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
    physFrontLightGuide = new G4PVPlacement(0, G4ThreeVector(0., 0., GS::zFrontFaceScintillator-GS::halfheightLightGuide), logicLightGuide, "physFrontLightGuide", logicWorld, false, 0, true);
    physBackLightGuide = new G4PVPlacement(0, G4ThreeVector(0., 0., GS::zBackFaceScintillator+GS::halfheightLightGuide), logicLightGuide, "physBackLightGuide", logicWorld, false, 1, true);
}



void MyDetectorConstruction::ConstructPCB()
{
    // PCB as cilinder
    solidPCB = new G4Tubs("solidPCB", 0, GS::radiusPCB, GS::halfheightPCB, 0.*deg, 360.*deg);
    logicPCB = new G4LogicalVolume(solidPCB, fFR4, "logicPCB");
    physFrontPCB = new G4PVPlacement(0, G4ThreeVector(0, 0, GS::zFrontFaceScintillator-2*(GS::halfheightLightGuide*fIsLightGuide)-2*GS::halfZsidePackageSiPM-GS::halfheightPCB), logicPCB, "physFrontPCB", logicWorld, false, 0, true);
    physBackPCB = new G4PVPlacement(0, G4ThreeVector(0, 0, GS::zBackFaceScintillator+2*(GS::halfheightLightGuide*fIsLightGuide)+2*GS::halfZsidePackageSiPM+GS::halfheightPCB), logicPCB, "physBackPCB", logicWorld, false, 1, true);
}



void MyDetectorConstruction::ConstructEndcap()
{   
    // Endcap as cilinder
    solidEndcap = new G4Tubs("solidEndcap", 0, GS::radiusEndcap, GS::halfheightEndcap, 0.*deg, 360.*deg);
    logicEndcap = new G4LogicalVolume(solidEndcap, fCarbonFiber, "logicEndcap");
    physFrontEndcap = new G4PVPlacement(0, G4ThreeVector(0, 0, GS::zFrontFaceScintillator-2*(GS::halfheightLightGuide*fIsLightGuide)-2*GS::halfZsidePackageSiPM-2*(GS::halfheightPCB*fIsPCB)-GS::halfheightEndcap), logicEndcap, "physFrontEndcap", logicWorld, false, 0, true);
    physBackEndcap = new G4PVPlacement(0, G4ThreeVector(0, 0, GS::zBackFaceScintillator+2*(GS::halfheightLightGuide*fIsLightGuide)+2*GS::halfZsidePackageSiPM+2*(GS::halfheightPCB*fIsPCB)+GS::halfheightEndcap), logicEndcap, "physBackEndcap", logicWorld, false, 1, true);
}



void MyDetectorConstruction::PositionSiPMs(G4int row, G4int col, G4int halfCols, G4int index)
{
    // Place the packages. When in back face, need to rotate them of 180°
    physFrontPackageSiPM = new G4PVPlacement(0,G4ThreeVector((2*GS::halfXsidePackageSiPM*(col-halfCols)), (2*GS::halfYsidePackageSiPM*(6-row)), GS::zFrontFaceScintillator-2*(GS::halfheightLightGuide*fIsLightGuide)-GS::halfZsidePackageSiPM), logicPackageSiPM, "physFrontPackageSiPM", logicWorld, false, index, true);
    
    G4Rotate3D rotXBackDet(180*deg, G4ThreeVector(1, 0, 0));
    G4Translate3D transBackDet(G4ThreeVector(2*GS::halfXsidePackageSiPM*(col-halfCols), 2*GS::halfYsidePackageSiPM*(6-row), GS::zBackFaceScintillator+2*(GS::halfheightLightGuide*fIsLightGuide)+GS::halfZsidePackageSiPM));
    G4Transform3D transformBackDet = (transBackDet)*(rotXBackDet);
                
    physBackPackageSiPM = new G4PVPlacement(transformBackDet, logicPackageSiPM, "physBackPackageSiPM", logicWorld, false, index, true);
}



void MyDetectorConstruction::DefineVisAttributes()
{
    // Endcaps
    if(fIsEndcap)
    {
        visEndcap = new G4VisAttributes();
        visEndcap->SetColour(0.5, 0.5, 0.5, 0.5);
        logicEndcap->SetVisAttributes(visEndcap);
    }
    
    // PCBs
    if(fIsPCB)
    {
        visPCB = new G4VisAttributes();
        visPCB->SetColour(0, 1, 0, 0.65);
        logicPCB->SetVisAttributes(visPCB);
    }

    // Light guides
    if(fIsLightGuide)
    {
        visLightGuide = new G4VisAttributes();
        visLightGuide->SetColour(1, 1, 1, 0.5);
        logicLightGuide->SetVisAttributes(visLightGuide);
    }

    // SiPM packages
    visPackage = new G4VisAttributes();
    visPackage->SetColour(1, 1, 0, 0.7);
    logicPackageSiPM->SetVisAttributes(visPackage);

    // SiPM windows
    visWindow = new G4VisAttributes();
    visWindow->SetColour(1, 1, 1, 0.7);
    logicWindowSiPM->SetVisAttributes(visWindow);

    // SiPM silicon layers
    visDetector = new G4VisAttributes();
    visDetector->SetColour(0.45, 0.25, 0, 0.7);
    logicDetector->SetVisAttributes(visDetector);

    // Scintillator
    visScintillator = new G4VisAttributes();
    visScintillator->SetColour(0, 0, 1, 0.98);
    logicScintillator->SetVisAttributes(visScintillator);

    // Coating
    visCoating = new G4VisAttributes();
    visCoating->SetVisibility(false);
    logicCoating->SetVisAttributes(visCoating);
}



void MyDetectorConstruction::DefineCommands()
{
    // Define my UD-messenger for the detector construction
    fMessenger = new G4GenericMessenger(this, "/my_construction/", "Construction settings");
    fMessenger->DeclareProperty("isLightGuide", fIsLightGuide, "Set if the two light guides are present");
    fMessenger->DeclareProperty("isPCB", fIsPCB, "Set if the two PCBs are present");
    fMessenger->DeclareProperty("isEndcap", fIsEndcap, "Set if the two endcaps are present");
    fMessenger->DeclareProperty("MaterialOfLightGuide", nLightGuideMat, "Set the material of light guide: 1 = Plexiglass, 2 = Sapphire");
}