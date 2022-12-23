/**************************************************************
 * \file SpectrumPlot.C
 * \brief Function to plot the spectrum of the dataset which is currently used and create an histogram. 
 *
 
 *******************************************************************************/
#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLatex.h"
#include <filesystem>
#include "TStyle.h"
#include "SpectrumPlot.h"

void SavePlot(TCanvas *c, TString namePlot){
  namespace fs = std::filesystem;

    TString fname = "./Plots/" + namePlot + ".pdf";
    //const char *fname = tmp.c_str();
    try
    {
        c->SaveAs(fname);
        if (!fs::is_directory("./Plots") || !fs::exists("./Plots"))
            throw("./Plots");
    }
    catch (const char *pathToData)
    {
        std::cerr << "Directory " << pathToData << " does not exist.\n"
                  << std::endl;
        std::cerr << "Creating directory...\n"
                  << std::endl;

        fs::create_directory(pathToData);
        std::cout << "Directory " << pathToData << " successfully created\n"
                  << std::endl;
    }
  c->SaveAs(fname);
}

TH1* SpectrumPlot(ROOT::RDF::RNode &df_cut,std::string nameFile){
  //Enable multi-threading
  ROOT::EnableImplicitMT(1);
  
  // Book histogram of dimuon mass spectrum
  const auto bins = 300; // Number of bins in the histogram
  const auto low = 8.5;// Lower edge of the histogram
  const auto up = 11.5;// Upper edge of the histogram


  auto hist = df_cut.Histo1D({"hist", "Dimuon mass", bins, low, up}, "Dimuon_mass");

  // Create canvas for plotting
  gStyle->SetOptStat(0);
  gStyle->SetTextFont(42);
  auto c = new TCanvas("c", "", 800, 700);
  

  // Draw histogram
  hist->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} [GeV]");
  hist->GetXaxis()->SetTitleSize(0.04);
  hist->GetYaxis()->SetTitle("N_{Events}");
  hist->GetYaxis()->SetTitleSize(0.04);
  hist->DrawClone();

  // Draw labels
  TLatex label;


  label.SetNDC(true); //cambio di coordinate di riferimento da quelle del grafico a quelle del pad normalizzate
  label.SetTextAlign(22); //22= centrale verticalmente e orizzontalmente
  label.DrawLatex(0.5, 0.7, "Y(1,2,3S)");
  label.SetTextAlign(11); //left bottom

  label.SetTextSize(0.04);
  label.DrawLatex(0.10, 0.92, "#bf{CMS Open Data}");
  label.SetTextAlign(31); //right bottom
  label.DrawLatex(0.90, 0.92, "11.6 fb^{-1} (8 TeV)");

  // Save plot
  SavePlot(c,nameFile+"_preliminary");
  //TH1*h = hist.GetPtr();

  TH1 *h = (TH1*)gDirectory->Get("hist"); //da riguardare, forse c'è un modo migliore per farlo
  return h;
}