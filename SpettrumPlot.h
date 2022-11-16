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
#include "ROOT/RVec.hxx" //per usare ROOT::VecOps::RVec<T>
#include "Math/Vector4Dfwd.h"
#include "Math/Vector4D.h" //per usare PtEtaPhiMVector e le sue funzioni
#include "TMath.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TStyle.h"

using namespace ROOT::VecOps;


float computeInvariantMass(RVec<float>& pt, RVec<float>& eta, RVec<float>& phi, RVec<float>& mass)
  {
    //potremmo implementare queste due righe con un ciclo su uno dei vettori,
    //definire un vettore si vettori di lorentz che se è più lungo di 2 mi
    //restituisce un messaggio di errore, sommare gli elementi del vettore
    //(cioè sommare i duei vettori di L e fare la massa invariante)
    ROOT::Math::PtEtaPhiMVector m1(pt[0], eta[0], phi[0], mass[0]);
    ROOT::Math::PtEtaPhiMVector m2(pt[1], eta[1], phi[1], mass[1]);
    return (m1 + m2).mass();
  }

float computePT(RVec<float>& pt)
  {
      float pt_Dimuon = TMath::Sqrt(pt[0]*pt[0] + pt[1]*pt[1]);
      return pt_Dimuon;
  }


//compute invariant mass

void SpettrumPlot(){
  //Enable multi-threading
  ROOT::EnableImplicitMT(1);

  // Create dataframe from NanoAOD files
  ROOT::RDataFrame df("Events", "root://eospublic.cern.ch//eos/opendata/cms/derived-data/AOD2NanoAODOutreachTool/Run2012BC_DoubleMuParked_Muons.root");

  //seci interessa solo l'ultimo DF, gli altri li possiamo sovrascrivere
  // Select events with exactly two muons
  auto df_2mu = df.Filter("nMuon == 2", "Events with exactly two muons");

  // Select events with two muons of opposite charge
  auto df_os = df_2mu.Filter("Muon_charge[0] != Muon_charge[1]", "Muons with opposite charge");

  // Compute invariant mass of the dimuon system
  auto df_mass = df_os.Define("Dimuon_mass", computeInvariantMass,
			      {"Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass"});
    
  //Compute pt and pseudorapidity of dimuon
  auto df_pt = df_mass.Define("Dimuon_pt", computePT,{"Muon_pt"});
    

  //Select events with 10 GeV < pT < 12 GeV
  //const auto pt_max = 12.;
  //const auto pt_min = 10.;
  auto df_pt1 = df_pt.Filter("Dimuon_pt > 10. ", "First cut on pt");
  auto df_pt2 = df_pt1.Filter("Dimuon_pt < 12. ", "Second cut on pt");   //sarebbe possibile unire i due tagli???

  // Book histogram of dimuon mass spectrum
  const auto bins = 30000; // Number of bins in the histogram
  const auto low = 0.25; // Lower edge of the histogram
  const auto up = 300.0; // Upper edge of the histogram
  auto hist = df_pt2.Histo1D({"hist", "Dimuon mass", bins, low, up}, "Dimuon_mass");

  // Request cut-flow report
  auto report = df_mass.Report();

  // Create canvas for plotting
  gStyle->SetOptStat(0);
  gStyle->SetTextFont(42);
  auto c = new TCanvas("c", "", 800, 700);
  c->SetLogx();
  c->SetLogy();


  // Draw histogram
  hist->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} [GeV]");
  hist->GetXaxis()->SetTitleSize(0.04);
  hist->GetYaxis()->SetTitle("N_{Events}");
  hist->GetYaxis()->SetTitleSize(0.04);
  hist->DrawClone(); //crea un clone dell'istogramma che sopravvive fuori dalla macro

  // Draw labels
  TLatex label;
  label.SetTextAlign(22); //22= centrale verticalmente e orizzontalmente --> fare diverse prove!
  label.DrawLatex(0.55, 3.0e4, "#eta");
  label.DrawLatex(0.77, 7.0e4, "#rho,#omega");
  label.DrawLatex(1.20, 4.0e4, "#phi");
  label.DrawLatex(4.40, 1.0e5, "J/#psi");
  label.DrawLatex(4.60, 1.0e4, "#psi'");
  label.DrawLatex(12.0, 2.0e4, "Y(1,2,3S)");
  label.DrawLatex(91.0, 1.5e4, "Z");
  label.SetNDC(true); //cambio di coordinate di riferimento da quelle del grafico a quelle del pad normalizzate
  label.SetTextAlign(11); //left bottom
  label.SetTextSize(0.04);
  label.DrawLatex(0.10, 0.92, "#bf{CMS Open Data}");
  label.SetTextAlign(31); //right bottom
  label.DrawLatex(0.90, 0.92, "#sqrt{s} = 8 TeV, L_{int} = 11.6 fb^{-1}");

  // Save plot
  c->SaveAs("dimuonSpectrum_cut_pt.pdf");

  // Print cut-flow report
  report->Print();
}


#endif /* SpettrumPlot_h */
