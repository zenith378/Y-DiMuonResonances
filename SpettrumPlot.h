//
//  SpettrumPlot.h
//
//
//  Created by Giulio Cordova on 10/11/22.
//
//

#ifndef SpettrumPlot_h
#define SpettrumPlot_h

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TStyle.h"
#include "Cuts.h"

using namespace ROOT::VecOps;


TH1* SpettrumPlot(ROOT::RDataFrame df_cut){
  //Enable multi-threading
  ROOT::EnableImplicitMT(1);
  
  // Book histogram of dimuon mass spectrum
  const auto bins = 300; // Number of bins in the histogram
  const auto low = 8.5;//0.25; // Lower edge of the histogram
  const auto up = 11.5;//300.0; // Upper edge of the histogram
  auto hist = df_cut.Histo1D({"hist", "Dimuon mass", bins, low, up}, "Dimuon_mass");

  // Request cut-flow report
  auto report = df_cut.Report();

  // Create canvas for plotting
  gStyle->SetOptStat(0);
  gStyle->SetTextFont(42);
  auto c = new TCanvas("c", "", 800, 700);
  //c->SetLogx();
  //c->SetLogy();


  // Draw histogram
  hist->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} [GeV]");
  hist->GetXaxis()->SetTitleSize(0.04);
  hist->GetYaxis()->SetTitle("N_{Events}");
  hist->GetYaxis()->SetTitleSize(0.04);
  hist->DrawClone(); //crea un clone dell'istogramma che sopravvive fuori dalla macro

  // Draw labels
  TLatex label;
  label.SetTextAlign(22); //22= centrale verticalmente e orizzontalmente

  label.DrawLatex(10.0, 200, "Y(1,2,3S)");

  label.SetNDC(true); //cambio di coordinate di riferimento da quelle del grafico a quelle del pad normalizzate
  label.SetTextAlign(11); //left bottom
  label.SetTextSize(0.04);
  label.DrawLatex(0.10, 0.92, "#bf{CMS Open Data}");
  label.SetTextAlign(31); //right bottom
  label.DrawLatex(0.90, 0.92, "#sqrt{s} = 8 TeV, L_{int} = 11.6 fb^{-1}");

  // Save plot
  c->SaveAs("dimuonSpectrum_cut_pt2.pdf");

  // Print cut-flow report
  report->Print();

  std::string hs;
  hs="hist";
  TH1 *h = (TH1*)gDirectory->Get(hs.c_str());
  return h;
}


#endif /* SpettrumPlot_h */
