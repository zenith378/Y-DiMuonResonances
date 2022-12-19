/**************************************************************
 * \file difffCrossection.h
 * \brief Function to plot the differential cross section for the Y1s,2s and 3s in trasverse momentum
 *
 
 *******************************************************************************/

#ifndef difffCrossection_h
#define difffCrossection_h

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "RooFitResult.h"
#include <filesystem>
#include "TSystem.h"
#include "RooAbsPdf.h"
#include "Cuts.h"
#include "SpectrumPlot.h"
#include "fitRoo.h"
#include "optionParse.h"
#include "diffCrossection.h"
#include "TApplication.h"
#include "TRootCanvas.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TMultiGraph.h"

struct dcsVec;

Double_t dCs(RooAbsPdf * model, int i, float wpt);

dcsVec set(float dpt);

int diffCrossection(ROOT::RDataFrame df);

#endif /* difffCrossection_h */
