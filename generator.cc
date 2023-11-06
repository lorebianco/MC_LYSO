#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    //Define my UD-messenger
    fMessenger_Run = new G4GenericMessenger(this, "/my_gun/", "Cinematical settings for initial gamma");
    fMessenger_Run->DeclareProperty("meanEnergy", meanEnergy, "Mean of the gaussian distribution of initial gamma energy");
    fMessenger_Run->DeclareProperty("sigmaEnergy", sigmaEnergy, "Sigma of the gaussian distribution of initial gamma energy");

    fMessenger_Calib = new G4GenericMessenger(this, "/my_gun/calibration/", "Settings for calibration");
    fMessenger_Calib->DeclareProperty("enable", isCalibrationEnabled, "If true activate the calibration system; then must decide front or back and wich LED switch ON");
    fMessenger_Calib->DeclareProperty("FrontOrBack", chooseFrontorBack, "Choose side of detector you want to calibrate: F(ront) or B(ack)");
    fMessenger_Calib->DeclareProperty("switchOnLED", switchOnLED, "Choose which LED turn ON: u(p), d(own), r(ight), l(eft)");
    //*********************************//
    
    isCalibrationEnabled = false;
    chooseFrontorBack = "F";
    switchOnLED = "u";

    meanEnergy = 1.*MeV;
    sigmaEnergy = 0.01*MeV;

    fParticleTable = G4ParticleTable::GetParticleTable();

    fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ThreeVector pos;
    G4ThreeVector mom;

    if(!isCalibrationEnabled)
    {
        pos = G4ThreeVector(0., 0., 0.);
        mom = G4ThreeVector(0., 0., 1.);

        G4double energy = G4RandGauss::shoot(meanEnergy, sigmaEnergy);

        G4ParticleDefinition* particle = fParticleTable->FindParticle("gamma");
        fParticleGun->SetParticleDefinition(particle);   
        fParticleGun->SetParticlePosition(pos);
        fParticleGun->SetParticleMomentumDirection(mom);
        fParticleGun->SetParticleEnergy(energy);
    }

    if(isCalibrationEnabled)
    {
        G4double posZLed;

        if(chooseFrontorBack=="F") posZLed = GS.zFrontFaceScintillator-GS.halfheightLightGuide; 
        if(chooseFrontorBack=="B") posZLed = GS.zBackFaceScintillator+GS.halfheightLightGuide; 

        if(switchOnLED=="u")
        {
            pos = G4ThreeVector(0, GS.radiusLightGuide-2.5*mm, posZLed);
            G4double cosTheta = 2*G4UniformRand() - 1.;
            G4double phi = CLHEP::twopi*G4UniformRand();
            G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
            G4double ux = sinTheta*std::cos(phi);
            G4double uy = sinTheta*std::sin(phi);
            G4double uz = cosTheta;
            
            mom = G4ThreeVector(ux,uy,uz);
        }
        if(switchOnLED=="d")
        {
            pos = G4ThreeVector(0, -GS.radiusLightGuide+2.5*mm, posZLed);
            G4double cosTheta = 2*G4UniformRand() - 1.;
            G4double phi = CLHEP::twopi*G4UniformRand();
            G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
            G4double ux = sinTheta*std::cos(phi);
            G4double uy = sinTheta*std::sin(phi);
            G4double uz = cosTheta;
            
            mom = G4ThreeVector(ux,uy,uz);
        }
        if(switchOnLED=="l")
        {
            pos = G4ThreeVector(GS.radiusLightGuide-2.5*mm, 0, posZLed);
            G4double cosTheta = 2*G4UniformRand() - 1.;
            G4double phi = CLHEP::twopi*G4UniformRand();
            G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
            G4double ux = sinTheta*std::cos(phi);
            G4double uy = sinTheta*std::sin(phi);
            G4double uz = cosTheta;
            
            mom = G4ThreeVector(ux,uy,uz);
        }
        if(switchOnLED=="r")
        {
            pos = G4ThreeVector(-GS.radiusLightGuide+2.5*mm, 0, posZLed);
            G4double cosTheta = 2*G4UniformRand() - 1.;
            G4double phi = CLHEP::twopi*G4UniformRand();
            G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
            G4double ux = sinTheta*std::cos(phi);
            G4double uy = sinTheta*std::sin(phi);
            G4double uz = cosTheta;
            
            mom = G4ThreeVector(ux,uy,uz);
        }

        G4ParticleDefinition* particle = fParticleTable->FindParticle("opticalphoton");
        fParticleGun->SetParticleDefinition(particle);
        fParticleGun->SetParticlePosition(pos);
        fParticleGun->SetParticleMomentumDirection(mom);        fParticleGun->SetParticleEnergy(2.77*eV);
    }
    


    G4int evt = anEvent->GetEventID();
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(3, 0, evt);
    man->FillNtupleDColumn(3, 1, fParticleGun->GetParticleEnergy());
    man->FillNtupleDColumn(3, 2, fParticleGun->GetParticlePosition().x());
    man->FillNtupleDColumn(3, 3, fParticleGun->GetParticlePosition().y());
    man->FillNtupleDColumn(3, 4, fParticleGun->GetParticlePosition().z());
    man->FillNtupleDColumn(3, 5, fParticleGun->GetParticleMomentumDirection().x());
    man->FillNtupleDColumn(3, 6, fParticleGun->GetParticleMomentumDirection().y());
    man->FillNtupleDColumn(3, 7, fParticleGun->GetParticleMomentumDirection().z());
    man->AddNtupleRow(3);
    
    
    fParticleGun->GeneratePrimaryVertex(anEvent);
}