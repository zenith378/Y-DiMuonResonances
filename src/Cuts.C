/*********************************
 \file Cuts.C
 \brief Selection of the data.
 The data are selected to be two muon of opposite charge with an invariant mass around the mass of the Y resonances. Also it is possible to choose other cuts on the trasverse momentum and the pseudorapidity of the dimuon state.
************************************************/

#include "ROOT/RDataFrame.hxx"
#include "TMath.h"
#include "Cuts.h"
#include <filesystem>

ROOT::RDF::RNode DFFilter(ROOT::RDataFrame df, int depth)
{
  switch (depth)
  {
  default:
  case 0:
  {
    auto df_cut = df.Filter([](unsigned int x)
                            { return x == 2; },
                            {"nMuon"}, {"Events with exactly two muons"})                       // Select events with exactly two muons
                      .Filter("Muon_charge[0] != Muon_charge[1]", "Muons with opposite charge") // Select events with two muons of opposite charge
                      .Filter([](float x)
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
                              { return x > 10. && x < 12.; },
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
                              { return x > 10. && x < 12.; },
                              {"Dimuon_pt"}, {"Pt between 10 and 100 GeV"}) // Select events with 10 GeV < pT < 12 GeV
                      .Filter([](float x)
                              { return x > -0.6 && x < 0.6; },
                              {"Dimuon_y"}, {"Rapidity between -0.6 and 0.6"});
    return df_cut;
  }
  }
  exit(1);
}

ROOT::RDF::RNode applyFilter(ROOT::RDF::RNode df_custom_cut, float var, ROOT::RDF::ColumnNames_t obs, std::string_view message)
{
  df_custom_cut = df_custom_cut.Filter([var](float x){ return x > var; },obs, {message});
  auto report = df_custom_cut.Report();
  // Print cut-flow report
  report->Print();
  std::cout<<"\n"<<std::endl;
  return df_custom_cut;
}

ROOT::RDF::RNode customFilter(ROOT::RDF::RNode df, float ptm, float ptM, float ym, float yM)
{
  ROOT::RDF::RNode df_custom_cut = df;
  if (ptm == ptm)
  {
    df_custom_cut = applyFilter(df_custom_cut, ptm, {"Dimuon_pt"}, "Custom cut on minimum pt");
  }
  if (ptM == ptM)
  {
    df_custom_cut = applyFilter(df_custom_cut, ptM, {"Dimuon_pt"}, "Custom cut on maximum pt");
  }
  if (ym == ym)
  {
    df_custom_cut = applyFilter(df_custom_cut, ym, {"Dimuon_y"}, "Custom cut on minimum rapidity");
  }
  if (yM == yM)
  {
    df_custom_cut = applyFilter(df_custom_cut, yM, {"Dimuon_y"}, "Custom cut on maximum rapidity");
  }
  return df_custom_cut;
}

/*********************************
 \brief Modified a muon DataFrame cutting on nMuon, Muon_charge, Dimuon_mass, Dimuon_pt and Dimuon_y
 UNa descrizione più dettagliata della funzione
 @param df Data Frame in input
 @param mm lower extreme for the cut on the dimuon invariant mass
 @param mM upper extreme for the cut on the dimuon invariant mass
 @param ptm lower extreme for the cut on the dimuon trasverse momentum
 @param ptM upper extreme for the cut on the dimuon trasverse momentum
 @param ym lower extreme for the cut on the dimuon pseudorapidity
 @param yM upper extreme for the cut on the dimuon pseudorapidity
 @return df_cut DataFrame withthe selection decided by the cuts
************************************************/

ROOT::RDF::RNode Cuts(ROOT::RDataFrame df, int depth, float ptm, float ptM, float ym, float yM)
{
  // Enable multi-threading
  ROOT::EnableImplicitMT(1);
  TString *fname;

  // Events selection
  switch (depth)
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

    auto df_cut = DFFilter(df, depth);
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

  ROOT::RDF::RNode df_def = customFilter(df_off, ptm, ptM, ym, yM);

  // const auto pt_max = 12.;
  // const auto pt_min = 10.;
  return df_def;
}
