/**
 * @file Z_resolutions.h
 * @brief Header file for Z_resolutions.cc
 * 
 * This file contains function declarations for analyzing and plotting 
 * the resolution of Z (reconstructed charge). The reconstructed Z histograms
 * of the runs without target, obtained by inverting the Bethe-Bloch formula with 
 * the calibrated energy loss and TOF values considering every TW bar, are fitted with
 * a gaussian function. The resolution is evaluated as sigma/mu.
 * The results are stored in LaTeX tables and plotted.s
 */

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TFile.h>
#include <TKey.h>
#include <TDirectory.h>
#include <TH1.h>
#include <TF1.h>
#include <TRandom3.h>
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
 * @brief Processes a ROOT file and calculates resolutions for Z (charge).
 * 
 * This function reads histograms from the specified ROOT file, performs fits, and stores the results
 * in vectors for resolutions and means. The results are also written to a LaTeX file.
 * 
 * @param fileName Name of the ROOT file to process.
 * @param energy Beam energy in MeV/u.
 * @param R_Z_vec Vector to store Z resolution values.
 * @param R_Z_err_vec Vector to store Z resolution uncertainties.
 * @param meanZ_vec Vector to store mean Z values.
 * @param meanZ_err_vec Vector to store mean Z uncertainties.
 * @param LatexFile_Z Output stream for the Z LaTeX table.
 */
void ProcessFile(
    const std::string& fileName,
    int energy,
    std::vector<double>& R_Z_vec,
    std::vector<double>& R_Z_err_vec,
    std::vector<double>& meanZ_vec,
    std::vector<double>& meanZ_err_vec,
    std::ofstream& LatexFile_Z
);

/**
 * @brief Fits histograms for Z resolutions.
 * 
 * This function processes histograms from the input ROOT file, performs fits, and stores the results
 * in vectors for resolutions and means. The results are also written to a LaTeX file.
 * 
 * @param inFile Pointer to the input ROOT file.
 * @param energy Beam energy in MeV/u.
 * @param R_Z_vec Vector to store Z resolution values.
 * @param R_Z_err_vec Vector to store Z resolution uncertainties.
 * @param meanZ_vec Vector to store mean Z values.
 * @param meanZ_err_vec Vector to store mean Z uncertainties.
 * @param LatexFile_Z Output stream for the Z LaTeX table.
 */
void FitHistograms(
    TFile* inFile,
    int energy,
    std::vector<double>& R_Z_vec,
    std::vector<double>& R_Z_err_vec,
    std::vector<double>& meanZ_vec,
    std::vector<double>& meanZ_err_vec,
    std::ofstream& LatexFile_Z
);

/**
 * @brief Fits a histogram using TSpectrum for peak detection.
 * 
 * This function detects peaks in the histogram using TSpectrum and performs a Gaussian fit.
 * 
 * @param hist Pointer to the histogram to fit.
 * @param energy Beam energy in MeV/u.
 * @return TFitResultPtr containing the fit results.
 */
TFitResultPtr FitWithTSpectrum(TH1D *hist, int energy);

/**
 * @brief Writes Z results to a LaTeX table.
 * 
 * This function writes the Z results, including mean, standard deviation, and resolution, to a LaTeX table.
 * 
 * @param outFile Output stream for the LaTeX file.
 * @param energy Beam energy in MeV/u.
 * @param meanZ Mean Z value.
 * @param stdZ Standard deviation of Z.
 * @param R_Z Resolution for Z.
 * @param meanErrZ Uncertainty in the mean Z value.
 * @param stdErrZ Uncertainty in the standard deviation of Z.
 * @param R_Z_err Uncertainty in the resolution for Z.
 */
void WriteZTable(std::ofstream& outFile, int energy, 
    double meanZ, double stdZ, double R_Z,
    double meanErrZ, double stdErrZ, double R_Z_err);

/**
 * @brief Opens a LaTeX file for Z results.
 * 
 * This function initializes the LaTeX file for storing Z results.
 * 
 * @param outFile Output stream for the LaTeX file.
 */
void OpenZFile(std::ofstream& outFile);

/**
 * @brief Closes a LaTeX file for Z results.
 * 
 * This function finalizes and closes the LaTeX file for Z results.
 * 
 * @param outFile Output stream for the LaTeX file.
 */
void CloseZFile(std::ofstream& outFile);

/**
 * @brief Plots resolution graphs for Z.
 * 
 * This function creates graphs for Z resolutions and saves them as PNG files.
 * The Z resolutions at each beam energy are plotted against the mean Z value.
 * 
 * @param R_Z_vec Vector containing Z resolution values.
 * @param R_Z_err_vec Vector containing Z resolution uncertainties.
 */
void PlotResolutionGraphs(
    const std::vector<double>& R_Z_vec,
    const std::vector<double>& R_Z_err_vec
);


/*! @copydoc RoundMeasurement(double,double)
*/
std::pair<std::string, std::string> RoundMeasurement(double value, double uncertainty);