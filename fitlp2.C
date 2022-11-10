#include "TDirectory.h"
#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
//#include "TLegend.h"
//#include "TCanvas.h"
#include "TStyle.h"
#include "TFitResult.h"
#include "TMatrixD.h"
#include "fitlp2.h"


//----------------------------------------------------------------------
double bwt2{0.01}; // global: bin width

//Lorentzion function
Double_t lorentzianPeak(Double_t *x, Double_t *par) {
    return (0.5*par[0]*par[1]/TMath::Pi()) / TMath::Max(1.e-10,(x[0]-par[2])*(x[0]-par[2])+ .25*par[1]*par[1]);
}
// Quadratic background function
Double_t background(Double_t *x, Double_t *par) {
    return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}

// Sum of background and peak function
Double_t fitfunction(Double_t *x, Double_t *par) {
    return background(x,par) + lorentzianPeak(x,&par[3]);
}

//----------------------------------------------------------------------
TFitResultPtr fitlp2( string hs, double x1=1, double x9=0 )
{
    TH1 *h = (TH1*)gDirectory->Get( hs.c_str() ); // perché quaggiù non passiamo fdirettamente l'istogramma anziché la stringa?

    if( h == NULL ){
        cout << hs << " does not exist\n";
        return nullptr;
    }

    h->SetMarkerStyle(21);
    h->SetMarkerSize(0.8);
    h->SetStats(1);
    gStyle->SetOptFit(101);

    //gROOT->ForceStyle();

    bwt2 = h->GetBinWidth(1);

    int nb = h->GetNbinsX();

    if( x9 < x1 ) {
        x1 = h->GetBinCenter(1);
        x9 = h->GetBinCenter(nb);
    }
    int i1 = h->FindBin(x1);
    int i9 = h->FindBin(x9);
    double n1 = h->GetBinContent(i1);
    double n9 = h->GetBinContent(i9);
    double bg = 0.5*(n1+n9);
    double slp = (n9-n1)/(x9-x1);

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

    cout << hs << ": " << x1 << " - " << x9 << endl;

    cout << hs << ": xpk=" << xpk << ", xhwhm=" << xhwhm << ", sm=" << sm << endl;

    cout << hs << ": bwt2=" << bwt2 << ", npk=" << npk << ", aa=" << aa << endl;

    // create a TF1 with the range from x1 to x9 and 7 parameters

    TF1 *lp2Fcn = new TF1( "lp2Fcn", fitfunction, x1, x9, 6 );

    lp2Fcn->SetParName( 0, "BG" );
    lp2Fcn->SetParName( 1, "slope" );
    lp2Fcn->SetParName( 2, "curv" );
    lp2Fcn->SetParName( 3, "area");
    lp2Fcn->SetParName( 5, "mean" );
    lp2Fcn->SetParName( 4, "sigma" );

    // set start values for some parameters:



    lp2Fcn->SetParameter( 0, bg );
    lp2Fcn->SetParameter( 1, slp );
    lp2Fcn->SetParameter( 2, 0 );
    lp2Fcn->SetParameter( 3, aa );
    lp2Fcn->SetParameter( 4, sm ); // width
    lp2Fcn->SetLineWidth(4);
    lp2Fcn->SetParameter( 5, xpk ); // peak position

    lp2Fcn->SetNpx(500);
    lp2Fcn->SetLineColor(kMagenta);

    TFitResultPtr r = h->Fit("lp2Fcn", "RS", "ep" );
    // h->Fit("tp2Fcn","V+","ep");

    cout << "Ndata = " << lp2Fcn->GetNumberFitPoints() << endl;
    cout << "Npar  = " << lp2Fcn->GetNumberFreeParameters() << endl;
    cout << "NDoF  = " << lp2Fcn->GetNDF() << endl;
    cout << "chisq = " << lp2Fcn->GetChisquare() << endl;
    cout << "prob  = " << lp2Fcn->GetProb() << endl;

    // data points on top:
    h->Draw("histpesame");
    TMatrixD cor = r->GetCorrelationMatrix();
    TMatrixD cov = r->GetCovarianceMatrix();
    //cov.Print();
    //cor.Print();
    return r;

}
