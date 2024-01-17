#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    DefineCommands();

    // Default values 
    fIsSpreadEnabled = false;
    fRadiusSpread = 10*mm;

    fIsCalibrationEnabled = false;
    fChooseFrontorBack = "F";
    fSwitchOnLED = "u";

    fMeanEnergy = 1.*MeV;
    fSigmaEnergy = 0.01*MeV;

    // Construct the Particle Gun
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

    if(!fIsCalibrationEnabled)
    {
        pos = G4ThreeVector(0., 0., 0.);
        
        if(!fIsSpreadEnabled)
        {
            mom = G4ThreeVector(0., 0., 1.);
        }
        
        if(fIsSpreadEnabled)
        {
            G4double fThetaMax = std::atan(fRadiusSpread/GS::zFrontFaceScintillator);
            G4double fCosThetaMax = std::cos(fThetaMax);

            G4double cosTheta = 1-G4UniformRand()*(1-fCosThetaMax);
            G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
            G4double phi = CLHEP::twopi*G4UniformRand();

            G4double ux = sinTheta*std::cos(phi);
            G4double uy = sinTheta*std::sin(phi);
            G4double uz = cosTheta;

            mom = G4ThreeVector(ux, uy, uz);
        }
        
        G4double energy = G4RandGauss::shoot(fMeanEnergy, fSigmaEnergy);

        G4ParticleDefinition* particle = fParticleTable->FindParticle("gamma");
        fParticleGun->SetParticleDefinition(particle);   
        fParticleGun->SetParticlePosition(pos);
        fParticleGun->SetParticleMomentumDirection(mom);
        fParticleGun->SetParticleEnergy(energy);
    }

    if(fIsCalibrationEnabled)
    {
        G4double posZLed;
        
        if(fChooseFrontorBack=="F") posZLed = GS::zFrontFaceScintillator-GS::halfheightLightGuide; 
        if(fChooseFrontorBack=="B") posZLed = GS::zBackFaceScintillator+GS::halfheightLightGuide; 

        if(fSwitchOnLED=="u") pos = G4ThreeVector(0, GS::radiusLightGuide-GS::depthLED, posZLed);
        if(fSwitchOnLED=="d") pos = G4ThreeVector(0, -GS::radiusLightGuide+GS::depthLED, posZLed);
        if(fSwitchOnLED=="l") pos = G4ThreeVector(GS::radiusLightGuide-GS::depthLED, 0, posZLed);
        if(fSwitchOnLED=="r") pos = G4ThreeVector(-GS::radiusLightGuide+GS::depthLED, 0, posZLed);

        G4double cosTheta = 2*G4UniformRand() - 1.;
        G4double phi = CLHEP::twopi*G4UniformRand();
        G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
        G4double ux = sinTheta*std::cos(phi);
        G4double uy = sinTheta*std::sin(phi);
        G4double uz = cosTheta;
        mom = G4ThreeVector(ux,uy,uz);

        G4ParticleDefinition* particle = fParticleTable->FindParticle("opticalphoton");
        fParticleGun->SetParticleDefinition(particle);
        fParticleGun->SetParticlePosition(pos);
        fParticleGun->SetParticleMomentumDirection(mom);        fParticleGun->SetParticleEnergy(GS::energyLED);
    }
    
    fParticleGun->GeneratePrimaryVertex(anEvent);
}



void MyPrimaryGenerator::DefineCommands()
{
    // Define my UD-messenger for primary gamma
    fMessenger_Gun = new G4GenericMessenger(this, "/my_gun/", "Cinematical settings for initial gamma");
    fMessenger_Gun->DeclarePropertyWithUnit("meanEnergy", "MeV", fMeanEnergy, "Mean of the gaussian distribution of initial gamma energy");
    fMessenger_Gun->DeclarePropertyWithUnit("sigmaEnergy", "MeV", fSigmaEnergy, "Sigma of the gaussian distribution of initial gamma energy");

    fMessenger_Gun->DeclareProperty("enableSpread", fIsSpreadEnabled, "If true spread run is enabled");
    fMessenger_Gun->DeclarePropertyWithUnit("radiusSpread", "mm", fRadiusSpread, "Set radius of the spread on the front face of scintillator");

    // Define my UD-messenger for calibration run
    fMessenger_Calib = new G4GenericMessenger(this, "/my_gun/calibration/", "Settings for calibration");
    fMessenger_Calib->DeclareProperty("enable", fIsCalibrationEnabled, "If true activate the calibration system; then must decide front or back and wich LED switch ON");
    fMessenger_Calib->DeclareProperty("FrontOrBack", fChooseFrontorBack, "Choose side of detector you want to calibrate: F(ront) or B(ack)");
    fMessenger_Calib->DeclareProperty("switchOnLED", fSwitchOnLED, "Choose which LED turn ON: u(p), d(own), r(ight), l(eft)");
}
