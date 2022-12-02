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
#include "TFile.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TStyle.h"
#include "Cuts.h"

using namespace ROOT::VecOps;


void SpettrumPlot(){
  //Enable multi-threading
  ROOT::EnableImplicitMT(1);
  //filename
  TString fname("./Data/data.root");
  //try opening file
  TFile *rootfile = TFile::Open(fname);
  //if file does not open
  if (!rootfile){
    std::cout << "Reading dataset from web"<< std::endl;
    //reading dataframe from online NanoAOD
    ROOT::RDataFrame df_temp("Events", "root://eospublic.cern.ch//eos/opendata/cms/derived-data/AOD2NanoAODOutreachTool/Run2012BC_DoubleMuParked_Muons.root");
    //write dataframe to file
    df_temp.Snapshot("Events","./Data/data.root");
      }
  else if(rootfile->IsZombie()){
    std::cout << "Problems reading file " << fname << std::endl;
    exit(1);
  }

  // read dataframe from file 
  ROOT::RDataFrame df("Events", fname);

  //Events selection
  TFile *cutfile = TFile::Open("./Data/data_cut.root");
  if (!cutfile){
    std::cout << "Recreating cut file"<< std::endl;
    Cuts(df);
    }
  else if(cutfile->IsZombie()){
    std::cout << "Problems reading file ./Data/data.root"<< std::endl;
    exit(1);
  }
  ROOT::RDataFrame df_cut("Cuts", "./Data/data_cut.root"); 
  
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
}


#endif /* SpettrumPlot_h */
