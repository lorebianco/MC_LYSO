/**
 * @file generator.cc
 * @brief Definition of the class @ref MyPrimaryGenerator
 */
#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    DefineCommands();

    // Default values     
    fModeType = 10;

    fMeanEnergy = 1.*MeV;
    fSigmaEnergy = 0.01*MeV;

    // Spread and Circle beam
    fRadiusSpread = 10*mm;
    fRadiusCircle = 10*mm;

    // 176Lu decay
    fPosFixedDecay = G4ThreeVector(0., 0., 200.*mm);

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
    delete fMessenger_Mode;
    delete fMessenger_Gun;
    delete fMessenger_Calib;
    delete fParticleGun;
}



void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    switch(fModeType)
    {
        // Standard mode
        case 10:  // Pointlike beam
        case 11:  // Spread beam
        case 12:  // Circle beam
            PrimariesForStandardMode();
            break;

        // Lu decay mode
        case 20:
        case 21:  // Fixed position
        case 22:  // Si trigger
            PrimariesForLuDecayMode();
            break;

        // Cosmic rays mode
        case 30:
            PrimariesForCosmicRaysMode();
            break;

        // LED-System mode
        case 40:
            PrimariesForLEDMode();
            break;

        default:
            G4cerr << "Not valid mode! Standard mode is selected!" << G4endl;
            fModeType = 10;
            PrimariesForStandardMode();
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



void MyPrimaryGenerator::PrimariesForStandardMode()
{
    // Set gamma as primary particle
    G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    fParticleGun->SetParticleDefinition(particle);

    // Sample the energy from Gaus
    G4double energy = G4RandGauss::shoot(fMeanEnergy, fSigmaEnergy);
    fParticleGun->SetParticleEnergy(energy);

    switch(fModeType)
    {
        case 10:  // Pointlike beam
            break;
        case 11:  // Spread beam
            PrimariesForSpreadBeam();
            break;    
        case 12:  // Circle beam
            PrimariesForCircleBeam();
            break;
    }
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
    G4double posZ, posR, posPhi;
    
    switch(fModeType)
    {
        case 20:  // Default
            // Compute random position inside crystal
            posZ = GS::zScintillator + 2*(G4UniformRand()-0.5)*GS::halfheightScintillator;
            posR = GS::radiusScintillator*std::sqrt(G4UniformRand());
            posPhi = CLHEP::twopi*G4UniformRand();    
            posDecay = G4ThreeVector(posR*std::cos(posPhi), posR*std::sin(posPhi), posZ);
            break;
        case 21:  // Fixed position
            posDecay = fPosFixedDecay;
            break;
        case 22:  // Si trigger test
            //Compute random position near ch57-Front
                // Very situational mode, it's fine to be hard-coded
            posDecay = G4ThreeVector(0.*mm, 0.*mm, GS::zFrontFaceScintillator+G4UniformRand()*25.*mm);
            //posDecay = G4ThreeVector(2*(G4UniformRand()-0.5)*4*mm, 2*(G4UniformRand()-0.5)*4*mm, (150.2+2*(G4UniformRand()-0.5)*0.2)*mm);
            break;
    }

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
    fParticleGun->SetParticleMomentumDirection(momDecay);
    fParticleGun->SetParticleEnergy(1*eV);
}



void MyPrimaryGenerator::PrimariesForCosmicRaysMode()
{
    G4ThreeVector posRay;
    G4ThreeVector momRay;

    // Compute initial random position in the up detector
    G4double posX = GS::xCosmicRayDetector + 2*(G4UniformRand()-0.5)*GS::halfZXsideCosmicRayDetector;
    G4double posY = GS::yCosmicRayDetector + GS::halfYsideCosmicRayDetector;
    G4double posZ = GS::zCosmicRayDetector + 2*(G4UniformRand()-0.5)*GS::halfZXsideCosmicRayDetector;

    posRay = G4ThreeVector(posX, posY, posZ);
    
    // Compute initial cosmic ray direction
    G4double cosTheta = -G4UniformRand(); // Note that cosine is in [-1, 0] now
    G4double cosThetaa = G4UniformRand(); // On the image of cos^2(Theta)
    G4double phi = CLHEP::twopi*G4UniformRand();
    G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
    G4double uz = sinTheta*std::cos(phi); // Remember the axis orientation
    G4double ux = sinTheta*std::sin(phi);
    G4double uy = cosTheta; // Y is vertical

    momRay = G4ThreeVector(ux, uy, uz);

    while(std::abs(ProjectOnBottomDetector(posRay, momRay).x() - GS::xCosmicRayDetector) > GS::halfZXsideCosmicRayDetector || std::abs(ProjectOnBottomDetector(posRay, momRay).z() - GS::zCosmicRayDetector) > GS::halfZXsideCosmicRayDetector) // Conditions on trigger
    {
        posX = GS::xCosmicRayDetector + 2*(G4UniformRand()-0.5)*GS::halfZXsideCosmicRayDetector;
        posZ = GS::zCosmicRayDetector + 2*(G4UniformRand()-0.5)*GS::halfZXsideCosmicRayDetector;

        posRay = G4ThreeVector(posX, posY, posZ);

        cosTheta = -G4UniformRand(); // Note that cosine is in [-1, 0] now
        cosThetaa = G4UniformRand(); // On the image of cos^2(Theta)
        phi = CLHEP::twopi*G4UniformRand();
        while(cosThetaa > (cosTheta*cosTheta)) // Mom-dir generation
        {
            cosTheta = -G4UniformRand();
            cosThetaa = G4UniformRand();
            
            phi = CLHEP::twopi*G4UniformRand();
        }
        sinTheta = std::sqrt(1. - cosTheta*cosTheta);
        uz = sinTheta*std::cos(phi);
        ux = sinTheta*std::sin(phi);
        uy = cosTheta;
    
        momRay = G4ThreeVector(ux, uy, uz);
    }
    
    
    // Cosmic ray energy
    G4double energy = 999.;
    G4double energyy = 1.; // On the image of pdf(E)
    G4double emin = 0.; // kinetic energy
    G4double emax = 1.*TeV;

    while(energyy > PDF_E_CosmicRay(energy))
    {   // Accept-Reject method
        energy = emin + emax*G4UniformRand();
            // Note that the maximum probability is at minimum energy
        energyy = PDF_E_CosmicRay(emin)*G4UniformRand();
    }

    // Set everything in fParticleGun
    G4ParticleDefinition* particle = nullptr;
    if(G4UniformRand() < 0.5)
        particle = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    else
        particle = G4ParticleTable::GetParticleTable()->FindParticle("mu+");

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticlePosition(posRay);
    fParticleGun->SetParticleMomentumDirection(momRay);        fParticleGun->SetParticleEnergy(energy);
}



G4double MyPrimaryGenerator::PDF_E_CosmicRay(G4double energy)
{
    if(energy < 3.4*GeV)
    {
        return std::pow(3.4*GeV, -2.7);
    }
    else
    {
        return std::pow(energy, -2.7);
    }
}



G4ThreeVector MyPrimaryGenerator::ProjectOnBottomDetector(G4ThreeVector pos0, G4ThreeVector mom0)
{   
    G4double y_b = -(GS::yCosmicRayDetector + GS::halfYsideCosmicRayDetector);
    G4double x_b, z_b;
    G4double t = (y_b - pos0.y())/mom0.y();
    
    x_b = pos0.x() + mom0.x()*t;
    z_b = pos0.z() + mom0.z()*t;

    return G4ThreeVector(x_b, y_b, z_b);
};



void MyPrimaryGenerator::DefineCommands()
{
    // Define my UD-messenger for mode selection
    fMessenger_Mode = new G4GenericMessenger(this, "/MC_LYSO/", "Commands for MC_LYSO run");
    fMessenger_Mode->DeclareProperty("Mode", fModeType, "Available modes are: 10 = Standard with pointlike beam, 11 = Standard with spread beam, 12 = Standard with circle beam, 20 = Lu decay, 21 = Lu decay with fixed position, 22 = Lu decay with Si trigger test, 30 = Cosmic Rays, 40 = LED-system");

    // Define my UD-messenger for primary gamma
    fMessenger_Gun = new G4GenericMessenger(this, "/MC_LYSO/myGun/", "Cinematical settings for primary particle");
    fMessenger_Gun->DeclarePropertyWithUnit("meanEnergy", "MeV", fMeanEnergy, "Mean of the gaussian distribution of initial gamma energy");
    fMessenger_Gun->DeclarePropertyWithUnit("sigmaEnergy", "MeV", fSigmaEnergy, "Sigma of the gaussian distribution of initial gamma energy");
    fMessenger_Gun->DeclarePropertyWithUnit("radiusSpread", "mm", fRadiusSpread, "Set radius of the spread on the front face of scintillator");
    fMessenger_Gun->DeclarePropertyWithUnit("radiusCircle", "mm", fRadiusCircle, "Set radius of the beam (in 'Circle' case)");
    fMessenger_Gun->DeclarePropertyWithUnit("posLuDecay", "mm", fPosFixedDecay, "Set position for Lu decay in fixed position mode");

    // Define my UD-messenger for LED mode
    fMessenger_Calib = new G4GenericMessenger(this, "/MC_LYSO/myGun/LED-System/", "Settings for LED-system calibration");
    fMessenger_Calib->DeclareProperty("FrontOrBack", fChooseFrontorBack, "Choose side of detector you want to calibrate: F(ront) or B(ack)");
    fMessenger_Calib->DeclareProperty("switchOnLED", fSwitchOnLED, "Choose which LED turn ON: u(p), d(own), r(ight), l(eft)");
}
