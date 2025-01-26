/**
 * @file CaloPeakEnergyDisplay.h
 * @brief Header file for plotting charge distribution histograms from ROOT files.
 * 
 * Macro that plots the charge given by the fit performed in AnalyzePeakCrystal.cc in all
 * the available crystals vs the beam energy. The fit results are stored in objects, named like
 * Fit_result_Charge_Calo_crystalId_1 inside the e.g. Calo/AnaFOOT_Calo_Decoded_HIT2022_140MeV_Fit.root file.
 * To be run with root -l -b -q 'CaloPeakEnergyDisplay.cc()' it loops on every crystalID and extracts the fit
 * values only on the available IDs.
 */

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TFile.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <iostream>
#include <sstream>
#endif

/**
 * @brief Converts a vector of energy values to a comma-separated string.
 * 
 * This function converts a vector of energy values to a comma-separated string.
 * 
 * @param energies A vector of energy values.
 * @return A string containing the energy values separated by commas.
 */
std::string ConvertFileNumbersToString(const std::vector<int>& energies);

/**
 * @brief Rounds a measurement value and its uncertainty.
 * 
 * This function rounds a measurement value and its uncertainty to the appropriate number of significant figures.
 * The uncertainty is rounded to 1 significant figure.
 * 
 * @param value The measurement value.
 * @param uncertainty The uncertainty of the measurement.
 * @return A pair of strings containing the rounded value and uncertainty.
 */
pair<std::string, std::string> RoundMeasurement(double value, double uncertainty);