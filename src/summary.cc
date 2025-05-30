/**
 * @file summary.cc
 * @brief Definition of the function @ref MC_summary() (and the auxiliary
 * function @ref extract_value())
 */
#include "summary.hh"

G4int beamType = 0, modeType = 0;
G4bool boolLightGuide = false;



G4String extract_value(const G4String& line, const G4String& keyword)
{
    // Find the position of the keyword in the line
    size_t start = line.find(keyword);

    // If the keyword is found, extract the value following it
    if(start != G4String::npos)
    {
        start += keyword.length();
        G4String value = line.substr(start);
        return value;
    }

    // Return an empty string if the keyword is not found
    return "";
}



void MC_summary(G4String macrofile, G4int seed, G4double duration, const G4String& output_filename)
{
    // Open the output file in append mode
    std::ofstream outfile(output_filename, std::ios::app);

    // Print general information about the simulation (seed, date, username, duration)
    outfile << "MonteCarlo serial number (Seed): " << seed << G4endl;
    outfile << G4endl;
    std::time_t now = std::time(0);
    std::tm* current_time = std::localtime(&now);
    outfile << "Date: " << std::asctime(current_time);
    outfile << "User Name: " << getlogin() << G4endl;
    outfile << "Duration of the simulation: " << duration << " s" << G4endl;
    outfile << G4endl;

    // Open the run macro file in read mode
    std::ifstream run_file(macrofile);
    if(!run_file)
    {
        G4cerr << "Can't open run macro file!" << G4endl;
        return;
    }

    G4String line;

    // Scan the run macro file
    while(std::getline(run_file, line))
    {
        // Trim leading whitespaces and tabs from each line
        line = line.substr(line.find_first_not_of(" \t"));

        // Find all the settings and write them on the output file
        if(line.find("/MC_LYSO/Mode") != G4String::npos)
        {
            modeType = std::stoi(extract_value(line, "/MC_LYSO/Mode"));
            switch(modeType)
            {
                case 10:
                default:
                    outfile << "Mode: Standard - Pointlike beam" << G4endl;
                    break;
                case 11:
                    outfile << "Mode: Standard - Spread beam" << G4endl;
                    break;
                case 12:
                    outfile << "Mode: Standard - Circle beam" << G4endl;
                    break;
                case 20:
                    outfile << "Mode: 176Lu decay" << G4endl;
                    break;
                case 21:
                    outfile << "Mode: 176Lu decay - Fixed position" << G4endl;
                    break;
                case 22:
                    outfile << "Mode: 176Lu decay - Si Trigger" << G4endl;
                    break;
                case 30:
                    outfile << "Mode: Cosmic rays" << G4endl;
                    break;
                case 40:
                    outfile << "Mode: LED system" << G4endl;
                    break;
            }
        }
        else if(modeType == 11 && line.find("/MC_LYSO/myGun/radiusSpread") != G4String::npos)
        {
            G4String radius_value = extract_value(line, "/MC_LYSO/myGun/radiusSpread");
            if(!radius_value.empty())
            {
                outfile << "RadiusSpread: " << radius_value << G4endl;
            }
        }
        else if(modeType == 12 && line.find("/MC_LYSO/myGun/radiusCircle") != G4String::npos)
        {
            G4String radius_value = extract_value(line, "/MC_LYSO/myGun/radiusCircle");
            if(!radius_value.empty())
            {
                outfile << "RadiusCircle: " << radius_value << G4endl;
            }
        }
        else if((modeType == 10 || modeType == 11 || modeType == 12) && line.find("/MC_LYSO/myGun/meanEnergy") != G4String::npos)
        {
            G4String energy_value = extract_value(line, "/MC_LYSO/myGun/meanEnergy");
            if(!energy_value.empty())
            {
                outfile << "Energy: " << energy_value << G4endl;
            }
        }
        else if((modeType == 10 || modeType == 11 || modeType == 12) && line.find("/MC_LYSO/myGun/sigmaEnergy") != G4String::npos)
        {
            G4String sigma_value = extract_value(line, "/MC_LYSO/myGun/sigmaEnergy");
            if(!sigma_value.empty())
            {
                outfile << "SigmaEnergy: " << sigma_value << G4endl;
            }
        }
        else if(modeType == 21 && line.find("/MC_LYSO/myGun/posLuDecay") != G4String::npos)
        {
            G4String posdecay_value = extract_value(line, "/MC_LYSO/myGun/posLuDecay");
            if(!posdecay_value.empty())
            {
                outfile << "176Lu isotope position: " << posdecay_value << G4endl;
            }
        }
        else if(modeType == 40 && line.find("/MC_LYSO/myGun/LED-System/FrontOrBack F") != G4String::npos)
        {
            outfile << "LED of Front detector" << G4endl;
        }
        else if(modeType == 40 && line.find("/MC_LYSO/myGun/LED-System/FrontOrBack B") != G4String::npos)
        {
            outfile << "LED of Back detector" << G4endl;
        }
        else if(modeType == 40 && line.find("/MC_LYSO/myGun/LED-System/switchOnLED u") != G4String::npos)
        {
            outfile << "LED turned on: up" << G4endl;
        }
        else if(modeType == 40 && line.find("/MC_LYSO/myGun/LED-System/switchOnLED d") != G4String::npos)
        {
            outfile << "LED turned on: down" << G4endl;
        }
        else if(modeType == 40 && line.find("/MC_LYSO/myGun/LED-System/switchOnLED r") != G4String::npos)
        {
            outfile << "LED turned on: right" << G4endl;
        }
        else if(modeType == 40 && line.find("/MC_LYSO/myGun/LED-System/switchOnLED l") != G4String::npos)
        {
            outfile << "LED turned on: left" << G4endl;
        }
        else if(line.find("/run/beamOn") != G4String::npos)
        {
            G4String events_value = extract_value(line, "/run/beamOn");
            if(!events_value.empty())
            {
                outfile << "Number of events: " << events_value << G4endl;
            }
        }
    }

    run_file.close();

    // Open the "construction.mac" file in read mode
    std::ifstream construction_file("construction.mac");
    if(!construction_file)
    {
        G4cerr << "Can't open file 'construction.mac'" << G4endl;
        return;
    }
    
    // Scan the "construction.mac" file
    while(std::getline(construction_file, line))
    {
        line = line.substr(line.find_first_not_of(" \t"));

        if(line.find("/MC_LYSO/myConstruction/isOpticalGrease true") != G4String::npos)
        {
            outfile << "Optical Grease: ON" << G4endl;
        }
        else if(line.find("/MC_LYSO/myConstruction/isOpticalGrease false") != G4String::npos)
        {
            outfile << "Optical Grease: OFF" << G4endl;
        }
        else if(line.find("/MC_LYSO/myConstruction/isLightGuide true") != G4String::npos)
        {
            outfile << "Light Guide: ON" << G4endl;
            boolLightGuide = true;
        }
        else if(line.find("/MC_LYSO/myConstruction/isLightGuide false") != G4String::npos)
        {
            outfile << "Light Guide: OFF" << G4endl;
            boolLightGuide = false;
        }
        else if(boolLightGuide && line.find("/MC_LYSO/myConstruction/MaterialOfLightGuide") != G4String::npos)
        {
            G4String material_value = extract_value(line, "/MC_LYSO/myConstruction/MaterialOfLightGuide");
            if(!material_value.empty())
            {
                outfile << "Material of Light Guide: " << material_value << G4endl;
            }
        }
        else if(line.find("/MC_LYSO/myConstruction/isPCB true") != G4String::npos)
        {
            outfile << "PCB: ON" << G4endl;
        }
        else if(line.find("/MC_LYSO/myConstruction/isPCB false") != G4String::npos)
        {
            outfile << "PCB: OFF" << G4endl;
        }
        else if(line.find("/MC_LYSO/myConstruction/isEndcap true") != G4String::npos)
        {
            outfile << "Endcap: ON" << G4endl;
        }
        else if(line.find("/MC_LYSO/myConstruction/isEndcap false") != G4String::npos)
        {
            outfile << "Endcap: OFF" << G4endl;
        }
        else if(line.find("/MC_LYSO/myConstruction/isCosmicRaysDetectors true") != G4String::npos)
        {
            outfile << "Cosmic rays detectors: ON" << G4endl;
        }
        else if(line.find("/MC_LYSO/myConstruction/isCosmicRaysDetectors false") != G4String::npos)
        {
            outfile << "Cosmic rays detectors: OFF" << G4endl;
        }
    }

    construction_file.close();

    outfile << G4endl;
    outfile << "########################################################" << G4endl;
    outfile << G4endl;
}