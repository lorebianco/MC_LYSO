/**
 * @file generator.cc
 * @brief Definition of the class @ref MyPrimaryGenerator
 */
#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    DefineCommands();

    // Default values     
    fModeType = 0;
    fBeamType = 0;

    fMeanEnergy = 1.*MeV;
    fSigmaEnergy = 0.01*MeV;

    // Spread and Circle beam
    fRadiusSpread = 10*mm;
    fRadiusCircle = 10*mm;

    // LED mode
    fChooseFrontorBack = "F";
    fSwitchOnLED = "u";


    // Construct the Particle Gun
    fParticleGun = new G4ParticleGun(1);
    
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}



MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fMessenger_Mode, fMessenger_Gun, fMessenger_Calib;
    delete fParticleGun;
}



void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ParticleDefinition* particle = nullptr;
    G4double energy;

    switch(fModeType)
    {
        case 0:  // Standard mode
        default:
            if(fModeType != 0) G4cerr << "Not valid mode! Standard mode is selected!" << G4endl;
            // Set gamma as primary particle
            particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
            fParticleGun->SetParticleDefinition(particle);

            // Sample the energy from Gaus
            energy = G4RandGauss::shoot(fMeanEnergy, fSigmaEnergy);
            fParticleGun->SetParticleEnergy(energy);
            
            switch(fBeamType)
            {
                case 0:  // Pointlike beam
                default:
                    if(fBeamType != 0) G4cerr << "Not valid beam type! Pointlike beam is selected!" << G4endl;
                    break;

                case 1:  // Spread beam
                    PrimariesForSpreadBeam();
                    break;
                
                case 2:  // Circle beam
                    PrimariesForCircleBeam();
                    break;
            }
            break;

        case 1:  // LED-System mode
            PrimariesForLEDMode();
            break;

        case 2:  // Lu decay mode
            PrimariesForLuDecayMode();
            break;

    }

    // Finally generate the primary vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}



void MyPrimaryGenerator::PrimariesForSpreadBeam()
{
    // Set position at the center
    G4ThreeVector pos(0., 0., 0.);

    // Sample the momentum direction uniformly within the given range
    G4double fThetaMax = std::atan(fRadiusSpread/GS::zFrontFaceScintillator);
    G4double fCosThetaMax = std::cos(fThetaMax);

    G4double cosTheta = 1-G4UniformRand()*(1-fCosThetaMax);
    G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
    G4double phi = CLHEP::twopi*G4UniformRand();

    G4double ux = sinTheta*std::cos(phi);
    G4double uy = sinTheta*std::sin(phi);
    G4double uz = cosTheta;

    G4ThreeVector mom(ux, uy, uz);

    // Set fParticleGun
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
}



void MyPrimaryGenerator::PrimariesForCircleBeam()
{
    G4double posR = fRadiusCircle*std::sqrt(G4UniformRand());
    G4double posPhi = CLHEP::twopi*G4UniformRand();

    G4ThreeVector pos(posR*std::cos(posPhi), posR*std::sin(posPhi), 0);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}



void MyPrimaryGenerator::PrimariesForLEDMode()
{
    G4ThreeVector posLed;
    G4ThreeVector momLed;

    // Which face
    G4double posZLed;
    if(fChooseFrontorBack=="F") posZLed = GS::zFrontFaceScintillator-GS::halfheightLightGuide; 
    else if(fChooseFrontorBack=="B") posZLed = GS::zBackFaceScintillator+GS::halfheightLightGuide;
    else
    {
        G4cerr << "Option for detector face not valid. 'Front' has been setted" << G4endl;
        posZLed = GS::zFrontFaceScintillator-GS::halfheightLightGuide;
    }

    // Which LED
    if(fSwitchOnLED=="u") posLed = G4ThreeVector(0, GS::radiusLightGuide-GS::depthLED, posZLed);
    else if(fSwitchOnLED=="d") posLed = G4ThreeVector(0, -GS::radiusLightGuide+GS::depthLED, posZLed);
    else if(fSwitchOnLED=="l") posLed = G4ThreeVector(GS::radiusLightGuide-GS::depthLED, 0, posZLed);
    else if(fSwitchOnLED=="r") posLed = G4ThreeVector(-GS::radiusLightGuide+GS::depthLED, 0, posZLed);
    else
    {
        G4cerr << "Option for LED not valid. 'up' has been setted" << G4endl;
        posLed = G4ThreeVector(0, GS::radiusLightGuide-GS::depthLED, posZLed);
    }

    // Isotropic emission (not accurate, should be Lambert's cosine law,
    // but need G4GeneralParticleSource for this)
    G4double cosTheta = 2*G4UniformRand() - 1.;
    G4double phi = CLHEP::twopi*G4UniformRand();
    G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
    G4double ux = sinTheta*std::cos(phi);
    G4double uy = sinTheta*std::sin(phi);
    G4double uz = cosTheta;
    momLed = G4ThreeVector(ux,uy,uz);

    // Set everything in fParticleGun
    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("opticalphoton");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticlePosition(posLed);
    fParticleGun->SetParticleMomentumDirection(momLed);        fParticleGun->SetParticleEnergy(GS::energyLED);
}



