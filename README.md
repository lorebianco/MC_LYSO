<a href="https://lorebianco.github.io/MC_LYSO/">
  <img align="center" src="https://github.com/lorebianco/MC_LYSO/actions/workflows/docs.yml/badge.svg" />
</a>

# MC_LYSO
The *MC_LYSO* project is a Geant4 simulation of an electromagnetic calorimeter prototype, consisting of a large cylindrical LYSO scintillator crystal coupled to MPPCs (SiPMs) on both faces for signal readout. The ultimate goal is to develop an auxiliary detector for the calibration of MEG-II calorimeter and investigate its potential as a prototype for future calorimeters designed for gamma particles with energies on the order of 50 MeV, offering ultra-precise time and energy resolution.

It is important to note that the output data from this application do not directly represent the final physical observables, due to the absence of the "digitization" process in the Monte Carlo. Thus, the generated data serves as input for the SiPM-response simulation: an early version of that can be found at [Bartender_LYSO](https://github.com/lorebianco/Bartender_LYSO).

Please refer to the [documentation](https://lorebianco.github.io/MC_LYSO/) for all info.


To compile and link and generate the executable, please ensure that the development environment for Geant4 is configured. If it's not already set up, you can do so by executing the following command:
```
$ . /path/to/Geant4Installation/share/Geant4/geant4make/geant4make.sh
```
Afterwards, proceed as follows:
```
$ cd /path/to/MC_LYSO     # go to directory which contains MC_LYSO
$ mkdir build
$ cd build
$ cmake ..
$ make -j N     # "N" is the number of processes 
```
This sequence of commands will compile and link your code, generating the executable.