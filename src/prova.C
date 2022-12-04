//##########################################################################################
//#                                                                                        #
//#g++ -o prova prova.C `root-config --cflag --libs` -L ../include/ -lnamelibrary          #
//#./prova                                                                                 #
//# oppure                                                                                 #
//#g++ prova.C $(root-config --glibs --cflag --libs) -L ../include/ -lnamelibrary -o prova #
//#./prova                                                                                 #
//#                                                                                        #
//##########################################################################################

#include "../include/df_set.h"
#include "../include/Cuts.h"
#include "../include/SpettrumPlot.h"
#include "../include/fitp1.h"
#include "ROOT/RDataFrame.hxx"


int prova(){
    ROOT::RDataFrame df=df_set();
    ROOT::RDataFrame df_cut=Cuts(df);
    TH1* h=SpettrumPlot(df_cut);
    fitp1(h,8.5,11.5);
    return 0;
}
