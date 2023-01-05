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
 * \brief Apply default cuts.
 *
 * The function cut the inserted dataframe with the default cuts:
 * * Depth 0: two muons of opposite charge and invariant mass between 8.5 and 11.5 GeV
 * * Depth 1: select dimuon trasverse momentum between 10 and 100 GeV
 * * Depth 2: select dimuon trasverse momentum between 10 and 100 GeV
 *          and a rapidity less than 0.6 in absolute value
 *
 * @param df dataframe to be cutted
 * @param dr depth of the cut to be applied
 *
 * \return Cutted Dataframe
 *
 ****************************************/
ROOT::RDF::RNode DFFilter(ROOT::RDF::RNode &df, int dr = 0);

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
 * \brief Looks for active otpion parameters and applies custom cuts with the function \c applyFilter().
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
ROOT::RDF::RNode customFilter(ROOT::RDF::RNode &df, float pmr = std::nanf("1"), float pMr = std::nanf("1"),
                              float ymr = std::nanf("1"), float yMr = std::nanf("1"));

/***************************************
 *
 * \brief Try to read cutted dataframe from file. If it does not exists, generate it.
 *
 * The function tries to open the cutted dataframe stored in file. If the file does not exists or the directory *Data*
 *does not exists, it creates them.
 *
 *
 * @param df dataframe to be cutted
 * @param dr depth set
 *
 * \return Cutted Dataframe
 *
 ****************************************/
ROOT::RDF::RNode generateDataFrame(ROOT::RDF::RNode &df, int dr = 0);

/*********************************
 \brief Apply default cuts set by depth. If there are any custom cuts, apply them, print a cut report and return df.

 @param df input dataframe
 @param dr depth set
 @param pmr lower extreme for the cut on the dimuon trasverse momentum
 @param pMr upper extreme for the cut on the dimuon trasverse momentum
 @param ymr lower extreme for the cut on the dimuon rapidity (in abs values)
 @param yMr upper extreme for the cut on the dimuon rapidity (in abs values)
 \return DataFrame with the selection decided by the cuts
************************************************/
ROOT::RDF::RNode Cuts(ROOT::RDF::RNode &df, int dr = 0, float pmr = std::nanf("1"), float pMr = std::nanf("1"),
                      float ymr = std::nanf("1"), float yMr = std::nanf("1"));

#endif /* Cuts_h */
