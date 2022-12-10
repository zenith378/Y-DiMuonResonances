/*********************************
 \file Cuts.C
 \brief Selection of the data.

 The data are selected to be two muon of opposite charge with an invariant mass around the mass of the Y resonances. Also it is possible to choose other cuts on the trasverse momentum and the pseudorapidity of the dimuon state.
************************************************/

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "Cuts.h"
#include <filesystem>


ROOT::RDF::RNode DFFilter(ROOT::RDataFrame df, int &dr)
{
  switch (dr)
  {
  default:
  case 0:
  {
    auto df_cut = df.Filter([](unsigned int x){ return x == 2; },{"nMuon"}, {"Events with exactly two muons"}).Filter([](float x)
                              { return x > 8.5 && x < 11.5; },
                              {"Dimuon_mass"}, {"Inviariant mass between 8.5 and 11.5"}); // Cut around the Ys
    return df_cut;
  }
  case 1:
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
                              {"Dimuon_pt"}, {"Pt between 10 and 100 GeV"}); // Select events with 10 GeV < pT < 12 GeV
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

ROOT::RDF::RNode applyFilter(ROOT::RDF::RNode df_custom_cut, std::string_view filter, std::string_view name){
  df_custom_cut = df_custom_cut.Filter(filter,name);

    auto count = df_custom_cut.Count();
    if(*count<800){
      std::cout << "Too few events to fit. Try relaxing cuts."<< std::endl;
      exit(1);
    }
    return df_custom_cut;
}


ROOT::RDF::RNode customFilter(ROOT::RDF::RNode df, float &pmr, float &pMr, float &ymr, float &yMr)
{
  ROOT::RDF::RNode df_custom_cut = df;
  if (pmr == pmr)
  {
    std::string fil = "Dimuon_pt >" +std::to_string(pmr);
    df_custom_cut = applyFilter(df_custom_cut,fil,"Custom cut on minimum pt");
  }
  if (pMr == pMr)
  {
    std::string fil = "Dimuon_pt <" +std::to_string(pMr);
    df_custom_cut = applyFilter(df_custom_cut,fil,"Custom cut on maximum pt");
  }

  if (ymr == ymr && yMr == yMr)
  {
    //std::string_view fil ="("+std::to_string(-yMr)+ "<Dimuon_y <" +std::to_string(-ymr)+")"
    //                      +"||("+std::to_string(ymr)+ "<Dimuon_y <" +std::to_string(yMr)+")";
    //df_custom_cut = applyFilter(df_custom_cut,fil,"Custom cut on rapidity");
    //std::string_view filt =std::to_string(ymr)+ "<Dimuon_y <" +std::to_string(yMr);
    //df_custom_cut = applyFilter(df_custom_cut,filt,"Custom cut on rapidity 2");
    
    df_custom_cut = df_custom_cut.Filter([ymr,yMr](float x)
                                         { return ((x > -yMr && x < -ymr)||(x > ymr && x > yMr)); },
                                         {"Dimuon_y"}, {"Custom cut on rapidity"});
    auto count = df_custom_cut.Count();
    if(*count<800){
      std::cout << "Too few events to fit. Try relaxing cuts."<< std::endl;
      exit(1);
    }
  }

  if (ymr == ymr && yMr!=yMr)
  {
    std::string fil ="Dimuon_y >" +std::to_string(ymr);
    df_custom_cut = applyFilter(df_custom_cut,fil,"Custom cut on minimum rapidity");
  }
  if (yMr == yMr && ymr!=ymr)
  {
    std::string fil ="Dimuon_y <" +std::to_string(yMr);
    df_custom_cut = applyFilter(df_custom_cut,fil,"Custom cut on maximum rapidity");
  }
  return df_custom_cut;
}

/*********************************
 \brief Modified a muon DataFrame cutting on nMuon, Muon_charge, Dimuon_mass, Dimuon_pt and Dimuon_y

 UNa descrizione più dettagliata della funzione
 @param df Data Frame in input
 @param pmr lower extreme for the cut on the dimuon trasverse momentum
 @param pMr upper extreme for the cut on the dimuon trasverse momentum
 @param ymr lower extreme for the cut on the dimuon pseudorapidity
 @param yMr upper extreme for the cut on the dimuon pseudorapidity
 @return df_cut DataFrame withthe selection decided by the cuts
************************************************/

ROOT::RDF::RNode Cuts(ROOT::RDataFrame df, int &dr, float &pmr, float &pMr, float &ymr, float &yMr)
{
  // Enable multi-threading
  ROOT::EnableImplicitMT(1);
  TString *fname;

  // Events selection
  switch (dr)
  {
  default:
  case 0:
  {
    fname = new TString("Data/data_cut0.root");
    break;
  }
  case 1:
  {
    fname = new TString("Data/data_cut1.root");
    break;
  }
  case 2:
  {
    fname = new TString("Data/data_cut2.root");
    break;
  }
  }

  TFile *cutfile = TFile::Open(*fname); // try open cut file
  if (!cutfile)
  { // if the cutfile does not exist
    std::cout << "Recreating cut file" << std::endl;

    auto df_cut = DFFilter(df, dr);
    // Request cut-flow report
    auto report = df_cut.Report();
    // Print cut-flow report
    report->Print();
    // save cut file
    namespace fs = std::filesystem;
    if (!fs::is_directory("./Data") || !fs::exists("./Data"))
    {                                 // Check if src folder exists
      fs::create_directory("./Data"); // create src folder
    }
    df_cut.Snapshot("Cuts", *fname);
  }

  else if (cutfile->IsZombie())
  { // file not read correctly
    std::cout << "Problems reading file " << *fname << std::endl;
    exit(1);
  }

  ROOT::RDataFrame df_off("Cuts", *fname);

  ROOT::RDF::RNode df_def = customFilter(df_off, pmr, pMr, ymr, yMr);

  auto report = df_def.Report();
  report->Print();
  std::cout <<"\n"<<std::endl;

  // const auto pt_max = 12.;
  // const auto pt_min = 10.;
  return df_def;
}
