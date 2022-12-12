/**************************************************************
 * \file main.C
 * \brief Main source code of the program
 *
 *
 *
 * Here the default options are defined and the functions are called in order to execute the programs
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
#include "fitRoo.h"
#include "optionParse.h"
#include "ROOT/RDataFrame.hxx"
#include <iostream>

int main(int argc, char *argv[])
{
    // initialize default values for options
    int depth = 0; //Depth value initialized to 0, i.e. no cuts
    int fitfunc = 0; //Fit Function initilized to 0, i.e. Breit-Wigner
    float ptm = std::nanf("1"); //The default parameters for the cuts are initialized to NaN, so that they are used only if they become a number
    float ptM = std::nanf("2"); //see first parameter
    float ym = std::nanf("3"); //see first parameter
    float yM = std::nanf("4"); //see first parameter
    std::string nameFile = "YResonancesFit"; //The name of the file in which the figure is saved
    int verbose = 0; //verbose flag initialized to zero, i.e. no output stream for Minuit
    ProcessArgs(argc, argv, depth, fitfunc, ptm, ptM, ym, yM, nameFile, verbose);
    ROOT::RDataFrame df = df_set();
    ROOT::RDF::RNode df_cut = Cuts(df, depth, ptm, ptM, ym, yM);
    TH1 *h = SpectrumPlot(df_cut);
    fitRoo(h, fitfunc, depth, ptm, ptM, ym, yM, nameFile, verbose);
    return 0;
}
