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

ROOT::RDF::RNode DFFilter(ROOT::RDataFrame df, int depth);

ROOT::RDF::RNode applyFilter(ROOT::RDF::RNode df_custom_cut, float var, ROOT::RDF::ColumnNames_t obs, std::string_view message);

ROOT::RDF::RNode customFilter(ROOT::RDF::RNode df, float ptm, float ptM, float ym, float yM );

ROOT::RDF::RNode Cuts(ROOT::RDataFrame df, int depth,float ptm, float ptM, float ym, float yM );

#endif /* Cuts_h */
