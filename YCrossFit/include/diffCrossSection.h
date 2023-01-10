/**************************************************************
 * \file diffCrossSection.h
 * \brief Function to calculate and plot the differential cross section for the Y(1S), Y(2S) and Y(3S) in trasverse
 *momentum
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
#include "diffCrossSection.h"
#include "TApplication.h"
#include "TRootCanvas.h"
#include "TGraph.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "SpectrumPlot.h"
#include "TROOT.h"

/***********************************************************
 * \brief Structure where are saved the value useful for each bin.
 *
 * In this structure are saved the value useful for each point in trasverse momentum.
 *****************************************************************************************/
struct dcsbin {
   float ptm; ///< lower edge of the bin
   float ptM; ///< upper edge of the bin
   Double_t
      s1; ///< value of the differential cross section (multiply by the relative branching ratio) of the Y(1S) resonance
   Double_t
      s2; ///< value of the differential cross section (multiply by the relative branching ratio) of the Y(2S) resonance
   Double_t
      s3; ///< value of the differential cross section (multiply by the relative branching ratio) of the Y(3S) resonance
   Double_t ds1; ///< uncertainties on s1
   Double_t ds2; ///< uncertainties on s2
   Double_t ds3; ///< uncertainties on s3
};

/********************************************************************************************************************
 * \brief Calculate the differential cross section given the number of events under the peak of the Y resonance and the
 *width of the \f$p_T \f$ bin (wpt)
 *
 * The equation used for calculating the different cross section
 * \f[
 * \frac{d\sigma}{dp_t} = \frac{N}{L \Delta p_t e_{uu} e_{sg} e_{vp} A}
 * \f]
 *
 * where \f$N\f$ is a fit parameter that says how many events are under the signal function and
 * \f$\Delta p_T\f$ is the width of the bin in \f$p_T\f$ (i.e. the width of cut
 * on the trasverse momentum). The value of the acceptance A is fixed to 1,
 * and the other values are some costants taken from the article
 * ["Measurements of the Υ(1S), Υ(2S), and Υ(3S) differential cross sections in pp collisions at \f$\sqrt(s) = 7 \f$
 *TeV"] (https://arxiv.org/pdf/1501.07750.pdf).
 *
 * @param N number of event under the peak of the resonance
 * @param wpt width of the bin
 *
 * @return differential cross section (multiplied by the relative branching ratio)
 *******************************************************************************************************************************************************************************/
Double_t diffCrossSec(double N, float wpt);

/********************************************************************************************************************
 * \brief Create a stucture where it save the edges of the bins and the cross section for each Y resonance.
 *
 * This function select the data from the data frame df using the function Cuts() defined in Cuts.h. The cut on the
 *trasverse momentum is given by the bin edges and the one on the rapidity is between\f$ -1.2\f$ and \f$1.2\f$. The data
 *are plotted and fitted using the function fitRoo defined in fitRoo.h. From the fit we can get the number of events
 *under the peak of each Y resonance and using the function diffCrossSec, defined in diffCrossection.h, calculate the
 *differential cross section for the bin. All the value useful for the plotting are save inside the structur returned.
 * @param ptm lower edge of the bin
 * @param ptM upper edge of the bin
 * @param df Dataframe with all the data
 * @param namefile name of the plot saved
 *
 * @return Structure where are saved the value usuful for the bin
 *******************************************************************************************************************************************************************************/
dcsbin setset(float ptm, float ptM, float ym, float yM, ROOT::RDF::RNode &df, std::string nameFile);

/********************************************************************************************************************
 * \brief Plot the differential cross section calculate for each bin for the Y(1S), Y(2S) and Y(3S) as a function of the
 *trasverse momentum
 *
 * The edge of the bin are defined in two array, these are used for calculate the differential cross section in each
 *bin. These are then plotted on the same canva.
 * @param df Dataframe where the unselected data are stored
 * @param ym lower edge on absolute value of rapidity
 * @param yM upper edge on absolute value of rapidity
 * @param cr canvas flag: if on, do not display application
 *
 ************************************************************************************************************************************************************************/
void PlotDiffCrossSection(ROOT::RDF::RNode &df, float ym = std::nanf("1"), float yM = 1.2, int cr = 0);

#endif /* difffCrossection_h */
