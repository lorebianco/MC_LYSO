#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    DefineMaterials();
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
    //Entries for LYSO properties
    const int nEntries = 3;


    G4NistManager *nist = G4NistManager::Instance();

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


    detectorMat = nist->FindOrBuildMaterial("G4_Si");
    G4MaterialPropertiesTable *mptDetector = new G4MaterialPropertiesTable();
    G4double DETECTOR_RINDEX[nEntries] = {1.55, 1.55, 1.55};
    mptDetector->AddProperty("RINDEX", Energies, DETECTOR_RINDEX, nEntries);

    detectorMat->SetMaterialPropertiesTable(mptDetector);


    tapeSurface = new G4OpticalSurface("tapeSurface");
    tapeSurface->SetType(dielectric_metal);
    tapeSurface->SetFinish(polished);
    tapeSurface->SetModel(unified);
    
    G4MaterialPropertiesTable *mptTape = new G4MaterialPropertiesTable();
    G4double TAPE_REFLECTIVITY[nEntries] = {0., 0., 0.};
    
    mptTape->AddProperty("REFLECTIVITY", Energies, TAPE_REFLECTIVITY, nEntries);
    
    tapeSurface->SetMaterialPropertiesTable(mptTape);
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4double coating_space = 0.12*mm; //se = 0 il coating assorbitore da 2 micron è a contatto con il cristallo, come un tape; se > 0 serve solo nella grafica per uccidere i fotoni ottici

    G4double xWorld = 0.5*m;
    G4double yWorld = 0.5*m;
    G4double zWorld = 0.5*m;

    G4double xScintillator = 0*cm;
    G4double yScintillator = 0*cm;
    G4double zScintillator = 20*cm;


    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    logicWorld = new G4LogicalVolume(solidWorld, fAir, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    solidScintillator = new G4Tubs("solidScintillator", 0.*cm, 3.5*cm, 5.*cm, 0.*deg, 360.*deg);
    logicScintillator = new G4LogicalVolume(solidScintillator, fLYSO, "logicScintillator");
    physScintillator = new G4PVPlacement(0, G4ThreeVector(xScintillator, yScintillator, zScintillator), logicScintillator, "physScintillator", logicWorld, false, 0, true);

    fScoringVolume = logicScintillator;

    solidCoating = new G4Tubs("solidCoating", (3.5+coating_space)*cm, (3.52+coating_space)*cm, 5.*cm, 0.*deg, 360.*deg);
    logicCoating = new G4LogicalVolume(solidCoating, fAluminium, "logicCoating");
    physCoating = new G4PVPlacement(0, G4ThreeVector(0., 0., 20.*cm), logicCoating, "physCoating", logicWorld, false, 0, true);
    
    skin = new G4LogicalSkinSurface("skin", logicCoating, tapeSurface);

    solidBackDetector = new G4Box("solidBackDetector", 3.*mm, 3.*mm, 0.05*mm);
    logicBackDetector = new G4LogicalVolume(solidBackDetector, detectorMat, "logicBackDetector");

    solidFrontDetector = new G4Box("solidFrontDetector", 3.*mm, 3.*mm, 0.05*mm);
    logicFrontDetector = new G4LogicalVolume(solidFrontDetector, detectorMat, "logicFrontDetector");

    //Definisco la disposizione dei SiPM, andrà corretto
    G4int indexDetector = 0;
    for(G4int i = 0; i<13; i++)
    {
        if(i==0 || i==12)
        {
            for(G4int j = 0; j<3; j++)
            {
                physBackDetector = new G4PVPlacement(0,G4ThreeVector((6*(j-1))*mm, (6*(6-i))*mm, (250+0.05)*mm), logicBackDetector, "physBackDetector", logicWorld, false, indexDetector, true);
                
                physFrontDetector = new G4PVPlacement(0,G4ThreeVector((6*(j-1))*mm, (6*(6-i))*mm, (150-0.05)*mm), logicFrontDetector, "physFrontDetector", logicWorld, false, indexDetector, true);
                
                indexDetector++;
            }
        }
        if(i==1 || i==11)
        {
            for(G4int j = 0; j<7; j++)
            {
                physBackDetector = new G4PVPlacement(0,G4ThreeVector((6*(j-3))*mm, (6*(6-i))*mm, (250+0.05)*mm), logicBackDetector, "physBackDetector", logicWorld, false, indexDetector, true);

                physFrontDetector = new G4PVPlacement(0,G4ThreeVector((6*(j-3))*mm, (6*(6-i))*mm, (150-0.05)*mm), logicFrontDetector, "physFrontDetector", logicWorld, false, indexDetector, true);
                
                indexDetector++;
            }
        }
        if(i==2 || i==10)
        {
            for(G4int j = 0; j<9; j++)
            {
                physBackDetector = new G4PVPlacement(0,G4ThreeVector((6*(j-4))*mm, (6*(6-i))*mm, (250+0.05)*mm), logicBackDetector, "physBackDetector", logicWorld, false, indexDetector, true);

                physFrontDetector = new G4PVPlacement(0,G4ThreeVector((6*(j-4))*mm, (6*(6-i))*mm, (150-0.05)*mm), logicFrontDetector, "physFrontDetector", logicWorld, false, indexDetector, true);

                indexDetector++;
            }
        }
        if(i>=3 && i<=9)
        {
            for(G4int j = 0; j<11; j++)
            {
                physBackDetector = new G4PVPlacement(0,G4ThreeVector((6*(j-5))*mm, (6*(6-i))*mm, (250+0.05)*mm), logicBackDetector, "physBackDetector", logicWorld, false, indexDetector, true);

                physFrontDetector = new G4PVPlacement(0,G4ThreeVector((6*(j-5))*mm, (6*(6-i))*mm, (150-0.05)*mm), logicFrontDetector, "physFrontDetector", logicWorld, false, indexDetector, true);

                indexDetector++;
            }
        }
    }




    return physWorld;
}



void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensBackDet = new MySensitiveDetector("SensitiveBackDetector");
    logicBackDetector->SetSensitiveDetector(sensBackDet);

    MySensitiveDetector *sensFrontDet = new MySensitiveDetector("SensitiveFrontDetector");
    logicFrontDetector->SetSensitiveDetector(sensFrontDet);
}