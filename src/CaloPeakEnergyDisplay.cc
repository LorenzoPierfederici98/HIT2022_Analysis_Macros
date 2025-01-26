// Macro that plots the charge given by the fit performed in AnalyzePeakCrystal.cc in all
// the available crystals vs the beam energy. The fit results are stored in objects, named like
// Fit_result_Charge_Calo_crystalId_1 inside the e.g. Calo/AnaFOOT_Calo_Decoded_HIT2022_140MeV_Fit.root file.
// To be run with root -l -b -q 'CaloPeakEnergyDisplay.cc()' it loops on every crystalID and extracts the fit
// values only on the available IDs.

#include "CaloPeakEnergyDisplay.h"

void CaloPeakEnergyDisplay() {

    std::vector<std::pair<std::string, double>> filesAndEnergies = {
        {"Calo/AnaFOOT_Calo_Decoded_HIT2022_100MeV_Fit.root", 100},
        {"Calo/AnaFOOT_Calo_Decoded_HIT2022_140MeV_Fit.root", 140},
        {"Calo/AnaFOOT_Calo_Decoded_HIT2022_180MeV_Fit.root", 180},
        {"Calo/AnaFOOT_Calo_Decoded_HIT2022_200MeV_Fit.root", 200},
        {"Calo/AnaFOOT_Calo_Decoded_HIT2022_220MeV_Fit.root", 220}
    };

    std::vector<int> energies = {100, 140, 180, 200, 220};

    for (int crystalID = 0; crystalID < 63; crystalID++) {
        TCanvas *canvas = new TCanvas("canvas", "Calo Fit Results vs Energies", 800, 600);

        TGraphErrors *graph = new TGraphErrors();
        int pointIndex = 0;

        for (const auto &[fileName, energy] : filesAndEnergies) {
            TFile *inFile = TFile::Open(fileName.c_str());

            if (!inFile || inFile->IsZombie()) {
                std::cerr << "Error: Could not open file " << fileName << std::endl;
                if (inFile) inFile->Close(); // Ensure file is closed if it exists
                continue;
            }

            TFitResult *fitresult = (TFitResult *)inFile->Get(Form("Fit_result_Charge_Calo_crystalId_%d", crystalID));
            if (!fitresult) {
                std::cerr << "Warning: Fit results not found in file " << fileName << std::endl;
                inFile->Close();
                continue;
            }

            // Retrieve mean charge and error
            Double_t mean_charge = fitresult->Parameter(1);
            Double_t mean_charge_error = fitresult->ParError(1);

            std::cout << "Crystal " << crystalID << ", Energy " << energy 
                      << " MeV: Mean Charge = " << mean_charge 
                      << " ± " << mean_charge_error << std::endl;

            // Add point to the graph
            graph->SetPoint(pointIndex, energy, mean_charge);
            graph->SetPointError(pointIndex, 0, mean_charge_error);

            ++pointIndex;
            inFile->Close();
        }

        if (graph->GetN() == 0) {
            std::cerr << "No valid points for crystal " << crystalID << std::endl;
            delete graph;
            delete canvas;
            continue;
        }

        string energiesStr = ConvertFileNumbersToString(energies);
        // Customize the graph
        graph->SetTitle(Form("Beam Energies HE: %s MeV | Crystal ID: %d", energiesStr.c_str(), crystalID));
        graph->SetMarkerStyle(24);
        graph->SetMarkerSize(1.2);
        graph->GetXaxis()->SetTitle("Beam Energy [MeV]");
        graph->GetYaxis()->SetTitle("Mean Charge [a.u.]");
        graph->SetMinimum(0.0);
        double x_max = graph->GetXaxis()->GetXmax();

        // Perform linear fit
        TF1 *f1 = new TF1("f1", "pol1", 0., graph->GetXaxis()->GetXmax());
        TFitResultPtr fitresult_linear = graph->Fit(f1, "S");

        if (!fitresult_linear.Get() || fitresult_linear->Status() != 0) {
            std::cerr << "Linear fit failed for crystal " << crystalID << std::endl;
        } else {
            auto [intercept, sigma_intercept] = RoundMeasurement(fitresult_linear->Parameter(0), fitresult_linear->ParError(0));
            auto [slope, sigma_slope] = RoundMeasurement(fitresult_linear->Parameter(1), fitresult_linear->ParError(1));
            //double chi2 = fitresult_linear->Chi2();
            //int ndf = fitresult_linear->Ndf();

            TPaveText *fitInfo = new TPaveText(0.3, 0.7, 0.45, 0.85, "NDC");
            fitInfo->SetFillColor(0);
            fitInfo->AddText(Form("Intercept [a.u.] = %s#pm %s", intercept.c_str(), sigma_intercept.c_str()));
            fitInfo->AddText(Form("Slope [a.u. / MeV] = %s#pm %s", slope.c_str(), sigma_slope.c_str()));
            //fitInfo->AddText(Form("#chi^{2} / ndf = %.2f / %d", chi2, ndf));
            fitInfo->SetTextSize(0.03);

            graph->GetXaxis()->SetLimits(0., x_max);

            f1->SetParameter(0, std::stod(intercept));
            f1->SetParameter(1, std::stod(slope));

            // Draw fit info
            graph->Draw("AP");
            f1->Draw("same");
            canvas->cd();
            fitInfo->Draw("same");
            canvas->Modified();
            canvas->Update();
            canvas->SaveAs(Form("Plots/Merged_Calo_FitCharge_Energy_Crystal_%d.png", crystalID));

            delete fitInfo;
        }

        delete f1;
        delete graph;
        delete canvas;
    }
}

std::string ConvertFileNumbersToString(const std::vector<int>& energies) {
    std::stringstream ss;
    for (size_t i = 0; i < energies.size(); ++i) {
        ss << energies[i];
        if (i != energies.size() - 1) {
            ss << ", ";  // Add a comma and space between numbers
        }
    }
    return ss.str();
}

pair<std::string, std::string> RoundMeasurement(double value, double uncertainty) {
    // Determine the order of magnitude of the uncertainty
    int uncertaintyOrder = (int)std::floor(std::log10(uncertainty));
    double roundingFactor = std::pow(10, uncertaintyOrder);

    // Round uncertainty to 1 significant figure
    double roundedUncertainty = std::round(uncertainty / roundingFactor) * roundingFactor;

    // Adjust the value to match the precision of the uncertainty
    double roundedValue = std::round(value / roundingFactor) * roundingFactor;

    // Determine the number of decimal places to display based on the rounded uncertainty
    int decimalPlaces = std::max(0, -uncertaintyOrder);

    // Format value and uncertainty into strings with the required precision
    std::ostringstream valueStream, uncertaintyStream;
    valueStream << std::fixed << std::setprecision(decimalPlaces) << roundedValue;
    uncertaintyStream << std::fixed << std::setprecision(decimalPlaces) << roundedUncertainty;

    return {valueStream.str(), uncertaintyStream.str()};
}

