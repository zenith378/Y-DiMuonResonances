#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx" //per usare ROOT::VecOps::RVec<T>
#include "Math/Vector4Dfwd.h"
#include "Math/Vector4D.h" //per usare PtEtaPhiMVector e le sue funzioni
#include "TMath.h"
#include "TFile.h"
#include <TSystem.h>
#include "df_set.h"
#include <filesystem>
#include "optionParse.h"

using namespace ROOT::VecOps;

ROOT::Math::PtEtaPhiMVector computeFourVec(RVec<float> &pt, RVec<float> &eta, RVec<float> &phi, RVec<float> &mass)
{
  /* Funzione che calcola il quadrivettore: idea di usare i metodi di questa quantità per calcolare
  le quantità di cui abbiamo bisongo (massa invariante, beta, rapidità, etc.)*/
  // potremmo implementare queste due righe con un ciclo su uno dei vettori,
  // definire un vettore si vettori di lorentz che se è più lungo di 2 mi
  // restituisce un messaggio di errore, sommare gli elementi del vettore

  ROOT::Math::PtEtaPhiMVector m1(pt[0], eta[0], phi[0], mass[0]);
  ROOT::Math::PtEtaPhiMVector m2(pt[1], eta[1], phi[1], mass[1]);
  return (m1 + m2);
}

float computeFourVecInvariantMass(ROOT::Math::PtEtaPhiMVector &fourvec)
{
  return fourvec.mass();
}

float computeFourVecPT(ROOT::Math::PtEtaPhiMVector &fourvec)
{
  return fourvec.pt();
}

float computeFourVecRapidity(ROOT::Math::PtEtaPhiMVector &fourvec)
{
  return fourvec.Rapidity();
}

float computeFourVecBeta(ROOT::Math::PtEtaPhiMVector &fourvec)
{
  return fourvec.Beta();
}

ROOT::RDataFrame df_set()
{
  // Enable multi-threading
  ROOT::EnableImplicitMT(1);
  namespace fs = std::filesystem;

  std::string fname("Data/data.root");
  ROOT::RDataFrame *df;
  // try opening file
  try
  {
    df = new ROOT::RDataFrame("Events", fname);
    // if file does not open
    if (!fs::is_directory("./Data") || !fs::exists("./Data"))
    {
      throw("./Data");
    }
    if (gSystem->AccessPathName(fname.c_str()))
    {
      throw(std::runtime_error("Problem reading data file (it might not exist or might be corrupted)\n"));
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
  }
  catch (std::exception &ex)
  {
    std::cerr << ex.what() << std::endl;
    std::cerr << "Recovering dataset from web...\n"
              << std::endl;
    // reading dataframe from online NanoAOD
    ROOT::RDataFrame df_temp("Events", "root://eospublic.cern.ch//eos/opendata/cms/derived-data/AOD2NanoAODOutreachTool/Run2012BC_DoubleMuParked_Muons.root");
    // define more variables in the dataframe
    auto df_set = df_temp.Define("Dimuon_FourVec", computeFourVec, {"Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass"})
                      .Define("Dimuon_mass", computeFourVecInvariantMass, {"Dimuon_FourVec"})
                      .Define("Dimuon_pt", computeFourVecPT, {"Dimuon_FourVec"}) // Compute pt and rapidity (y) of dimuon
                      .Define("Dimuon_beta", computeFourVecBeta, {"Dimuon_FourVec"})
                      .Define("Dimuon_y", computeFourVecRapidity, {"Dimuon_FourVec"});

    df_set.Snapshot("Events", fname);
    ROOT::RDataFrame df_new("Events", fname);

    return df_new;
  }
  catch (...)
  {
    unknownErrorHandling();
  }
  return (*df);
}