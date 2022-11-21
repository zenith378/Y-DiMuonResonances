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
    return (m1 + m2).mass();}



ROOT::RDataFrame Cuts(ROOT::RDataFrame df){
  //Enable multi-threading
  ROOT::EnableImplicitMT(1);

  //seci interessa solo l'ultimo DF, gli altri li possiamo sovrascrivere
  // Select events with exactly two muons
  auto df_2mu = df.Filter("nMuon == 2", "Events with exactly two muons");

  // Select events with two muons of opposite charge
  auto df_os = df_2mu.Filter("Muon_charge[0] != Muon_charge[1]", "Muons with opposite charge");

  //add lorentz four vector to dataframe
  auto df_fv = df_os.Define("Dimuon_FourVec",computeFourVec,{"Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass"});

  // Compute invariant mass of the dimuon system
  //auto df_mass = df_os.Define("Dimuon_mass", computeInvariantMass,{"Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass"});
  auto df_mass = df_fv.Define("Dimuon_mass", computeFourVecInvariantMass,{"Dimuon_FourVec"});

  //Cut around the Ys
  auto df_mass1 = df_mass.Filter("Dimuon_mass > 8.5 ", "First cut on minv");
  auto df_mass2 = df_mass1.Filter("Dimuon_mass < 11.5 ", "Second cut on minv");
    
  //Compute pt and rapidity (y) of dimuon
  auto df_pt = df_mass2.Define("Dimuon_pt", computeFourVecPT,{"Dimuon_FourVec"});
  //auto df_beta = df_pt.Define("Dimuon_beta", computeFourVecBeta,{"Dimuon_FourVec"});
  auto df_y = df_beta.Define("Dinmuon_y", computeFourVecRapidity, {"Dimuon_FourVec"});

  //Select events with 10 GeV < pT < 12 GeV
  //const auto pt_max = 12.;
  //const auto pt_min = 10.;
  auto df_pt1 = df_y.Filter("Dimuon_pt > 10. ", "First cut on pt");
  auto df_pt2 = df_pt1.Filter("Dimuon_pt < 12. ", "Second cut on pt");   //sarebbe possibile unire i due tagli???

    return df_pt2;
}


#endif /* Cuts_h */
