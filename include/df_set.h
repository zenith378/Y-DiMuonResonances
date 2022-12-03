//
//  df_set.h
//
//
//  Created by Giulio Cordova on 21/11/22.
//

#ifndef df_set_h
#define df_set_h

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx" //per usare ROOT::VecOps::RVec<T>
#include "Math/Vector4Dfwd.h"
#include "Math/Vector4D.h" //per usare PtEtaPhiMVector e le sue funzioni
#include "TMath.h"
#include "TFile.h"


using namespace ROOT::VecOps;

/*
void printLVec(RVec<float>& pt, RVec<float>& eta, RVec<float>& phi, RVec<float>& mass){
    ROOT::Math::PtEtaPhiMVector m1(pt[0], eta[0], phi[0], mass[0]);
    std::cout << m1 << std::endl;
    return;}
*/

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
/*
float computeInvariantMass(RVec<float>& pt, RVec<float>& eta, RVec<float>& phi, RVec<float>& mass){
    //potremmo implementare queste due righe con un ciclo su uno dei vettori,
    //definire un vettore si vettori di lorentz che se è più lungo di 2 mi
    //restituisce un messaggio di errore, sommare gli elementi del vettore
    //(cioè sommare i duei vettori di L e fare la massa invariante)
    ROOT::Math::PtEtaPhiMVector m1(pt[0], eta[0], phi[0], mass[0]);
    ROOT::Math::PtEtaPhiMVector m2(pt[1], eta[1], phi[1], mass[1]);
    return (m1 + m2).mass();
}
*/


ROOT::RDataFrame df_set(){
  //Enable multi-threading
  ROOT::EnableImplicitMT(1);

  TString fname("Data/data.root");
  //try opening file
  TFile *rootfile = TFile::Open(fname);
  //if file does not open
  if (!rootfile){
    std::cout << "Reading dataset from web"<< std::endl;
    //reading dataframe from online NanoAOD
    ROOT::RDataFrame df_temp("Events", "root://eospublic.cern.ch//eos/opendata/cms/derived-data/AOD2NanoAODOutreachTool/Run2012BC_DoubleMuParked_Muons.root");
    //define more variables in the dataframe
    auto df_set = df_temp.Define("Dimuon_FourVec",computeFourVec,{"Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass"})
                                     .Define("Dimuon_mass", computeFourVecInvariantMass,{"Dimuon_FourVec"})
                                     .Define("Dimuon_pt", computeFourVecPT,{"Dimuon_FourVec"})   //Compute pt and rapidity (y) of dimuon
                                     .Define("Dimuon_beta", computeFourVecBeta,{"Dimuon_FourVec"})
                                     .Define("Dimuon_y", computeFourVecRapidity, {"Dimuon_FourVec"});

    
    //write dataframe to file
    df_set.Snapshot("Events",fname);
    }
  else if(rootfile->IsZombie()){ //file not read corectly
    std::cout << "Problems reading file " << fname << std::endl;
    exit(1);
  }

  // read dataframe from file 
  ROOT::RDataFrame df("Events", fname);

  return df;
}


#endif /* Cuts_h */
