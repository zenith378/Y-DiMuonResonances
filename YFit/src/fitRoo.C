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
#include "TLatex.h"
#include "SpectrumPlot.h"
#include "TStyle.h"
#include "TString.h"
#include <string.h>
#include <filesystem>

using namespace RooFit;

TString formatPtString(int dr, float pmr, float pMr)
{
    TString tmp;
    if (pmr != pmr && pMr != pMr){
        if (dr == 1 || dr == 2)
            tmp.Form("10 < p_{T} < 100 GeV");
    }
    if (pmr == pmr && pMr == pMr)
        tmp.Form("%.1f < p_{T} < %.1f GeV",pmr,pMr); //    std::to_string((int)pmr) + " < p_{T}< " + std::to_string((int)pMr) + " GeV";

    if (pmr == pmr && pMr != pMr)
        tmp.Form("p_{T} > %.1f GeV",pmr);
        //tmp = tmp + "p_{T} > " + std::to_string((int)pmr) + " GeV";

    if (pmr != pmr && pMr == pMr)
        tmp.Form("p_{T} < %.1f GeV",pMr); //tmp = tmp + "p_{T}< " + std::to_string((int)pMr) + " GeV";


    return tmp;
}

TString formatYString(int dr, float ymr, float yMr)
{
    TString tmp;
    if (ymr != ymr && yMr != yMr){
        if (dr == 2)
            tmp.Form("|y| < 0.6");
    }
    if (ymr == ymr && yMr == yMr)
        tmp.Form("%.2f < |y| < %.2f",ymr,yMr);

    if (ymr == ymr && yMr != yMr)
        tmp.Form("|y| > %.2f",ymr);

    if (ymr != ymr && yMr == yMr)
        tmp.Form("|y| < %.2f",yMr);
    
    return tmp;
}



