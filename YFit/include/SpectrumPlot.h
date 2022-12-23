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
 * Function to save the plot created in the folder Plots. If the folder does not exits, it creates it. 
 * @param c1 canvas containing the plot
 * @param nameFile name to give to the pdf
 * \return histogram of the data
 *************************************************************/
void SavePlot(TCanvas *c, TString namePlot);

/***********************************************************
 * Function to plot the spectrum of the dataset which is currently used and create an histogram. 
 * @param df_cut Dataframe containg the data
 * @param nameFile name to give to the pdf (a string "preliminary" is going to be added to the filename)
 * \return histogram of the data
 *************************************************************/
TH1* SpectrumPlot(ROOT::RDF::RNode &df_cut,std::string nameFile);


#endif /* SpectrumPlot_h */