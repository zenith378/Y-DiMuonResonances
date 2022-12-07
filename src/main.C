/**************************************************************
* \file main.C
* \brief main code
*
* dalla cartella Y-Dimuon
*
* cmake -S . -B ./build
* questo dice dove buildare il codice da quale sorgente (e crea il makefile)
*
* make -C build
* con questo si compila propriamente e si crea la libreria
*
* ./build/ComputingMethodsCorCar
* questo è un normale comando che dice di far partire l'eseguibile
*
*******************************************************************************/




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
    fitRoo(h,0);
    return 0;
}
