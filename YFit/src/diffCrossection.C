/*********************************
 \file diffCrossection.C
 \brief
************************************************/

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "Cuts.h"
#include <filesystem>
#include "TSystem.h"
#include "fitRoo.h"


struct dcsVec{
  double ptm, ptM;
  double s1, s2, s3; //differential cross section for Y 1s, 2s and 3s
};

const float L =11.6; //[fb^-1]
const float e_uu = 0.75;
const float e_sg = 0.5;
const float e_vp = 0.99;
const std::list BF=[2.48/100,1.93/100,2.18/100];   // Branching fraction Y(is) -> mu+ mu- from the PDG


/******************
 * \brief calculate the differential cross section given the functin that describe the mass distribution (model), which Y is (i), and the width of the pt bin (wpt)
 ***************************/
double dCs(rooAbsPdf model, int i, float wpt){
    RooRealVar x("x", "x", 8., 12.);
    x.setRange("signal", 8.5, 11.5);
    int N = model.createIntegral(x, Range("signal")) ;
    return N/(BF[i] * L * wpt * e_uu * e_sg * e_vp * A);
}

//per ogni deltaPT estrarre le funzini uscenti dal fit e calcolare la sezine d'urto differenziale in quel bin

dcsVec set(float ptm, float ptM) /
{
    // initialize default values for options
    int depth = 0; //Depth value initialized to 0, i.e. no cuts
    int fitfunc = 0; //Fit Function initilized to 0, i.e. Breit-Wigner
    float ym = -1.2; //see first parameter
    float yM = 1.2; //see first parameter
    std::string nameFile = "YResonances"; //The name of the file in which the figure is saved
    int verbose = 0; //verbose flag initialized to zero, i.e. no output stream for Minuit
    ROOT::RDataFrame df = df_set(); //call df set
    ROOT::RDF::RNode df_cut = Cuts(df, depth, ptm, ptM, ym, yM);
    TH1 *h = SpectrumPlot(df_cut);
    RooFitResult *fitResult = fitRoo(h, fitfunc, depth, ptm, ptM, ym, yM, nameFile, verbose);
    
    //-------------------------------------------------------------
    //sicuramente sbagliato!i parametri sono da ridefinire??
    //forse è meglio fare questa operazione dentro a roofit
    RooAbsPdf* sig1 = fitResult.createHessePdf(RooArgSet(fsig1,mean1,sigma1));
    RooAbsPdf* sig1 = fitResult.createHessePdf(RooArgSet(fsig2,mean2,sigma2));
    RooAbsPdf* sig1 = fitResult.createHessePdf(RooArgSet(fsig3,mean3,sigma3));
    //-------------------------------------------------------------

    //dove salvo questi valori-> dovrei stamparli su un file?riempire un albero?
    Double_t dcs1 = dCs(sig1, 1, ptM-ptm);
    Double_t dcs2 = dCs(sig2, 2, ptM-ptm);
    Double_t dcs3 = dCs(sig3, 3, ptM-ptm);
    dcsVec avec{ ptm, ptM,
        dcs1, dcs2, dcs3};
    return avec;
}
int plotsigma(){
    std::list ptm=[10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,43,46,50,55,60,70];
    std::list ptM=[12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,43,46,50,55,60,70,100];
    
    vector <dcsVec> dcs;
    for(int i;i<22;i++){
        avec = set(pmT[i]-pmt[i]);
        dcs.push_back(avec)
    }
    
    
}

//ripetere quest'operazione per ogni bin
//salvare le sezioni d'urto per ogni bin in una lista o un dataset
//plottare : istogramma con bin diversi o TGraph?

//    std::list ptm=[10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,43,46,50,55,60,70];
//    std::list ptM=[12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,43,46,50,55,60,70,100];

//auto filename = "pt_cpp.csv";
//if(gSystem->AccessPathName(filename)){
//    std::cout << "I can not find the file you are looking for" << std::endl;
//    exit(1);
//}
//auto d = ROOT::RDF::MakeCsvDataFrame(filename,true, ',', -1);
//auto df1 = d.Define("dpt", "ptM-ptm");
//auto dt = d.Define("dcs1", set, {"ptm", "ptM"});
