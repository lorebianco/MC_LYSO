#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    //Define my UD-messenger
    fMessenger = new G4GenericMessenger(this, "/my_construction/", "Construction settings");
    fMessenger->DeclareProperty("isLightGuide", isLightGuide, "Set if the two light guides are present");
    fMessenger->DeclareProperty("isPCB", isPCB, "Set if the two PCBs are present");
    fMessenger->DeclareProperty("isEndcap", isEndcap, "Set if the two endcaps are present");
    fMessenger->DeclareProperty("MaterialOfLightGuide", nLightGuideMat, "Set the material of light guide: 1 = Plexiglass, 2 = Sapphire");
    //*********************************//
    
    isLightGuide = false;
    nLightGuideMat = 1;

    isPCB = true;
    isEndcap = true;

    DefineMaterialsAndSurfaces();
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterialsAndSurfaces()
{
    //Entries for LYSO properties
    const int nEntries = 3;

    G4NistManager *nist = G4NistManager::Instance();

    //*******************************************************************//
    //**********************       MATERIALS       **********************//
    //*******************************************************************//
    
    //******************       LYSO       ******************//
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
    //******************************************************//
    
    //******************       AIR       ******************//
    fAir = nist->FindOrBuildMaterial("G4_AIR");
    
    G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
    
    G4double AIR_RINDEX[nEntries] = {1.000293, 1.000293, 1.000293};
    mptAir->AddProperty("RINDEX", Energies, AIR_RINDEX, nEntries);

    fAir->SetMaterialPropertiesTable(mptAir);
    //*****************************************************//

    //******************       VACUUM       ******************//
    fVacuum = nist->FindOrBuildMaterial("G4_Galactic");

    G4MaterialPropertiesTable *mptVacuum = new G4MaterialPropertiesTable();
    
    G4double VACUUM_RINDEX[nEntries] = {1., 1., 1.};
    mptVacuum->AddProperty("RINDEX", Energies, VACUUM_RINDEX, nEntries);

    fVacuum->SetMaterialPropertiesTable(mptVacuum);
    //********************************************************//

    //******************       ALUMINIUM       ******************//
    fAluminium = nist->FindOrBuildMaterial("G4_Al");
    //***********************************************************//

    //******************       SILICON       ******************//
    fSilicon = nist->FindOrBuildMaterial("G4_Si");
    G4MaterialPropertiesTable *mptDetector = new G4MaterialPropertiesTable();
    G4double DETECTOR_RINDEX[nEntries] = {1.55, 1.55, 1.55}; //need to change? should be 3.88, but I THINK it is included in the PDE, so let's fix it at the same value of window (Total transmission)
    mptDetector->AddProperty("RINDEX", Energies, DETECTOR_RINDEX, nEntries);

    fSilicon->SetMaterialPropertiesTable(mptDetector);
    //*******************************************************************//

    //***********************       EPOXY       ***********************//
    fEpoxy = new G4Material("Epoxy", 1.2*g/cm3, 2);
    fEpoxy->AddElement(nist->FindOrBuildElement("H"), 2);
    fEpoxy->AddElement(nist->FindOrBuildElement("C"), 2);
    G4MaterialPropertiesTable *mptEpoxy = new G4MaterialPropertiesTable();
    G4double EPOXY_RINDEX[nEntries] = {1.55, 1.55, 1.55};
    mptEpoxy->AddProperty("RINDEX", Energies, EPOXY_RINDEX, nEntries);

    fEpoxy->SetMaterialPropertiesTable(mptEpoxy);
    //*****************************************************************//

    //******************       FR4       ******************//
    fFR4 = new G4Material("FR4", 1.85*g/cm3, 2);
    fFR4->AddMaterial(nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"), 52.8*perCent);
    fFR4->AddMaterial(fEpoxy, 47.2*perCent);
    //*****************************************************//

    //******************       CARBBON FIBER (d=???)       ******************//
    fCarbonFiber = new G4Material("Carbon Fiber", 1.5*g/cm3, 1);
    fCarbonFiber->AddElement(nist->FindOrBuildElement("C"), 1);
    //***********************************************************************//

    //******************       PLEXIGLASS       ******************//
    fPlexiglass = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
    
    G4MaterialPropertiesTable *mptPlexiglass = new G4MaterialPropertiesTable();
    
    G4double PLEXIGLASS_RINDEX[nEntries] = {1.49, 1.49, 1.49};
    mptPlexiglass->AddProperty("RINDEX", Energies, PLEXIGLASS_RINDEX, nEntries);
    
    fPlexiglass->SetMaterialPropertiesTable(mptPlexiglass);
    
    //************************************************************//

    //******************       SAPPHIRE       ******************//
    fSapphire = new G4Material("Sapphire", 3.98*g/cm3, 2, kStateSolid);
    fSapphire->AddElement(nist->FindOrBuildElement("Al"), 2);
    fSapphire->AddElement(nist->FindOrBuildElement("O"), 3);

    G4MaterialPropertiesTable *mptSapphire = new G4MaterialPropertiesTable();

    G4double SAPPHIRE_RINDEX[nEntries] = {1.77, 1.77, 1.77};
    mptSapphire->AddProperty("RINDEX", Energies, SAPPHIRE_RINDEX, nEntries);

    fSapphire->SetMaterialPropertiesTable(mptSapphire);

    //**********************************************************//




    
    //********************************************************************//
    //***********************       SURFACES       ***********************//
    //********************************************************************//

    //******************       TAPE SURFACE       ******************//
    tapeSurface = new G4OpticalSurface("tapeSurface");
    tapeSurface->SetType(dielectric_metal);
    tapeSurface->SetFinish(polished);
    tapeSurface->SetModel(unified);
    
    G4MaterialPropertiesTable *mptTape = new G4MaterialPropertiesTable();
    G4double TAPE_REFLECTIVITY[nEntries] = {0., 0., 0.};
    
    mptTape->AddProperty("REFLECTIVITY", Energies, TAPE_REFLECTIVITY, nEntries);
    
    tapeSurface->SetMaterialPropertiesTable(mptTape);
    //**************************************************************//

}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    //Light Guide
    G4double this_radiusLightGuide = GS.radiusLightGuide;
    G4double this_halfheightLightGuide = GS.halfheightLightGuide;
    if(!isLightGuide)
    {
        this_radiusLightGuide = 0.;
        this_halfheightLightGuide = 0.;
    }

    //PCB
    G4double this_radiusPCB = GS.radiusPCB;
    G4double this_halfheightPCB = GS.halfheightPCB;
    if(!isPCB)
    {
        this_radiusPCB = 0.;
        this_halfheightPCB = 0.;
    }

    //Endcap
    G4double this_radiusEndcap = GS.radiusEndcap;
    G4double this_halfheightEndcap = GS.halfheightEndcap;
    if(!isEndcap)
    {
        this_radiusEndcap = 0.;
        this_halfheightEndcap = 0.;
    }

    //************************************************************************//


    solidWorld = new G4Box("solidWorld", GS.halfXsideWorld, GS.halfYsideWorld, GS.halfZsideWorld);
    logicWorld = new G4LogicalVolume(solidWorld, fAir, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    solidScintillator = new G4Tubs("solidScintillator", 0.*cm, GS.radiusScintillator, GS.halfheightScintillator, 0.*deg, 360.*deg);
    logicScintillator = new G4LogicalVolume(solidScintillator, fLYSO, "logicScintillator");
    physScintillator = new G4PVPlacement(0, G4ThreeVector(GS.xScintillator, GS.yScintillator, GS.zScintillator), logicScintillator, "physScintillator", logicWorld, false, 0, true);

    fScoringVolume = logicScintillator;

    if(isLightGuide)
    {
        G4Material *fLightGuideMaterial = 0;

        solidLightGuide = new G4Tubs("solidLightGuide", 0, this_radiusLightGuide, this_halfheightLightGuide, 0.*deg, 360.*deg);

        //Drill 4 holes in the light guide for LEDs 
        solidHoleUP = new G4Tubs("solidHoleLightGuide", 0, GS.radiusHole, GS.depthHole, 0.*deg, 360*deg);
        solidHoleDOWN = new G4Tubs("solidHoleLightGuide", 0, GS.radiusHole, GS.depthHole, 0.*deg, 360*deg);
        solidHoleRIGHT = new G4Tubs("solidHoleLightGuide", 0, GS.radiusHole, GS.depthHole, 0.*deg, 360*deg);
        solidHoleLEFT = new G4Tubs("solidHoleLightGuide", 0, GS.radiusHole, GS.depthHole, 0.*deg, 360*deg);

        G4Rotate3D rotXholeUP(90*deg, G4ThreeVector(1, 0, 0));
        G4Translate3D transYholeUP(G4ThreeVector(0, this_radiusLightGuide, 0));
        G4Transform3D transformHoleUP = (transYholeUP)*(rotXholeUP);

        G4Rotate3D rotXholeDOWN(90*deg, G4ThreeVector(1, 0, 0));
        G4Translate3D transYholeDOWN(G4ThreeVector(0, -this_radiusLightGuide, 0));
        G4Transform3D transformHoleDOWN = (transYholeDOWN)*(rotXholeDOWN);

        G4Rotate3D rotYholeRIGHT(90*deg, G4ThreeVector(0, 1, 0));
        G4Translate3D transYholeRIGHT(G4ThreeVector(this_radiusLightGuide, 0, 0));
        G4Transform3D transformHoleRIGHT = (transYholeRIGHT)*(rotYholeRIGHT);

        G4Rotate3D rotYholeLEFT(90*deg, G4ThreeVector(0, 1, 0));
        G4Translate3D transYholeLEFT(G4ThreeVector(-this_radiusLightGuide, 0, 0));
        G4Transform3D transformHoleLEFT = (transYholeLEFT)*(rotYholeLEFT);

        solidHoles = new G4MultiUnion("solidHoles");
        solidHoles->AddNode(*solidHoleUP, transformHoleUP);
        solidHoles->AddNode(*solidHoleDOWN, transformHoleDOWN);
        solidHoles->AddNode(*solidHoleRIGHT, transformHoleRIGHT);
        solidHoles->AddNode(*solidHoleLEFT, transformHoleLEFT);
        solidHoles->Voxelize();

        realsolidLightGuide = new G4SubtractionSolid("LightGuide-Holes", solidLightGuide, solidHoles);
        
        //Options for material
        if(nLightGuideMat==1) //Plexiglass
        {
            fLightGuideMaterial = fPlexiglass;
        }
        if(nLightGuideMat==2) //Sapphire
        {
            fLightGuideMaterial = fSapphire;
        }

        logicLightGuide = new G4LogicalVolume(realsolidLightGuide, fLightGuideMaterial, "logicLightGuide");
        physFrontLightGuide = new G4PVPlacement(0, G4ThreeVector(0., 0., GS.zFrontFaceScintillator-this_halfheightLightGuide), logicLightGuide, "physFrontLightGuide", logicWorld, false, 0, true);
        physBackLightGuide = new G4PVPlacement(0, G4ThreeVector(0., 0., GS.zBackFaceScintillator+this_halfheightLightGuide), logicLightGuide, "physBackLightGuide", logicWorld, false, 1, true);
    }
    
    solidCoating = new G4Tubs("solidCoating", (GS.radiusScintillator + GS.coating_space), (GS.radiusScintillator + GS.coating_space + GS.coating_thickness), GS.halfheightScintillator+2*this_halfheightLightGuide, 0.*deg, 360.*deg);
    logicCoating = new G4LogicalVolume(solidCoating, fAluminium, "logicCoating");
    physCoating = new G4PVPlacement(0, G4ThreeVector(GS.xScintillator, GS.yScintillator, GS.zScintillator), logicCoating, "physCoating", logicWorld, false, 0, true);
    
    tapeSkin = new G4LogicalSkinSurface("tapeSkin", logicCoating, tapeSurface);

    if(isPCB)
    {
        solidPCB = new G4Tubs("solidPCB", 0, this_radiusPCB, this_halfheightPCB, 0.*deg, 360.*deg);
        logicPCB = new G4LogicalVolume(solidPCB, fFR4, "logicPCB");
        physFrontPCB = new G4PVPlacement(0, G4ThreeVector(0, 0, GS.zFrontFaceScintillator-2*this_halfheightLightGuide-2*GS.halfZsidePackageSiPM-this_halfheightPCB), logicPCB, "physFrontPCB", logicWorld, false, 0, true);
        physBackPCB = new G4PVPlacement(0, G4ThreeVector(0, 0, GS.zBackFaceScintillator+2*this_halfheightLightGuide+2*GS.halfZsidePackageSiPM+this_halfheightPCB), logicPCB, "physBackPCB", logicWorld, false, 1, true);
    }

    if(isEndcap)
    {
        solidEndcap = new G4Tubs("solidEndcap", 0, this_radiusEndcap, this_halfheightEndcap, 0.*deg, 360.*deg);
        logicEndcap = new G4LogicalVolume(solidEndcap, fCarbonFiber, "logicEndcap");
        physFrontEndcap = new G4PVPlacement(0, G4ThreeVector(0, 0, GS.zFrontFaceScintillator-2*this_halfheightLightGuide-2*GS.halfZsidePackageSiPM-2*this_halfheightPCB-this_halfheightEndcap), logicEndcap, "physFrontEndcap", logicWorld, false, 0, true);
        physBackEndcap = new G4PVPlacement(0, G4ThreeVector(0, 0, GS.zBackFaceScintillator+2*this_halfheightLightGuide+2*GS.halfZsidePackageSiPM+2*this_halfheightPCB+this_halfheightEndcap), logicEndcap, "physBackEndcap", logicWorld, false, 1, true);
    }


    solidFrontPackageSiPM = new G4Box("solidFrontPackageSiPM", GS.halfXsidePackageSiPM, GS.halfYsidePackageSiPM, GS.halfZsidePackageSiPM);
    solidBackPackageSiPM = new G4Box("solidBackPackageSiPM", GS.halfXsidePackageSiPM, GS.halfYsidePackageSiPM, GS.halfZsidePackageSiPM);
    
    logicFrontPackageSiPM = new G4LogicalVolume(solidFrontPackageSiPM, fFR4, "logicPackageSiPM");
    logicBackPackageSiPM = new G4LogicalVolume(solidBackPackageSiPM, fFR4, "logicPackageSiPM");


    solidFrontWindowSiPM = new G4Box("solidFrontWindowSiPM", GS.halfXsideWindowSiPM, GS.halfYsideWindowSiPM, GS.halfZsideWindowSiPM);
    solidBackWindowSiPM = new G4Box("solidBackWindowSiPM", GS.halfXsideWindowSiPM, GS.halfYsideWindowSiPM, GS.halfZsideWindowSiPM);

    logicFrontWindowSiPM = new G4LogicalVolume(solidFrontWindowSiPM, fEpoxy, "logicFrontWindowSiPM");
    logicBackWindowSiPM = new G4LogicalVolume(solidBackWindowSiPM, fEpoxy, "logicBackWindowSiPM");

    physFrontWindowsSiPM = new G4PVPlacement(0, G4ThreeVector(GS.xWindowSiPM, GS.yWindowSiPM, GS.zFrontWindowSiPM), logicFrontWindowSiPM, "physFrontWindowsSiPM", logicFrontPackageSiPM, false, 0, true);
    physBackWindowsSiPM = new G4PVPlacement(0, G4ThreeVector(GS.xWindowSiPM, GS.yWindowSiPM, GS.zBackWindowSiPM), logicBackWindowSiPM, "physBackWindowsSiPM", logicBackPackageSiPM, false, 0, true);


    solidFrontDetector = new G4Box("solidFrontDetector", GS.halfXsideDetector, GS.halfYsideDetector, GS.halfZsideDetector);
    solidBackDetector = new G4Box("solidBackDetector", GS.halfXsideDetector, GS.halfYsideDetector, GS.halfZsideDetector);
    
    logicFrontDetector = new G4LogicalVolume(solidFrontDetector, fSilicon, "logicFrontDetector");
    logicBackDetector = new G4LogicalVolume(solidBackDetector, fSilicon, "logicBackDetector");

    physFrontDetector = new G4PVPlacement(0, G4ThreeVector(GS.xDetector, GS.yDetector, GS.zFrontDetector), logicFrontDetector, "physFrontDetector", logicFrontWindowSiPM, false, 0, true);
    physBackDetector = new G4PVPlacement(0, G4ThreeVector(GS.xDetector, GS.yDetector, GS.zBackDetector), logicBackDetector, "physBackDetector", logicBackWindowSiPM, false, 0, true);


    //Define the arrangement of SiPMs
    G4int indexDetector = 0;
    for(G4int i = 0; i<13; i++)
    {
        if(i==0 || i==12)
        {
            for(G4int j = 0; j<3; j++)
            {
                physBackPackageSiPM = new G4PVPlacement(0,G4ThreeVector(2*GS.halfXsidePackageSiPM*(j-1), 2*GS.halfYsidePackageSiPM*(6-i), GS.zBackFaceScintillator+2*this_halfheightLightGuide+GS.halfZsidePackageSiPM), logicBackPackageSiPM, "physBackPackageSiPM", logicWorld, false, indexDetector, true);
                
                physFrontPackageSiPM = new G4PVPlacement(0,G4ThreeVector((2*GS.halfXsidePackageSiPM*(j-1))*mm, (2*GS.halfYsidePackageSiPM*(6-i))*mm, GS.zFrontFaceScintillator-2*this_halfheightLightGuide-GS.halfZsidePackageSiPM), logicFrontPackageSiPM, "physFrontPackageSiPM", logicWorld, false, indexDetector, true);
                
                indexDetector++;
            }
        }
        if(i==1 || i==11)
        {
            for(G4int j = 0; j<7; j++)
            {
                physBackPackageSiPM = new G4PVPlacement(0,G4ThreeVector(2*GS.halfXsidePackageSiPM*(j-3), 2*GS.halfYsidePackageSiPM*(6-i), GS.zBackFaceScintillator+2*this_halfheightLightGuide+GS.halfZsidePackageSiPM), logicBackPackageSiPM, "physBackPackageSiPM", logicWorld, false, indexDetector, true);

                physFrontPackageSiPM = new G4PVPlacement(0,G4ThreeVector(2*GS.halfXsidePackageSiPM*(j-3), 2*GS.halfYsidePackageSiPM*(6-i), GS.zFrontFaceScintillator-2*this_halfheightLightGuide-GS.halfZsidePackageSiPM), logicFrontPackageSiPM, "physFrontPackageSiPM", logicWorld, false, indexDetector, true);
                
                indexDetector++;
            }
        }
        if(i==2 || i==10)
        {
            for(G4int j = 0; j<9; j++)
            {
                physBackPackageSiPM = new G4PVPlacement(0,G4ThreeVector(2*GS.halfXsidePackageSiPM*(j-4), 2*GS.halfYsidePackageSiPM*(6-i), GS.zBackFaceScintillator+2*this_halfheightLightGuide+GS.halfZsidePackageSiPM), logicBackPackageSiPM, "physBackPackageSiPM", logicWorld, false, indexDetector, true);

                physFrontPackageSiPM = new G4PVPlacement(0,G4ThreeVector(2*GS.halfXsidePackageSiPM*(j-4), 2*GS.halfYsidePackageSiPM*(6-i), GS.zFrontFaceScintillator-2*this_halfheightLightGuide-GS.halfZsidePackageSiPM), logicFrontPackageSiPM, "physFrontPackageSiPM", logicWorld, false, indexDetector, true);

                indexDetector++;
            }
        }
        if(i>=3 && i<=9)
        {
            for(G4int j = 0; j<11; j++)
            {
                physBackPackageSiPM = new G4PVPlacement(0,G4ThreeVector(2*GS.halfXsidePackageSiPM*(j-5), 2*GS.halfYsidePackageSiPM*(6-i), GS.zBackFaceScintillator+2*this_halfheightLightGuide+GS.halfZsidePackageSiPM), logicBackPackageSiPM, "physBackPackageSiPM", logicWorld, false, indexDetector, true);

                physFrontPackageSiPM = new G4PVPlacement(0,G4ThreeVector(2*GS.halfXsidePackageSiPM*(j-5), 2*GS.halfYsidePackageSiPM*(6-i), GS.zFrontFaceScintillator-2*this_halfheightLightGuide-GS.halfZsidePackageSiPM), logicFrontPackageSiPM, "physFrontPackageSiPM", logicWorld, false, indexDetector, true);

                indexDetector++;
            }
        }
    }


    DefineVisAttributes();

    return physWorld;
}



void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensFrontDet = new MySensitiveDetector("SensitiveFrontDetector");
    logicFrontDetector->SetSensitiveDetector(sensFrontDet);

    MySensitiveDetector *sensBackDet = new MySensitiveDetector("SensitiveBackDetector");
    logicBackDetector->SetSensitiveDetector(sensBackDet);
}



