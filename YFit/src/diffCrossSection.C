/*********************************
 \file diffCrossection.C
 \brief
************************************************/

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
#include "diffCrossection.h"
#include "TApplication.h"
#include "TRootCanvas.h"
#include "TGraph.h"
#include "TLatex.h"
# include "TLegend.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "SpectrumPlot.h"


struct dcsbin{
    float ptm, ptM;
    Double_t s1, s2, s3; //differential cross section for Y 1s, 2s and 3s
    Double_t ds1, ds2, ds3;
};

const int n=21;
const float L =11.6; //[fb^-1]
const float e_uu = 0.75;
const float e_sg = 0.5;
const float e_vp = 0.99;
const float A=1; //accetanza
const double BF[3]={2.48/100,1.93/100,2.18/100};   //Branching fraction Y(is) -> mu+ mu- from the PDG


Double_t diffCrossSec(double N, float wpt){
    return N/(L * wpt * e_uu * e_sg * e_vp * A);
}

//per ogni deltaPT estrarre le funzini uscenti dal fit e calcolare la sezine d'urto differenziale in quel bin

dcsbin setset(float ptm, float ptM, ROOT::RDF::RNode &df, std::string nameFile)
{
    // initialize default values for options
    int depth = 0; //Depth value initialized to 0, i.e. no cuts
    int fitfunc = 1; //Fit Function initilized to 0, i.e. Breit-Wigner
    float ym = std::nanf("0"); //see first parameter
    float yM = 1.2; //see first parameter
    int verbose = 0; //verbose flag initialized to zero, i.e. no output stream for Minuit
    ROOT::RDF::RNode df_cut = Cuts(df, depth, ptm, ptM, ym, yM);
    TH1 *h = SpectrumPlot(df_cut);
    
    
    RooFitResult * fitResult = fitRoo(h, fitfunc, depth, ptm, ptM, ym, yM, nameFile, verbose);

    RooArgList lf = fitResult->floatParsFinal();
    TMatrixDSym cov= fitResult->covarianceMatrix();
//    std::cout <<"lf=" << lf << std::endl;
//    std::cout <<"nsig1=" << lf[7] << std::endl;   //nsig1
//    std::cout <<"nsig2=" << lf[8] << std::endl; //nsig2
//    std::cout <<"nsig3=" << lf[9] << std::endl; //nsig3

    Double_t nsig1=static_cast<RooAbsReal&>(lf[7]).getVal();
    Double_t nsig2=static_cast<RooAbsReal&>(lf[8]).getVal();
    Double_t nsig3=static_cast<RooAbsReal&>(lf[9]).getVal();
    
    Double_t s1 = diffCrossSec(nsig1, ptM-ptm);
    Double_t s2 = diffCrossSec(nsig2, ptM-ptm);
    Double_t s3 = diffCrossSec(nsig3, ptM-ptm);
    
//------------------------------------------------------------------------------------
//  trovare gli errori si nsig a partire dalla matrice di covarianza cov
    Double_t dnsig1=std::sqrt(cov[7][7]); //differenza tra cov(7,7) e cov[7][7]
    Double_t dnsig2=std::sqrt(cov[8][8]);
    Double_t dnsig3=std::sqrt(cov[9][9]);
    Double_t ds1 = diffCrossSec(dnsig1, ptM-ptm);
    Double_t ds2 = diffCrossSec(dnsig2, ptM-ptm);
    Double_t ds3 = diffCrossSec(dnsig3, ptM-ptm);
//------------------------------------------------------------------------------------
    
    dcsbin abin{ ptm,ptM,
        s1, s2, s3,
        ds1, ds2, ds3
    };
    return abin;
}


