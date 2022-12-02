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
    return (0.5*par[0]*par[2]/TMath::Pi()) / TMath::Max(1.e-10,(x[0]-par[1])*(x[0]-par[1])+ .25*par[2]*par[2]);
}

Double_t gaussianPeak(Double_t *x, Double_t *par) {
    return par[0]*exp(-0.5* ((x[0]-par[1])/par[2]) * ((x[0]-par[1])/par[2]) /(par[2] *TMath::Sqrt(2*TMath::Pi())));
}

// Quadratic background function
Double_t background(Double_t *x, Double_t *par) {
    return par[0] + par[1]*x[0];
}

// Sum of background and peak function
Double_t fitfunction(Double_t *x, Double_t *par) {
    return background(x,par) + lorentzianPeak(x,&par[2])+ lorentzianPeak(x,&par[5])+ lorentzianPeak(x,&par[8]);
}

//----------------------------------------------------------------------
TFitResultPtr fitp1( string hs, double x1=1, double x9=0 )
{
    TH1 *h = (TH1*)gDirectory->Get( hs.c_str() ); // perché quaggiù non passiamo fdirettamente l'istogramma anziché la stringa?
    
    if( h == NULL ){
        cout << hs << " does not exist\n";
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
    
    double bwt2 = h->GetBinWidth(1);
    
    int nb = h->GetNbinsX();
    
    if( x9 < x1 ) {
        x1 = h->GetBinCenter(1);
        x9 = h->GetBinCenter(nb);
    }
    int i1 = h->FindBin(x1);
    int i9 = h->FindBin(x9);
    double n1 = h->GetBinContent(i1);
    double n9 = h->GetBinContent(i9);
    double slp = (n9-n1)/(x9-x1);
    double bg = n1 + slp*x1; //NO VA IMPLEMENTATA MEGLIO PERCHè BG è PER X=0
    
    
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
    cout << hs << ": " << x1 << " - " << x9 << endl;
    cout << hs << ": xpk=" << xpk << ", xhwhm=" << xhwhm << ", sm=" << sm << endl;
    cout << hs << ": bwt2=" << bwt2 << ", npk=" << npk << ", aa=" << aa << endl;
    
    // create a TF1 with the range from x1 to x9 and 11 parameters
    
    TF1 *lp2Fcn = new TF1( "lp2Fcn", fitfunction, x1, x9, 11 );
    
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
    
    //trovare una logica per implementare questi valori
//    double nm1=160;
//    double nm2=50;
//    double nm3=40;
    double me1=9.45;
    double me2=10.0;
    double me3=10.34;
    double nm1=h->GetBinContent(h->FindBin(me1));
    double nm2=h->GetBinContent(h->FindBin(me2));
    double nm3=h->GetBinContent(h->FindBin(me3));
    double sig1=0.08;
    double sig2=0.08;
    double sig3=0.08;
    
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
    
    lp2Fcn->SetLineWidth(4);
    lp2Fcn->SetNpx(500);
    lp2Fcn->SetLineColor(kMagenta);
    lp2Fcn->Draw("same");
    
    lp2Fcn->SetLineColor(kBlue);
    TFitResultPtr r = h->Fit("lp2Fcn", "RS", "ep" );
    
    // h->Fit("tp2Fcn","V+","ep");
    
    //stampiamo su shell qualche valore utile
    cout << "Ndata = " << lp2Fcn->GetNumberFitPoints() << endl;
    cout << "Npar  = " << lp2Fcn->GetNumberFreeParameters() << endl;
    cout << "NDoF  = " << lp2Fcn->GetNDF() << endl;
    cout << "chisq = " << lp2Fcn->GetChisquare() << endl;
    cout << "prob  = " << lp2Fcn->GetProb() << endl;
    
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
