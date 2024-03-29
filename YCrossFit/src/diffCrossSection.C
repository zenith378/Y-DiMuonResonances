#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "RooFitResult.h"
#include <filesystem>
#include "TSystem.h"
#include "RooAbsPdf.h"
#include "Cuts.h"
#include "SpectrumPlot.h"
#include "fitRoo.h"
#include "optionParse.h"
#include "diffCrossSection.h"
#include "TApplication.h"
#include "TRootCanvas.h"
#include "TGraph.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "SpectrumPlot.h"
#include "TROOT.h"

Double_t diffCrossSec(double N, float wpt)
{
   const float L = 11.6;    //[fb^-1]
   const float e_uu = 0.75; // efficiencies
   const float e_sg = 0.5;
   const float e_vp = 0.99;
   return N / (L * wpt * e_uu * e_sg * e_vp);
}

dcsbin setset(float ptm, float ptM, float ym, float yM, ROOT::RDF::RNode &df, std::string nameFile)
{
   // initialize default values for options
   int fitfunc = 1; // Fit Function initilized to 0, i.e. Breit-Wigner
   int verbose = 0; // verbose flag initialized to zero, i.e. no output stream for Minuit
   ROOT::RDF::RNode df_cut = Cuts(df, ptm, ptM, ym, yM);
   TH1 *h = SpectrumPlot(df_cut, nameFile, 1);
   RooFitResult *fitResult = fitRoo(h, 1, fitfunc, ptm, ptM, ym, yM, nameFile, verbose);

   // list of the parameter values of the fitted function
   RooArgList lf = fitResult->floatParsFinal();
   // covariant matrix
   TMatrixDSym cov = fitResult->covarianceMatrix();

   // convert from RooAbsArg to double
   Double_t nsig1 = static_cast<RooAbsReal &>(lf[7]).getVal();
   Double_t nsig2 = static_cast<RooAbsReal &>(lf[8]).getVal();
   Double_t nsig3 = static_cast<RooAbsReal &>(lf[9]).getVal();
   Double_t s1 = diffCrossSec(nsig1, ptM - ptm);
   Double_t s2 = diffCrossSec(nsig2, ptM - ptm);
   Double_t s3 = diffCrossSec(nsig3, ptM - ptm);

   //  find the nsig uncertainties from the covariant matrix
   Double_t dnsig1 = std::sqrt(cov[7][7]);
   Double_t dnsig2 = std::sqrt(cov[8][8]);
   Double_t dnsig3 = std::sqrt(cov[9][9]);
   Double_t ds1 = diffCrossSec(dnsig1, ptM - ptm);
   Double_t ds2 = diffCrossSec(dnsig2, ptM - ptm);
   Double_t ds3 = diffCrossSec(dnsig3, ptM - ptm);

   // structure containing the necessary values for the differential cross section plot
   dcsbin abin{ptm, ptM, s1, s2, s3, ds1, ds2, ds3};
   return abin;
}

