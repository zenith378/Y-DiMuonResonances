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
#include "TMultiGraph.h"


struct dcsVec{
    float ptm, ptM;
    Double_t s1, s2, s3; //differential cross section for Y 1s, 2s and 3s
};

const int n=21;
const float L =11.6; //[fb^-1]
const float e_uu = 0.75;
const float e_sg = 0.5;
const float e_vp = 0.99;
const float A=1; //accetanza va ridefinita in un altro modo bin per bin
const double BF[3]={2.48/100,1.93/100,2.18/100};   //Branching fraction Y(is) -> mu+ mu- from the PDG


/******************
 * \brief calculate the differential cross section given the functin that describe the mass distribution (model), which Y is (i), and the width of the pt bin (wpt)
 ***************************/
Double_t diffCrossSec(double N, int i, float wpt){
    return N/(BF[i] * L * wpt * e_uu * e_sg * e_vp * A);
}

//per ogni deltaPT estrarre le funzini uscenti dal fit e calcolare la sezine d'urto differenziale in quel bin

dcsVec setset(float ptm, float ptM, ROOT::RDataFrame df, std::string nameFile)
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
    //-------------------------------------------------------------
    //sicuramente sbagliato!i parametri si riaggiornano dopo il fit o no??????
    //forse è meglio fare questa operazione dentro a roofit? not really
    RooArgList lf = fitResult->floatParsFinal();
    std::cout <<"lf=" << lf << std::endl;
    std::cout <<"nsig1=" << lf[7] << std::endl;   //nsig1
    std::cout <<"nsig2=" << lf[8] << std::endl; //nsig2
    std::cout <<"nsig3=" << lf[9] << std::endl; //nsig3

    Double_t nsig1=static_cast<RooAbsReal&>(lf[7]).getVal();
    Double_t nsig2=static_cast<RooAbsReal&>(lf[8]).getVal();
    Double_t nsig3=static_cast<RooAbsReal&>(lf[9]).getVal();

    //-------------------------------------------------------------

    Double_t s1 = diffCrossSec(nsig1, 1, ptM-ptm);
    Double_t s2 = diffCrossSec(nsig2, 2, ptM-ptm);
    Double_t s3 = diffCrossSec(nsig3, 3, ptM-ptm);
    dcsVec avec{ ptm,ptM,
        s1, s2, s3};
    return avec;
}


int diffCrossection(ROOT::RDataFrame df){
    //TApplication *theApp = new TApplication("app", 0, 0);
    double ptm[n] = {12.,14.,16.,18.,20.,22.,24.,26.,28.,30.,32.,34.,36.,38.,40.,43.,46.,50.,55.,60.,70.};
    double ptM[n] = {14.,16.,18.,20.,22.,24.,26.,28.,30.,32.,34.,36.,38.,40.,43.,46.,50.,55.,60.,70.,100.};
    double x[n], y1[n], y2[n], y3[n];
    for(int i;i<n;i++){
        std::string nameFile = "YResonances_"+std::to_string(i); //The name of the file in which the figure is saved

        dcsVec avec = setset(ptm[i], ptM[i], df,nameFile);
        x[i]=ptM[i]-ptm[i];
        y1[i]=avec.s1;
        y2[i]=avec.s2;
        y3[i]=avec.s3;
    }
    
    TCanvas * c1 = new TCanvas("cross section", "Y Resonances differential Cross Section", 950, 800);
    //TRootCanvas *rc = (TRootCanvas *)c1->GetCanvasImp();
    //rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");  
    //disegnare su grafico i punti trovati --> esiste tgraph in roofit????
    auto mg = new TMultiGraph();
    auto g1 = new TGraph(n,x,y1);
    g1->SetTitle("Y(1S,2S,3S)");
    //g1->Draw("AC*");
    mg->Add(g1);
    auto g2 = new TGraph(n,x,y2);
    //g2->Draw("AC*same");
    mg->Add(g2);
    auto g3 = new TGraph(n,x,y3);
    //g3->Draw("AC* same");
    mg->Add(g3);
    mg->Draw("ap");
    c1->Update();
    //theApp->Run();
    c1->SaveAs("./Plots/diffCrossSection.pdf");
    return 0;
}




