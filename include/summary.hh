#ifndef SUMMARY_HH
#define SUMMARY_HH

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <unistd.h>

G4bool boolSpread = false;
G4bool boolLightGuide = false;

// Funzione per estrarre il valore a destra di una keyword
std::string extract_value(const std::string& line, const std::string& keyword) {
    size_t start = line.find(keyword);
    if (start != std::string::npos) {
        start += keyword.length();
        std::string value = line.substr(start);
        return value;
    }
    return "";
}

// Funzione per stampare il nome utente e il seed su un file
void printGlobals(int seed, double duration, std::ofstream& outfile) {
    outfile << "MonteCarlo serial number (Seed): " << seed << std::endl;
    outfile << std::endl;
    std::time_t now = std::time(0);
    std::tm* current_time = std::localtime(&now);
    outfile << "Date: " << std::asctime(current_time);
    outfile << "User Name: " << getlogin() << std::endl;
    outfile << "Duration of the simulation: " << duration << std::endl;
    outfile << std::endl;
}

void MC_summary(G4String macrofile, int seed, double duration, const std::string& output_filename) {

    // Apri il file di output in modalità scrittura
    std::ofstream outfile(output_filename, std::ios::app);

    // Chiama la funzione per stampare data, nome utente e il seed su un file
    printGlobals(seed, duration, outfile);

    // Apri il file "run.mac" in modalità lettura
    std::ifstream run_file(macrofile);
    if (!run_file) {
        std::cerr << "Impossibile aprire il file 'run.mac'" << std::endl;
        return;
    }

    std::string line;

    while (std::getline(run_file, line)) {
        line = line.substr(line.find_first_not_of(" \t"));

        if (line.find("/my_gun/spread/enable true") != std::string::npos) {
            outfile << "Spread: ON" << std::endl;
            boolSpread = true;
        } else if (line.find("/my_gun/spread/enable false") != std::string::npos) {
            outfile << "Spread: OFF" << std::endl;
            boolSpread = false;
        } else if (boolSpread && line.find("/my_gun/spread/radiusSpread") != std::string::npos) {
            std::string radius_value = extract_value(line, "/my_gun/spread/radiusSpread");
            if (!radius_value.empty()) {
                outfile << "RadiusSpread: " << radius_value << std::endl;
            }
        } else if (line.find("/my_gun/meanEnergy") != std::string::npos) {
            std::string energy_value = extract_value(line, "/my_gun/meanEnergy");
            if (!energy_value.empty()) {
                outfile << "Energy: " << energy_value << std::endl;
            }
        } else if (line.find("/my_gun/sigmaEnergy") != std::string::npos) {
            std::string sigma_value = extract_value(line, "/my_gun/sigmaEnergy");
            if (!sigma_value.empty()) {
                outfile << "SigmaEnergy: " << sigma_value << std::endl;
            }
        } else if (line.find("/run/beamOn") != std::string::npos) {
            std::string events_value = extract_value(line, "/run/beamOn");
            if (!events_value.empty()) {
                outfile << "Number of events: " << events_value << std::endl;
            }
        }
    }

    run_file.close();

    // Apri il file "construction.mac" in modalità lettura
    std::ifstream construction_file("construction.mac");
    if (!construction_file) {
        std::cerr << "Impossibile aprire il file 'construction.mac'" << std::endl;
        return;
    }

    while (std::getline(construction_file, line)) {
        line = line.substr(line.find_first_not_of(" \t"));

        if (line.find("/my_construction/isLightGuide true") != std::string::npos) {
            outfile << "Light Guide: ON" << std::endl;
            boolLightGuide = true;
        } else if (line.find("/my_construction/isLightGuide false") != std::string::npos) {
            outfile << "Light Guide: OFF" << std::endl;
            boolLightGuide = false;
        } else if (boolLightGuide && line.find("/my_construction/MaterialOfLightGuide") != std::string::npos) {
            std::string material_value = extract_value(line, "/my_construction/MaterialOfLightGuide");
            if (!material_value.empty()) {
                outfile << "Material of Light Guide: " << material_value << std::endl;
            }
        } else if (line.find("/my_construction/isPCB true") != std::string::npos) {
            outfile << "PCB: ON" << std::endl;
        } else if (line.find("/my_construction/isPCB false") != std::string::npos) {
            outfile << "PCB: OFF" << std::endl;
        } else if (line.find("/my_construction/isEndcap true") != std::string::npos) {
            outfile << "Endcap: ON" << std::endl;
        } else if (line.find("/my_construction/isEndcap false") != std::string::npos) {
            outfile << "Endcap: OFF" << std::endl;
        }
    }

    construction_file.close();

    outfile << std::endl;
    outfile << "########################################################" << std::endl;
    outfile << std::endl;
}

#endif