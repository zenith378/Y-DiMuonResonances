#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooBreitWigner.h"
#include "RooAddPdf.h"
#include "RooGlobalFunc.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "TH1.h"
#include "RooDataHist.h"
#include "RooHist.h"
#include "TRootCanvas.h"
#include "TApplication.h"
#include "TStyle.h"
#include <string.h>

using namespace RooFit;
/*
RooAddPdf model_gaus(RooPolynomial bakg, RooRealVar x,RooRealVar mean1, RooRealVar mean2,RooRealVar mean3,RooRealVar sigma1,RooRealVar sigma2,RooRealVar sigma3){
    std::cout << "Using gaus" << std::endl;
    RooGaussian *sig_gaus1("sig1_gaus", "Signal component 1", x, mean1, sigma1);
    RooGaussian *sig_gaus2("sig2_gaus", "Signal component 2", x, mean2, sigma2);
    RooGaussian *sig_gaus3("sig3_gaus", "Signal component 3", x, mean3, sigma3);
    RooAddPdf* model_gaus("model_gaus", "model", RooArgList(*sig_gaus1, *sig_gaus2, *sig_gaus3, bkg), RooArgList(fsig1, fsig2, fsig3), kTRUE);


}
*/
/**************************************************************
 \brief three peaks fit plus a linear background
 
 
 *******************************************************************************/
