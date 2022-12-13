//
//  Cuts.h
//
//
//  Created by Giulio Cordova on 21/11/22.
//
#ifndef Cuts_h
#define Cuts_h

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"

ROOT::RDF::RNode DFFilter(ROOT::RDataFrame df, int &dr);

ROOT::RDF::RNode applyFilter(ROOT::RDF::RNode df_custom_cut, std::string_view filter, std::string_view name);

ROOT::RDF::RNode customFilter(ROOT::RDataFrame df, float &pmr, float &pMr, float &ymr, float &yMr );

ROOT::RDataFrame generateDataFrame(ROOT::RDataFrame df, int &dr);

ROOT::RDF::RNode Cuts(ROOT::RDataFrame df, int &dr,float &pmr, float &pMr, float &ymr, float &yMr );

#endif /* Cuts_h */
