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

ROOT::RDataFrame Cuts(ROOT::RDataFrame df){
  //Enable multi-threading
  ROOT::EnableImplicitMT(1);

  //Events selection
  TFile *cutfile = TFile::Open("./Data/data_cut.root"); //try open cut file
  if (!cutfile){ //if the cutfile does not exist
    std::cout << "Recreating cut file"<< std::endl;

    auto df_cut = df.Filter("nMuon == 2", "Events with exactly two muons")   // Select events with exactly two muons
                    .Filter("Muon_charge[0] != Muon_charge[1]", "Muons with opposite charge") // Select events with two muons of opposite charge
                    //.Filter([](double Dimuon_mass) { return Dimuon_mass> 8.5 && Dimuon_mass < 11.5; })
                    //.Filter([](double Dimuon_pt) { return Dimuon_pt > 10. && Dimuon_pt < 12.; });
                    .Filter("Dimuon_mass > 8.5 ", "First cut on minv")  //Cut around the Ys
                    .Filter("Dimuon_mass < 11.5 ", "Second cut on minv")
                    .Filter("Dimuon_pt > 10. ", "First cut on pt")   //Select events with 10 GeV < pT < 12 GeV
                    .Filter("Dimuon_pt < 12. ", "Second cut on pt")
                    .Filter("Dimuon_y < 0.6 ", "First cut on rapidity")
                    .Filter("Dimuon_y > -0.6 ", "Second cut on rapidity");
    //save cut file
    df_cut.Snapshot("Cuts","./Data/data_cut.root");
    }

  else if(cutfile->IsZombie()){ //file not read correctly
    std::cout << "Problems reading file ./Data/data.root"<< std::endl;
    exit(1);
  }
  ROOT::RDataFrame df_cut("Cuts", "./Data/data_cut.root"); 
  
  //const auto pt_max = 12.;
  //const auto pt_min = 10.;
  return df_cut;
}


#endif /* Cuts_h */