void fitRoo(TH1 *hh, int functype)
{
    // Set up   component   pdfs
    // ---------------------------------------
    

    // Declare observable x
    RooRealVar x("x", "m_{#mu^{+}#mu^{-}} (GeV/c^{2})", 8.5, 11.5);
    RooDataHist rh("rh", "rh", x, Import(*hh));
    // create application to display the canvas while root runs
    TApplication *theApp = new TApplication("app", 0, 0);

    // Build polynomial pdf
    RooRealVar a0("a0", "a0", 3101, 0, 10000);
    RooRealVar a1("a1", "a1", -140.3, -300., 300.);
    RooRealVar a2("a2", "a2", 0, -300., 300.);
    RooPolynomial bkg("bkg", "Background", x, RooArgSet(a0, a1,a2));

    // Create three Gaussian PDFs and their parameters
    RooRealVar mean1("mean1", "mean of gaussians", 9.45, 3.3, 9.6);
    RooRealVar mean2("mean2", "mean of gaussians", 10.01, 9.9, 10.2);
    RooRealVar mean3("mean3", "mean of gaussians", 10.35, 10.2, 10.5);
    RooRealVar sigma1("sigma1", "width of gaussians", 0.054, 0.001, 10);
    RooRealVar sigma2("sigma2", "width of gaussians", 0.032, 0.001, 10);
    RooRealVar sigma3("sigma3", "width of gaussians", 0.020, 0.001, 10);

    // Add signal and background

    RooRealVar fsig1("fsig1", "signal1", 0.1, 0., 1.);
    RooRealVar fsig2("fsig2", "signal2", 0.1, 0., 1.);
    RooRealVar fsig3("fsig3", "signal3", 0.1, 0., 1.);

    RooPlot *xframe = x.frame(Title("Y Resonances Fit"));

    switch (functype)
    {

    case 0: {
        std::cout << "Using Breit-Wigner" << std::endl;
        RooBreitWigner sig_bw1("sig1_bw1", "Signal component 1", x, mean1, sigma1);
        RooBreitWigner sig_bw2("sig2_bw2", "Signal component 2", x, mean2, sigma2);
        RooBreitWigner sig_bw3("sig3_bw3", "Signal component 3", x, mean3, sigma3);
        RooAddPdf model_bw("model_bw", "model", RooArgList(sig_bw1, sig_bw2, sig_bw3, bkg), RooArgList(fsig1, fsig2, fsig3), kTRUE);

        //  Fit model to data
        model_bw.fitTo(rh);

        // Draw options
        // ---------------------------------------

        // Plot data and PDF overlaid
        rh.plotOn(xframe, MarkerStyle(6), MarkerSize(1));

        // Overlay the background component of model with a dashed line
        model_bw.plotOn(xframe, Components(bkg), LineColor(kBlue), LineStyle(kDashed));
        // Overlay the sig1 components of model with a dashed-dotted line
        model_bw.plotOn(xframe, Components(RooArgSet(sig_bw1)), LineColor(kRed), LineStyle(8));
        // Overlay the background+sig2 components of model with a long dashed line
        model_bw.plotOn(xframe, Components(RooArgSet(sig_bw2)), LineColor(kGreen), LineStyle(9));

        model_bw.plotOn(xframe, Components(RooArgSet(sig_bw3)), LineStyle(kDotted));

        // Print structure of composite pdf
        model_bw.Print("t"); // previous was t

        model_bw.plotOn(xframe);
        break;
    }
    case 1:{
        std::cout << "Using gaus" << std::endl;
        RooGaussian sig_gaus1("sig1_gaus", "Signal component 1", x, mean1, sigma1);
        RooGaussian sig_gaus2("sig2_gaus", "Signal component 2", x, mean2, sigma2);
        RooGaussian sig_gaus3("sig3_gaus", "Signal component 3", x, mean3, sigma3);

        RooAddPdf model_gaus("model_gaus", "model", RooArgList(sig_gaus1, sig_gaus2, sig_gaus3, bkg), RooArgList(fsig1, fsig2, fsig3), kTRUE);
        //  Fit model to data
        model_gaus.fitTo(rh);

        // Draw options
        // ---------------------------------------

        // Plot data and PDF overlaid
        rh.plotOn(xframe, MarkerStyle(6), MarkerSize(1));

        // Overlay the background component of model with a dashed line
        model_gaus.plotOn(xframe, Components(bkg), LineColor(kBlue), LineStyle(kDashed));
        // Overlay the sig1 components of model with a dashed-dotted line
        model_gaus.plotOn(xframe, Components(RooArgSet(sig_gaus1)), LineColor(kRed), LineStyle(8));
        // Overlay the background+sig2 components of model with a long dashed line
        model_gaus.plotOn(xframe, Components(RooArgSet(sig_gaus2)), LineColor(kGreen), LineStyle(9));

        model_gaus.plotOn(xframe, Components(RooArgSet(sig_gaus3)), LineStyle(kDotted));

        // Print structure of composite pdf
        model_gaus.Print("t"); // previous was t

        model_gaus.plotOn(xframe);
        break;
    }
    }

    /*


    if(strcmp(functype,"gaus")==0){
    std::cout << "Using gaus" << std::endl;
        sig1 = new RooGaussian("sig1", "Signal component 1", x, mean1, sigma1);
        sig2 = new RooGaussian("sig2", "Signal component 2", x, mean2, sigma2);
        sig3 = new RooGaussian("sig3", "Signal component 3", x, mean3, sigma3);
    } else if(strcmp(functype,"bw")==0) {
        std::cout << "Using gaus" << std::endl;
        sig1 = new RooBreitWigner("sig1", "Signal component 1", x, mean1, sigma1);
        sig2 = new RooBreitWigner("sig2", "Signal component 2", x, mean2, sigma2);
        sig3 = new RooBreitWigner("sig3", "Signal component 3", x, mean3, sigma3);
    } else {
        std::cout << "Function must be gaus or bw" << std::endl;
        std::cout << "Using Breit-Wigner (default)" << std::endl;
    }
    */

    // Define model and fit
    // ---------------------------------------

    RooHist *hpull = xframe->pullHist();
    hpull->SetMarkerSize(1);
    hpull->SetMarkerStyle(6);

    // Draw the frame on the canvas
    auto c1 = new TCanvas("Fit", "Y Resonances Fit", 800, 800);
    TRootCanvas *rc = (TRootCanvas *)c1->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

    TPad *pad1 = new TPad("pad1", "The pad 80 of the height", 0.0, 0.2, 1.0, 1.0);
    TPad *pad2 = new TPad("pad2", "The pad 20 of the height", 0.0, 0.05, 1.0, 0.25);
    pad1->Draw();
    pad2->Draw();
    pad1->cd();

    xframe->GetYaxis()->SetTitleOffset(1.5);
    xframe->GetXaxis()->SetTitleSize(0);
    //xframe->GetXaxis()->SetLabelSize(0);
    // xframe->GetXaxis()->SetTitleOffset(999);
    xframe->SetMinimum(0.001);

    xframe->Draw();
    pad2->cd();
    pad2->SetBottomMargin(0.4);
    // auto axis = hpull->GetXaxis();

    // axis->SetLimits(8.5001,11.5);
    hpull->GetYaxis()->SetNdivisions(6);
    // hpull->SetMinimum(-9.999);
    hpull->GetXaxis()->SetTitleOffset(1.3);
    hpull->GetYaxis()->SetTitle("Pull");
    hpull->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} [GeV]");
    hpull->GetXaxis()->SetLabelFont(43);
    hpull->GetXaxis()->SetLabelSize(21);
    hpull->GetYaxis()->SetLabelFont(43);
    hpull->GetYaxis()->SetLabelSize(21);
    hpull->GetXaxis()->SetTitleSize(21);
    hpull->GetXaxis()->SetTitleFont(43);
    hpull->GetYaxis()->SetTitleSize(21);
    hpull->GetYaxis()->SetTitleFont(43);
    hpull->SetTitle("");

    hpull->Draw();
    c1->Update();

    c1->SaveAs("RooFit.pdf");
    theApp->Run();
}
