#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx" //per usare ROOT::VecOps::RVec<T>
#include "Math/Vector4Dfwd.h"
#include "Math/Vector4D.h" //per usare PtEtaPhiMVector
#include "SpettrumPlot.h"

using namespace ROOT::VecOps;

float computeInvariantMass(RVec<float>& pt, RVec<float>& eta, RVec<float>& phi, RVec<float>& mass) {
  //potremmo implementare queste due righe con un ciclo su uno dei vettori, definire un vettore si vettori di lorentz che se è più lungo di 2 mi restituisce un messaggio di errore, sommare gli elementi del vettore (cioè sommare i duei vettori di L e fare la massa invariante)
  ROOT::Math::PtEtaPhiMVector m1(pt[0], eta[0], phi[0], mass[0]);
  ROOT::Math::PtEtaPhiMVector m2(pt[1], eta[1], phi[1], mass[1]);
  return (m1 + m2).mass();
}

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

  // Book histogram of dimuon mass spectrum                                   
  const auto bins = 30000; // Number of bins in the histogram                 
  const auto low = 0.25; // Lower edge of the histogram                       
  const auto up = 300.0; // Upper edge of the histogram                       
  auto hist = df_mass.Histo1D({"hist", "Dimuon mass", bins, low, up}, "Dimuon_mass");

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
}
