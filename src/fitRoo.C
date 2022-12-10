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
#include "TStyle.h"
#include <string.h>
#include <filesystem>

using namespace RooFit;

std::string formatPtString(int &dr, float &pmr, float &pMr)
{
    std::string tmp = "";

    if (pmr == pmr && pMr == pMr)
    {
        tmp = std::to_string((int)pmr) + " < p_{T}< " + std::to_string((int)pMr) + " GeV";
    }
    if (pmr == pmr && pMr != pMr)
    {
        tmp = tmp + "p_{T} > " + std::to_string((int)pmr) + " GeV";
    }
    if (pmr != pmr && pMr == pMr)
    {
        tmp = tmp + "p_{T}< " + std::to_string((int)pMr) + " GeV";
    }
    if (pmr != pmr && pMr != pMr)
    {
        if (dr == 1 || dr == 2)
            tmp = "10 < p_{T}< 100 GeV";
    }
    return tmp;
}

std::string formatYString(int &dr, float &ymr, float &yMr)
{
    std::string tmp = "";

    if (ymr == ymr && yMr == yMr)
    {
        tmp = std::to_string(ymr).substr(0, std::to_string(ymr).find(".") + 2 + 1) + " < |y| < " + std::to_string(yMr).substr(0, std::to_string(yMr).find(".") + 2 + 1);
    }
    if (ymr == ymr && yMr != yMr)
    {
        tmp = tmp + "|y| > " + std::to_string(ymr).substr(0, std::to_string(ymr).find(".") + 2 + 1);
    }
    if (ymr != ymr && yMr == yMr)
    {
        tmp = tmp + "|y| < " + std::to_string(yMr).substr(0, std::to_string(yMr).find(".") + 2 + 1);
    }
    if (ymr != ymr && yMr != yMr)
    {
        if (dr == 2)
            tmp = "|y| < 0.6";
    }
    return tmp;
}

/**************************************************************
 \brief three peaks fit plus a linear background


 *******************************************************************************/
