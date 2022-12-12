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
#include "TStyle.h"
#include "SpectrumPlot.h"

/***********************************************************
 * Simple Function to plot the spectrum of the dataset which is currently used and create an histogram. 
 * @param df_cut dataframe containg the data
 * \return histogram of the data
 *************************************************************/
TH1* SpectrumPlot(ROOT::RDF::RNode df_cut){
  //Enable multi-threading
  ROOT::EnableImplicitMT(1);
  
  /// Book histogram of dimuon mass spectrum
  const auto bins = 300; /// Number of bins in the histogram
  const auto low = 8.5;/// Lower edge of the histogram
  const auto up = 11.5;/// Upper edge of the histogram
  auto hist = df_cut.Histo1D({"hist", "Dimuon mass", bins, low, up}, "Dimuon_mass");

  /// Create canvas for plotting
  gStyle->SetOptStat(0);
  gStyle->SetTextFont(42);
  auto c = new TCanvas("c", "", 800, 700);


  /// Draw histogram
  hist->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} [GeV]");
  hist->GetXaxis()->SetTitleSize(0.04);
  hist->GetYaxis()->SetTitle("N_{Events}");
  hist->GetYaxis()->SetTitleSize(0.04);

  /// Draw labels
  TLatex label;
  label.SetTextAlign(22); //22= centrale verticalmente e orizzontalmente

  label.DrawLatex(10.0, 200, "Y(1,2,3S)");

  label.SetNDC(true); //cambio di coordinate di riferimento da quelle del grafico a quelle del pad normalizzate
  label.SetTextAlign(11); //left bottom
  label.SetTextSize(0.04);
  label.DrawLatex(0.10, 0.92, "#bf{CMS Open Data}");
  label.SetTextAlign(31); //right bottom
  label.DrawLatex(0.90, 0.92, "#sqrt{s} = 8 TeV, L_{int} = 11.6 fb^{-1}");

  /// Save plot
  c->SaveAs("Plots/Preliminary_histo.pdf");

  std::string hs;
  hs="hist";
  TH1 *h = (TH1*)gDirectory->Get(hs.c_str()); //da riguardare, forse c'è un modo migliore per farlo
  return h;
}