void MyPrimaryGenerator::PrimariesForLuDecayMode()
{
    G4ThreeVector posDecay;
    G4ThreeVector momDecay;

    // Compute random position inside crystal
    G4double posZ = GS::zScintillator + 2*(G4UniformRand()-0.5)*GS::halfheightScintillator;
    G4double posR = GS::radiusScintillator*std::sqrt(G4UniformRand());
    G4double posPhi = CLHEP::twopi*G4UniformRand();

    posDecay = G4ThreeVector(posR*std::cos(posPhi), posR*std::sin(posPhi), posZ);

    // Isotropic emission
    G4double cosTheta = 2*G4UniformRand() - 1.;
    G4double phi = CLHEP::twopi*G4UniformRand();
    G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
    G4double ux = sinTheta*std::cos(phi);
    G4double uy = sinTheta*std::sin(phi);
    G4double uz = cosTheta;
    
    momDecay = G4ThreeVector(ux,uy,uz);

    // Set everything in fParticleGun
    G4int Z = 71, A = 176;
    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z, A, 0.*keV);
    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleCharge(0.*eplus);
    fParticleGun->SetParticlePosition(posDecay);
    fParticleGun->SetParticleMomentumDirection(momDecay);        fParticleGun->SetParticleEnergy(1*eV);
}



void MyPrimaryGenerator::DefineCommands()
{
    // Define my UD-messenger for mode selection
    fMessenger_Mode = new G4GenericMessenger(this, "/MC_LYSO/", "Commands for MC_LYSO run");
    fMessenger_Mode->DeclareProperty("Mode", fModeType, "Available modes are: 0 = Standard, 1 = LED system, 2 = Lu decay");

    // Define my UD-messenger for primary gamma
    fMessenger_Gun = new G4GenericMessenger(this, "/MC_LYSO/myGun/", "Cinematical settings for initial gamma");
    fMessenger_Gun->DeclareProperty("BeamType", fBeamType, "Available types are: 0 = Pointlike, 1 = Spread, 2 = Circle");
    fMessenger_Gun->DeclarePropertyWithUnit("meanEnergy", "MeV", fMeanEnergy, "Mean of the gaussian distribution of initial gamma energy");
    fMessenger_Gun->DeclarePropertyWithUnit("sigmaEnergy", "MeV", fSigmaEnergy, "Sigma of the gaussian distribution of initial gamma energy");
    fMessenger_Gun->DeclarePropertyWithUnit("radiusSpread", "mm", fRadiusSpread, "Set radius of the spread on the front face of scintillator");
    fMessenger_Gun->DeclarePropertyWithUnit("radiusCircle", "mm", fRadiusCircle, "Set radius of the beam (in 'Circle' case)");

    // Define my UD-messenger for calibration run
    fMessenger_Calib = new G4GenericMessenger(this, "/MC_LYSO/myGun/calibration/", "Settings for LED-system calibration");
    fMessenger_Calib->DeclareProperty("FrontOrBack", fChooseFrontorBack, "Choose side of detector you want to calibrate: F(ront) or B(ack)");
    fMessenger_Calib->DeclareProperty("switchOnLED", fSwitchOnLED, "Choose which LED turn ON: u(p), d(own), r(ight), l(eft)");
}
