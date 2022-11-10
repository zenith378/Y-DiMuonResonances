//
//  fitlp2.h
//
//
//  Created by Giulio Cordova on 10/11/22.
//
//

#ifndef fitlp2_h
#define fitlp2_h

#include "TDirectory.h"
#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
//#include "TLegend.h"
//#include "TCanvas.h"
#include "TStyle.h"
#include "TFitResult.h"
#include "TMatrixD.h"


Double_t lorentzianPeak(Double_t *x, Double_t *par); //Funzione di fit del picco lorentziano

Double_t background(Double_t *x, Double_t *par); //Funzione di fit per il background

Double_t fitfunction(Double_t *x, Double_t *par); // funzione che esegue il fit

#endif /* fitlp2_h */
