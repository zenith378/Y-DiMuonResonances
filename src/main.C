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
#include <getopt.h>
#include <iostream>

int depth = 0;
int fitfunc = 0;
float ptm=std::nanf("1");
float ptM=std::nanf("2");
float ym=std::nanf("3");
float yM=std::nanf("4");

void PrintHelp()
{
    std::cout << 
    "--cutDepth <n>:            Choose Cut Depth between the default options"
    "                           (long description of the default options...)\n "
    "--fitFunction <PDFName>:   Choose PDF to use as Fit Function. "
    "                            It can either be 'guas'(Gaussian) or"
    "                            'bw'(Breit - Wigner) \n "
    "--ptmin <val>:             Set minimum cut on pt\n"
    "--ptMax <val>:             Set maximum cut on pt\n"
    "--ymin <val>:              Set minimum cut on rapidity\n"
    "--yMax <val>:              Set maximum cut on rapidity\n"
    "--help:                    Show help\n";
    exit(1);
}

void ProcessArgs(int argc, char **argv)
{
    const char *const short_opts = "d:f:p:P:y:Y:h";
    const option long_opts[] = {
        {"cutDepth", required_argument, nullptr, 'd'},
        {"fitFunction", required_argument, nullptr, 'f'},
        {"ptmin", required_argument, nullptr, 'p'},
        {"ptMax", required_argument, nullptr, 'P'},
        {"ymin", required_argument, nullptr, 'y'},
        {"yMax", required_argument, nullptr, 'Y'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, no_argument, nullptr, 0}};

    while (true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
        case 'd':
        {
            depth = std::stoi(optarg);
            std::cout << "Cut Depth set to: " << depth << std::endl;
            break;
        }
        case 'f':
        {
            if (strcmp(optarg, "gaus") == 0)
                fitfunc = 1;
            else if (strcmp(optarg, "bw") == 0)
                fitfunc = 0;
            else if (strcmp(optarg, "stud") == 0)
                fitfunc = 2;    
            /*else
            {
                std::cout << "First argument bust be either gaus or bw" << std::endl;
                exit(1);
            }
            std::cout << "Fit Function set to: " << optarg << std::endl;
            */
            break;
        }
        case 'p':
        {
            ptm = std::stof(optarg);
            std::cout << "Minimum pt set to: " << ptm << std::endl;
            break;
        }
        case 'P':
        {
            ptM = std::stof(optarg);
            std::cout << "Maximum pt set to: " << ptM << std::endl;
            break;
        }
        case 'y':
        {
            ym = std::stof(optarg);
            std::cout << "Minimum rapidity set to: " << optarg << std::endl;
            break;
        }
        case 'Y':
        {
            yM = std::stof(optarg);
            std::cout << "Maximum rapidity set to: " << optarg << std::endl;
            break;
        }
        case 'h': // -h or --help
        case '?': // Unrecognized option
        default:
            PrintHelp();
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    ProcessArgs(argc, argv);
    ROOT::RDataFrame df = df_set();
    ROOT::RDF::RNode df_cut = Cuts(df, depth, ptm,  ptM,  ym,  yM );
    TH1 *h = SpectrumPlot(df_cut);
    fitRoo(h, fitfunc, depth, ptm,  ptM,  ym,  yM);
    return 0;
}
