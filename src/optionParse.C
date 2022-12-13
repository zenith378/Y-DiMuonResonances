/**************************************************************
 * \file optionParse.C
 * \brief Handling flags and option parameters
 *
 *
 *
 * In this file are defined the functions used in order to handle flags and command line options.
 * In particular, it is defined:
 * 
 * void PrintHelp(): output of the option help. It shows the available options and flags;
 * 
 * void outOfRangeErrorHandling(std::string opt, std::string range, const char *insrtvl): 
 * it handles an exception of type "Out of Range";
 * 
 * void conversionErrorHandling(std::string opt, std::string range, std::invalid_argument err):
 * it handles an exception of type Conversion Error (string to float or string to int);
 * 
 * void unknowErrorHandling(): it handles an error of uknown type;
 * 
 * voi ProcessArgs(...): it handles command line inputs and stores the values of flags or options.
 *
 *******************************************************************************/
#include <iostream>
#include "getopt.h"
#include <string>
#include <algorithm>

/***************************************
 * PrintHelp()
 * \brief Print help stream for understanding what options and flags are available
 * 
 * When the flag --help [-h] is called, it prints the following output:
 * 
 * --cutDepth [-d] <n>:             Choose Cut Depth between the options:
 *                                  0 (default): select events with two muons of opposite charge
 *                                  1: select dimuon pT between 10 and 100 GeV
 *                                  2: select dimuon pT between 10 and 100 GeV
 *                                  and a rapidity less than 0.6 in abs value
 * --fitFunction [-f] <PDFName>:    Choose PDF to use as Fit Function between three options:
 *                                  gaus: Gaussian PDF
 *                                  bw: Breit - Wigner PDF
 *                                  stud: t-Student PDF
 * --nameFig [-n] <figName>         name of file in which the figure of the fit is going to be saved
 * --ptmin [-p] <val>:              Set minimum cut on pt (GeV)
 * --ptMax [-P] <val>:              Set maximum cut on pt (GeV)
 * --ymin [-y] <val>:               Set minimum cut on rapidity
 * --yMax [-Y] <val>:               Set maximum cut on rapidity
 * --help [-h]:                     Show help
 * --verbose [-v]:                  Verbose Fit, shows minimisation steps.
 ****************************************/
void PrintHelp()
{
    std::cout << "--cutDepth [-d] <n>:            Choose Cut Depth between the options:\n"
                 "                                0 (default): select events with two muons of opposite charge\n"
                 "                                1: select dimuon pT between 10 and 100 GeV\n"
                 "                                2: select dimuon pT between 10 and 100 GeV\n"
                 "                                and a rapidity less than 0.6 in abs value\n"
                 "--fitFunction [-f] <PDFName>:   Choose PDF to use as Fit Function between three options:\n"
                 "                                gaus: Gaussian PDF\n"
                 "                                bw: Breit - Wigner PDF \n"
                 "                                stud: t-Student PDF\n"
                 "--nameFig [-n] <figName>        name of file in which the figure of the fit is going to be saved\n"
                 "--ptmin [-p] <val>:             Set minimum cut on pt (GeV)\n"
                 "--ptMax [-P] <val>:             Set maximum cut on pt (GeV)\n"
                 "--ymin [-y] <val>:              Set minimum cut on rapidity\n"
                 "--yMax [-Y] <val>:              Set maximum cut on rapidity\n"
                 "--help [-h]:                    Show help\n"
                 "--verbose [-v]:                 Verbose Fit, shows minimisation steps.\n";
    exit(0);
}

/***************************************
 * outOfRangeErrorHandling
 * \brief it handles an exception of type "Out of Range";
 *
 * @param opt option in case (e.g. depth, fitFunction, etc...)
 * @param range range accepted of the desired option (e.g. 0,1,2 for depth)
 * @param insrtvl invalid value entered by the user in the command line stream
 * 
 * \return error exit
 * 
 ****************************************/
void outOfRangeErrorHandling(std::string opt, std::string range, const char *insrtvl)
{
    std::cerr << opt << " option must be " << range << std::endl;
    std::cerr << "You have entered " << insrtvl << std::endl;
    std::cerr << "Please enter a correct value and retry\n" << std::endl;
    exit(1);
}

/***************************************
 * conversionErrorHandling()
 * \brief it handles an exception of type "conversion Error";
 * @param opt option in case (e.g. depth, fitFunction, etc...)
 * @param range range accepted of the desired option (e.g. 0,1,2 for depth)
 * @param err invalid argument raised by the standard library
 * 
 * \return error exit
 * 
 ****************************************/
void conversionErrorHandling(std::string opt, std::string range, std::invalid_argument err)
{
    std::cerr << err.what() << std::endl;
    std::cerr << opt << " insertion incorrect, please use " << range <<"\n"<< std::endl;
    exit(1);
}

/***************************************
 * unknownErrorHandling()
 * \brief it handles an exception of type "unknown"
 * 
 * \return error exit
 * 
 ****************************************/
void unknownErrorHandling()
{
    std::cout << "Unknown error occured. Please contact the authors or open an issue at https://github.com/zenith378/Y-DiMuonResonances\n" << std::endl;
    exit(1);
}

