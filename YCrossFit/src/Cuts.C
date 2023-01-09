#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "Cuts.h"
#include <filesystem>
#include "TSystem.h"
#include "optionParse.h"

ROOT::RDF::RNode DFFilter(ROOT::RDF::RNode &df, int dr)
{
   ROOT::EnableImplicitMT(1);

   switch (dr) // switch over depth
   {
   default:
   case 0: // depth 0
   {
      auto df_cut = df.Filter([](float x) { return x > 8.5 && x < 11.5; }, {"Dimuon_mass"},
                              {"Inviariant mass between 8.5 and 11.5"}); // Cut around the Ys
      return df_cut;
   }
   case 1: // depth 1
   {
      auto df_cut = df.Filter([](float x) { return x > 8.5 && x < 11.5; }, {"Dimuon_mass"},
                              {"Inviariant mass between 8.5 and 11.5"}) // Cut around the Ys
                       .Filter([](float x) { return x > 10. && x < 100.; }, {"Dimuon_pt"},
                               {"Pt between 10 and 100 GeV"}); // Select events with 10 GeV < pT < 100 GeV
      return df_cut;
   }
   case 2: // depth 2
   {
      auto df_cut = df.Filter([](float x) { return x > 8.5 && x < 11.5; }, {"Dimuon_mass"},
                              {"Inviariant mass between 8.5 and 11.5"}) // Cut around the Ys
                       .Filter([](float x) { return x > 10. && x < 100.; }, {"Dimuon_pt"},
                               {"Pt between 10 and 100 GeV"}) // Select events with 10 GeV < pT < 100 GeV
                       .Filter([](float x) { return x > -0.6 && x < 0.6; }, {"Dimuon_y"},
                               {"Rapidity between -0.6 and 0.6"}); // Absolute value of rapidity less than 0.6
      return df_cut;
   }
   }
}

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

ROOT::RDF::RNode customFilter(ROOT::RDF::RNode &df, float pmr, float pMr, float ymr, float yMr)
{
   ROOT::EnableImplicitMT(1);
   ROOT::RDF::RNode df_custom_cut = df; // initialize datafrmae
   if (pmr == pmr)                      // if pmr is not nan
   {
      std::string fil = "Dimuon_pt >" + std::to_string(pmr); // format string to pass to the filter function
      df_custom_cut = applyFilter(df_custom_cut, fil, "Custom cut on minimum pt"); // apply filter
   }
   if (pMr == pMr) // similar as above
   {
      std::string fil = "Dimuon_pt <" + std::to_string(pMr);
      df_custom_cut = applyFilter(df_custom_cut, fil, "Custom cut on maximum pt");
   }

   if (ymr == ymr && yMr == yMr) // similar as above
   {
      df_custom_cut =
         df_custom_cut.Filter([ymr, yMr](float x) { return ((x > -yMr && x < -ymr) || (x > ymr && x > yMr)); },
                              {"Dimuon_y"}, {"Custom cut on rapidity"});

      auto count = df_custom_cut.Count();
      if (*count < 800) {
         std::cerr << "WARNING: Few events. Fit might not converge.\n" << std::endl;
      }
   }

   if (ymr == ymr && yMr != yMr) // similar as above
   {
      std::string fil = "Dimuon_y >" + std::to_string(ymr);
      df_custom_cut = applyFilter(df_custom_cut, fil, "Custom cut on minimum rapidity");
   }
   if (yMr == yMr && ymr != ymr) // similar as above
   {
      std::string fil = "Dimuon_y <" + std::to_string(yMr);
      df_custom_cut = applyFilter(df_custom_cut, fil, "Custom cut on maximum rapidity");
   }
   return df_custom_cut;
}

ROOT::RDF::RNode generateDataFrame(ROOT::RDF::RNode &df, int dr)
{

   ROOT::EnableImplicitMT(1);
   ROOT::RDataFrame *df_off; // initialize dataframe
   namespace fs = std::filesystem;
   std::string *fname; // initialize string to read

   // Events selection
   switch (dr) {
   default:
   case 0: // depth 0
   {
      fname = new std::string("./Data/data_cut0.root");
      break;
   }
   case 1: // depth 1
   {
      fname = new std::string("./Data/data_cut1.root");
      break;
   }
   case 2: // depth 1
   {
      fname = new std::string("./Data/data_cut2.root");
      break;
   }
   }
   try {
      try // try to open file defined in string fname
      {
         df_off = new ROOT::RDataFrame("Cuts", *fname);
         // if directory does not exist
         if (!fs::is_directory("./Data") || !fs::exists("./Data")) {
            throw("./Data");
         }
         if (gSystem->AccessPathName(fname->c_str())) // if file cannot be accessed
         {
            throw(std::runtime_error("Problem reading cut file (it might not exist or it might be corrupted)\n"));
         }
      } catch (const char *pathToData) // directory pathToData does not exists
      {
         std::cerr << "Directory " << pathToData << " does not exist.\n" << std::endl;
         std::cerr << "Creating directory...\n" << std::endl;

         fs::create_directory(pathToData); // create directory
         std::cout << "Directory " << pathToData << " successfully created\n" << std::endl;
         throw(std::runtime_error("Problem reading cut file (it might not exist or it might be corrupted)\n"));
      }
   } catch (std::exception &exp) // file cannot be read
   {

      std::cerr << exp.what() << std::endl;
      std::cout << "Recreating cut dataframe...\n" << std::endl;
      auto df_cut = DFFilter(df, dr); // apply default cuts
      std::cout << "Cut Dataframe recreated. Cut report follows\n" << std::endl;

      // Request cut-flow report
      auto report = df_cut.Report();
      // Print cut-flow report
      report->Print();
      std::cout << "\nSaving cut file for future usage...\n" << std::endl;
      df_cut.Snapshot("Cuts", *fname); // save file
      std::cout << "Cut File successfully saved\n" << std::endl;
      return df_cut;
   } catch (...) {
      unknownErrorHandling();
   }
   return (*df_off);
}

ROOT::RDF::RNode Cuts(ROOT::RDF::RNode &df, int dr, float pmr, float pMr, float ymr, float yMr)
{
   ROOT::EnableImplicitMT(1);
   // generate dataframe from depth value
   ROOT::RDF::RNode df_ret = generateDataFrame(df, dr);
   if (ymr == ymr || yMr == yMr || ymr == ymr || yMr == yMr) { // if custom cuts are defined
      df_ret = customFilter(df_ret, pmr, pMr, ymr, yMr);       // apply custom cuts
      auto report = df_ret.Report();                           // request cut report
      report->Print();                                         // print cut report on the terminal
      std::cout << "\n" << std::endl;
      return df_ret; // return custom cut df
   }

   return df_ret; // return default cut df
}
