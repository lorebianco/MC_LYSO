/**
 * @file summary.hh
 * @brief Declaration of the function @ref MC_summary() (and the auxiliary
 * function @ref extract_value())
 */
#ifndef SUMMARY_HH
#define SUMMARY_HH

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "globals.hh"


/**
 * @brief Auxiliary function called by @ref MC_summary() to extract settings
 * from the macro files used for the Monte Carlo.
 *
 * @param line The line from the macro file
 * @param keyword The string representing the command in the macro files, after
 * which the setting is taken
 * @return The extracted value
 */
G4String extract_value(const G4String& line, const G4String& keyword);

/**
 * @brief Writes the summary of the Monte Carlo to a text file, updating
 * it at the end of every simulation.
 *
 * The summary includes:
 * - Monte Carlo ID (the seed of the run), date, username and
 * duration;
 * - Settings related to the primary generator;
 * - Settings related to the construction.
 *
 * @param macrofile The name of the run macro file
 * @param seed The seed of the simulation
 * @param duration The duration (in seconds) of the simulation
 * @param output_filename The name of the text output file
 */
void MC_summary(G4String macrofile, G4int seed, G4double duration, const G4String& output_filename);

#endif  // SUMMARY_HH