RooFitResult *fitRoo(TH1 *hh, int mr,int fr, int dr, float pmr, float pMr, float ymr, float yMr, std::string nfr, int vr)
{
    // Declare observable x
    RooRealVar x("x", "m_{#mu^{+}#mu^{-}} (GeV/c^{2})", 8.5, 11.5);
    RooDataHist rh("rh", "rh", x, Import(*hh));
    
    Int_t nb = hh->GetNbinsX();
    Double_t x1 = hh->GetBinCenter(1);
    Double_t x9 = hh->GetBinCenter(nb);
    Int_t i1 = hh->FindBin(x1);
    Int_t i9 = hh->FindBin(x9);
    Double_t n1 = hh->GetBinContent(i1);
    Double_t n9 = hh->GetBinContent(i9);
    Double_t slp = (n9-n1)/(x9-x1);
    Double_t bg = n1 - slp*x1; 
     //numero di eventi dentro all'istogramma
    Int_t entries = hh->GetEntries();
    std::cout << "Entries: " << entries << std::endl;
    // Set up   component   pdfs
    // ---------------------------------------

    // Build polynomial pdf
    RooRealVar a0("a0", "a0", bg, -10000, 100000);
    RooRealVar a1("a1", "a1", slp, -10000., 1000.);
    RooRealVar a2("a2", "a2", 0, -1000., 1000.);
    RooPolynomial bkg("bkg", "Background", x, RooArgSet(a0, a1,a2));
    RooRealVar nback("nback", "nback", 0.32*entries, 0.01*entries, entries); //0.32

    // Create parameters
    RooRealVar mean1("mean1", "mean of gaussians", 9.45, 9.3, 9.6);
    RooRealVar mean2("mean2", "mean of gaussians", 10.01, 9.9, 10.2);
    RooRealVar mean3("mean3", "mean of gaussians", 10.35, 10.25, 10.5);
    RooRealVar sigma1("sigma1", "width of gaussians", 0.054, 0.001, 0.3);
    RooRealVar sigma2("sigma2", "width of gaussians", 0.032, 0.001, 0.3);
    RooRealVar sigma3("sigma3", "width of gaussians", 0.020, 0.001, 0.3);

    RooRealVar r1("r1", "r1", 10, 0.001, 10000);
    RooRealVar r2("r2", "r2", 1, 0.001, 10000);
    RooRealVar r3("r3", "r3", 1, 0.001, 10000);

    // Define signal

    RooAbsPdf *sig1;
    RooAbsPdf *sig2;
    RooAbsPdf *sig3;

    RooRealVar nsig1("nsig1", "signal1", 0.42*entries, 0.01*entries, entries); //0.32
    RooRealVar nsig2("nsig2", "signal2", 0.31*entries, 0.01*entries, entries); //0.21
    RooRealVar nsig3("nsig3", "signal3", 0.22*entries, 0.01*entries, entries); //0.22


    switch (fr)
    {

    case 0:
    default:
    {
        std::cout << "Using Breit-Wigner" << std::endl;

        sig1 = new RooBreitWigner("sig1", "Signal component 1", x, mean1, sigma1);
        sig2 = new RooBreitWigner("sig2", "Signal component 2", x, mean2, sigma2);
        sig3 = new RooBreitWigner("sig3", "Signal component 3", x, mean3, sigma3);

        break;
    }

    case 1:
    {
        std::cout << "Using gaus" << std::endl;
        sig1 = new RooGaussian("sig1", "Signal component 1", x, mean1, sigma1);
        sig2 = new RooGaussian("sig2", "Signal component 2", x, mean2, sigma2);
        sig3 = new RooGaussian("sig3", "Signal component 3", x, mean3, sigma3);
        break;
    }
    case 2:
    {
        // tstudent
        sig1 = new RooGenericPdf("sig1", "((exp(lgamma((r1+1)/2.0)-lgamma(r1/2.0)))/(sqrt((22/7)*r1)*sigma1))*pow((1+(((x-mean1)/sigma1)*((x-mean1)/sigma1))/r1),-(r1+1)/2.0)", RooArgList(x, r1, mean1, sigma1));
        sig2 = new RooGenericPdf("sig2", "((exp(lgamma((r2+1)/2.0)-lgamma(r2/2.0)))/(sqrt((22/7)*r2)*sigma2))*pow((1+(((x-mean2)/sigma2)*((x-mean2)/sigma2))/r2),-(r2+1)/2.0)", RooArgList(x, r2, mean2, sigma2));
        sig3 = new RooGenericPdf("sig3", "((exp(lgamma((r3+1)/2.0)-lgamma(r3/2.0)))/(sqrt((22/7)*r3)*sigma3))*pow((1+(((x-mean3)/sigma3)*((x-mean3)/sigma3))/r3),-(r3+1)/2.0)", RooArgList(x, r3, mean3, sigma3));

        break;
    }
    }

    RooAddPdf model("model", "model", RooArgList(*sig1, *sig2, *sig3, bkg), RooArgList(nsig1, nsig2, nsig3, nback));

    RooFitResult *fitResult;
    try
    {
        if (vr == 0)
            fitResult = model.fitTo(rh, Verbose(false), Warnings(false), Save(),RecoverFromUndefinedRegions(1), PrintEvalErrors(-1), PrintLevel(-1));

        if (vr == 1)
            fitResult = model.fitTo(rh, Timer(true), Save(),RecoverFromUndefinedRegions(1),PrintEvalErrors(0));
 

        // Print structure of composite pdf
        fitResult->Print("v"); // previous was t

        if (fitResult->edm()>1e4||(fitResult->status()>4)||(fitResult->status()>3&&fitResult->edm()>1e3)) // fitResult->covQual() < 2
        {
            throw(std::runtime_error("Fit did not converge."));
        }
    }
    catch (std::exception &msg)
    {
        std::cerr << msg.what() << std::endl;
        std::cerr <<"Exit with fit status: " << fitResult->status() << std::endl;
        std::cerr << "Try relaxing cut filters or changing PDF." << std::endl;
        if(vr==0)
        std::cerr <<"Set verbose flag on [-v], in order to print the parameters that have been updated in each minimisation step (MINUIT LOG).\n" << std::endl;
        exit(1);
    }


    // Draw options
    // ---------------------------------------
    RooPlot *xframe = x.frame(Title("Y Resonances Fit"));

    // Plot data and PDF overlaid
    rh.plotOn(xframe, MarkerStyle(6), MarkerSize(1));

    // Overlay the background component of model with a dashed line
    model.plotOn(xframe, Components(bkg), LineColor(41), LineStyle(kDashed));
    // Overlay the sig1 components of model with a dashed-dotted line
    model.plotOn(xframe, Components(*sig1), LineColor(46), LineStyle(8));
    // Overlay the background+sig2 components of model with a long dashed line
    model.plotOn(xframe, Components(*sig2), LineColor(30), LineStyle(9));

    model.plotOn(xframe, Components(*sig3), LineColor(38), LineStyle(kDotted));

    model.plotOn(xframe, LineWidth(2), LineColor(kRed));

    // Define model and fit
    // ---------------------------------------

    RooHist *hpull = xframe->pullHist();
    // hpull->SetMarkerSize(1);
    hpull->SetMarkerStyle(6);
    hpull->SetLineWidth(0);
    TApplication *theApp2 = new TApplication("app2", 0, 0);

    // Draw the frame on the canvas
    TCanvas * c2 = new TCanvas("Fit", "Y Resonances Fit", 950, 800);

    TPad *pad1 = new TPad("pad1", "The pad 80 of the height", 0.0, 0.2, 1.0, 1.0);
    TPad *pad2 = new TPad("pad2", "The pad 20 of the height", 0.0, 0.05, 1.0, 0.25);
    pad1->Draw();
    pad2->Draw();
    pad1->cd();

    xframe->GetYaxis()->SetTitleOffset(1.5);
    xframe->GetXaxis()->SetTitleSize(0);
    // xframe->GetXaxis()->SetLabelSize(0);
    //  xframe->GetXaxis()->SetTitleOffset(999);
    xframe->SetMinimum(0.001);
    xframe->Draw();
    TString cut2 = formatYString(dr, ymr, yMr);
    TString cut1 = formatPtString(dr, pmr, pMr);

    TLatex label;
    label.SetNDC(true); // cambio di coordinate di riferimento da quelle del grafico a quelle del pad normalizzate
    label.SetTextSize(0.05);
    label.SetTextAlign(22); // central vertically and horizontally
    label.DrawLatex(0.7, 0.8, ("#bf{" + cut1 + "}"));
    label.DrawLatex(0.7, 0.7, ("#bf{" + cut2 + "}"));

    label.SetTextSize(0.04);
    label.SetTextAlign(11); // left bottom
    label.DrawLatex(0.10, 0.92, "#bf{CMS Open Data}");
    label.SetTextAlign(31); // right bottom
    label.DrawLatex(0.90, 0.92, "11.6 fb^{-1} (8 TeV)"); //#sqrt{s} = 8 TeV, L_{int} = 11.6 fb^{-1}
    label.Draw();

    pad2->cd();
    pad2->SetBottomMargin(0.4);

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
    c2->Update();

    SavePlot(c2,nfr);
    if(mr==0){
    TRootCanvas *rc2 = (TRootCanvas *)c2->GetCanvasImp();
    rc2->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    theApp2->Run();
    }

    return fitResult;
}
