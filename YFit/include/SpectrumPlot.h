/**************************************************************
 * \file SpectrumPlot.h
 * \brief Function to plot the spectrum of the dataset which is currently used and create an histogram. 
 *
 
 *******************************************************************************/

#ifndef SpectrumPlot_h
#define SpectrumPlot_h

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TStyle.h"
#include "Cuts.h"

/***********************************************************
 * Simple Function to plot the spectrum of the dataset which is currently used and create an histogram. 
 * @param df_cut dataframe containg the data
 * \return histogram of the data
 *************************************************************/
TH1* SpectrumPlot(ROOT::RDF::RNode df_cut);


#endif /* SpectrumPlot_h */
