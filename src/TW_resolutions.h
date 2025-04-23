/**
 * @file TW_resolutions.h
 * @brief Header file for TW_resolutions.cc
 * 
 * This file contains function declarations for analyzing and plotting 
 * the resolution of Time-Of-Flight (TOF) and energy loss (Eloss).
 * The energy loss and TOF histograms, considering the response of all the TW bars,
 * for the merged runs without target at each beam energy are fitted with gaussian functions;
 * the resolution on the energy loss is retrieved as sigma/mu, the resolution on TOF is evaluated
 * as sigma. The results are stored in LaTeX tables and plotted.
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
 * @brief Fits histograms for ToF and Eloss resolutions.
 * 
 * This function processes histograms from the input ROOT file, performs fits, and stores the results
 * in vectors for resolutions and means. The results are also written to LaTeX files.
 * 
 * @param inFile Pointer to the input ROOT file.
 * @param energy Beam energy in MeV/u.
 * @param R_He_vec Vector to store He resolution values.
 * @param R_He_err_vec Vector to store He resolution uncertainties.
 * @param meanEloss_vec Vector to store mean Eloss values.
 * @param meanEloss_err_vec Vector to store mean Eloss uncertainties.
 * @param R_Tof_vec Vector to store ToF resolution values.
 * @param R_Tof_err_vec Vector to store ToF resolution uncertainties.
 * @param meanTof_vec Vector to store mean ToF values.
 * @param meanTof_err_vec Vector to store mean ToF uncertainties.
 * @param LatexFile_Eloss Output stream for the Eloss LaTeX table.
 * @param LatexFile_Tof Output stream for the ToF LaTeX table.
 */
void FitHistograms(
    TFile* inFile,
    int energy,
    std::vector<double>& R_He_vec,
    std::vector<double>& R_He_err_vec,
    std::vector<double>& meanEloss_vec,
    std::vector<double>& meanEloss_err_vec,
    std::vector<double>& R_Tof_vec,
    std::vector<double>& R_Tof_err_vec,
    std::vector<double>& meanTof_vec,
    std::vector<double>& meanTof_err_vec,
    std::ofstream& LatexFile_Eloss,
    std::ofstream& LatexFile_Tof
);

/**
 * @brief Processes a ROOT file and calculates resolutions for ToF and Eloss.
 * 
 * This function reads histograms from the specified ROOT file, performs fits, and stores the results
 * in vectors for resolutions and means.
 * 
 * @param fileName Name of the ROOT file to process.
 * @param energy Beam energy in MeV/u.
 * @param R_He_vec Vector to store He resolution values.
 * @param R_He_err_vec Vector to store He resolution uncertainties.
 * @param meanEloss_vec Vector to store mean Eloss values.
 * @param meanEloss_err_vec Vector to store mean Eloss uncertainties.
 * @param R_Tof_vec Vector to store ToF resolution values.
 * @param R_Tof_err_vec Vector to store ToF resolution uncertainties.
 * @param meanTof_vec Vector to store mean ToF values.
 * @param meanTof_err_vec Vector to store mean ToF uncertainties.
 * @param LatexFile_Eloss Output stream for the Eloss LaTeX table.
 * @param LatexFile_Tof Output stream for the ToF LaTeX table.
 */
void ProcessFile(
    const std::string& fileName,
    int energy,
    std::vector<double>& R_He_vec,
    std::vector<double>& R_He_err_vec,
    std::vector<double>& meanEloss_vec,
    std::vector<double>& meanEloss_err_vec,
    std::vector<double>& R_Tof_vec,
    std::vector<double>& R_Tof_err_vec,
    std::vector<double>& meanTof_vec,
    std::vector<double>& meanTof_err_vec,
    std::ofstream& LatexFile_Eloss,
    std::ofstream& LatexFile_Tof
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
 * @brief Writes Eloss results to a LaTeX table.
 * 
 * This function writes the Eloss results, including mean, standard deviation, and resolution, to a LaTeX table.
 * 
 * @param outFile Output stream for the LaTeX file.
 * @param energy Beam energy in MeV/u.
 * @param meanElossHe Mean Eloss value for He.
 * @param stdElossHe Standard deviation of Eloss for He.
 * @param R_He Resolution for He.
 * @param meanElossErrHe Uncertainty in the mean Eloss for He.
 * @param stdElossErrHe Uncertainty in the standard deviation of Eloss for He.
 * @param R_He_err Uncertainty in the resolution for He.
 */
void WriteElossTable(std::ofstream& outFile, int energy, 
    double meanElossHe, double stdElossHe, double R_He,
    double meanElossErrHe, double stdElossErrHe, double R_He_err);

/**
 * @brief Writes ToF results to a LaTeX table.
 * 
 * This function writes the ToF results, including mean and resolution, to a LaTeX table.
 * 
 * @param outFile Output stream for the LaTeX file.
 * @param energy Beam energy in MeV/u.
 * @param meanTof Mean ToF value.
 * @param meanTofErr Uncertainty in the mean ToF value.
 * @param R_Tof Resolution for ToF.
 * @param R_Tof_err Uncertainty in the resolution for ToF.
 */
void WriteTofTable(std::ofstream& outFile, int energy, 
    double meanTof, double meanTofErr,
    double R_Tof, double R_Tof_err);

/**
 * @brief Opens a LaTeX file for Eloss results.
 * 
 * @param outFile Output stream for the LaTeX file.
 */
void OpenElossFile(std::ofstream& outFile);

/**
 * @brief Opens a LaTeX file for ToF results.
 * 
 * @param outFile Output stream for the LaTeX file.
 */
void OpenTofFile(std::ofstream& outFile);

/**
 * @brief Closes a LaTeX file for Eloss results.
 * 
 * @param outFile Output stream for the LaTeX file.
 */
void CloseElossFile(std::ofstream& outFile);

/**
 * @brief Closes a LaTeX file for ToF results.
 * 
 * @param outFile Output stream for the LaTeX file.
 */
void CloseTofFile(std::ofstream& outFile);

/**
 * @brief Plots resolution graphs for ToF and Eloss.
 * 
 * This function creates graphs for ToF and Eloss resolutions and saves them as PNG files.
 * The eloss resolutions at each beam energy are plotted against the mean energy loss value,
 * the TOF resolutions against the mean TOF value.
 * 
 * @param R_He_vec Vector containing He resolution values.
 * @param R_He_err_vec Vector containing He resolution uncertainties.
 * @param meanEloss_vec Vector containing mean Eloss values.
 * @param meanEloss_err_vec Vector containing mean Eloss uncertainties.
 * @param R_Tof_vec Vector containing ToF resolution values.
 * @param R_Tof_err_vec Vector containing ToF resolution uncertainties.
 * @param meanTof_vec Vector containing mean ToF values.
 * @param meanTof_err_vec Vector containing mean ToF uncertainties.
 */
void PlotResolutionGraphs(
    const std::vector<double>& R_He_vec,
    const std::vector<double>& R_He_err_vec,
    const std::vector<double>& meanEloss_vec,
    const std::vector<double>& meanEloss_err_vec,
    const std::vector<double>& R_Tof_vec,
    const std::vector<double>& R_Tof_err_vec,
    const std::vector<double>& meanTof_vec,
    const std::vector<double>& meanTof_err_vec
);


/// \copydoc RoundMeasurement
/// \copybrief RoundMeasurement
std::pair<std::string, std::string> RoundMeasurement(double value, double uncertainty);
