/************************************************************
* \file testRes.C
* \brief Unit test per decidere come implementare il grafico dei resuidi del fit di un istogramma
**********************************************************************************************/
#include "TDirectory.h"
#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TStyle.h"
#include "TRootCanvas.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"

using namespace RooFit;

/***************************************************************************
* \brief residui calcolati manualmente
*******************************************************************************************************************/
void plotResidual1() {
   //generazione di eventi distribuiti come una gaussiana
   TH1D * h1 = new TH1D("h1","h1",50,-3, 3);
   h1->FillRandom("gaus");

   // fit the histogram
   h1->Fit("gaus");

   // make residual plot
   TH1D * h2 = new TH1D("h2","Fit Residuals",50,-3, 3);
   TF1 * fittedFunc = h1->GetFunction("gaus");
   
   for (int ibin = 1; ibin <= 50; ++ibin) {
      double res =  (h1->GetBinContent(ibin)- fittedFunc->Eval( h1->GetBinCenter(ibin) ) )/h1->GetBinError(ibin);
      h2->SetBinContent(ibin, res  );
      h2->SetBinError(ibin, 1  );
   } 

   TCanvas * c1 = new TCanvas(); 
   c1->Divide(1,2);
   c1->cd(1);
   h1->Draw(); 
   c1->cd(2);
   h2->Draw("E");
}

/***************************************************************************
* \brief residui calcolati con il tool RooFit
*******************************************************************************************************************/
void plotResidual2() {
    
    //creiamo una pdf gaussiana con valori fissati
    RooRealVar x("x", "x", -3, 3);
    RooRealVar mean("mean", "mean of gaussian", 0, -3, 3);
    RooRealVar sigma("sigma", "width of gaussian", 1, 0.1, 3);
    RooGaussian gauss("gauss", "gaussian PDF", x, mean, sigma);
    
    //generiamo un sample di eventi distribuiti come la gaussiana ddescritta sopra
    RooDataSet *dataset = gauss.generate(x, 10000);
    
    mean.setVal(2);
    // Fit pdf only to data in "signal" range
    RooFitResult *r = gauss.fitTo(*dataset);
    
    //disegnamo la funzione gaussiana e il set generato
    RooPlot *frame1 = x.frame(Title("Data with distorted Gaussian pdf"), Bins(40));
    dataset->plotOn(frame1, DataError(RooAbsData::SumW2));
    gauss.plotOn(frame1);
    
    // Construct a histogram with the residuals of the data w.r.t. the curve
    RooHist *hpull = frame1->pullHist();
    RooPlot *frame2 = x.frame(Title("Residual Distribution"));
    frame2->addPlotable(hpull, "P");
    
    //mostra la giusta cancvas
    TCanvas * c3 = new TCanvas();
    c3->Divide(1,2);
    c3->cd(1);
    frame1->Draw();
    c3->cd(2);
    frame2->Draw();
    
//    RooHist* pull = frame_d0_pip->pullHist(data, fittedFunc, normaliza=true);
//    RooPlot* pframe = d0_pip_momentum.frame();
//    pframe->addPlotable(pull, "P");

 }

int testRes(){
    plotResidual1();
    plotResidual2();
    return 0;
}
