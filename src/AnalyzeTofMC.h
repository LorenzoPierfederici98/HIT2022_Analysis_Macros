/**
 * @file AnalyzeTofMC.h
 * @brief Header file for AnalyzeTofMC.cc
 * 
 * This file contains function declarations for fitting ToF (Time of Flight) distributions from the AnalyzeTWFragMC.cc output files 
 * (for MC runs). The fits are performed with a gaussian function, the fit limits depend on the bar and on the beam energy. 
 * The histogram peaks are automatically found with TSpectrum; the fits are then performed within a certain bin-range centered 
 * around the peak, depending on the specific bar and beam energy. The fit results are stored in output files, created if they don't
 * already exist, or overwritten if they do.
 */

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TFile.h>
#include <TKey.h>
#include <TDirectory.h>
#include <TH1.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TNamed.h>
#include <TSpectrum.h>
#include <iostream>
#include <vector>
#include <map>
#include <regex>
#endif

/**
 * @brief Processes a ROOT file and fits the histograms.
 * 
 * This function processes the specified ROOT file, fits the histograms in the specified directories,
 * and stores the fit results in the provided maps. The fit results are stored in output files.
 * 
 * @param fileName Name of the ROOT file.
 * @param energy Beam energy.
 * @param TofMeansMC Map to store the mean values of the fits.
 * @param TofErrorsMC Map to store the errors of the fits.
 */
void ProcessFile(
    const std::string& fileName,
    int energy,
    std::map<TString, std::map<int, double>>& TofMeansMC,
    std::map<TString, std::map<int, double>>& TofErrorsMC
);

/**
 * @brief Fits histograms in a directory and stores the results.
 * 
 * This function fits the histograms in the specified directory using a gaussian function.
 * The fit results are stored in the provided maps and written to the output file.
 * 
 * @param dir Pointer to the directory containing the histograms.
 * @param energy Beam energy.
 * @param TofMeansMC Map to store the mean values of the fits.
 * @param TofErrorsMC Map to store the errors of the fits.
 * @param outputFile Pointer to the output ROOT file to store the fitted histograms.
 */
void FitHistogramsInDirectory(
    TDirectory *dir,
    int energy,
    std::map<TString, std::map<int, double>>& TofMeansMC,
    std::map<TString, std::map<int, double>>& TofErrorsMC,
    TFile* outputFile
);

/**
 * @brief Fits peaks in a histogram using TSpectrum.
 * 
 * This function fits peaks in the specified histogram using TSpectrum and a gaussian function.
 * The fit results are returned as a TFitResultPtr.
 * 
 * @param hist Pointer to the histogram.
 * @param energy Beam energy.
 * @param layerBarCombination String representing the layer and bar combination.
 * @return TFitResultPtr containing the fit results.
 */
TFitResultPtr FitPeaksWithTSpectrum(TH1D *hist, int energy, const TString &layerBarCombination);

