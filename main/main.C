/**************************************************************
 * \file main.C
 * \brief Main source code of the program: define default options for parameters and call the functions in order to
 *execute the program
 *
 *
 *
 * Here the default options of the command line arguments are defined and the functions are called in order to execute
 *the programs.
 *
 * The default options are:
 *
 * * Depth=0
 * * Fit function=0 (Breit-Wigner)
 * * ptm=ptM=ym=yM=NaN
 * * nameFile="YResonancesFit"
 * * verbose=0, i.e. non verbose
 * * mode=1, i.e. calculate differential cross section
 *
 *
 *******************************************************************************/

#include <iostream>
#include "ROOT/RDataFrame.hxx"
#include "df_set.h"
#include "Cuts.h"
#include "SpectrumPlot.h"
#include "fitRoo.h"
#include "optionParse.h"
#include "diffCrossSection.h"
#include "RooFitResult.h"
#include "TRootCanvas.h"

int main(int argc, char *argv[])
{
   TApplication *theApp;
   // initialize default values for options
   int fitfunc = 0; // Fit Function initilized to 0, i.e. Breit-Wigner
   // The default parameters for the cuts are initialized to NaN, so that they are used only if they become a number
   float ptm = std::nanf("1");
   float ptM = std::nanf("2");
   float ym = std::nanf("3");
   float yM = std::nanf("4");
   std::string nameFile = "YResonancesFit"; // Name of the file in which the figure is saved
   int verbose = 0;                         // verbose flag initialized to zero, i.e. no output stream for Minuit
   int mode = 1;                            // mode initialized to 1, i.e. calculate differential cross section
   int canvas = 0;                          // canvas initialized to 0, i.e. displaycanvas

   // replace the default options with the customized ones when passed to the command line
   ProcessArgs(argc, argv, fitfunc, ptm, ptM, ym, yM, nameFile, verbose, mode, canvas);
   // launch the application if the muteCanvas flag is set to on
   if (canvas == 0)
      theApp = new TApplication("theApp", 0, 0);
   // create and set up the dataframe used in the analysis
   ROOT::RDF::RNode df = df_set();

   if (mode == 0) { // if mode is set to zero, start query for the fit
      ROOT::RDF::RNode df_cut = df;

      if (ptm == ptm || ptM == ptM || ym == ym || yM == yM) // if custom cuts are defined
         df_cut = Cuts(df, ptm, ptM, ym, yM);               // apply cuts to the dataframe

      TH1 *h = SpectrumPlot(df_cut, nameFile, mode); // draw preliminary histogram
      // fit the preliminary histogram to the fitfunction, plot the result and print fit values on terminal
      RooFitResult *fitresult = fitRoo(h, mode, fitfunc, ptm, ptM, ym, yM, nameFile, verbose, canvas);
   }

   if (mode == 1) // if mode is set to one, start query for the differential cross section calculation
   {
      PlotDiffCrossSection(df, ym, yM, canvas);
   }
   // run the application if the muteCanvas flag is set to on
   if (canvas == 0)
      theApp->Run();

   return 0;
}
