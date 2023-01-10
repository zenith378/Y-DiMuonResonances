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
#include <filesystem>
#include "TStyle.h"
#include "optionParse.h"

/***********************************************************
 * \brief Save the plot created in the folder Plots.
 *
 * If the folder does not exits, it creates it.
 * @param c1 canvas containing the plot
 * @param nameFile name to give to the pdf
 *
 *************************************************************/
void SavePlot(TCanvas *c, TString namePlot);

/***********************************************************
 * \brief Create histogram of currently used df, and plot the spectrum.
 *
 * @param df_cut Dataframe containg the data
 * @param nameFile name to give to the pdf (a string "preliminary" is going to be added to the filename)
 * @param mr reference of mode. If fit mode is selected, display canvas.
 * \return histogram of the data
 *************************************************************/
TH1 *SpectrumPlot(ROOT::RDF::RNode &df_cut, std::string nameFile, int mr=0);

#endif /* SpectrumPlot_h */
