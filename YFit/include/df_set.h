/**************************************************************
 * \file df_set.h
 * \brief Header file of df_set.C
 *
 *
 *
 * In this file are defined the functions used in order to read the dataframe (or create it if it is not already there) and setting it up with useful variables
 * In particular, it is defined:
 * 
 * computeFourVector(): computes a four vector of a dimuon starting from pt, eta, phi and mass
 * 
 * computeFourVecInvariantMass(): computes invariant mass of given four vector
 * 
 * computeFourVecPT(): computes pt of given FourVector
 * 
 * computeFourVecRapidity(): computes rapidity of given four vector
 * 
 * computeFourVecBeta(): computes beta of given four vector
 * 
 * df_set(): defines the dataframe
 *******************************************************************************/
#ifndef df_set_h
#define df_set_h

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx" //per usare ROOT::VecOps::RVec<T>
#include "Math/Vector4Dfwd.h"
#include "Math/Vector4D.h" //per usare PtEtaPhiMVector e le sue funzioni
#include "TMath.h"
#include "TFile.h"


using namespace ROOT::VecOps;

/***************************************
 * 
 * \brief it calculates the four vector of a dimuon starting from pt, eta, phi and mass of muon pair
 * @param pt transverse momentum
 * @param eta 
 * @param phi 
 * @param mass mass
 * 
 * \return PtEtaPhiMVector
 * 
 ****************************************/
ROOT::Math::PtEtaPhiMVector computeDiMuonFourVec(RVec<float>& pt, RVec<float>& eta, RVec<float>& phi, RVec<float>& mass);

/***************************************
 * 
 * \brief it calculates the invariant mass of a dimuon Four Vector
 * @param fourvec four vector of dimuon pair
 * 
 * \return mass
 * 
 ****************************************/
float computeDiMuonInvariantMass(ROOT::Math::PtEtaPhiMVector& fourvec);

/***************************************
 * 
 * \brief{it calculates the transverse momentum of a dimuon Four Vector}
 * @param fourvec four vector of dimuon pair
 * 
 * \return pt (transverse momentum)
 * 
 ****************************************/
float computeDiMuonPT(ROOT::Math::PtEtaPhiMVector& fourvec);

/***************************************
 * 
 * \brief{it calculates the rapidity of a dimuon Four Vector //inserire formula rapidità}
 * @param fourvec four vector of dimuon pair
 * 
 * \return rapidity
 * 
 ****************************************/
float computeDiMuonRapidity(ROOT::Math::PtEtaPhiMVector& fourvec);

/***************************************
 * 
 * \brief{it calculates beta (p_t/E) of a dimuon Four Vector}
 * @param fourvec four vector of dimuon pair
 * 
 * \return beta
 * 
 ****************************************/
float computeDiMuonBeta(ROOT::Math::PtEtaPhiMVector& fourvec);

/***************************************
 * 
 * \brief{it defines the dataframe used in the analysis or it creates it if it is not found}
 * 
 * \return Dataframe
 * 
 ****************************************/
ROOT::RDF::RNode df_set();


#endif /* Cuts_h */