int diffCrossSection(ROOT::RDF::RNode &df){
    //TApplication *theApp = new TApplication("app", 0, 0);
    double ptm[n] = {12.,14.,16.,18.,20.,22.,24.,26.,28.,30.,32.,34.,36.,38.,40.,43.,46.,50.,55.,60.,70.};
    double ptM[n] = {14.,16.,18.,20.,22.,24.,26.,28.,30.,32.,34.,36.,38.,40.,43.,46.,50.,55.,60.,70.,100.};
    double x[n], y1[n], y2[n], y3[n], dx[n], dy1[n], dy2[n], dy3[n];
    
    for(int i=0;i<n;i++){
        std::string nameFile = "YResonances_"+std::to_string(i); //The name of the file in which the figure is saved

        dcsbin abin = setset(ptm[i], ptM[i], df,nameFile);
        x[i]=(ptm[i]+ptM[i])/2; //il centro del bin
        dx[i]=(ptM[i]-ptm[i])/2;
        y1[i]=abin.s1;
        y2[i]=abin.s2;
        y3[i]=abin.s3;
        dy1[i]=abin.ds1;
        dy2[i]=abin.ds2;
        dy3[i]=abin.ds3;
//        std::cout << y1[i] << std::endl; //OK
    }
    
    TCanvas * c1 = new TCanvas("cross section", "Y Resonances differential Cross Section", 950, 800);
    gPad->SetLogy();
    
    //TRootCanvas *rc = (TRootCanvas *)c1->GetCanvasImp();
    //rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

    auto mg = new TMultiGraph();
    mg->SetTitle("Differential Cross Section;p_{T}  [GeV];#frac{d#sigma}{dp_{T}} x Br(#mu^{-}#mu^{+}) [fb/GeV]");
    
//    auto g1 = new TGraph(n,x,y1);
    auto g1 = new TGraphErrors(n,x,y1,dx,dy1);
//    g1->SetTitle("Y(1s)");
    g1->SetMarkerColor(46);
    g1->SetLineColor(46);
    g1->SetMarkerStyle(7);
    //g1->Draw("AC*");
    mg->Add(g1);

//    auto g2 = new TGraph(n,x,y2);
    auto g2 = new TGraphErrors(n,x,y2,dx,dy2);
//    g2->SetTitle("Y(2s)");
    g2->SetMarkerColor(30);
    g2->SetLineColor(30);
    g2->SetMarkerStyle(7);
    //g2->Draw("AP*SAME");
    mg->Add(g2);

//    auto g3 = new TGraph(n,x,y3);
    auto g3 = new TGraphErrors(n,x,y3,dx,dy3);
//    g3->SetTitle("Y(3s)");
    g3->SetMarkerColor(38);
    g3->SetLineColor(38);
    g3->SetMarkerStyle(7);
    //g3->Draw("AP*SAME");
    mg->Add(g3);
    

    
    mg->Draw("ap");
    mg->GetXaxis()->SetLabelFont(43);
    mg->GetYaxis()->SetLabelFont(43);
    mg->GetXaxis()->SetLabelSize(22);
    mg->GetYaxis()->SetLabelSize(22);
    mg->GetXaxis()->SetTitleFont(43);
    mg->GetYaxis()->SetTitleFont(43);
    mg->GetXaxis()->SetTitleSize(22);
    mg->GetYaxis()->SetTitleSize(22);
    
    //testo e legenda
    //------------------------------------------------------------------------------------
    TLatex label;
    label.SetNDC(true); // cambio di coordinate di riferimento da quelle del grafico a quelle del pad normalizzate
    label.SetTextSize(0.035);
    label.SetTextAlign(22); // central vertically and horizontally
    label.DrawLatex(0.75, 0.78, "#bf{|y| < 1.2}");
    label.SetTextAlign(11); // left bottom
    label.DrawLatex(0.1, 0.92, "#bf{CMS Open Data}");
    label.SetTextAlign(31); // right bottom
    label.DrawLatex(0.90, 0.92, "11.6 fb^{-1} (8 TeV)"); //#sqrt{s} = 8 TeV, L_{int} = 11.6 fb^{-1}
    label.Draw();
    
    TLegend *leg = new TLegend(0.68, 0.6, 0.85, 0.75);  //xmin, ymin, xmax, ymax
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->AddEntry(g1, "Y(1S)", "lp");
    leg->AddEntry(g2, "Y(2S)", "lp");
    leg->AddEntry(g3, "Y(3S)", "lp");
    leg->Draw();;
    //------------------------------------------------------------------------------------
    
    c1->Update();
    SavePlot(c1,"diffCrossSection");
    //theApp->Run();
    return 0;
}
