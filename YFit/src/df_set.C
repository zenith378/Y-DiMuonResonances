/**************************************************************
 * \file df_set.C
 * \brief Function for creating and setting up dataframe used in the analysis
 *
 *
 *******************************************************************************/
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx" //per usare ROOT::VecOps::RVec<T>
#include "Math/Vector4Dfwd.h"
#include "Math/Vector4D.h" //per usare PtEtaPhiMVector e le sue funzioni
#include "TMath.h"
#include "TFile.h"
#include <TSystem.h>
//#include "df_set.h"
#include <filesystem>
//#include "optionParse.h"

using namespace ROOT::VecOps;


ROOT::Math::PtEtaPhiMVector computeDiMuonFourVec(RVec<float> &pt, RVec<float> &eta, RVec<float> &phi, RVec<float> &mass)
{
  ROOT::Math::PtEtaPhiMVector m1(pt[0], eta[0], phi[0], mass[0]);
  ROOT::Math::PtEtaPhiMVector m2(pt[1], eta[1], phi[1], mass[1]);
  return (m1 + m2);
}


float computeDiMuonInvariantMass(ROOT::Math::PtEtaPhiMVector &fourvec)
{
  return fourvec.mass();
}

float computeDiMuonPT(ROOT::Math::PtEtaPhiMVector &fourvec)
{
  return fourvec.pt();
}


float computeDiMuonRapidity(ROOT::Math::PtEtaPhiMVector &fourvec)
{
  return fourvec.Rapidity();
}

float computeDiMuonBeta(ROOT::Math::PtEtaPhiMVector &fourvec)
{
  return fourvec.Beta();
}


ROOT::RDataFrame df_set()
{
  // Enable multi-threading
  ROOT::EnableImplicitMT(1);
  namespace fs = std::filesystem;

  std::string fname("./Data/data.root");
  ROOT::RDataFrame *df;
  try //try opening file
  {
    df = new ROOT::RDataFrame("Events", fname);
    
    if (!fs::is_directory("./Data") || !fs::exists("./Data")) //if data directory does not exists
    {
      throw("./Data"); //throw an exception to handle
    }
    if (gSystem->AccessPathName(fname.c_str())) //if filesystem cannot access pathname
    {
      throw(std::runtime_error("Problem reading data file (it might not exist or might be corrupted)\n")); ///throw exception to handle
    }
  }
  catch (const char *pathToData) //handle exception of not found directory
  {
    std::cerr << "Directory " << pathToData << " does not exist.\n"
              << std::endl;
    std::cerr << "Creating directory...\n"
              << std::endl;

    fs::create_directory(pathToData);
    std::cout << "Directory " << pathToData << " successfully created\n"
              << std::endl;
    throw(std::runtime_error("Problem reading data file (it might not exist or might be corrupted)\n")); ///throw exception to handle
  }
  catch (std::exception &ex) //hadles exception of not found file 
  {
    std::cerr << ex.what() << std::endl;
    std::cerr << "Recovering dataset from web...\n"
              << std::endl;
    // reading dataframe from online NanoAOD
    ROOT::RDataFrame df_temp("Events", "root://eospublic.cern.ch//eos/opendata/cms/derived-data/AOD2NanoAODOutreachTool/Run2012BC_DoubleMuParked_Muons.root");
    // define more useful variables in the dataframe
      auto df_set = df_temp.Define("goodmu", "Muon_pt > 20 && Muon_eta < 2.0 && Muon_eta>-2.0")
          .Filter("Sum(goodmu)>=2")
          .Define("mu0", "Nonzero(goodmu)[0]")
          .Define("oppositeCharge","goodmu && Muon_charge[mu0]*Muon_charge < 0")
          .Filter("Sum(oppositeCharge)>0")
          .Define("mu1","Nonzero(oppositeCharge)[0]");
//      auto df_set = df_0.Define("Dimuon_FourVec", computeDiMuonFourVec, {
//          Take("Muon_pt", {"mu0", "mu1"}),
//          Take("Muon_eta", {"mu0", "mu1"}),
//          Take("Muon_phi", {"mu0", "mu1"}),
//          Take("Muon_mass", {"mu0", "mu1"})})
//          .Define("Dimuon_mass", computeDiMuonInvariantMass, {"Dimuon_FourVec"})
//          .Define("Dimuon_pt", computeDiMuonPT, {"Dimuon_FourVec"}) // Compute pt and rapidity (y) of dimuon
//          .Define("Dimuon_beta", computeDiMuonBeta, {"Dimuon_FourVec"})
//          .Define("Dimuon_y", computeDiMuonRapidity, {"Dimuon_FourVec"});

    df_set.Snapshot("Events", fname); //save dataframe in a root file to avoid downloading it all the times
    ROOT::RDataFrame df_new("Events", fname); //read dataframe from file stored
    //this last step is necessary in order to be coherent with return type of the function df_set()

    return df_new;
  }
//  catch (...) //if it happens anything that is not caught
//  {
//    unknownErrorHandling(); //function defined in optionParse.C
//  }
  return (*df);
}//end df_set
