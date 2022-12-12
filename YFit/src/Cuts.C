/*********************************
 \file Cuts.C
 \brief Selection of the data.

 The data are selected to be two muon of opposite charge with an invariant mass around the mass of the Y resonances. Also it is possible to choose other cuts on the trasverse momentum and the pseudorapidity of the dimuon state.
************************************************/

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "Cuts.h"
#include <filesystem>
#include "TSystem.h"
#include "optionParse.h"

ROOT::RDF::RNode DFFilter(ROOT::RDataFrame df, int &dr)
{
  switch (dr)
  {
  default:
  case 0:
  {
    auto df_cut = df.Filter([](unsigned int x)
                            { return x == 2; },
                            {"nMuon"}, {"Events with exactly two muons"})
                      .Filter([](float x)
                              { return x > 8.5 && x < 11.5; },
                              {"Dimuon_mass"}, {"Inviariant mass between 8.5 and 11.5"}); /// Cut around the Ys
    return df_cut;
  }
  case 1:
  {
    auto df_cut = df.Filter([](unsigned int x)
                            { return x == 2; },
                            {"nMuon"}, {"Events with exactly two muons"})                       /// Select events with exactly two muons
                      .Filter("Muon_charge[0] != Muon_charge[1]", "Muons with opposite charge") /// Select events with two muons of opposite charge
                      .Filter([](float x)
                              { return x > 8.5 && x < 11.5; },
                              {"Dimuon_mass"}, {"Inviariant mass between 8.5 and 11.5"}) /// Cut around the Ys
                      .Filter([](float x)
                              { return x > 10. && x < 100.; },
                              {"Dimuon_pt"}, {"Pt between 10 and 100 GeV"}); /// Select events with 10 GeV < pT < 100 GeV
    return df_cut;
  }
  case 2:
  {
    auto df_cut = df.Filter([](unsigned int x)
                            { return x == 2; },
                            {"nMuon"}, {"Events with exactly two muons"})                       // Select events with exactly two muons
                      .Filter("Muon_charge[0] != Muon_charge[1]", "Muons with opposite charge") // Select events with two muons of opposite charge
                      .Filter([](float x)
                              { return x > 8.5 && x < 11.5; },
                              {"Dimuon_mass"}, {"Inviariant mass between 8.5 and 11.5"}) // Cut around the Ys
                      .Filter([](float x)
                              { return x > 10. && x < 100.; },
                              {"Dimuon_pt"}, {"Pt between 10 and 100 GeV"}) // Select events with 10 GeV < pT < 100 GeV
                      .Filter([](float x)
                              { return x > -0.6 && x < 0.6; },
                              {"Dimuon_y"}, {"Rapidity between -0.6 and 0.6"});
    return df_cut;
  }
  }
  exit(1);
}

ROOT::RDF::RNode applyFilter(ROOT::RDF::RNode df_custom_cut, std::string_view filter, std::string_view name)
{

  df_custom_cut = df_custom_cut.Filter(filter, name); /// apply filter 
  try
  {
    auto count = df_custom_cut.Count(); ///count remaining events
    if (*count < 800)
    {
      throw(std::runtime_error("Too few events. Fit might not converge. Please relaxe cuts.\n"));
    }
  }
  catch (std::exception &ex) ///handles exception thrown at 74
  {
    std::cerr << ex.what() << std::endl;
  }
  return df_custom_cut;
}

