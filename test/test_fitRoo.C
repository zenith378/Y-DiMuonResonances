
#include<iostream>
#include "optionParse.h"
#include <cmath>
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooPolynomial.h"
#include "RooDataHist.h"
#include "RooFitResult.h"
#include "fitRoo.h"
#include "TH1.h" 

int main(){
    RooRealVar x("x", "m_{#mu^{+}#mu^{-}} (GeV/c^{2})", 8.5, 11.5);
    RooRealVar a0("a0", "a0",  10213);
    RooRealVar a1("a1", "a1", -114.8);
    RooRealVar a2("a2", "a2", 2.9);
    RooPolynomial bkg("bkg", "Background", x, RooArgSet(a0, a1,a2));
    RooRealVar nback("nback", "back", 0.2*100e3);

    // Create parameters
    RooRealVar mean1("mean1", "mean of gaussians", 9.45);
    RooRealVar mean2("mean2", "mean of gaussians", 10.01);
    RooRealVar mean3("mean3", "mean of gaussians", 10.35);
    RooRealVar sigma1("sigma1", "width of gaussians", 0.054);
    RooRealVar sigma2("sigma2", "width of gaussians", 0.032);
    RooRealVar sigma3("sigma3", "width of gaussians", 0.020);
    RooRealVar nsig1("nsig1", "signal1", 0.3*100e3);
    RooRealVar nsig2("nsig2", "signal2", 0.3*100e3);
    RooRealVar nsig3("nsig3", "signal3", 0.2*100e3);

    RooAbsPdf * sig1 = new RooGaussian("sig1", "Signal component 1", x, mean1, sigma1);
    RooAbsPdf * sig2 = new RooGaussian("sig2", "Signal component 2", x, mean2, sigma2);
    RooAbsPdf * sig3 = new RooGaussian("sig3", "Signal component 3", x, mean3, sigma3);

    // Add signal and background
    RooAddPdf model("model", "model", RooArgList(*sig1, *sig2, *sig3, bkg), RooArgList(nsig1, nsig2, nsig3,nback));

    RooDataSet *data = model.generate(x, 100e3);
    TH1 *hdata = data->createHistogram("x", 300);
    RooFitResult* fitResult = fitRoo(hdata,1);
    
    //se non fa il fit il test fallisce
    if (fitResult->status()!=0) exit(1);// fitResult->covQual() < 2
    
    //manca da controllare che i valori uscenti siano quelli inizializzati
    RooArgList lf = fitResult->floatParsFinal();
    std::cout << lf << std::endl;
    RooArgList lv = {a0,a1,a2,mean1,mean2,mean3,nback,nsig1,nsig2,nsig3,sigma1,sigma2,sigma3};
    const TMatrixDSym cov = fitResult->covarianceMatrix();
    for(int i=0; i<12; i++){
        /* Se la differenza tra il valore vero e quello uscente dal fit è maggiore di una deviazione std allora il test fallisce (return -1)*/
        Double_t a=static_cast<RooAbsReal&>(lf[i]).getVal();
        Double_t b=static_cast<RooAbsReal&>(lv[i]).getVal();
        if(std::abs(a-b) > 5 * std::sqrt(cov(i,i))){
            std::cerr<< "The parameter " <<lf[i] <<" exceeds expected value by " << abs(a-b) <<std::endl;
            exit(1);
        }
    }
    return 0;
}
