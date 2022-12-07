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

int main(int argc, char *argv[])
{
    ROOT::RDataFrame df = df_set();
    ROOT::RDataFrame df_cut = Cuts(df);
    TH1 *h = SpectrumPlot(df_cut);
    // fitp1(h,8.5,11.5);
    int i = 0;
    if (argc > 1)
    {
        if (strcmp(argv[1], "gaus") == 0)
        {
            i = 1;
        }
        else if (strcmp(argv[1], "bw") == 0)
        {
            i = 0;
        }
        else
        {
            std::cout << "First argument bust be either gaus or bw" << std::endl;
            std::cout << "Fitting to bw as default" << std::endl;
            i = 0;
        }
    }
    fitRoo(h, i);
    return 0;
}
