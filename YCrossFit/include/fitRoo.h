/**************************************************************
 *
 * \file fitRoo.h
 * \brief Function for fitting the data and displying the canvas
 *
 *******************************************************************************/

#ifndef fitRoo_h
#define fitRoo_h

#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooBreitWigner.h"
#include "RooAddPdf.h"
#include "RooAbsPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "TH1.h"
#include "RooGenericPdf.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "RooHist.h"
#include "TRootCanvas.h"
#include "TApplication.h"
#include "TLatex.h"
#include "SpectrumPlot.h"
#include "TStyle.h"
#include "TString.h"
#include <string.h>
#include <filesystem>
#include "TROOT.h"

using namespace RooFit;

/***********************************************************
 * \brief Formatting the string to be displayed on the canvas regarding the cuts made on pt.
 *
 * If there are no custom cut, the default ones are printed.
 *
 * @param pmr recerence of ptm (minimum pt) defined in main
 * @param pMr recerence of ptM (maximum pt) defined in main
 *
 * \return string formatted to print on the canvas
 *************************************************************/
TString formatPtString(float pmr = std::nanf("1"), float pMr = std::nanf("1"));

/***********************************************************
 * \brief Formatting the string to be displayed on the canvas regarding the cuts made on y
 *
 * If there are no custom cut, the default ones are printed.
 *
 * @param ymr recerence of ym (minimum rapidity) defined in main
 * @param yMr recerence of yM (maximum rapidity) defined in main
 *
 * \return string formatted to print on the canvas
 *************************************************************/
TString formatYString(float ymr = std::nanf("1"), float yMr = std::nanf("1"));

/***********************************************************
 * \brief Implementation of the fit function
 *
 * Define a model function as sum of a quadratic background and three peak signals
 * that can be chosen between breit wigner, gaussian, or t-student.
 * Fit the histogram in input with the model function and print the result on the terminal.
 * The data, the fitted model and its components are drawn on the canvas.
 * On the canvas are also printed the cuts on tranverse momentum and rapidity, and the luminosity
 * and CM energy as well.
 * The result is saved in a pdf file with the name defined by parameter nfr. If the mode is set
 * to "fit" an application with the canvas is displayed.
 *
 * @param hh histogram to be fitted
 * @param fr reference of fitFunction defined in main
 * @param pmr recerence of ptm (minimum pt) defined in main
 * @param pMr recerence of ptM (maximum pt) defined in main
 * @param ymr recerence of ym (minimum rapidity) defined in main
 * @param yMr recerence of yM (maximum rapidity) defined in main
 * @param nfr recerence of nameFile defined in main
 * @param vr reference of varaible verbose defined in main
 * @param cr canvas flag: if on, do not display application
 *
 * \return result of the fit
 *************************************************************/
RooFitResult *fitRoo(TH1 *hh, int mr=0, int fr = 0, float pmr = std::nanf("1"), float pMr = std::nanf("1"),
                     float ymr = std::nanf("1"), float yMr = std::nanf("1"), std::string fnr = "YResonancesFit",
                     int vr = 0, int cr = 0);

#endif
