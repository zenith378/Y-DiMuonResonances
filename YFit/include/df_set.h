/**************************************************************
 * \file df_set.h
 * \brief Function for creating and setting up dataframe used in the analysis
 *
 *
 *
 * In this file are defined the functions used in order to read the dataframe (or create it if it is not already there) and setting it up with useful variables
 * 
 * 
 *******************************************************************************/
#ifndef df_set_h
#define df_set_h

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx" 
#include "Math/Vector4Dfwd.h"
#include "Math/Vector4D.h" 
#include "TMath.h"
#include "TFile.h"
#include <TSystem.h>
#include "df_set.h"
#include <filesystem>
#include "optionParse.h"


using namespace ROOT::VecOps;

/***************************************
 * 
 * \brief Calculate the four vector of a dimuon starting from pt, eta, phi and mass of muon pair
 * 
 * Create two four vectors of type PtEtaPhiM from the indeces im0 and im1, then add them two together
 * 
 * @param pt transverse momentum
 * @param eta pseudorapidity
 * @param phi azimuthal angle
 * @param mass mass
 * @param im0 index of first good muon
 * @param im1 index of second good muon
 * 
 * \return PtEtaPhiMVector of dimuon pair
 * 
 ****************************************/
ROOT::Math::PtEtaPhiMVector computeDiMuonFourVec(RVec<float> &pt, RVec<float> &eta, RVec<float> &phi, RVec<float> &mass, unsigned long &im0, unsigned long &im1);
/***************************************
 * 
 * \brief Calculate the invariant mass of a dimuon Four Vector
 * @param fourvec four vector of dimuon pair
 * 
 * \return invariant mass of fourvector
 * 
 ****************************************/
float computeDiMuonInvariantMass(ROOT::Math::PtEtaPhiMVector& fourvec);

/***************************************
 * 
 * \brief Calculate the transverse momentum of a dimuon Four Vector
 * @param fourvec four vector of dimuon pair
 * 
 * \return pt (transverse momentum)
 * 
 ****************************************/
float computeDiMuonPT(ROOT::Math::PtEtaPhiMVector& fourvec);

/***************************************
 * 
 * \brief Calculate the rapidity \begin{equation} \eta=\text{arctanh}(\beta) \end{eqaution } of a dimuon Four Vector
 * @param fourvec four vector of dimuon pair
 * 
 * \return rapidity of fourvector
 * 
 ****************************************/
float computeDiMuonRapidity(ROOT::Math::PtEtaPhiMVector& fourvec);

/***************************************
 * 
 * \brief Calculate beta (p_t/E) of a dimuon Four Vector
 * @param fourvec four vector of dimuon pair
 * 
 * \return beta of fourvector
 * 
 ****************************************/
float computeDiMuonBeta(ROOT::Math::PtEtaPhiMVector& fourvec);

/***************************************
 * 
 * \brief Define the dataframe used in the analysis or it creates it if it is not found
 * 
 * This function tries to open the file data.root in the folder Data. If the folder or the data do not exist, 
 * the function downloads the dataset from the CMS open data web page and save it in a file for further usage.
 * Before saving the file, the dataset is filtered for good muons (as defined in the article) and new columns
 * are defined in order to implement functionalities.
 * 
 * \return Filtered and shaped dataframe 
 * 
 ****************************************/
ROOT::RDF::RNode df_set();


#endif /* Cuts_h */
