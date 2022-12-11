//
//  fitp1.h
//  
//
//  Created by Matilde Carminati on 22/11/22.
//

#ifndef fitp1_h
#define fitp1_h

#include "TDirectory.h"
#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
//#include "TLegend.h"
//#include "TCanvas.h"
#include "TStyle.h"
#include "TFitResult.h"
#include "TMatrixD.h"



Double_t lorentzianPeak(Double_t *x, Double_t *par);

Double_t gaussianPeak(Double_t *x, Double_t *par);

Double_t background(Double_t *x, Double_t *par);

Double_t fitfunction(Double_t *x, Double_t *par);

Double_t findxPeaks(Double_t centralbin, Double_t slope, Double_t background, TH1 *h);

Double_t findHwhm(Int_t binpeak,Double_t slope, Double_t background, TH1 *h);

TFitResultPtr fitp1( TH1* h, Double_t x1=1, Double_t x9=0 );

#endif /* fitp1_h */
