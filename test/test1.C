#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooBreitWigner.h"
#include "RooAddPdf.h"
#include "RooAbsPdf.h"
#include "RooGenericPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "TH1.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "RooHist.h"
#include "TRootCanvas.h"
#include "TApplication.h"
#include "TStyle.h"
#include <string.h>
#include "fitRoo.h"

using namespace RooFit;

int test1(){
    int j=0;
    RooRealVar x("x", "m_{#mu^{+}#mu^{-}} (GeV/c^{2})", 8.5001, 11.5);
    
    // Build polynomial pdf
    RooRealVar a0("a0", "a0", 3101, 0, 10000);
    RooRealVar a1("a1", "a1", -140.3, -300., 0.);
    RooRealVar a2("a2", "a2", 0, -30., 30.);
    RooPolynomial bkg("bkg", "Background", x, RooArgSet(a0, a1, a2));

    // Create three Gaussian PDFs and their parameters
    RooRealVar mean1("mean1", "mean of gaussians", 9.);
    RooRealVar mean2("mean2", "mean of gaussians", 9.5);
    RooRealVar mean3("mean3", "mean of gaussians", 10.);
    RooRealVar sigma1("sigma1", "width of gaussians", 0.15, 0.001, 1);
    RooRealVar sigma2("sigma2", "width of gaussians", 0.2, 0.001, 1);
    RooRealVar sigma3("sigma3", "width of gaussians", 0.12, 0.001, 1);

    RooRealVar fsig1("fsig1", "signal1", 0.6, 0., 1.);
    RooRealVar fsig2("fsig2", "signal2", 0.3, 0., 1.);
    RooRealVar fsig3("fsig3", "signal3", 0.2, 0., 1.);

    RooAbsPdf * sig1 = new RooGaussian("sig1", "Signal component 1", x, mean1, sigma1);
    RooAbsPdf * sig2 = new RooGaussian("sig2", "Signal component 2", x, mean2, sigma2);
    RooAbsPdf * sig3 = new RooGaussian("sig3", "Signal component 3", x, mean3, sigma3);

    // Add signal and background
    RooAddPdf model("model", "model", RooArgList(*sig1, *sig2, *sig3, bkg), RooArgList(fsig1, fsig2, fsig3), kTRUE);

    RooDataSet *data = model.generate(x, 100e3);
    TH1 *hdata = data->createHistogram("x", 300);
    RooFitResult* fitResult = fitRoo(hdata, 1, 0, std::nanf("1"), std::nanf("2"), std::nanf("3"), std::nanf("4"), "testFit", 0);
    
    //se non fa il fit il test fallisce
    if (fitResult->status()!=0) j=-1;// fitResult->covQual() < 2
    
    //manca da controllare che i valori uscenti siano quelli inizializzati
    RooArgList lf = fitResult->floatParsFinal();
    RooArgList lv = [a0,a1,a2,fsig1,fsig2,fsig3,mean1,mean2,mean3,sigma1,sigma2,sigma3];
    const TMatrixDSym cov = fitResult->covarianceMatrix();
    for(int i=0; i<12; i++){
        /* Se la differenza tra il valore vero e quello uscente dal fit è maggiore di una deviazione std allora il test fallisce (return -1)*/
        Double_t a=static_cast<RooAbsReal&>(lf[i]).getVal();
        Double_t b=static_cast<RooAbsReal&>(lv[i]).getVal();
        if(std::abs(a-b) > 1 * std::sqrt(cov(i,i))){
            j=-1;
        }
    }
    return j;
}
