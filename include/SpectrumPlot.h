//
// gcc -c -fpic SpettrumPlot.c Cuts.c df_set.c fitlp2.c fitp1.c -o librarycode.o
// gcc -shared -fpic -o libnamelibrary.so librarycode.o
//
//  SpettrumPlot.h
//
//
//  Created by Giulio Cordova on 10/11/22.
//
//

#ifndef SpectrumPlot_h
#define SpectrumPlot_h

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TStyle.h"
#include "Cuts.h"

using namespace ROOT::VecOps;

TH1* SpectrumPlot(ROOT::RDataFrame df_cut);


#endif /* SpectrumPlot_h */