void MyDetectorConstruction::DefineVisAttributes()
{
    if(isEndcap)
    {
        visEndcap = new G4VisAttributes();
        visEndcap->SetColour(0.5, 0.5, 0.5, 0.5);
        logicEndcap->SetVisAttributes(visEndcap);
    }
    
    if(isPCB)
    {
        visPCB = new G4VisAttributes();
        visPCB->SetColour(0, 1, 0, 0.65);
        logicPCB->SetVisAttributes(visPCB);
    }

    if(isLightGuide)
    {
        visLightGuide = new G4VisAttributes();
        visLightGuide->SetColour(1, 1, 1, 0.5);
        logicLightGuide->SetVisAttributes(visLightGuide);
    }

    visPackage = new G4VisAttributes();
    visPackage->SetColour(1, 1, 0, 0.7);
    logicFrontPackageSiPM->SetVisAttributes(visPackage);
    logicBackPackageSiPM->SetVisAttributes(visPackage);

    visWindow = new G4VisAttributes();
    visWindow->SetColour(1, 1, 1, 0.7);
    logicFrontWindowSiPM->SetVisAttributes(visWindow);
    logicBackWindowSiPM->SetVisAttributes(visWindow);

    visDetector = new G4VisAttributes();
    visDetector->SetColour(0.45, 0.25, 0, 0.7);
    //visDetector->SetColour(0.5, 0.5, 0, 0.7);
    logicBackDetector->SetVisAttributes(visDetector);
    logicFrontDetector->SetVisAttributes(visDetector);

    visScintillator = new G4VisAttributes();
    visScintillator->SetColour(0, 0, 1, 0.98);
    //visScintillator->SetColour(1, 1, 0, 0.8);
    logicScintillator->SetVisAttributes(visScintillator);

    visCoating = new G4VisAttributes();
    //visCoating->SetColour(0, 0, 0, 0);
    visCoating->SetVisibility(false);
    logicCoating->SetVisAttributes(visCoating);
}