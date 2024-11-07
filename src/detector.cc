/**
 * @file detector.cc
 * @brief Definition of the class @ref MySensitiveDetector
 */
#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name, G4String hitsCollectionName) : G4VSensitiveDetector(name)
{
    fEfficiencySetting = fIsNominalEfficiency;

    // Add the collection name to collectionName vector
    collectionName.insert(hitsCollectionName);

    switch(fEfficiencySetting)
    {
        case fIsNominalEfficiency:
        default:
            fPDE = new G4PhysicsFreeVector(GS::pdeEnergies, GS::pdeValues);
            break;
        case fIsFixedEfficiency:
            fFixedEfficiency = GS::meanPDE;
            break;
        case fIsRandomEfficiency:
            RandomizeEfficiencies();
            break;
        case fIsAssignedEfficiency:
            GetEfficienciesFromFile();
            break;
    }
}



void MySensitiveDetector::Initialize(G4HCofThisEvent *hce)
{
    // Add fHitsCollection to the GHCofThisEvent container
    fHitsCollection = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    hce->AddHitsCollection(hcID, fHitsCollection);
}



G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // Access the useful objects
    G4Track *track = aStep->GetTrack();
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    const G4VTouchable *touchable = preStepPoint->GetTouchable();
    G4double phEnergy = preStepPoint->GetTotalEnergy();

    // Here's implemented the PDE
    switch(fEfficiencySetting)
    {
        case fIsNominalEfficiency:
            if(G4UniformRand() > fPDE->Value(phEnergy)) return false;
            break;
        case fIsFixedEfficiency:
            if(G4UniformRand() > fFixedEfficiency) return false;
            break;
        case fIsRandomEfficiency:
        case fIsAssignedEfficiency:
            G4int ch = touchable->GetCopyNumber(2);
            G4bool isFront = (touchable->GetVolume(2)->GetTranslation().z() < GS::zScintillator);
            if(isFront)
            {
                if(G4UniformRand() > fFrontEfficiency[ch]) return false;
            }
            else
            {
                if(G4UniformRand() > fBackEfficiency[ch]) return false;
            }
            break;
    }

    // Save, stop and kill only optical photons
    if(track->GetParticleDefinition()->GetPDGEncoding()==-22)
        track->SetTrackStatus(fStopAndKill);
    else
        return false;
    
    
    // Create a new MyHit object
    MyHit *newHit = new MyHit();
    
    // Time of detection
    newHit->SetDetectionTime(preStepPoint->GetGlobalTime());
    
    // Position of detector (Note that the position of the package is taken)
    newHit->SetDetectorPosition(touchable->GetVolume(2)->GetTranslation());

    // Channel of detector
    newHit->SetDetectorChannel(touchable->GetCopyNumber(2));


    // Insert the hit
    fHitsCollection->insert(newHit);
    
    return true;
}



void MySensitiveDetector::RandomizeEfficiencies()
{
    G4cout << "\n Randomization of efficiencies... \n" << G4endl;

    // Create the efficiencies file
    std::ofstream file("random_efficiencies.txt");

    // Check if file is opened
    if(!file.is_open())
    {
        G4cerr << "Can't open the file!" << G4endl;
        return;
    }

    file << "# Channel    Eff Front    Eff Back" << G4endl;

    // Generate and save efficiencies
    for(G4int ch = 0; ch < GS::nOfSiPMs; ch++)
    {
        fFrontEfficiency[ch] = (fPDE->GetMaxValue()/2)*(1 + G4UniformRand());
        fBackEfficiency[ch] = (fPDE->GetMaxValue()/2)*(1 + G4UniformRand());

        file << ch << "\t" << fFrontEfficiency[ch] << "\t" << fBackEfficiency[ch] << G4endl;
    }

    // Close the file
    file.close();
}



void MySensitiveDetector::GetEfficienciesFromFile()
{
    // Open the file
    std::ifstream file("random_efficiencies.txt");

    // Check if file is opened
    if(!file.is_open())
    {
        G4cerr << "Can't open the file!" << G4endl;
        return;
    }

    G4String line;
    G4int ch = 0;

    // Read data
    while(std::getline(file, line))
    {
        // Ignore '#' lines
        if(line[0] == '#')
            continue;

        std::istringstream iss(line);
        G4double effFront, effBack;

        // Parsing of the line
        if(!(iss >> ch >> effFront >> effBack))
        {
            G4cerr << "Error in reading the file!" << G4endl;
            break;
        }

        // Assign values
        fFrontEfficiency[ch] = effFront;
        fBackEfficiency[ch] = effBack;
    }

    // Close the file
    file.close();

    G4cout << "\n Data read from random_efficiencies.txt successfully. \n" << G4endl;
}