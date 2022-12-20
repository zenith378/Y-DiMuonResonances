/*********************************
 * \file Cuts.h
 * \brief Selection of the data.
 * 
 * The data are selected to be two muons of opposite charge with an invariant mass round the mass of the Y resonances.
 * Also it is possible to choose other cuts on the trasverse momentum and the pseudorapidity of the dimuon state.
 ************************************************/

#ifndef Cuts_h
#define Cuts_h

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"

/***************************************
 * 
 * \brief it applies default cuts 
 * 
 * Depth 0: two muons of opposite charge and invariant mass between 8.5 and 11.5 GeV
 * Depth 1: select dimuon pT between 10 and 100 GeV
 * Depth 2: select dimuon pT between 10 and 100 GeV
 *          and a rapidity less than 0.6 in abs value
 * 
 * @param df dataframe to be cutted
 * @param dr depth of the cut to be applied
 * 
 * \return Cutted Dataframe
 * 
 ****************************************/
ROOT::RDF::RNode DFFilter(ROOT::RDataFrame df, int &dr);

/***************************************
 * 
 * \brief it applies custom cuts on a variable given string containing the filter 
 * 
 *
 * @param df_custom_cut dataframe to be cutted
 * @param filter string containing the definition of the filter to apply
 * @param name string containing the name of the filter
 * 
 * \return Cutted Dataframe
 * 
 ****************************************/
ROOT::RDF::RNode applyFilter(ROOT::RDF::RNode df_custom_cut, std::string_view filter, std::string_view name);

/***************************************
 * 
 * \brief looks for active otpion parameters and applies custom cuts
 * 
 *
 * @param df dataframe to be cutted
 * @param pmr minimum pt
 * @param pMr maximum pt
 * @param ymr minimum rapidity
 * @param yMr maximum rapidity
 * 
 * \return Cutted Dataframe
 * 
 ****************************************/
ROOT::RDF::RNode customFilter(ROOT::RDataFrame df, float &pmr, float &pMr, float &ymr, float &yMr );

/***************************************
 * 
 * \brief tries to read cutted dataframe from file and generates it, if it does not exists
 * 
 *
 * @param df dataframe to be cutted
 * @param dr depth set
 * 
 * \return Cutted Dataframe
 * 
 ****************************************/
ROOT::RDataFrame generateDataFrame(ROOT::RDataFrame df, int &dr);

/*********************************
 \brief Modified a muon DataFrame cutting on nMuon, Muon_charge, Dimuon_mass, Dimuon_pt and Dimuon_y

 UNa descrizione più dettagliata della funzione
 @param df Data Frame in input
 @param pmr lower extreme for the cut on the dimuon trasverse momentum
 @param pMr upper extreme for the cut on the dimuon trasverse momentum
 @param ymr lower extreme for the cut on the dimuon pseudorapidity
 @param yMr upper extreme for the cut on the dimuon pseudorapidity
 \return df_cut DataFrame with the selection decided by the cuts
************************************************/
ROOT::RDF::RNode Cuts(ROOT::RDataFrame df, int &dr,float &pmr, float &pMr, float &ymr, float &yMr );

#endif /* Cuts_h */
