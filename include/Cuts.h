//
//  Cuts.h
//
//
//  Created by Giulio Cordova on 21/11/22.
//
// è da modificare. io voglio che in questo file ci sia una funzione che mi faccia dei tagli a seconda dei paramteri di 
// ingresso della funzione cuts. Se nessun parametro è impostato fa i tagli nostri.

#ifndef Cuts_h
#define Cuts_h

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"

ROOT::RDataFrame Cuts(ROOT::RDataFrame df);

/*
ROOT::RDataFrame Cuts(ROOT::RDataFrame df){
  //Enable multi-threading
  ROOT::EnableImplicitMT(1);

  //Events selection
  TString fname("Data/data_cut.root");
  TFile *cutfile = TFile::Open(fname); //try open cut file
  if (!cutfile){ //if the cutfile does not exist
    std::cout << "Recreating cut file"<< std::endl;

    auto df_cut = df.Filter([](unsigned int x) { return x==2;}, {"nMuon"}, {"Events with exactly two muons"})   // Select events with exactly two muons
                    .Filter("Muon_charge[0] != Muon_charge[1]", "Muons with opposite charge") // Select events with two muons of opposite charge
                    .Filter([](float x) { return x> 8.5 && x < 11.5; }, {"Dimuon_mass"}, {"Inviariant mass between 8.5 and 11.5"}) //Cut around the Ys
                    .Filter([](float x) { return x > 10. && x < 12.; }, {"Dimuon_pt"}, {"Pt between 10 and 100 GeV"}) //Select events with 10 GeV < pT < 12 GeV
                    .Filter([](float x) { return x > -0.6 && x < 0.6; }, {"Dimuon_y"}, {"Rapidity between -0.6 and 0.6"});

    // Request cut-flow report
    auto report = df_cut.Report();
    // Print cut-flow report
    report->Print();  
    //save cut file
    df_cut.Snapshot("Cuts",fname);
    }

  else if(cutfile->IsZombie()){ //file not read correctly
    std::cout << "Problems reading file" << fname << std::endl;
    exit(1);
  }
  ROOT::RDataFrame df_cut("Cuts", fname); 
  
  //const auto pt_max = 12.;
  //const auto pt_min = 10.;
  return df_cut;
}
*/

#endif /* Cuts_h */
