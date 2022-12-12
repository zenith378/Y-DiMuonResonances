/**************************************************************
 * \file df_set.C
 * \brief Function for creating and setting up dataframe used in the analysis
 *
 *
 *
 * In this file are defined the functions used in order to read the dataframe (or create it if it is not already there)
 * and setting it up with useful variables
 * In particular, it is defined:
 * 
 * computeFourVector(): computes a four vector of a dimuon starting from pt, eta, phi and mass
 * 
 * computeFourVecInvariantMass(): computes invariant mass of given four vector
 * 
 * computeFourVecPT(): computes pt of given FourVector
 * 
 * computeFourVecRapidity(): computes rapidity of given four vector
 * 
 * computeFourVecBeta(): computes beta of given four vector
 * 
 * df_set(): defines the dataframe
 *******************************************************************************/
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

/***************************************
 * 
 * \brief it calculates the four vector of a dimuon starting from pt, eta, phi and mass of muon pair
 * @param pt transverse momentum
 * @param eta azimuthal angle
 * @param phi angle
 * @param mass mass
 * 
 * \return PtEtaPhiMVector
 * 
 ****************************************/
ROOT::Math::PtEtaPhiMVector computeFourVec(RVec<float> &pt, RVec<float> &eta, RVec<float> &phi, RVec<float> &mass)
{
  ROOT::Math::PtEtaPhiMVector m1(pt[0], eta[0], phi[0], mass[0]);
  ROOT::Math::PtEtaPhiMVector m2(pt[1], eta[1], phi[1], mass[1]);
  return (m1 + m2);
}

/***************************************
 * 
 * \brief it calculates the invariant mass of a dimuon Four Vector
 * @param fourvec four vector of dimuon pair
 * 
 * \return mass
 * 
 ****************************************/
float computeFourVecInvariantMass(ROOT::Math::PtEtaPhiMVector &fourvec)
{
  return fourvec.mass();
}

/***************************************
 * 
 * \brief it calculates the transverse momentum of a dimuon Four Vector
 * @param fourvec four vector of dimuon pair
 * 
 * \return pt (transverse momentum)
 * 
 ****************************************/
float computeFourVecPT(ROOT::Math::PtEtaPhiMVector &fourvec)
{
  return fourvec.pt();
}

/***************************************
 * 
 * \brief it calculates the rapidity of a dimuon Four Vector //inserire formula rapidità
 * @param fourvec four vector of dimuon pair
 * 
 * \return rapidity
 * 
 ****************************************/
float computeFourVecRapidity(ROOT::Math::PtEtaPhiMVector &fourvec)
{
  return fourvec.Rapidity();
}

/***************************************
 * 
 * \brief it calculates beta (p_t/E) of a dimuon Four Vector
 * @param fourvec four vector of dimuon pair
 * 
 * \return beta
 * 
 ****************************************/
float computeFourVecBeta(ROOT::Math::PtEtaPhiMVector &fourvec)
{
  return fourvec.Beta();
}

/***************************************
 * 
 * \brief it defines the dataframe used in the analysis or it creates it if it is not found
 * 
 * \return Dataframe
 * 
 ****************************************/
ROOT::RDataFrame df_set()
{
  // Enable multi-threading
  ROOT::EnableImplicitMT(1);
  namespace fs = std::filesystem;

  std::string fname("Data/data.root");
  ROOT::RDataFrame *df;
  try ///try opening file
  {
    df = new ROOT::RDataFrame("Events", fname);
    
    if (!fs::is_directory("./Data") || !fs::exists("./Data")) ///if data directory does not exists
    {
      throw("./Data"); ///throw an exception to handle
    }
    if (gSystem->AccessPathName(fname.c_str())) ///if filesystem cannot access pathname
    {
      throw(std::runtime_error("Problem reading data file (it might not exist or might be corrupted)\n")); ///throw exception to handle
    }
  }
  catch (const char *pathToData) ///handle exception of not found directory
  {
    std::cerr << "Directory " << pathToData << " does not exist.\n"
              << std::endl;
    std::cerr << "Creating directory...\n"
              << std::endl;

    fs::create_directory(pathToData);
    std::cout << "Directory " << pathToData << " successfully created\n"
              << std::endl;
  }
  catch (std::exception &ex) ///hadles exception of not found file 
  {
    std::cerr << ex.what() << std::endl;
    std::cerr << "Recovering dataset from web...\n"
              << std::endl;
    /// reading dataframe from online NanoAOD
    ROOT::RDataFrame df_temp("Events", "root://eospublic.cern.ch//eos/opendata/cms/derived-data/AOD2NanoAODOutreachTool/Run2012BC_DoubleMuParked_Muons.root");
    /// define more useful variables in the dataframe
    auto df_set = df_temp.Define("Dimuon_FourVec", computeFourVec, {"Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass"})
                      .Define("Dimuon_mass", computeFourVecInvariantMass, {"Dimuon_FourVec"})
                      .Define("Dimuon_pt", computeFourVecPT, {"Dimuon_FourVec"}) // Compute pt and rapidity (y) of dimuon
                      .Define("Dimuon_beta", computeFourVecBeta, {"Dimuon_FourVec"})
                      .Define("Dimuon_y", computeFourVecRapidity, {"Dimuon_FourVec"});

    df_set.Snapshot("Events", fname); ///save dataframe in a root file to avoid downloading it all the times
    ROOT::RDataFrame df_new("Events", fname); ///read dataframe from file stored
    ///this last step is necessary in order to be coherent with return type of the function df_set()

    return df_new;
  }
  catch (...) ///if it happens anything that is not caught
  {
    unknownErrorHandling(); ///function defined in optionParse.C
  }
  return (*df);
}