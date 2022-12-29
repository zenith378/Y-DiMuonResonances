#include <iostream>
#include "getopt.h"
#include <string>
#include <algorithm>

void PrintHelp() // it prints the available options and flags, then exit
{
   std::cout << "--cutDepth [-d] <n>:            Choose Cut Depth between the options:\n"
                "                                0 (default): select events with two muons of opposite charge\n"
                "                                1: select dimuon pT between 10 and 100 GeV\n"
                "                                2: select dimuon pT between 10 and 100 GeV\n"
                "                                and a rapidity less than 0.6 in abs value\n"
                "--mode [-m] <mode>              Choose functionality of the program between:\n"
                "                                fit: execute fit with given parameters\n"
                "                                cross (default): calculate differential cross section in pt.\n"
                "--fitFunction [-f] <PDFName>:   Choose PDF to use as Fit Function between three options:\n"
                "                                gaus: Gaussian PDF\n"
                "                                bw (default): Breit - Wigner PDF \n"
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

void outOfRangeErrorHandling(std::string opt, std::string range, const char *insrtvl)
{
   std::cerr << opt << " option must be " << range << std::endl;
   std::cerr << "You have entered " << insrtvl << std::endl;
   std::cerr << "Please enter a correct value and retry\n" << std::endl;
   exit(1);
}

void conversionErrorHandling(std::string opt, std::string range, std::invalid_argument err)
{
   std::cerr << err.what() << std::endl;
   std::cerr << opt << " insertion incorrect, please use " << range << "\n" << std::endl;
   exit(1);
}

void unknownErrorHandling()
{
   std::cout << "Unknown error occured. Please contact the authors or open an issue at "
                "https://github.com/zenith378/Y-DiMuonResonances\n"
             << std::endl;
   exit(1);
}

void ProcessArgs(int argc, char **argv, int &dr, int &fr, float &pmr, float &pMr, float &ymr, float &yMr,
                 std::string &nfr, int &vr, int &mr)
{
   const char *const short_opts  = "d:f:m:n:p:P:y:Y:hv"; // Define short options and whether an argument is expected
   const option      long_opts[] = {                     // define long options
                               {"cutDepth", required_argument, 0, 'd'},
                               {"fitFunction", required_argument, 0, 'f'},
                               {"mode", required_argument, 0, 'm'},
                               {"nameFig", required_argument, 0, 'n'},
                               {"ptmin", required_argument, 0, 'p'},
                               {"ptMax", required_argument, 0, 'P'},
                               {"ymin", required_argument, 0, 'y'},
                               {"yMax", required_argument, 0, 'Y'},
                               {"help", no_argument, 0, 'h'},
                               {"verbose", no_argument, 0, 'v'},
                               {0, 0, 0, 0}};
        int option_index = 0; // initialize index to 0
   while (true) {
      const auto opt = getopt_long(argc, argv, short_opts, long_opts, &option_index);

      if (-1 == opt) // if there are no other arguments to read, exit the while
         break;

      switch (opt) // switch over the flags
      {
      case 'd': // depth flag
      {
         try {
            dr = std::stoi(optarg); // convert from string to integer, if the convertion is aborted, std::stdoi() throws
                                    // a std::invalid_argument
            if (dr != 0 && dr != 1 && dr != 2) // if the argument is not in range
            {
               throw(optarg); // throw exception to handle in out of range
            }
         } catch (const char *errore) // errore is the value inserted by the user
         {
            outOfRangeErrorHandling("depth", "0, 1, or 2", errore); // handle exception of type out of range
         } catch (const std::invalid_argument &eD)                  // catch exception from standard library
         {
            conversionErrorHandling("depth", "an integer number (namely 0, 1 or 2)", eD); // handle the exception
         } catch (...) // if exception is an unknown type
         {
            unknownErrorHandling(); // handle the exception
         }
         std::cout << "Cut Depth set to: " << dr << "\n" << std::endl; // if successful, print the customized value

         break;
      }
      case 'f': // fit function flag
      {
         try {
            if (strcmp(optarg, "gaus") == 0)    // compare string from user with string gaus
               fr = 1;                          // if comparison successful, assign 1 to fr
            else if (strcmp(optarg, "bw") == 0) // similar as above
               fr = 0;
            else if (strcmp(optarg, "stud") == 0) // similar as above
               fr = 2;
            else
               throw(optarg); // if argument is not gaus, bw or stud throw exception
         } catch (const char *value) {
            outOfRangeErrorHandling("fitFunction", "gaus, bw or stud", value); // handle out of range option
         } catch (...) {
            unknownErrorHandling(); // handle unknown error
         }
         std::cout << "Fit Function set to: " << optarg << "\n" << std::endl;
         break;
      }
      case 'm': // mode of program (fit or cross). The query is analogue to the fit flag.
      {
         try {
            if (strcmp(optarg, "fit") == 0)
               mr = 0;
            else if (strcmp(optarg, "cross") == 0)
               mr = 1;
            else
               throw(optarg);
         } catch (const char *value) {
            outOfRangeErrorHandling("mode", "fit or cross", value);
         } catch (...) {
            unknownErrorHandling();
         }
         std::cout << "mode set to: " << optarg << "\n" << std::endl;
         break;
      }
      case 'n': // name file
      {
         std::string forbiddenChars("\\/:?\"<>|");          // define list of forbidden chars for a file name
         const char *invalidChars = forbiddenChars.c_str(); // convert it to const char
         try {
            nfr = optarg;
            if (strpbrk(nfr.c_str(), invalidChars) != NULL) // if inserted string contains an invalid char
               throw(nfr);                                  // throw exception to handle
         } catch (std::string nf) {
            std::cerr << "You have inserted illegal characters" << std::endl;
            std::cerr << "Illegal characters will be replaced with an underscore" << std::endl;

            for (auto it = nfr.begin(); it < nfr.end(); ++it) // loop over the chars of inserted string
            {
               bool found = forbiddenChars.find(*it) != std::string::npos; // find invalid characters
               if (found)                                                  // if found
               {
                  *it = '_'; // substitute invalid char with underscore
               }
            }
         } catch (...) {
            unknownErrorHandling(); // handle unknown exception
         }
         std::cout << "Filename set to: " << nfr << "\n" << std::endl;
         break;
      }
      case 'p': // option minimum transverse momentum. Query is similar to fit function
      {
         try {
            pmr = std::stof(optarg);
         } catch (const std::invalid_argument &ep) {
            conversionErrorHandling("ptmin", "a number (float or int)", ep);
         } catch (...) {
            unknownErrorHandling();
         }
         std::cout << "Minimum pt set to: " << pmr << "\n" << std::endl;
         break;
      }
      case 'P': // option maximum transverse momentum. Query is similar to fit function
      {
         try {
            pMr = std::stof(optarg);
         } catch (const std::invalid_argument &eP) {
            conversionErrorHandling("ptMax", "a number (float or int)", eP);
         } catch (...) {
            unknownErrorHandling();
         }
         std::cout << "Maximum pt set to: " << pMr << "\n" << std::endl;
         break;
      }
      case 'y': // option minimum rapidity. Query is similar to fit function
      {
         try {
            ymr = std::stof(optarg);
         } catch (const std::invalid_argument &ey) {
            conversionErrorHandling("ymin", "a number (float or int)", ey);
         } catch (...) {
            unknownErrorHandling();
         }
         std::cout << "Minimum rapidity set to: " << ymr << "\n" << std::endl;
         break;
      }
      case 'Y': // option maximum rapidity. Query is similar to fit function
      {
         try {
            yMr = std::stof(optarg);
         } catch (std::invalid_argument &eY) {
            conversionErrorHandling("yMax", "a number (float or int)", eY);
         } catch (...) {
            unknownErrorHandling();
         }
         std::cout << "Maximum rapidity set to: " << yMr << "\n" << std::endl;
         break;
      }
      case 'v': // verbose flag
      {
         std::cout << "Verbose flag set on\n" << std::endl;
         vr = 1;
         break;
      }
      case '?': // Unrecognized option
         std::cout << "Unrecognized option. Options and flags accepted are the followings:\n" << std::endl;

      case 'h': // -h or --help
      default: PrintHelp(); break;
      }
   }
}
