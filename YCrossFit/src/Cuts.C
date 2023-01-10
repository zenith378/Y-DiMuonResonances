#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "Cuts.h"
#include <filesystem>
#include "TSystem.h"
#include "optionParse.h"

ROOT::RDF::RNode applyFilter(ROOT::RDF::RNode &df_custom_cut, std::string_view filter, std::string_view name)
{
   ROOT::EnableImplicitMT(1);
   df_custom_cut = df_custom_cut.Filter(filter, name); // apply custom filter
   auto count = df_custom_cut.Count();                 // count remaining events
   if (*count < 800) {
      std::cerr << "WARNING: Few events. Fit might not converge.\n" << std::endl;
   }

   return df_custom_cut;
}

ROOT::RDF::RNode Cuts(ROOT::RDF::RNode &df, float pmr, float pMr, float ymr, float yMr)
{
   ROOT::EnableImplicitMT(1);
   ROOT::RDF::RNode df_cut = df; // initialize datafrmae
   if (pmr == pmr)               // if pmr is not nan
   {
      std::string fil = "Dimuon_pt >" + std::to_string(pmr);         // format string to pass to the filter function
      df_cut = applyFilter(df_cut, fil, "Custom cut on minimum pt"); // apply filter
   }
   if (pMr == pMr) // similar as above
   {
      std::string fil = "Dimuon_pt <" + std::to_string(pMr);
      df_cut = applyFilter(df_cut, fil, "Custom cut on maximum pt");
   }

   if (ymr == ymr && yMr == yMr) // similar as above
   {
      df_cut = df_cut.Filter([ymr, yMr](float x) { return ((x > -yMr && x < -ymr) || (x > ymr && x > yMr)); },
                             {"Dimuon_y"}, {"Custom cut on rapidity"});

      auto count = df_cut.Count();
      if (*count < 800) {
         std::cerr << "WARNING: Few events. Fit might not converge.\n" << std::endl;
      }
   }

   if (ymr == ymr && yMr != yMr) // similar as above
   {
      std::string fil = "Dimuon_y >" + std::to_string(ymr);
      df_cut = applyFilter(df_cut, fil, "Custom cut on minimum rapidity");
   }
   if (yMr == yMr && ymr != ymr) // similar as above
   {
      std::string fil = "Dimuon_y <" + std::to_string(yMr);
      df_cut = applyFilter(df_cut, fil, "Custom cut on maximum rapidity");
   }
   auto report = df_cut.Report(); // request cut report
   std::cout << "\n" << std::endl;
   report->Print(); // print cut report on the terminal
   std::cout << "\n" << std::endl;
   return df_cut;
}
