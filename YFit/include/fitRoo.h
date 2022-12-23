/**************************************************************
 * \file fitRoo.h
 * \brief Function for fitting the data and displying the canvas
 *
 *
 *
 * In this file are defined the function used to fit the data and to fit the canvas
 * In particular, it is defined:
 * 
 * formatPtString: it formats the string regarding the cut on pt to be displayed on the canvas
 * 
 * formatYString: it format the string regarding the cut on Y to be displayed on the canvas
 * 
 * fitRoo: function that fit the data, print the result and the canvas
 *******************************************************************************/

#ifndef fitRoo_h
#define fitRoo_h

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "TH1.h"
#include "TString.h"
#include "RooDataHist.h"

using namespace RooFit;
 
/***********************************************************
 * Formatting the string to be displayed on the canvas regarding the cuts made on pt
 * 
 * @param dr reference of depth defined in main
 * @param pmr recerence of ptm (minimum pt) defined in main
 * @param pMr recerence of ptM (maximum pt) defined in main
 * 
 * \return result of the fit
 *************************************************************/
TString formatPtString(int dr=0, float pmr= std::nanf("1"), float pMr= std::nanf("1"));

/***********************************************************
 * Formatting the string to be displayed on the canvas regarding the cuts made on y
 * 
 * @param dr reference of depth defined in main
 * @param ymr recerence of ym (minimum rapidity) defined in main
 * @param yMr recerence of yM (maximum rapidity) defined in main
 * 
 * \return result of the fit
 *************************************************************/
TString formatYString(int dr=0, float ymr= std::nanf("1"), float yMr= std::nanf("1"));

/***********************************************************
 * Implementation of the fit function
 * 
 * @param hh histogram to be fitted
 * @param fr reference of fitFunction defined in main
 * @param dr reference of depth defined in main
 * @param pmr recerence of ptm (minimum pt) defined in main
 * @param pMr recerence of ptM (maximum pt) defined in main
 * @param ymr recerence of ym (minimum rapidity) defined in main
 * @param yMr recerence of yM (maximum rapidity) defined in main
 * @param nfr recerence of nameFile defined in main
 * @param vr reference of varaible verbose defined in main
 * 
 * \return result of the fit
 *************************************************************/
RooFitResult* fitRoo(TH1* hh, int fr=0, int dr=0, float pmr= std::nanf("1"), float pMr= std::nanf("1"), float ymr= std::nanf("1"), float yMr= std::nanf("1"), std::string fnr="YResonancesFit", int vr=0);

#endif
