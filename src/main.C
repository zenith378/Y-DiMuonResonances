/**************************************************************
 * \file main.C
 * \brief Main source code of the program
 *
 *
 *
 * Here are implemented the options and the functions are called
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
#include "optionParse.h"
#include "ROOT/RDataFrame.hxx"
#include <iostream>

int main(int argc, char *argv[])
{
    // initialize default values for options
    int depth = 0;
    int fitfunc = 0;
    float ptm = std::nanf("1");
    float ptM = std::nanf("2");
    float ym = std::nanf("3");
    float yM = std::nanf("4");
    std::string nameFile = "YResonancesFit";

    ProcessArgs(argc, argv, depth, fitfunc, ptm, ptM, ym, yM, nameFile);

    ROOT::RDataFrame df = df_set();

    ROOT::RDF::RNode df_cut = Cuts(df, depth, ptm, ptM, ym, yM);

    TH1 *h = SpectrumPlot(df_cut);

    fitRoo(h, fitfunc, depth, ptm, ptM, ym, yM, nameFile);

    return 0;
}
