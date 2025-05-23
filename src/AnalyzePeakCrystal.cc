// Macro that fits the 1D charge histograms for a given crystal of the calorimeter.
// The histograms are retireved form the AnaLyzeCalo.cc merged output files (the histograms are automatically summed).
// To be run with e.g.  root -l -b -q 'AnalyzePeakCrystal.cc()', -b doesn't display plots. 
// In an interval between x_min and x_max a peak beyond fit_thresh is automatically found with TSpectrum and fitted.
// The fit results are inserted in files namekd like e.g. Calo/AnaFOOT_Calo_Decoded_HIT2022_140MeV_Fit.root which also
// store the crystalID charge histogram and the fit plot restricted in [x_min, x_max].

#include "AnalyzePeakCrystal.h"

void AnalyzePeakCrystal() {
    std::vector<std::pair<std::string, int>> filesAndEnergies = {
        {"Calo/AnaFOOT_Calo_Decoded_HIT2022_100MeV.root", 100},
        {"Calo/AnaFOOT_Calo_Decoded_HIT2022_140MeV.root", 140},
        {"Calo/AnaFOOT_Calo_Decoded_HIT2022_180MeV.root", 180},
        {"Calo/AnaFOOT_Calo_Decoded_HIT2022_200MeV.root", 200},
        {"Calo/AnaFOOT_Calo_Decoded_HIT2022_220MeV.root", 220}
    };

    double x_min = 0.2;
    double x_max = 0.6;

    for (const auto &[fileName, energy] : filesAndEnergies) {
        TFile *inFile = TFile::Open(fileName.c_str());
        if (!inFile || inFile->IsZombie()) {
            std::cerr << "Error: Cannot open file " << fileName << std::endl;
            continue;
        }

        for (int crystal_ID = 0; crystal_ID < 63; crystal_ID++) {
            TCanvas *canvas = new TCanvas(Form("c1_%d", crystal_ID), "Fit Results", 800, 600);
            TString histName = Form("Charge_Calo_crystalId_%d", crystal_ID);
            TString histName_total = "Charge_Calo";

            auto [Charge_Calo_crystal, Charge_Calo] = FindHistograms(inFile, histName_total, histName);

            if (!Charge_Calo_crystal || Charge_Calo_crystal->GetEntries() == 0 || !Charge_Calo || Charge_Calo->GetEntries() == 0) {
                std::cerr << "Warning: Histogram for crystal " << crystal_ID << " is empty or missing." << std::endl;
                delete canvas;
                continue;
            }
            double threshold = 0.0004;
            if (Charge_Calo_crystal->GetEntries() > threshold * Charge_Calo->GetEntries()) {
                canvas->SetTitle(Form("Beam Energy: %d MeV/u | Crystal ID: %d", energy, crystal_ID));

                TH1D *Charge_Calo_fullrange = static_cast<TH1D *>(Charge_Calo_crystal->Clone());
                if (crystal_ID == 0) {
                    if (energy == 100) {
                        Charge_Calo_crystal->GetXaxis()->SetRangeUser(0.08, 0.18);
                    }
                    else if (energy == 140) {
                        Charge_Calo_crystal->GetXaxis()->SetRangeUser(0.16, 0.24);
                    }
                    else {
                        Charge_Calo_crystal->GetXaxis()->SetRangeUser(0.24, 0.4);
                    }
                }
                else {
                    if (energy == 200) {
                        x_min = 0.25;
                    }
                    Charge_Calo_crystal->GetXaxis()->SetRangeUser(
                        (energy == 100) ? 0.1 : x_min,
                        (energy == 100) ? 0.35 : x_max
                    );
                }

                if ((energy == 200 && crystal_ID == 21) || (energy == 180 && crystal_ID == 24) || (energy == 220 && crystal_ID == 25) || (energy == 140 && crystal_ID == 34))
                {
                    delete Charge_Calo_fullrange;
                    continue;
                }

                TFitResultPtr fitResult = FitPeakWithTSpectrum(Charge_Calo_crystal, energy, crystal_ID);
                if (fitResult.Get() != nullptr) {
                    std::cout << "Beam energy: " << energy << " MeV/u " << " crystalID: " << crystal_ID << " ";
                    double meanCharge = fitResult->Parameter(1);
                    double meanChargeErr = fitResult->ParError(1);
                    double stdCharge = fitResult->Parameter(2);
                    TPaveText *fitInfo = new TPaveText(0.15, 0.75, 0.35, 0.85, "NDC");
                    fitInfo->AddText(Form("%d MeV/u", energy));
                    fitInfo->AddText(Form("mean = %f", meanCharge));
                    fitInfo->AddText(Form("sigma = %f", stdCharge));
                    fitInfo->AddText(Form("res (perc.) = %.3f", 100.* stdCharge / meanCharge));
                    fitInfo->SetTextSize(0.02);
                    fitInfo->Draw("same");
                    PrintMeasurement(meanCharge, meanChargeErr);
                    if (meanCharge / stdCharge - 1 < 0.1 || stdCharge == 0. || meanCharge / meanChargeErr - 1 < 0.5) {
                        std::cout << "Skipping fit because the mean/std or mean/meanErr charge ratios are close to 1" << std::endl;
                        continue;
                    }
                } else {
                    std::cerr << "Fit failed for crystal " << crystal_ID << std::endl;
                    continue;
                }

                TString outputFileName = fileName;
                outputFileName.ReplaceAll(".root", "_Fit.root");
                SaveFitResultsToFile(canvas, Charge_Calo_fullrange, fitResult, outputFileName);

                delete Charge_Calo_fullrange;
            }

            delete canvas;
        }
        inFile->Close();
        delete inFile;
    }
}


