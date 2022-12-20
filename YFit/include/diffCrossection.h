/**************************************************************
 * \file diffCrossection.h
 * \brief Function to plot the differential cross section for the Y1S, 2S and 3S in trasverse momentum
 *
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

/***********************************************************
 * \brief Structure where are saved the value usuful for each bin.
 *
 * In this structure are saved the value useful for each point in trasverse momentum:
 *  - ptm, is the lower edge of the bin
 *  - ptM, is the upper edge of the bin
 *  - s1, s2 and s3, are the value of the differential cross section (multiply by the relative branching ratio) of the three Y resonances: Y(1S), Y(2S) and Y(3S)
 *  - ds1, ds2 and ds3, are the uncertainties on s1, s2 and s3
 *****************************************************************************************/
struct dcsbin;

/********************************************************************************************************************
 * \brief calculate the differential cross section given the functin that describe the mass distribution (model), which Y is (i), and the width of the pt bin (wpt)
 *
 * @param N number of event under the peak of the resonance
 * @param wpt width of the bin
 *
 * @return differential cross section (multiply by the relative branching ratio)
 *******************************************************************************************************************************************************************************/
Double_t diffCrossSec(double N, float wpt);

/********************************************************************************************************************
 * \brief Calculate the differential cross section (multiply by the relative branching ratio) for one bin in trasverso momentum
 *
 * This function select the data from the data frame df using the function Cuts() defined in Cuts.h. The cut on the trasverse momentum is given by the bin edges and the one on the rapidity is between -1.2 and 1.2. The data are plotted and fitted using the function fitRoo defined in fitRoo.h. From the fit we can get the number of events under the peak of each Y resonance and using the function diffCrossSec, defined in diffCrossection.h, calculate the differential cross section for the bin. All the value usuful for the plotting are save inside the structur returned.
 * @param ptm lower edge of the bin
 * @param ptM is the upper edge of the bin
 * @param df data frame with all the data
 * @param namefile name of the plot saved
 *
 * @return Structure where are saved the value usuful for the bin
 *******************************************************************************************************************************************************************************/
dcsbin setset(float ptm, float ptM, ROOT::RDataFrame df, std::string nameFile);


/********************************************************************************************************************
 * \brief Plot the differential cross section calculate for each bin for the Y1S, 2S and 3S as a function of the trasverse momentum
 *
 * The edge of the bin are defined in two array, these are used for calculate the differential cross section in each bin. These are then plotted on the same canva.
 * @param df data frame where the unselected data are stored
 ************************************************************************************************************************************************************************/
int diffCrossection(ROOT::RDataFrame df);

#endif /* difffCrossection_h */
