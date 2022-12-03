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


Double_t lorentzianPeak(Double_t *x, Double_t *par) {
    /*Definition of the lorentzian pdf:
     par[0] is the area
     par[2] is FWHWM
     par[1] is the mean*/
    return (par[0]) / TMath::Max(1.e-10,(x[0]-par[1])*(x[0]-par[1])+ par[2]*par[2]);
}

Double_t gaussianPeak(Double_t *x, Double_t *par) {
    return par[0]*exp(-0.5* ((x[0]-par[1])/par[2]) * ((x[0]-par[1])/par[2]));
}

// Quadratic background function
Double_t background(Double_t *x, Double_t *par) {
    return par[0] + par[1]*x[0];
}

// Sum of background and peak function
Double_t fitfunction(Double_t *x, Double_t *par) {

        if(par[11]==0) return background(x,par) + lorentzianPeak(x,&par[2])+ lorentzianPeak(x,&par[5])+ lorentzianPeak(x,&par[8]);
        if(par[11]==1) return background(x,par) + gaussianPeak(x,&par[2])+ gaussianPeak(x,&par[5])+ gaussianPeak(x,&par[8]);
        else exit(1);
}

Double_t findPeaks(Double_t centralbin, Double_t slope, Double_t background, TH1 *h) {
    Double_t initial_x=h->FindBin(centralbin);
    Double_t initial_guess=(h->GetBinContent(initial_x)-background-slope*initial_x);
    Double_t peak=initial_guess;
    for( int i=initial_x-3; i <= initial_x+3; i++ ) {
        if( h->GetBinContent(i) > peak ) peak = h->GetBinContent(i);
    }
    return peak;
}

