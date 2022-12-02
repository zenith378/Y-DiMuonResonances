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
#include "ROOT/RVec.hxx" //per usare ROOT::VecOps::RVec<T>
#include "Math/Vector4Dfwd.h"
#include "Math/Vector4D.h" //per usare PtEtaPhiMVector e le sue funzioni
#include "TMath.h"

using namespace ROOT::VecOps;

void printLVec(RVec<float>& pt, RVec<float>& eta, RVec<float>& phi, RVec<float>& mass){
    ROOT::Math::PtEtaPhiMVector m1(pt[0], eta[0], phi[0], mass[0]);
    std::cout << m1 << std::endl;
    return;}


ROOT::Math::PtEtaPhiMVector computeFourVec(RVec<float>& pt, RVec<float>& eta, RVec<float>& phi, RVec<float>& mass){
    /* Funzione che calcola il quadrivettore: idea di usare i metodi di questa quantità per calcolare
    le quantità di cui abbiamo bisongo (massa invariante, beta, rapidità, etc.)*/
    //potremmo implementare queste due righe con un ciclo su uno dei vettori,
    //definire un vettore si vettori di lorentz che se è più lungo di 2 mi
    //restituisce un messaggio di errore, sommare gli elementi del vettore

    ROOT::Math::PtEtaPhiMVector m1(pt[0], eta[0], phi[0], mass[0]);
    ROOT::Math::PtEtaPhiMVector m2(pt[1], eta[1], phi[1], mass[1]);
    return (m1 + m2);}

float computeFourVecInvariantMass(ROOT::Math::PtEtaPhiMVector& fourvec){
  return fourvec.mass();
}

float computeFourVecPT(ROOT::Math::PtEtaPhiMVector& fourvec){
  return fourvec.pt();
}

float computeFourVecRapidity(ROOT::Math::PtEtaPhiMVector& fourvec){
  return fourvec.Rapidity();
}

float computeFourVecBeta(ROOT::Math::PtEtaPhiMVector& fourvec){
  return fourvec.Beta();
}

float computeInvariantMass(RVec<float>& pt, RVec<float>& eta, RVec<float>& phi, RVec<float>& mass){
    //potremmo implementare queste due righe con un ciclo su uno dei vettori,
    //definire un vettore si vettori di lorentz che se è più lungo di 2 mi
    //restituisce un messaggio di errore, sommare gli elementi del vettore
    //(cioè sommare i duei vettori di L e fare la massa invariante)
    ROOT::Math::PtEtaPhiMVector m1(pt[0], eta[0], phi[0], mass[0]);
    ROOT::Math::PtEtaPhiMVector m2(pt[1], eta[1], phi[1], mass[1]);
    return (m1 + m2).mass();
}



ROOT::RDF::RInterface<ROOT::Detail::RDF::RJittedFilter, void> Cuts(ROOT::RDataFrame df){
  //Enable multi-threading
  ROOT::EnableImplicitMT(1);
  auto df_def = df.Define("Dimuon_FourVec",computeFourVec,{"Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass"}) //add lorentz four vector to dataframe
                  .Define("Dimuon_mass", computeFourVecInvariantMass,{"Dimuon_FourVec"})  // Compute invariant mass of the dimuon system
                  .Define("Dimuon_pt", computeFourVecPT,{"Dimuon_FourVec"})   //Compute pt and rapidity (y) of dimuon
                  .Define("Dimuon_beta", computeFourVecBeta,{"Dimuon_FourVec"})
                  .Define("Dinmuon_y", computeFourVecRapidity, {"Dimuon_FourVec"});

  //seci interessa solo l'ultimo DF, gli altri li possiamo sovrascrivere
  auto df_cut = df_def.Filter("nMuon == 2", "Events with exactly two muons")   // Select events with exactly two muons
                      .Filter("Muon_charge[0] != Muon_charge[1]", "Muons with opposite charge") // Select events with two muons of opposite charge
                      //.Filter([](double Dimuon_mass) { return Dimuon_mass> 8.5 && Dimuon_mass < 11.5; })
                      //.Filter([](double Dimuon_pt) { return Dimuon_pt > 10. && Dimuon_pt < 12.; });
                      .Filter("Dimuon_mass > 8.5 ", "First cut on minv")  //Cut around the Ys
                      .Filter("Dimuon_mass < 11.5 ", "Second cut on minv")
                      .Filter("Dimuon_pt > 10. ", "First cut on pt")   //Select events with 10 GeV < pT < 12 GeV
                      .Filter("Dimuon_pt < 12. ", "Second cut on pt");
  
  //const auto pt_max = 12.;
  //const auto pt_min = 10.;
  df_cut.Snapshot("Cuts","./Data/data_cut.root");
  return df_cut;
}


#endif /* Cuts_h */
