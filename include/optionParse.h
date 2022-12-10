#ifndef optionParse_h
#define optionParse_h

#include <iostream>
#include "getopt.h"

/***************************************
 * Print help stream for understanding what options are available
 *
 ****************************************/
void PrintHelp();

void outOfRangeErrorHandling(std::string opt, std::string range, const char *insrtvl);

void conversionErrorHandling(std::string opt, std::string range, std::invalid_argument err);

void unknownErrorHandling();


/***********************************************************
 * Implementation of the option arguments with the library <getopt>
 * @param argc
 * @param argv
 * @param dr
 * @param fr
 * @param pmr
 * @param pMr
 * @param ymr
 * @param yMr
 *************************************************************/
void ProcessArgs(int argc, char **argv,int &dr,int &fr, float &pmr, float &pMr, float &ymr, float &yMr, std::string &nfr);

#endif