/***********************************************************
 * Implementation of the option arguments with the library <getopt>
 * @param argc command line arguments number
 * @param argv array of command line arguments passed
 * @param dr reference of depth defined in main
 * @param fr reference of fitFunction defined in main
 * @param pmr recerence of ptm (minimum pt) defined in main
 * @param pMr recerence of ptM (maximum pt) defined in main
 * @param ymr recerence of ym (minimum rapidity) defined in main
 * @param yMr recerence of yM (maximum rapidity) defined in main
 * @param nfr recerence of nameFile defined in main
 * @param vr reference of varaible verbose defined in main
 *************************************************************/
void ProcessArgs(int argc, char **argv, int &dr, int &fr, float &pmr, float &pMr, float &ymr, float &yMr, std::string &nfr, int &vr)
{
    const char *const short_opts = "d:f:n:p:P:y:Y:hv"; ///Define short options and whether an argument is expected
    const option long_opts[] = { ///define long options
        {"cutDepth", required_argument, 0, 'd'},
        {"fitFunction", required_argument, 0, 'f'},
        {"nameFig", required_argument, 0, 'n'},
        {"ptmin", required_argument, 0, 'p'},
        {"ptMax", required_argument, 0, 'P'},
        {"ymin", required_argument, 0, 'y'},
        {"yMax", required_argument, 0, 'Y'},
        {"help", no_argument, 0, 'h'},
        {"verbose", no_argument, 0, 'v'},
        {0, 0, 0, 0}};
    int option_index = 0; ///initialize index to 0
    while (true) ///
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, &option_index);

        if (-1 == opt)
            break;

        switch (opt)
        {
        case 'd':
        {
            try
            {
                dr = std::stoi(optarg);
                if (dr != 0 && dr != 1 && dr != 2)
                {
                    throw(optarg);
                }
            }
            catch (const char *errore)
            {
                outOfRangeErrorHandling("depth", "0, 1, or 2", errore);
            }
            catch (const std::invalid_argument &eD)
            {
                conversionErrorHandling("depth", "an integer number (namely 0, 1 or 2)", eD);
            }
            catch (...)
            {
                unknownErrorHandling();
            }
            std::cout << "Cut Depth set to: " << dr << "\n" << std::endl;

            break;
        }
        case 'f':
        {
            try
            {
                if (strcmp(optarg, "gaus") == 0)
                    fr = 1;
                else if (strcmp(optarg, "bw") == 0)
                    fr = 0;
                else if (strcmp(optarg, "stud") == 0)
                    fr = 2;
                else
                    throw(optarg);
            }
            catch (const char *value)
            {
                outOfRangeErrorHandling("fitFunction", "gaus, bw or stud", value);
            }
            catch (...)
            {
                unknownErrorHandling();
            }
            std::cout << "Fit Function set to: " << optarg << "\n" << std::endl;
            break;
        }
        case 'n':
        {
            std::string forbiddenChars("\\/:?\"<>|"); //E SE VOGLIO DARE IL PATH DI UN FILE? MI SERVE "/"
            const char * invalidChars= forbiddenChars.c_str();
            try
            {
                nfr = optarg;
                if (strpbrk(nfr.c_str(), invalidChars) != NULL)
                    throw(nfr);
            }
            catch (std::string nf)
            {
                std::cerr << "You have inserted illegal characters" << std::endl;
                std::cerr << "Illegal characters will be replaced with an underscore" << std::endl;
                auto it = nfr.begin();
                for (it = nfr.begin(); it < nfr.end(); ++it)
                {
                    bool found = forbiddenChars.find(*it) != std::string::npos;
                    if (found)
                    {
                        *it = '_';
                    }
                }
            }
            catch (...)
            {
                unknownErrorHandling();
            }   
            std::cout << "Filename set to: " << nfr << "\n" << std::endl;
            break;

        }
        case 'p':
        {
            try
            {
                pmr = std::stof(optarg);
            }
            catch (const std::invalid_argument &ep)
            {
                conversionErrorHandling("ptmin", "a number (float or int)", ep);
            }
            catch (...)
            {
                unknownErrorHandling();
            }
            std::cout << "Minimum pt set to: " << pmr << "\n" << std::endl;
            break;
        }
        case 'P':
        {
            try
            {
                pMr = std::stof(optarg);
            }
            catch (const std::invalid_argument &eP)
            {
                conversionErrorHandling("ptMax", "a number (float or int)", eP);
            }
            catch (...)
            {
                unknownErrorHandling();
            }
            std::cout << "Maximum pt set to: " << pMr << "\n" << std::endl;
            break;
        }
        case 'y':
        {
            try
            {
                ymr = std::stof(optarg);
            }
            catch (const std::invalid_argument &ey)
            {
                conversionErrorHandling("ymin", "a number (float or int)", ey);
            }
            catch (...)
            {
                unknownErrorHandling();
            }
            std::cout << "Minimum rapidity set to: " << ymr << "\n" << std::endl;
            break;
        }
        case 'Y':
        {
            try
            {
                yMr = std::stof(optarg);
            }
            catch (std::invalid_argument &eY)
            {
                conversionErrorHandling("yMax", "a number (float or int)", eY);
            }
            catch (...)
            {
                unknownErrorHandling();
            }
            std::cout << "Maximum rapidity set to: " << yMr <<  "\n" << std::endl;
            break;
        }
        case 'v': 
        {
            std::cout << "Verbose flag set on\n" << std::endl;
            vr=1;
            break;
        }
        case '?': // Unrecognized option
            std::cout << "Unrecognized option. Options and flags accepted are the followings:\n" << std::endl;

        case 'h': // -h or --help
        default:
            PrintHelp();
            break;
        }
    }
}
