/**************************************************************
 * \file optionParse.h
 * \brief Handling flags and option parameters
 *
 *
 *
 * In this file are defined the functions used in order to handle flags and command line options.
 *
 *******************************************************************************/
#ifndef optionParse_h
#define optionParse_h

#include <iostream>
#include "getopt.h"
#include <string>
#include <algorithm>

/***************************************
 *
 * \brief Print help stream for understanding what options and flags are available, then exit
 *
 * Print the following output:
 *
 * --cutDepth [-d] <n>:             Choose Cut Depth between the options:
 * 
 *                                  0 (default): select events with two muons of opposite charge
 * 
 *                                               and invariant mass around Y-State
 * 
 *                                  1: select dimuon pT between 10 and 100 GeV
 * 
 *                                  2: select dimuon pT between 10 and 100 GeV
 * 
 *                                  and a rapidity less than 0.6 in abs value
 * 
 * --mode [-m] <mode>:              Choose functionality of the program between:
 * 
 *                                  fit: execute fit with given parameters
 * 
 *                                  cross (default): calculate differential cross section in pt.
 * 
 * --fitFunction [-f] <PDFName>:    Choose PDF to use as Fit Function between three options:
 * 
 *                                  gaus: Gaussian PDF
 * 
 *                                  bw (default): Breit - Wigner PDF
 * 
 *                                  stud: t-Student PDF
 * 
 * --nameFig [-n] <figName>         name of file in which the figure of the fit is going to be saved
 * 
 * --ptmin [-p] <val>:              Set minimum cut on pt (GeV)
 * 
 * --ptMax [-P] <val>:              Set maximum cut on pt (GeV)
 * 
 * --ymin [-y] <val>:               Set minimum cut on rapidity
 * 
 * --yMax [-Y] <val>:               Set maximum cut on rapidity
 * 
 * --help [-h]:                     Show help
 * 
 * --verbose [-v]:                  Verbose Fit, shows minimisation steps.
 * 
 ****************************************/
void PrintHelp();

/***************************************
 *
 * \brief Handle an exception of type "Out of Range".
 *
 * Print the range for the option passed to the function,
 * then prints the out-of-range value inserted
 * by the user and ask to insert an acceptable value.
 * Then exit with error.
 *
 * @param opt option in case (e.g. depth, fitFunction, etc...)
 * @param range range accepted of the desired option (e.g. 0,1,2 for depth)
 * @param insrtvl invalid value entered by the user in the command line stream
 *
 * \return error exit
 *
 ****************************************/
void outOfRangeErrorHandling(std::string opt, std::string range, const char *insrtvl);

/***************************************
 *
 * \brief Handles an exception of type "conversion Error".
 *
 * Print the type of exception (thrown by the stardard library),
 * then it prints the accepted type for the option and exit with failure.
 *
 * @param opt option in case (e.g. depth, fitFunction, etc...)
 * @param range type accepted for the option (e.g. float for ym)
 * @param err invalid argument raised by the standard library
 *
 * \return error exit
 *
 ****************************************/
void conversionErrorHandling(std::string opt, std::string range, std::invalid_argument err);

/***************************************
 *
 * \brief Handle an exception of type "unknown".
 *
 * Print the string "*Unknown error occured. Please contact the authors or open an issue at
 *https://github.com/zenith378/Y-DiMuonResonances*" and exit with error.
 *
 * \return error exit
 *
 ****************************************/
void unknownErrorHandling();

/***********************************************************
 * \brief Implementation of the option arguments with the library <getopt>.
 *
 * Read options and flags from command line and store it in the function arguments passed as reference
 * If the command line arguments do not exists, are out of range, or the format is invalid, the fucntions handles
 * these cases as exceptions described in the functions \c outOfRangeErrorHandling() ,
 * \c coversionErrorHandling() , \c unknownErrorHandling() .
 *
 * If no command arguments are passed, the function do nothing and the values remain the same defined in \c main() .
 *
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
 * @param mr reference of option mode
 *************************************************************/
void ProcessArgs(int argc, char **argv, int &dr, int &fr, float &pmr, float &pMr, float &ymr, float &yMr,
                 std::string &nfr, int &vr, int &mr);

#endif
