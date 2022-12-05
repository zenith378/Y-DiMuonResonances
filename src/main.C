#include "df_set.h"
#include "Cuts.h"
#include "SpectrumPlot.h"
#include "fitp1.h"
#include "fitRoo.h"
#include "ROOT/RDataFrame.hxx"


int main(){
    ROOT::RDataFrame df=df_set();
    ROOT::RDataFrame df_cut=Cuts(df);
    TH1* h=SpectrumPlot(df_cut);
    //fitp1(h,8.5,11.5);
    fitRoo(h);
    return 0;
}
