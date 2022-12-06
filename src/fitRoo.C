#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "TH1.h"
#include "RooDataHist.h"
#include "RooHist.h"
#include "TRootCanvas.h"
#include "TApplication.h"
#include "TStyle.h"

using namespace RooFit;

/**************************************************************
 \brief three peaks fit plus a linear background
 
 
 *******************************************************************************/
void fitRoo(TH1 *hh)
{
    // S e t u p   c o m p o n e n t   p d f s
    // ---------------------------------------
    
    //Histogram style
    hh->SetMarkerStyle(21);
    hh->SetMarkerSize(0.2);
    hh->SetStats(1);
    
    // Declare observable x
    RooRealVar x("x", "m_{#mu^{+}#mu^{-}} (GeV/c^{2})", 8.5, 11.5);
    RooDataHist rh("rh", "rh", x, Import(*hh));
    TApplication *theApp = new TApplication("app", 0, 0);

    // Create two Gaussian PDFs g1(x,mean1,sigma) anf g2(x,mean2,sigma) and their parameters
    RooRealVar mean1("mean1", "mean of gaussians", 9.45, 3.3, 9.6);
    RooRealVar mean2("mean2", "mean of gaussians", 10.01, 9.9, 10.2);
    RooRealVar mean3("mean3", "mean of gaussians", 10.35, 10.2, 10.5);
    RooRealVar sigma1("sigma1", "width of gaussians", 0.054, 0.001, 0.1);
    RooRealVar sigma2("sigma2", "width of gaussians", 0.032, 0.001, 0.1);
    RooRealVar sigma3("sigma3", "width of gaussians", 0.020, 0.001, 0.1);

    RooGaussian sig1("sig1", "Signal component 1", x, mean1, sigma1);
    RooGaussian sig2("sig2", "Signal component 2", x, mean2, sigma2);
    RooGaussian sig3("sig3", "Signal component 3", x, mean3, sigma3);

    // Build Chebychev polynomial pdf
    RooRealVar a0("a0", "a0", 3101, 0, 10000);
    RooRealVar a1("a1", "a1", -140.3, -300., 300.);
    RooPolynomial bkg("bkg", "Background", x, RooArgSet(a0, a1));

    // ---------------------------------------------
    // M E T H O D   1 - T w o   R o o A d d P d f s
    // =============================================

    // A d d   s i g n a l   c o m p o n e n t s
    // ------------------------------------------

    RooRealVar fsig1("fsig1", "signal1", 0.1, 0., 1.);
    RooRealVar fsig2("fsig2", "signal2", 0.1, 0., 1.);
    RooRealVar fsig3("fsig3", "signal3", 0.1, 0., 1.);

    RooAddPdf model("model", "model", RooArgList(sig1, sig2, sig3, bkg), RooArgList(fsig1, fsig2, fsig3), kTRUE);

    // Sum the signal components into a composite signal pdf
    // RooRealVar sig1frac("sig1frac", "fraction of component 1 in signal", 0.55, 0., 1.);
    // RooRealVar sig2frac("sig2frac", "fraction of component 2 in signal", 0.26, 0., 1.);
    // RooAddPdf sig("sig", "Signal", RooArgList(sig1, sig2,sig3), sig1frac);

    // A d d  s i g n a l   a n d   b a c k g r o u n d
    // ------------------------------------------------

    // Sum the composite signal and background
    // RooRealVar bkgfrac("bkgfrac", "fraction of background", 0.5, 0., 1.);
    // RooAddPdf model("model", "g1+g2+a", RooArgList(bkg, sig), bkgfrac);

    // S a m p l e ,   f i t   a n d   p l o t   m o d e l
    // ---------------------------------------------------

    // model.getParameters(rh)->setAttribAll("Constant",kTRUE) ;
    // fsig1.setConstant(kFALSE) ;
    //  Fit model to data
    model.fitTo(rh);

    // Plot data and PDF overlaid
    RooPlot *xframe = x.frame(Title("Y Resonances Fit"));
    rh.plotOn(xframe, MarkerStyle(6),MarkerSize(1));

    model.plotOn(xframe);

    // Overlay the background component of model with a dashed line
    model.plotOn(xframe, Components(bkg), LineColor(kBlue), LineStyle(kDashed));
    // Overlay the sig1 components of model with a dashed-dotted line
    model.plotOn(xframe, Components(RooArgSet(sig1)), LineColor(kRed), LineStyle(8));
    // Overlay the background+sig2 components of model with a long dashed line
    model.plotOn(xframe, Components(RooArgSet(sig2)), LineColor(kGreen), LineStyle(9));

    model.plotOn(xframe, Components(RooArgSet(sig3)), LineStyle(kDotted));

    // Print structure of composite pdf
    model.Print("t");

    RooHist *hpull = xframe->pullHist();
    hpull->SetMarkerStyle(21);
    hpull->SetMarkerSize(0.2);

    RooPlot *frame2 = x.frame(Title(" "));
    frame2->addPlotable(hpull, "P");
    
    // Draw the frame on the canvas
    TCanvas * c1 = new TCanvas("rf201_composite", "rf201_composite", 800, 600);
    /*c1->Divide(1,2);
    c1->cd(1);
    xframe->Draw();
    c1->cd(2);
    frame2->Draw();*/

    TRootCanvas *rc = (TRootCanvas *)c1->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

    TPad *pad1 = new TPad("pad1", "The pad 80 of the height",0.0,0.2,1.0,1.0);
    TPad *pad2 = new TPad("pad2", "The pad 20 of the height",0.0,0.0,1.0,0.2);
    pad1->Draw();
    pad2->Draw();
    pad1->cd();
    xframe->GetYaxis()->SetTitleOffset(1.4);
    xframe->Draw();
    pad2->cd();
    xframe->GetYaxis()->SetTitleOffset(1.4);
    frame2->Draw();
    
    
    c1->SaveAs("RooFit1.pdf");
    theApp->Run();
}
