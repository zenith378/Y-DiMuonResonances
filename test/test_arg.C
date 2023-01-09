
#include <iostream>
#include "optionParse.h"
#include <cmath>

int main(int argc, char *argv[])
{
   // initialize default values for options
   int depth = 0;   // Depth value initialized to 0, i.e. no cuts
   int fitfunc = 0; // Fit Function initilized to 0, i.e. Breit-Wigner

   float ptm = std::nan("1");  // The default parameters for the cuts are initialized to NaN, so that they are used only
                               // if they become a number
   float ptM = std::nanf("2"); // see first parameter
   float ym = std::nanf("3");  // see first parameter
   float yM = std::nanf("4");  // see first parameter
   std::string nameFile = "YResonancesFit"; // The name of the file in which the figure is saved
   int verbose = 0;                         // verbose flag initialized to zero, i.e. no output stream for Minuit
   int mode = 0;
   int ca = 0;
   ProcessArgs(argc, argv, depth, fitfunc, ptm, ptM, ym, yM, nameFile, verbose, mode, ca); // call default option
   // check if the variable are set correctly
   if (depth != 0 && fitfunc != 0 && ptm == ptm && ptM == ptM && ym == ym && yM == yM && nameFile != "YResonancesFit" &&
       verbose != 0 && mode != 0 && ca!=0)
      exit(1);
   // redefine parameters
   depth = 1;
   fitfunc = 1;
   ptm = 25;
   ptM = std::nanf("2");
   ym = 0.1;
   yM = 0.8;
   verbose = 1;
   mode = 1;
   ca = 1;
   nameFile = "Y?Resonances\\/:?\"<>|Fit";
   // check if parameters are adjourned
   ProcessArgs(argc, argv, depth, fitfunc, ptm, ptM, ym, yM, nameFile, verbose, mode,ca);
   if (depth != 1 && fitfunc != 1 && ptm != 25 && ptM == ptM && ym != 0.1 && yM != 0.8 &&
       nameFile != "Y_Resonances________Fit" && verbose != 1 && mode != 1 && ca !=1)
      exit(1);

   return 0;
}