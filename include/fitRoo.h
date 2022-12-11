//
//  fitRoo.h
//
//
//  Created by Giulio Cordova on 5/12/22.
//


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
#include "RooDataHist.h"

using namespace RooFit;
 
std::string formatPtString(int &dr, float &pmr, float &pMr);

std::string formatYString(int &dr, float &ymr, float &yMr);

RooFitResult* fitRoo(TH1* hh, int &fr, int &dr, float &pmr, float &pMr, float &ymr, float &yMr, std::string &fnr, int &vr);

#endif