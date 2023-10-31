#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    //Definisco il mio UD-messenger
    fMessenger = new G4GenericMessenger(this, "/my_gun/", "Cinematical settings for initial gamma");
    fMessenger->DeclareProperty("meanEnergy", meanEnergy, "Mean of the gaussian distribution of initial gamma energy");
    fMessenger->DeclareProperty("sigmaEnergy", sigmaEnergy, "Sigma of the gaussian distribution of initial gamma energy");
    //*********************************//

    fParticleGun = new G4ParticleGun(1);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma");
    fParticleGun->SetParticleDefinition(particle);

    G4ThreeVector pos(0., 0., 0.);
    G4ThreeVector mom(0., 0., 1.);
    
    meanEnergy = 1.*MeV;
    sigmaEnergy = 0.01*MeV;

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4double energy = G4RandGauss::shoot(meanEnergy, sigmaEnergy);

    fParticleGun->SetParticleEnergy(energy);


    
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