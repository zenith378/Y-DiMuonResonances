#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooBreitWigner.h"
#include "RooAddPdf.h"
#include "RooAbsPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "TH1.h"
#include "RooGenericPdf.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "RooHist.h"
#include "TRootCanvas.h"
#include "TApplication.h"
#include "TLatex.h"
#include "SpectrumPlot.h"
#include "TStyle.h"
#include "TString.h"
#include <string.h>
#include <filesystem>
#include "TROOT.h"

using namespace RooFit;

TString formatPtString(int dr, float pmr, float pMr)
{
   TString tmp;                    // define tmp string
   if (pmr != pmr && pMr != pMr) { // values are nan
      if (dr == 1 || dr == 2)      // if depth set to 1 or 2
         tmp.Form("10 < p_{T} < 100 GeV");
   }
   if (pmr == pmr && pMr == pMr) // if both minimum and maximum are defined
      tmp.Form("%.1f < p_{T} < %.1f GeV", pmr, pMr);

   if (pmr == pmr && pMr != pMr) // if only minimum pt is defined
      tmp.Form("p_{T} > %.1f GeV", pmr);

   if (pmr != pmr && pMr == pMr) // if only maximum is defined
      tmp.Form("p_{T} < %.1f GeV", pMr);

   return tmp;
}

TString formatYString(int dr, float ymr, float yMr) // see documentation for formatPtString()
{
   TString tmp;
   if (ymr != ymr && yMr != yMr) {
      if (dr == 2)
         tmp.Form("|y| < 0.6");
   }
   if (ymr == ymr && yMr == yMr)
      tmp.Form("%.2f < |y| < %.2f", ymr, yMr);

   if (ymr == ymr && yMr != yMr)
      tmp.Form("|y| > %.2f", ymr);

   if (ymr != ymr && yMr == yMr)
      tmp.Form("|y| < %.2f", yMr);

   return tmp;
}