void PrintMeasurement(double value, double uncertainty) {
    int sigFigs = static_cast<int>(std::ceil(-std::log10(uncertainty))) + 1;
    double roundFactor = std::pow(10, sigFigs);
    std::cout << std::fixed << std::setprecision(sigFigs) 
              << "Peak position: " << std::round(value * roundFactor) / roundFactor 
              << " ± " << std::round(uncertainty * roundFactor) / roundFactor << std::endl;
}


TFitResultPtr FitPeakWithTSpectrum(TH1D *hist, int energy, int crystal_ID) {
    TSpectrum spectrum(1);
    int nPeaks = spectrum.Search(hist, 2, "", 0.002);

    if (nPeaks == 0) {
        std::cerr << "No peak found" << std::endl;
        return TFitResultPtr(nullptr);
    }

    double peakPos = spectrum.GetPositionX()[0];
    if (energy == 200 && crystal_ID == 15) peakPos = 0.514;
    else if (energy == 200 && crystal_ID == 6) peakPos = 0.467;
    std::cout << "Peak found at x = " << peakPos << std::endl;

    int binMax = hist->FindBin(peakPos);
    int bins_fit = (energy == 220 && crystal_ID == 9) ? 5 : 3;
    if ((energy == 140) && crystal_ID == 0) bins_fit = 2;
    else if (energy == 200 && crystal_ID == 1) bins_fit = 4;
    int binLow = std::max(1, binMax - bins_fit);
    int binHigh = std::min(hist->GetNbinsX(), binMax + bins_fit);

    std::cout << "Fitting range: [" << hist->GetBinLowEdge(binLow) << ", " << hist->GetBinLowEdge(binHigh + 1) << "]" << std::endl;
    return hist->Fit("gaus", "S", "", hist->GetBinLowEdge(binLow), hist->GetBinLowEdge(binHigh + 1));
}


std::tuple<TH1D *, TH1D *> FindHistograms(TFile *inFile, const TString &histName_total, const TString &histName) {
    if (!inFile || inFile->IsZombie()) return {nullptr, nullptr};

    TH1D *hist = dynamic_cast<TH1D *>(inFile->Get(histName));
    TH1D *hist_total = dynamic_cast<TH1D *>(inFile->Get(histName_total));

    return {hist, hist_total};
}

void SaveFitResultsToFile(TCanvas *canvas, TH1D *hist, TFitResultPtr fitResult, const TString &outputFileName) {
    TFile *outFile = TFile::Open(outputFileName, "UPDATE");
    if (!outFile || outFile->IsZombie()) {
        std::cerr << "Error: Cannot create output file " << outputFileName << std::endl;
        return;
    }

    canvas->cd();
    //gPad->SetLogy();
    canvas->Update();
    hist->Write(hist->GetName(), TObject::kOverwrite);
    canvas->Write(Form("Canvas_Crystal_%s", hist->GetName()),  TObject::kOverwrite);
    fitResult->Write(Form("Fit_result_%s", hist->GetName()),  TObject::kOverwrite);
    outFile->Close();
    std::cout << "Saved fit results to " << outputFileName << std::endl;
}