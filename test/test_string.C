#include <iostream>
#include "fitRoo.h"
#include "TString.h"

int main()
{
   TString provapt = formatPtString(std::nanf("1"), std::nanf("2"));
   TString provay = formatYString(std::nanf("1"), std::nanf("2"));
   int status1 = provapt.CompareTo("");
   int status2 = provay.CompareTo("");
   if (status1 != 0 && status2 != 0) {
      std::cerr << "Could not format default value" << std::endl;
      std::cerr << provapt << std::endl;
      std::cerr << provay << std::endl;
      exit(1);
   }
   float ptm = 5;
   float ptM = 2.143948;
   float ym = 0.1;
   float yM = 2.35725;
   provapt = formatPtString(ptm, ptM);
   provay = formatYString(ym, yM);
   int status = provapt.CompareTo("5.0 < p_{T} < 2.1 GeV");
   if (status != 0) {
      std::cerr << "Could not format custom values" << std::endl;
      std::cerr << provapt << std::endl;
      std::cerr << "5.0 < p_{T} < 2.1 GeV" << std::endl;
      exit(1);
   }
   status = provay.CompareTo("0.10 < |y| < 2.36");
   if (status != 0) {
      std::cerr << "Could not format custom values" << std::endl;
      std::cerr << provay << std::endl;
      exit(1);
   }
   provapt = formatPtString(std::nanf("1"), ptM);
   provay = formatYString(ym, std::nanf("2"));
   status1 = provapt.CompareTo("p_{T} < 2.1 GeV");
   status2 = provay.CompareTo("|y| < 0.10");
   if (status1 != 0 && status2 != 0) {
      std::cerr << "Could not handle nan values" << std::endl;
      std::cerr << provapt << std::endl;
      std::cerr << provay << std::endl;
      exit(1);
   }
   return 0;
}