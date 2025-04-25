/**
 * @file AnalyzePeakCrystal.h
 * @brief Header file for AnalyzePeakCrystal.cc
 * 
 * This file contains function declarations for analyzing and fitting charge histograms
 * for calorimeter crystals. The charge histograms of every crystal (provided that it has
 * enough statistics to display a peak) are fitted with a Gaussian function in a region 
 * surrounding the peak, found with TSpectrum. The fit results are saved in output files,
 * along with the histograms and fit plots.
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
 * @brief Prints the measurement value and its uncertainty.
 * 
 * This function formats and prints the peak position and its uncertainty with significant figures
 * based on the uncertainty.
 * 
 * @param value The measurement value (e.g., peak position).
 * @param uncertainty The uncertainty in the measurement.
 */
void PrintMeasurement(double value, double uncertainty);

/**
 * @brief Fits a peak in the given histogram using TSpectrum and a Gaussian function.
 * 
 * This function detects peaks in the histogram using TSpectrum and fits the detected peak
 * with a Gaussian function. The fit results are returned as a TFitResultPtr.
 * 
 * @param hist Pointer to the histogram to fit.
 * @param energy Beam energy in MeV/u.
 * @param crystal_ID The ID of the crystal being analyzed.
 * @return TFitResultPtr containing the fit results.
 */
TFitResultPtr FitPeakWithTSpectrum(TH1D *hist, int energy, int crystal_ID);

/**
 * @brief Retrieves histograms for a specific crystal and the total charge from the input ROOT file.
 * 
 * This function searches for and retrieves the specified histograms from the input file.
 * It returns pointers to the crystal-specific histogram and the total charge histogram.
 * 
 * @param inFile Pointer to the input ROOT file.
 * @param histName_total Name of the total charge histogram.
 * @param histName Name of the histogram for the specific crystal.
 * @return A tuple containing pointers to the crystal histogram and the total charge histogram.
 */
std::tuple<TH1D*, TH1D*> FindHistograms(TFile *inFile, const TString &histName_total, const TString &histName);

/**
 * @brief Saves the fit results, histograms, and plots to an output ROOT file.
 * 
 * This function writes the histogram, fit results, and canvas to the specified output file.
 * It creates the output file if it does not exist or updates it if it does.
 * 
 * @param canvas Pointer to the canvas containing the fit plot.
 * @param hist Pointer to the histogram being analyzed.
 * @param fitResult The fit results to save.
 * @param outputFileName Name of the output ROOT file.
 */
void SaveFitResultsToFile(TCanvas* canvas, TH1D* hist, TFitResultPtr fitResult, const TString& outputFileName);