ROOT::RDF::RNode customFilter(ROOT::RDataFrame df, float &pmr, float &pMr, float &ymr, float &yMr)
{
  ROOT::RDF::RNode df_custom_cut = df; ///initialize datafrmae
  if (pmr == pmr) ///if pmr is not nan
  {
    std::string fil = "Dimuon_pt >" + std::to_string(pmr); /// format string to pass to the filter function
    df_custom_cut = applyFilter(df_custom_cut, fil, "Custom cut on minimum pt"); ///apply filter
  }
  if (pMr == pMr)
  {
    std::string fil = "Dimuon_pt <" + std::to_string(pMr);
    df_custom_cut = applyFilter(df_custom_cut, fil, "Custom cut on maximum pt");
  }

  if (ymr == ymr && yMr == yMr)
  {
    df_custom_cut = df_custom_cut.Filter([ymr, yMr](float x)
                                         { return ((x > -yMr && x < -ymr) || (x > ymr && x > yMr)); },
                                         {"Dimuon_y"}, {"Custom cut on rapidity"});
    try
    {
      auto count = df_custom_cut.Count();
      if (*count < 800)
      {
        throw(std::runtime_error("Too few events. Fit might not converge. Try relaxing cuts.\n"));
      }
    }
    catch (std::exception &ex)
    {
      std::cerr << ex.what() << std::endl;
    }
  }

  if (ymr == ymr && yMr != yMr)
  {
    std::string fil = "Dimuon_y >" + std::to_string(ymr);
    df_custom_cut = applyFilter(df_custom_cut, fil, "Custom cut on minimum rapidity");
  }
  if (yMr == yMr && ymr != ymr)
  {
    std::string fil = "Dimuon_y <" + std::to_string(yMr);
    df_custom_cut = applyFilter(df_custom_cut, fil, "Custom cut on maximum rapidity");
  }
  return df_custom_cut;
}


ROOT::RDataFrame generateDataFrame(ROOT::RDataFrame df, int &dr){
  
  ROOT::EnableImplicitMT(1);
  ROOT::RDataFrame *df_off;
  namespace fs = std::filesystem;
  std::string *fname;

  // Events selection
  switch (dr)
  {
  default:
  case 0:
  {
    fname = new std::string("./Data/data_cut0.root");
    break;
  }
  case 1:
  {
    fname = new std::string("./Data/data_cut1.root");
    break;
  }
  case 2:
  {
    fname = new std::string("./Data/data_cut2.root");
    break;
  }
  }
  try
  {
    df_off = new ROOT::RDataFrame("Cuts", *fname);
    // if file does not open
    if (!fs::is_directory("./Data") || !fs::exists("./Data"))
    {
      throw("./Data");
    }
    if (gSystem->AccessPathName(fname->c_str()))
    {
      throw(std::runtime_error("Problem reading cut file (it might not exist or it might be corrupted)\n"));
    }
  }
  catch (const char *pathToData)
  {
    std::cerr << "Directory " << pathToData << " does not exist.\n"
              << std::endl;
    std::cerr << "Creating directory...\n"
              << std::endl;

    fs::create_directory(pathToData);
    std::cout << "Directory " << pathToData << " successfully created\n"
              << std::endl;
    throw(std::runtime_error("Problem reading cut file (it might not exist or it might be corrupted)\n"));

  }
  catch (std::exception &exp)
  {

    std::cerr << exp.what() << std::endl;
    std::cout << "Recreating cut dataframe...\n"
              << std::endl;
    auto df_cut = DFFilter(df, dr);
    std::cout << "Cut Dataframe recreated. Cut report follows\n"
              << std::endl;

    // Request cut-flow report
    auto report = df_cut.Report();
    // Print cut-flow report
    report->Print();
    std::cout << "\nSaving cut file for future usage...\n"
              << std::endl;
    df_cut.Snapshot("Cuts", *fname); // qui forse devo mettere un'altra exception
    std::cout << "Cut File successfully saved\n"
              << std::endl;
    ROOT::RDataFrame df_upt("Cuts", *fname);
    std::cout << " Cut File successfully opened\n"
              << std::endl;

    return df_upt;
  }
  catch (...)
  {
    unknownErrorHandling();
  }
  return (*df_off);
}


ROOT::RDF::RNode Cuts(ROOT::RDataFrame df, int &dr, float &pmr, float &pMr, float &ymr, float &yMr)
{
  // Enable multi-threading
  ROOT::EnableImplicitMT(1);

  ROOT::RDataFrame df_off= generateDataFrame(df, dr);

  ROOT::RDF::RNode df_def = customFilter(df_off, pmr, pMr, ymr, yMr);

  auto report = df_def.Report();
  report->Print();
  std::cout << "\n"
            << std::endl;

  return df_def;
}
