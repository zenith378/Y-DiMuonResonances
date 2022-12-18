/**************************************************************
 * \file difffCrossection.h
 * \brief Function to plot the differential cross section for the Y1s,2s and 3s in trasverse momentum
 *
 
 *******************************************************************************/

#ifndef difffCrossection_h
#define difffCrossection_h

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "../include/Cuts.h"
#include <filesystem>
#include "TSystem.h"
#include "RooAbsPdf.h"
#include "../include/fitRoo.h"

struct dcsVec;

Double_t dCs(RooAbsPdf * model, int i, float wpt);

dcsVec set(float dpt);

int diffCrossection(ROOT::RDataFrame df);

#endif /* difffCrossection_h */