//----------------------------------------------------------------------
TFitResultPtr fitp1( TH1* h, Double_t x1=1, Double_t x9=0 )
{
    
    if( h == NULL ){
        std::cout << "histogram does not exist\n";
        return nullptr;
    }
    
    h->SetMarkerStyle(21);
    h->SetMarkerSize(0.2);
    h->SetStats(1);
    
    gStyle->SetOptFit(101);
    gStyle->SetStatX(0.95); // right edge
    gStyle->SetStatY(0.95); // top edge
    gStyle->SetStatW(0.15); // width
    gStyle->SetStatH(0.15); // height
    
    //gROOT->ForceStyle();
    
    Double_t bwt2 = h->GetBinWidth(1);
    
    Int_t nb = h->GetNbinsX();
    
    if( x9 < x1 ) {
        x1 = h->GetBinCenter(1);
        x9 = h->GetBinCenter(nb);
    }
    Int_t i1 = h->FindBin(x1);
    Int_t i9 = h->FindBin(x9);
    Double_t n1 = h->GetBinContent(i1);
    Double_t n9 = h->GetBinContent(i9);
    Double_t slp = (n9-n1)/(x9-x1);
    Double_t bg = n1 - slp*x1; //NO VA IMPLEMENTATA MEGLIO PERCHè BG è PER X=0
    
    
    // find peak in boundaries:
    
    double npk = bg;
    double xpk = 0.5*(x1+x9);
    
    double xhwhm = x1;
    
    for( int ii = i1; ii <= i9; ++ii ) {
        if( h->GetBinContent(ii) > npk ) {
            npk = h->GetBinContent(ii);
            xpk = h->GetBinCenter(ii);
        }
        if( h->GetBinContent(ii)-bg > 0.5*(npk-bg) )
            xhwhm = h->GetBinCenter(ii); // overwritten until right flank
        
    }
    
    
    double sm = 2*(xhwhm-xpk);
    double aa = 2.5 * ( npk - bg ) * sm / bwt2;
    
    //stampiamo su shell qualche valore utile
    //cout << hs << ": " << x1 << " - " << x9 << endl;
    //cout << hs << ": xpk=" << xpk << ", xhwhm=" << xhwhm << ", sm=" << sm << endl;
    //cout << hs << ": bwt2=" << bwt2 << ", npk=" << npk << ", aa=" << aa << endl;
    
    // create a TF1 with the range from x1 to x9 and 11 parameters
    TF1 *lp2Fcn = new TF1( "lp2Fcn", fitfunction, x1, x9, 12 );
    
    //Set name parameters
    lp2Fcn->SetParName( 0, "BG" );
    lp2Fcn->SetParName( 1, "slope" );
    lp2Fcn->SetParName( 2, "norm1");
    lp2Fcn->SetParName( 3, "mean1" );
    lp2Fcn->SetParName( 4, "sigma1" );
    lp2Fcn->SetParName( 5, "norm2");
    lp2Fcn->SetParName( 6, "mean2" );
    lp2Fcn->SetParName( 7, "sigma2" );
    lp2Fcn->SetParName( 8, "norm3");
    lp2Fcn->SetParName( 9, "mean3" );
    lp2Fcn->SetParName( 10, "sigma3" );
//    double nm1=160;
//    double nm2=50;
//    double nm3=40;
    double me1=9.45;
    double me2=10.01;
    double me3=10.35;
    double sig1=0.054;
    double sig2=0.032;
    double sig3=0.020;
    double nm1=findPeaks(me1,slp,bg,h);
    double nm2=findPeaks(me2,slp,bg,h);
    double nm3=findPeaks(me3,slp,bg,h);
    //me1=h->FindBin(nm1);
    //me2=h->FindBin(nm2);
    //me3=h->FindBin(nm3);

    std::cout << "nm1:" << nm1 << std::endl;
    std::cout << "nm2:" << nm2 << std::endl;
    std::cout << "nm3:" << nm3 << std::endl;
    
    // set start values for some parameters:
    lp2Fcn->SetParameter( 0, bg );
    lp2Fcn->SetParameter( 1, slp );
    lp2Fcn->SetParameter( 2, nm1 ); //norm
    lp2Fcn->SetParameter( 3, me1 ); // mean
    lp2Fcn->SetParameter( 4, sig1 ); // sigma
    lp2Fcn->SetParameter( 5, nm2 );
    lp2Fcn->SetParameter( 6, me2 );
    lp2Fcn->SetParameter( 7, sig2 );
    lp2Fcn->SetParameter( 8, nm3 );
    lp2Fcn->SetParameter( 9, me3 );
    lp2Fcn->SetParameter( 10, sig3 );
    lp2Fcn->FixParameter(11,0);
    
    lp2Fcn->SetLineWidth(4);
    lp2Fcn->SetNpx(500);
    lp2Fcn->SetLineColor(kMagenta);
    lp2Fcn->Draw("same");
    
    lp2Fcn->SetLineColor(kBlue);
    TFitResultPtr r = h->Fit("lp2Fcn", "RS", "ep" );
    
    // h->Fit("tp2Fcn","V+","ep");
    
    //stampiamo su shell qualche valore utile
    std::cout << "Ndata = " << lp2Fcn->GetNumberFitPoints() << std::endl;
    std::cout << "Npar  = " << lp2Fcn->GetNumberFreeParameters() << std::endl;
    std::cout << "NDoF  = " << lp2Fcn->GetNDF() << std::endl;
    std::cout << "chisq = " << lp2Fcn->GetChisquare() << std::endl;
    std::cout << "prob  = " << lp2Fcn->GetProb() << std::endl;
    
    ///*
    auto c1 = new TCanvas("c", "", 800, 700);
    //c1->SetLogx();
    //c1->SetLogy();//*/
    
    // data points on top:
    h->Draw("histpesame");
    
    //TMatrixD cor = r->GetCorrelationMatrix();
    //TMatrixD cov = r->GetCovarianceMatrix();
    //cov.Print();
    //cor.Print();
    
    lp2Fcn->Draw("same");
    c1->SaveAs("fitp1Y.pdf");
    return r;
    
}

//TFitResultPtr fitlp2( string hs, double x1=1, double x9=0 );//esecuzione fit


#endif /* fitp1_h */
