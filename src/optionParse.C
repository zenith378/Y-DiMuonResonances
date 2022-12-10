#include <iostream>
#include "getopt.h"
#include <string>
#include <algorithm>

/***************************************
 * Print help stream for understanding what options are available
 *
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
                 "--help [-h]:                    Show help\n";
    exit(0);
}

void outOfRangeErrorHandling(std::string opt, std::string range, const char *insrtvl)
{
    std::cerr << opt << " option must be " << range << std::endl;
    std::cerr << "You have entered " << insrtvl << std::endl;
    std::cerr << "Please enter a correct value and retry" << std::endl;
    exit(1);
}

void conversionErrorHandling(std::string opt, std::string range, std::invalid_argument err)
{
    std::cerr << err.what() << "\n";
    std::cerr << opt << " insertion incorrect, please use " << range << std::endl;
    exit(1);
}

void unknownErrorHandling()
{
    std::cout << "Unknown error occured. Please contact the authors or open an issue at https://github.com/zenith378/Y-DiMuonResonances" << std::endl;
    exit(1);
}

/***********************************************************
 * Implementation of the option arguments with the library <getopt>
 * @param argc
 * @param argv
 *************************************************************/
void ProcessArgs(int argc, char **argv, int &dr, int &fr, float &pmr, float &pMr, float &ymr, float &yMr, std::string &nfr)
{
    const char *const short_opts = "d:f:n:p:P:y:Y:h";
    const option long_opts[] = {
        {"cutDepth", required_argument, 0, 'd'},
        {"fitFunction", required_argument, 0, 'f'},
        {"nameFig", required_argument, 0, 'n'},
        {"ptmin", required_argument, 0, 'p'},
        {"ptMax", required_argument, 0, 'P'},
        {"ymin", required_argument, 0, 'y'},
        {"yMax", required_argument, 0, 'Y'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}};
    int option_index = 0;
    while (true)
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
            std::cout << "Cut Depth set to: " << dr << std::endl;

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
            std::cout << "Fit Function set to: " << optarg << std::endl;
            break;
        }
        case 'n':
        {
            std::string forbiddenChars("\\/:?\"<>|");
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
            std::cout << "Filename set to: " << nfr << std::endl;
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
            std::cout << "Minimum pt set to: " << pmr << std::endl;
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
            std::cout << "Maximum pt set to: " << pMr << std::endl;
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
            std::cout << "Minimum rapidity set to: " << ymr << std::endl;
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
            std::cout << "Maximum rapidity set to: " << yMr << std::endl;
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