RooFitResult *fitRoo(TH1 *hh, int &fr, int &dr, float &pmr, float &pMr, float &ymr, float &yMr, std::string &nfr)
{
    // Set up   component   pdfs
    // ---------------------------------------

    // Declare observable x
    RooRealVar x("x", "m_{#mu^{+}#mu^{-}} (GeV/c^{2})", 8.5001, 11.5);
    RooDataHist rh("rh", "rh", x, Import(*hh));
    // create application to display the canvas while root runs
    TApplication *theApp = new TApplication("app", 0, 0);

    // Build polynomial pdf
    RooRealVar a0("a0", "a0", 3101, 0, 10000);
    RooRealVar a1("a1", "a1", -140.3, -300., 0.);
    RooRealVar a2("a2", "a2", 0, -30., 30.);
    RooPolynomial bkg("bkg", "Background", x, RooArgSet(a0, a1, a2));

    // Create three Gaussian PDFs and their parameters
    RooRealVar mean1("mean1", "mean of gaussians", 9.45, 9.3, 9.6);
    RooRealVar mean2("mean2", "mean of gaussians", 10.01, 9.8, 10.2);
    RooRealVar mean3("mean3", "mean of gaussians", 10.35, 10.15, 10.5);
    RooRealVar sigma1("sigma1", "width of gaussians", 0.054, 0.001, 10);
    RooRealVar sigma2("sigma2", "width of gaussians", 0.032, 0.001, 10);
    RooRealVar sigma3("sigma3", "width of gaussians", 0.020, 0.001, 10);

    RooRealVar r1("r1", "r1", 10, 0.00, 100);
    RooRealVar r2("r2", "r2", 1, 0.00, 100);
    RooRealVar r3("r3", "r3", 1, 0.00, 100);

    // Add signal and background

    RooRealVar fsig1("fsig1", "signal1", 0.4, 0., 1.);
    RooRealVar fsig2("fsig2", "signal2", 0.3, 0., 1.);
    RooRealVar fsig3("fsig3", "signal3", 0.2, 0., 1.);

    RooPlot *xframe = x.frame(Title("Y Resonances Fit"));

    RooAbsPdf *sig1;
    RooAbsPdf *sig2;
    RooAbsPdf *sig3;

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

    RooAddPdf model("model", "model", RooArgList(*sig1, *sig2, *sig3, bkg), RooArgList(fsig1, fsig2, fsig3), kTRUE);

    //  Fit model to data
    RooFitResult *fitResult = model.fitTo(rh, Verbose(false), PrintLevel(-1), Warnings(false), PrintEvalErrors(-1), Timer(true), Save());

    // Draw options
    // ---------------------------------------

    // Plot data and PDF overlaid
    rh.plotOn(xframe, MarkerStyle(6), MarkerSize(1));

    // Overlay the background component of model with a dashed line
    model.plotOn(xframe, Components(bkg), LineColor(41), LineStyle(kDashed));
    // Overlay the sig1 components of model with a dashed-dotted line
    model.plotOn(xframe, Components(RooArgSet(*sig1)), LineColor(46), LineStyle(8));
    // Overlay the background+sig2 components of model with a long dashed line
    model.plotOn(xframe, Components(RooArgSet(*sig2)), LineColor(30), LineStyle(9));

    model.plotOn(xframe, Components(RooArgSet(*sig3)), LineColor(38), LineStyle(kDotted));

    // Print structure of composite pdf
    fitResult->Print("v"); // previous was t

    model.plotOn(xframe, LineWidth(2), LineColor(kRed));

    // Define model and fit
    // ---------------------------------------

    RooHist *hpull = xframe->pullHist();
    // hpull->SetMarkerSize(1);
    hpull->SetMarkerStyle(6);
    hpull->SetLineWidth(0);

    // Draw the frame on the canvas
    auto c1 = new TCanvas("Fit", "Y Resonances Fit", 950, 800);
    TRootCanvas *rc = (TRootCanvas *)c1->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

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
    std::string cut1 = formatPtString(dr, pmr, pMr);
    std::string cut2 = formatYString(dr, ymr, yMr);

    TLatex label;
    label.SetNDC(true);     // cambio di coordinate di riferimento da quelle del grafico a quelle del pad normalizzate
    label.SetTextSize(0.05);
    label.SetTextAlign(22);
    label.DrawLatex(0.75, 0.7, ("#bf{"+cut1+"}").c_str());
    label.DrawLatex(0.75, 0.6, ("#bf{"+cut2+"}").c_str());
    //auto text1 = new TLatex(0.65, 0.7, result1);

    //auto text2 = new TLatex(0.65, 0.6, result2);

    // Draw labels
    //label.SetTextAlign(22); // 22= centrale verticalmente e orizzontalmente
    //label.SetNDC(true);     // cambio di coordinate di riferimento da quelle del grafico a quelle del pad normalizzate
    //label.DrawLatex(0.5, 0.85, "Y(1,2,3S)");
    label.SetTextSize(0.04);
    label.SetTextAlign(11); // left bottom
    label.DrawLatex(0.10, 0.92, "#bf{CMS Open Data}");
    label.SetTextAlign(31); // right bottom
    label.DrawLatex(0.90, 0.92, "#sqrt{s} = 8 TeV, L_{int} = 11.6 fb^{-1}");
    label.Draw();

    pad2->cd();
    pad2->SetBottomMargin(0.4);
    // auto axis = hpull->GetXaxis();

    // axis->SetLimits(8.5001,11.5);
    hpull->GetYaxis()->SetNdivisions(6);
    // hpull->SetMinimum(-9.999);
    hpull->GetXaxis()->SetTitleOffset(1.3);
    hpull->GetXaxis()->SetRangeUser(8.501, 11.5);
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

    namespace fs = std::filesystem;
    if (!fs::is_directory("./Plots") || !fs::exists("./Plots"))
    {
        fs::create_directory("./Plots");
    }
    std::string tmp = "./Plots/" + nfr + ".pdf";
    const char *fname = tmp.c_str();
    c1->SaveAs(fname);
    theApp->Run();
    return fitResult;
}
