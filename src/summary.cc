/**
 * @file summary.cc
 * @brief Definition of the function @ref MC_summary() (and the auxiliary
 * function @ref extract_value())
 */
#include "summary.hh"

G4bool boolSpread = false;
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
    outfile << "Duration of the simulation: " << duration << G4endl;
    outfile << G4endl;

    // Open the run macro file in read mode
    std::ifstream run_file(macrofile);
    if(!run_file)
    {
        G4cerr << "Can't open file 'run.mac'" << G4endl;
        return;
    }

    G4String line;

    // Scan the run macro file
    while(std::getline(run_file, line))
    {
        // Trim leading whitespaces and tabs from each line
        line = line.substr(line.find_first_not_of(" \t"));

        // Find all the settings and write them on the output file
        if(line.find("/MC_LYSO/my_gun/meanEnergy") != G4String::npos)
        {
            G4String energy_value = extract_value(line, "/MC_LYSO/my_gun/meanEnergy");
            if(!energy_value.empty())
            {
                outfile << "Energy: " << energy_value << G4endl;
            }
        }
        else if(line.find("/MC_LYSO/my_gun/sigmaEnergy") != G4String::npos)
        {
            G4String sigma_value = extract_value(line, "/MC_LYSO/my_gun/sigmaEnergy");
            if(!sigma_value.empty())
            {
                outfile << "SigmaEnergy: " << sigma_value << G4endl;
            }
        }
        else if(line.find("/MC_LYSO/my_gun/enableSpread true") != G4String::npos)
        {
            outfile << "Spread: ON" << G4endl;
            boolSpread = true;
        }
        else if(line.find("/MC_LYSO/my_gun/enableSpread false") != G4String::npos)
        {
            outfile << "Spread: OFF" << G4endl;
            boolSpread = false;
        }
        else if(boolSpread && line.find("/MC_LYSO/my_gun/radiusSpread") != G4String::npos)
        {
            G4String radius_value = extract_value(line, "/MC_LYSO/my_gun/radiusSpread");
            if(!radius_value.empty())
            {
                outfile << "RadiusSpread: " << radius_value << G4endl;
            }
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

        if(line.find("/MC_LYSO/my_construction/isLightGuide true") != G4String::npos)
        {
            outfile << "Light Guide: ON" << G4endl;
            boolLightGuide = true;
        }
        else if(line.find("/MC_LYSO/my_construction/isLightGuide false") != G4String::npos)
        {
            outfile << "Light Guide: OFF" << G4endl;
            boolLightGuide = false;
        }
        else if(boolLightGuide && line.find("/MC_LYSO/my_construction/MaterialOfLightGuide") != G4String::npos)
        {
            G4String material_value = extract_value(line, "/MC_LYSO/my_construction/MaterialOfLightGuide");
            if(!material_value.empty())
            {
                outfile << "Material of Light Guide: " << material_value << G4endl;
            }
        }
        else if(line.find("/MC_LYSO/my_construction/isPCB true") != G4String::npos)
        {
            outfile << "PCB: ON" << G4endl;
        }
        else if(line.find("/MC_LYSO/my_construction/isPCB false") != G4String::npos)
        {
            outfile << "PCB: OFF" << G4endl;
        }
        else if(line.find("/MC_LYSO/my_construction/isEndcap true") != G4String::npos)
        {
            outfile << "Endcap: ON" << G4endl;
        }
        else if(line.find("/MC_LYSO/my_construction/isEndcap false") != G4String::npos)
        {
            outfile << "Endcap: OFF" << G4endl;
        }
    }

    construction_file.close();

    outfile << G4endl;
    outfile << "########################################################" << G4endl;
    outfile << G4endl;
}