void PlotDiffCrossSection(ROOT::RDF::RNode &df, float ym, float yM, int cr)
{
   const int n = 21; // number of points for the plot of the differential cross section
   // Define array for binning the differential cross section
   double ptm[n] = {12., 14., 16., 18., 20., 22., 24., 26., 28., 30., 32.,
                    34., 36., 38., 40., 43., 46., 50., 55., 60., 70.};
   double ptM[n] = {14., 16., 18., 20., 22., 24., 26., 28., 30., 32., 34.,
                    36., 38., 40., 43., 46., 50., 55., 60., 70., 100.};

   double A1[n] = {0.37, 0.43, 0.48, 0.52, 0.56, 0.59, 0.62, 0.64, 0.66, 0.67, 0.69,
                   0.70, 0.71, 0.72, 0.74, 0.75, 0.77, 0.79, 0.80, 0.82, 0.87};
   double A2[n] = {0.36, 0.42, 0.47, 0.52, 0.55, 0.58, 0.60, 0.62, 0.64, 0.66, 0.68,
                   0.69, 0.71, 0.71, 0.73, 0.75, 0.76, 0.78, 0.80, 0.82, 0.87};
   double A3[n] = {0.37, 0.42, 0.47, 0.51, 0.54, 0.58, 0.61, 0.63, 0.65, 0.67, 0.68,
                   0.69, 0.71, 0.72, 0.74, 0.75, 0.77, 0.79, 0.81, 0.83, 0.88};
   // define arrays for constructing the Graph of the differential cross section
   double x[n], y1[n], y2[n], y3[n], dx[n], dy1[n], dy2[n], dy3[n];
   if (yM != yM)
      yM = 1.2;                  // cut on absolute value of rapidity if not already inizialized
   for (int i = 0; i < n; i++) { // loop over the points
      // The name of the file in which the figure is saved for every iteration of fitRoo
      std::string nameFile = "YResonances_" + std::to_string(i);
      // in the structure abin the values of the three cross section are saved
      dcsbin abin = setset(ptm[i], ptM[i], ym, yM, df, nameFile);
      x[i] = (ptm[i] + ptM[i]) / 2;  // center of the bin
      dx[i] = (ptM[i] - ptm[i]) / 2; // coverage of binning
      y1[i] = abin.s1 / A1[i];       // cross section of Y(1S)
      y2[i] = abin.s2 / A2[i];       // cross section of Y(2S)
      y3[i] = abin.s3 / A3[i];       // cross section of Y(3S)
      dy1[i] = abin.ds1 / A1[i];     // error of cross section of Y(1S)
      dy2[i] = abin.ds2 / A2[i];     // error of cross section of Y(2S)
      dy3[i] = abin.ds3 / A3[i];     // error of cross section of Y(3S)
   }
   // if mute canvas flag is on, do not display canvas
   if (cr == 1)
      gROOT->SetBatch(1);
   TCanvas *c1 = new TCanvas("cross section", "Y Resonances differential Cross Section", 950, 800);

   // DRAWING

   // log scale
   gPad->SetLogy();

   // multigraph for plotting the cross section in one canvas
   auto mg = new TMultiGraph();
   mg->SetTitle("Differential Cross Section;p_{T}  [GeV];#frac{d#sigma}{dp_{T}} x Br(#mu^{-}#mu^{+}) [fb/GeV]");

   // Y(1S)
   auto g1 = new TGraphErrors(n, x, y1, dx, dy1);
   g1->SetMarkerColor(46);
   g1->SetLineColor(46);
   g1->SetMarkerStyle(7);
   mg->Add(g1);

   // Y(2S)
   auto g2 = new TGraphErrors(n, x, y2, dx, dy2);
   g2->SetMarkerColor(30);
   g2->SetLineColor(30);
   g2->SetMarkerStyle(7);
   mg->Add(g2);

   // Y(3S)
   auto g3 = new TGraphErrors(n, x, y3, dx, dy3);
   g3->SetMarkerColor(38);
   g3->SetLineColor(38);
   g3->SetMarkerStyle(7);
   mg->Add(g3);

   // styling options
   mg->Draw("ap");
   mg->GetXaxis()->SetLabelFont(43);
   mg->GetYaxis()->SetLabelFont(43);
   mg->GetXaxis()->SetLabelSize(22);
   mg->GetYaxis()->SetLabelSize(22);
   mg->GetXaxis()->SetTitleFont(43);
   mg->GetYaxis()->SetTitleFont(43);
   mg->GetXaxis()->SetTitleSize(22);
   mg->GetYaxis()->SetTitleSize(22);

   // text printed on the canvas
   TLatex label;
   label.SetNDC(true); // normalized pad coordinates
   label.SetTextSize(0.035);
   label.SetTextAlign(22); // central vertically and horizontally
   // cuts
   if (ym != ym && yM != yM)
      label.DrawLatex(0.75, 0.78, "#bf{|y| < 1.2}");
   else {
      TString cut = formatYString(ym, yM);
      label.DrawLatex(0.75, 0.78, ("#bf{" + cut + "}"));
   }
   label.SetTextAlign(11);
   label.DrawLatex(0.1, 0.92, "#bf{CMS Open Data}");
   label.SetTextAlign(31);
   label.DrawLatex(0.90, 0.92, "11.6 fb^{-1} (8 TeV)"); // #sqrt{s} = 8 TeV, L_{int} = 11.6 fb^{-1}
   label.Draw();

   // legend
   TLegend *leg = new TLegend(0.68, 0.6, 0.85, 0.75); // xmin, ymin, xmax, ymax
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   leg->AddEntry(g1, "Y(1S)", "lp");
   leg->AddEntry(g2, "Y(2S)", "lp");
   leg->AddEntry(g3, "Y(3S)", "lp");
   leg->Draw();
   if (cr == 0) {
      TRootCanvas *rc = (TRootCanvas *)c1->GetCanvasImp();
      rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
   }
   // update the canvas, save it and display it
   c1->Update();
   SavePlot(c1, "diffCrossSection");
}