RooFitResult *
fitRoo(TH1 *hh, int mr, int fr, int dr, float pmr, float pMr, float ymr, float yMr, std::string nfr, int vr, int cr)
{
   // if mute canvas flag is on, do not display canvas

   if (cr == 1)
      gROOT->SetBatch(1);

   // Declare observable x
   RooRealVar x("x", "m_{#mu^{+}#mu^{-}} (GeV/c^{2})", 8.5, 11.5);
   // generate an histogram readable by roofit
   RooDataHist rh("rh", "rh", x, Import(*hh));

   Int_t nb = hh->GetNbinsX();           // number of bins
   Double_t x1 = hh->GetBinCenter(1);    // center of the first bin
   Double_t x9 = hh->GetBinCenter(nb);   // center of last bin
   Double_t n1 = hh->GetBinContent(1);   // content of first bin
   Double_t n9 = hh->GetBinContent(nb);  // content of last bin
   Double_t slp = (n9 - n1) / (x9 - x1); // initialize value of slope
   Double_t bg = n1 - slp * x1;          // initiazlize values for background

   Int_t entries = hh->GetEntries(); // events in the histogram

   // Set up   component   pdfs
   // ---------------------------------------

   // Build polynomial pdf
   RooRealVar a0("a0", "a0", bg, -10000, 100000);
   RooRealVar a1("a1", "a1", slp, -10000., 1000.);
   RooRealVar a2("a2", "a2", 0, -1000., 1000.);
   RooPolynomial bkg("bkg", "Background", x, RooArgSet(a0, a1, a2));
   RooRealVar nback("nback", "Number of events in background", 0.32 * entries, 0.01 * entries, entries);

   // Create fit function parameters
   // mean and sigma are useful for all the fit function, r1,r2,r3 only for t-student
   RooRealVar mean1("mean1", "mean1", 9.45, 9.3, 9.6); // values initialized as PDG
   RooRealVar mean2("mean2", "mean2", 10.01, 9.9, 10.2);
   RooRealVar mean3("mean3", "mean3", 10.35, 10.25, 10.5);
   RooRealVar sigma1("sigma1", "width1", 0.054, 0.001, 0.3);
   RooRealVar sigma2("sigma2", "width2", 0.032, 0.001, 0.3);
   RooRealVar sigma3("sigma3", "width3", 0.020, 0.001, 0.3);
   RooRealVar r1("r1", "r1", 10, 0.001, 10000);
   RooRealVar r2("r2", "r2", 1, 0.001, 10000);
   RooRealVar r3("r3", "r3", 1, 0.001, 10000);

   RooRealVar nsig1("nsig1", "Number of events in signal1", 0.42 * entries, 0.01 * entries, entries);
   RooRealVar nsig2("nsig2", "Number of events in signal2", 0.31 * entries, 0.01 * entries, entries);
   RooRealVar nsig3("nsig3", "Number of events in signal3", 0.22 * entries, 0.01 * entries, entries);

   // Define signal

   RooAbsPdf *sig1; // initialize signals
   RooAbsPdf *sig2;
   RooAbsPdf *sig3;

   switch (fr) // switch over fit function
   {

   case 0: // breit wigner
   default: {
      std::cout << "Using Breit-Wigner PDF" << std::endl;

      sig1 = new RooBreitWigner("sig1", "Signal component 1", x, mean1, sigma1);
      sig2 = new RooBreitWigner("sig2", "Signal component 2", x, mean2, sigma2);
      sig3 = new RooBreitWigner("sig3", "Signal component 3", x, mean3, sigma3);

      break;
   }

   case 1: // gaussian
   {
      std::cout << "Using Gaussian PDF" << std::endl;
      sig1 = new RooGaussian("sig1", "Signal component 1", x, mean1, sigma1);
      sig2 = new RooGaussian("sig2", "Signal component 2", x, mean2, sigma2);
      sig3 = new RooGaussian("sig3", "Signal component 3", x, mean3, sigma3);
      break;
   }
   case 2: // tstudent
   {
      std::cout << "Using t-student PDF" << std::endl;
      sig1 = new RooGenericPdf("sig1",
                               "((exp(lgamma((r1+1)/2.0)-lgamma(r1/2.0)))/(sqrt((22/7)*r1)*sigma1))*pow((1+(((x-mean1)/"
                               "sigma1)*((x-mean1)/sigma1))/r1),-(r1+1)/2.0)",
                               RooArgList(x, r1, mean1, sigma1));
      sig2 = new RooGenericPdf("sig2",
                               "((exp(lgamma((r2+1)/2.0)-lgamma(r2/2.0)))/(sqrt((22/7)*r2)*sigma2))*pow((1+(((x-mean2)/"
                               "sigma2)*((x-mean2)/sigma2))/r2),-(r2+1)/2.0)",
                               RooArgList(x, r2, mean2, sigma2));
      sig3 = new RooGenericPdf("sig3",
                               "((exp(lgamma((r3+1)/2.0)-lgamma(r3/2.0)))/(sqrt((22/7)*r3)*sigma3))*pow((1+(((x-mean3)/"
                               "sigma3)*((x-mean3)/sigma3))/r3),-(r3+1)/2.0)",
                               RooArgList(x, r3, mean3, sigma3));

      break;
   }
   }
   // Define model and fit
   // ---------------------------------------
   RooAddPdf model("model", "model", RooArgList(*sig1, *sig2, *sig3, bkg), RooArgList(nsig1, nsig2, nsig3, nback));

   RooFitResult *fitResult; // initialize object that contains fit results
   if (vr == 0)             // if verbose flag set to 0, print as less as possible
      fitResult = model.fitTo(rh, Verbose(false), Warnings(false), Save(), RecoverFromUndefinedRegions(1),
                              PrintEvalErrors(-1), PrintLevel(-1));

   if (vr == 1) // if verbose flag set to 1, print evaluation errors, timer, warnings and minuit log
      fitResult = model.fitTo(rh, Timer(true), Save(), RecoverFromUndefinedRegions(1), PrintEvalErrors(0));

   // Print results on terminal
   fitResult->Print("v");
   // if fit do not converge
   if (fitResult->edm() > 1e4 || (fitResult->status() > 4) || (fitResult->status() > 3 && fitResult->edm() > 1e3)) {
      std::cerr << "Fit did not converge." << std::endl;
      std::cerr << "Exit with fit status: " << fitResult->status() << std::endl;
      std::cerr << "Try relaxing cut filters or changing PDF." << std::endl;
      if (vr == 0)
         std::cerr << "Set verbose flag on [-v], in order to print the parameters that have been updated in each "
                      "minimisation step (MINUIT LOG).\n"
                   << std::endl;
      exit(1);
   }

   // Draw options
   // ---------------------------------------
   RooPlot *xframe = x.frame(Title("Y Resonances Fit"));

   // Plot data
   rh.plotOn(xframe, MarkerStyle(6), MarkerSize(1));

   // Overlay the background component of model with a dashed line
   model.plotOn(xframe, Components(bkg), LineColor(41), LineStyle(kDashed));
   // Overlay the sig1 components of model with a dashed-dotted red line
   model.plotOn(xframe, Components(*sig1), LineColor(46), LineStyle(8));
   // Overlay the sig2 components of model with a long dashed green line
   model.plotOn(xframe, Components(*sig2), LineColor(30), LineStyle(9));
   // Overlay the sig3 components of model with a dotted blue line
   model.plotOn(xframe, Components(*sig3), LineColor(38), LineStyle(kDotted));
   // Overlay the model function with a red line
   model.plotOn(xframe, LineWidth(2), LineColor(kRed));

   // create pull graph
   RooHist *hpull = xframe->pullHist();
   hpull->SetMarkerStyle(6);
   hpull->SetLineWidth(0);

   // Draw the frame on the canvas
   TCanvas *c2 = new TCanvas("Fit", "Y Resonances Fit", 950, 800);

   // define two pads for histogram and residual
   TPad *pad1 = new TPad("pad1", "The pad 80 of the height", 0.0, 0.2, 1.0, 1.0);
   TPad *pad2 = new TPad("pad2", "The pad 20 of the height", 0.0, 0.05, 1.0, 0.25);
   pad1->Draw();
   pad2->Draw();
   pad1->cd();

   // Drawing options
   xframe->GetYaxis()->SetTitleOffset(1.5);
   xframe->GetXaxis()->SetTitleSize(0);
   xframe->SetMinimum(0.001);
   xframe->Draw();
   // format string for printing cuts on canvas
   TString cut2 = formatYString(dr, ymr, yMr);
   TString cut1 = formatPtString(dr, pmr, pMr);

   TLatex label;
   label.SetNDC(true); // normalized coordinates
   label.SetTextSize(0.05);
   label.SetTextAlign(22);                           // central vertically and horizontally
   label.DrawLatex(0.7, 0.8, ("#bf{" + cut1 + "}")); // print cuts
   label.DrawLatex(0.7, 0.7, ("#bf{" + cut2 + "}"));

   label.SetTextSize(0.04);
   label.SetTextAlign(11); // left bottom
   label.DrawLatex(0.10, 0.92, "#bf{CMS Open Data}");
   label.SetTextAlign(31);                              // right bottom
   label.DrawLatex(0.90, 0.92, "11.6 fb^{-1} (8 TeV)"); // #sqrt{s} = 8 TeV, L_{int} = 11.6 fb^{-1}
   label.Draw();

   pad2->cd();
   pad2->SetBottomMargin(0.4);

   // other drawing options
   hpull->GetYaxis()->SetNdivisions(6);
   hpull->GetXaxis()->SetTitleOffset(1.3);
   hpull->GetXaxis()->SetRangeUser(8.51, 11.49);
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


   if (mr == 0 && cr==0) // if mode is set to fit, close program while closing the canvas
   {
      TRootCanvas *rc2 = (TRootCanvas *)c2->GetCanvasImp();

      rc2->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

   } 
   c2->Update();

   // save plot
   SavePlot(c2, nfr);
   
   if(mr==1) {
      delete sig1;
      delete sig2;
      delete sig3;
      delete pad1;
      delete pad2;
      delete c2;
   }
   
   return fitResult;
}
