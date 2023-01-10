/*********************************
 * \file Cuts.h
 * \brief Selection of the data.
 * The data are selected to be two muons of opposite charge with an invariant mass round the mass of the Y resonances.
 * Also it is possible to choose other cuts on the trasverse momentum and the rapidity of the dimuon state.
 ************************************************/

#ifndef Cuts_h
#define Cuts_h

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "Cuts.h"
#include <filesystem>
#include "TSystem.h"
#include "optionParse.h"

/***************************************
 *
 * \brief Apply custom cuts on a variable given string containing the filter. Print a warning if there are less than 800
 *events in the cutted dataframe
 *
 *
 * @param df_custom_cut dataframe to be cutted
 * @param filter string containing the definition of the filter to apply
 * @param name string containing the name of the filter
 *
 * \return Cutted Dataframe
 *
 ****************************************/
ROOT::RDF::RNode applyFilter(ROOT::RDF::RNode &df_custom_cut, std::string_view filter, std::string_view name);

/***************************************
 *
 * \brief Apply custom cuts with the function \c applyFilter() and print a cutt report
 *
 *
 * @param df dataframe to be cutted
 * @param pmr lower extreme for the cut on the dimuon trasverse momentum
 * @param pMr upper extreme for the cut on the dimuon trasverse momentum
 * @param ymr lower extreme for the cut on the dimuon rapidity (in abs values)
 * @param yMr upper extreme for the cut on the dimuon rapidity (in abs values)
 *
 * \return DataFrame with the selection decided by the cuts
 *
 ****************************************/
ROOT::RDF::RNode Cuts(ROOT::RDF::RNode &df, float pmr = std::nanf("1"), float pMr = std::nanf("1"),
                      float ymr = std::nanf("1"), float yMr = std::nanf("1"));

#endif /* Cuts_h */
