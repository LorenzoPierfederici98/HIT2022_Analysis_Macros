/**
 * @file Calo_resolutions.h
 * @brief Header file for Calo_resolutions.cc
 * 
 * This file contains function declarations and necessary includes for analyzing and plotting the resolution
 * of calorimeter crystals as a function of beam energy. The analysis is performed using ROOT libraries, and
 * the resolution is calculated based on the mean and standard deviation of Gaussian fits to charge histograms.
 * The results are stored and plotted for further analysis.
 */

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TFile.h>
#include <TKey.h>
#include <TDirectory.h>
#include <TH1.h>
#include <TF1.h>
#include <TRandom3.h>
#include <numeric>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TNamed.h>
#include <TSpectrum.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#endif

/**
 * @brief Processes a ROOT file and calculates the resolution for each crystal.
 * 
 * This function reads the fit results from the specified ROOT file, calculates the resolution
 * (standard deviation divided by the mean, expressed as a percentage), and stores the results
 * in the provided maps.
 * 
 * @param fileName Name of the ROOT file to process.
 * @param energy Beam energy in MeV/u.
 * @param Res Map to store the calculated resolutions for each crystal.
 * @param Res_err Map to store the uncertainties of the calculated resolutions for each crystal.
 */
void ProcessFile(const std::string& fileName, const int energy, std::map<int, std::vector<std::pair<int, double>>>& Res, std::map<int, std::vector<std::pair<int, double>>>& Res_err);

/// \cond
/// \copydoc RoundMeasurement
std::pair<double, double> RoundMeasurement(double value, double uncertainty);
/// \endcond

/**
 * @brief Plots the resolution as a function of beam energy for each crystal.
 * 
 * This function creates a graph for each crystal, showing the resolution as a function of beam energy.
 * The graphs are saved as PNG files in the `Plots` directory.
 * 
 * @param Res Map containing the resolutions for each crystal at different beam energies.
 * @param Res_err Map containing the uncertainties of the resolutions for each crystal at different beam energies.
 */
void PlotResolutions(const std::map<int, std::vector<std::pair<int, double>>>& Res,
    const std::map<int, std::vector<std::pair<int, double>>>& Res_err);