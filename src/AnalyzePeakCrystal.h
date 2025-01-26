/**
 * @file AnalyzePeakCrystal.h
 * @brief Header file for processing and fitting charge distribution histograms from ROOT files.
 * 
 * Macro that fits the 1D charge histograms for a given crystal of the calorimeter.
 * The histograms are retrieved from the AnaLyzeCalo.cc merged output files (the histograms are automatically summed).
 * To be run with e.g. root -l -b -q 'AnalyzePeakCrystal.cc(x_min, x_max, fit_thresh)', -b doesn't display plots. 
 * In an interval between x_min and x_max a peak beyond fit_thresh is automatically found with TSpectrum and fitted.
 * The fit results are inserted in files named like e.g. Calo/AnaFOOT_Calo_Decoded_HIT2022_140MeV_Fit.root which also
 * store the crystalID charge histogram and the fit plot restricted in [x_min, x_max].
 */

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TFitResultPtr.h>
#include <TF1.h>
#include <TLegend.h>
#include <TObjString.h>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <TSpectrum.h>
#endif

/**
 * @brief Prints a measurement value and its uncertainty.
 * 
 * This function prints a measurement value and its uncertainty to the console.
 * 
 * @param value The measurement value.
 * @param uncertainty The uncertainty of the measurement.
 */
void PrintMeasurement(double value, double uncertainty);

/**
 * @brief Fits a peak in a histogram using TSpectrum.
 * 
 * This function uses TSpectrum to search for a peak in a histogram and fits it with a Gaussian function.
 * 
 * @param hist The histogram to fit.
 * @param threshold The threshold for peak detection.
 * @return A TFitResultPtr containing the fit result.
 */
TFitResultPtr FitPeakWithTSpectrum(TH1D *hist, double threshold);

/**
 * @brief Finds histograms in a ROOT file.
 * 
 * This function finds and retrieves histograms from a ROOT file.
 * 
 * @param inFile The input ROOT file.
 * @param histName_total The name of the total histogram.
 * @param histName The name of the specific histogram.
 * @return A tuple containing pointers to the total histogram and the specific histogram.
 */
std::tuple<TH1D*, TH1D*> FindHistograms(TFile *inFile, const TString &histName_total, const TString &histName);

/**
 * @brief Saves fit results to a ROOT file.
 * 
 * This function saves the fit results and the corresponding histogram to a ROOT file.
 * 
 * @param canvas The canvas containing the fit plot.
 * @param hist The histogram to save.
 * @param fitResult The fit result to save.
 * @param outputFileName The name of the output ROOT file.
 */
void SaveFitResultsToFile(TCanvas* canvas, TH1D* hist, TFitResultPtr fitResult, const TString& outputFileName);