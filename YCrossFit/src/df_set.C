
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "Math/Vector4Dfwd.h"
#include "Math/Vector4D.h"
#include "TMath.h"
#include "TFile.h"
#include <TSystem.h>
#include "df_set.h"
#include <filesystem>
#include "optionParse.h"

using namespace ROOT::VecOps;

ROOT::Math::PtEtaPhiMVector computeDiMuonFourVec(RVec<float> &pt, RVec<float> &eta, RVec<float> &phi, RVec<float> &mass,
                                                 unsigned long &im0, unsigned long &im1)
{ // create two four vector of type PtEtaPhiM for the two muons in the event with index im0 and im1.
   ROOT::Math::PtEtaPhiMVector m1(pt[im0], eta[im0], phi[im0], mass[im0]);
   ROOT::Math::PtEtaPhiMVector m2(pt[im1], eta[im1], phi[im1], mass[im1]);
   // create a four vector of the same type for the dimuon state
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

ROOT::RDF::RNode df_set()
{
   // Enable multi-threading
   ROOT::EnableImplicitMT(1);

   namespace fs = std::filesystem;

   std::string fname("./Data/data.root"); // path to data file
   ROOT::RDataFrame *df;
   try {
      try // try opening file
      {
         df = new ROOT::RDataFrame("Events", fname);

         if (!fs::is_directory("./Data") || !fs::exists("./Data")) // if data directory does not exists
         {
            throw("./Data"); // throw an exception to handle
         }
         if (gSystem->AccessPathName(fname.c_str())) // if filesystem cannot access pathname
         {
            throw(std::runtime_error(
               "Problem reading data file (it might not exist or might be corrupted)\n")); /// throw exception to handle
         }
      } catch (const char *pathToData) // handle exception of not found directory
      {
         std::cerr << "Directory " << pathToData << " does not exist.\n" << std::endl;
         std::cerr << "Creating directory...\n" << std::endl;

         fs::create_directory(pathToData); // create directory
         std::cout << "Directory " << pathToData << " successfully created\n" << std::endl;
         throw(std::runtime_error(
            "Problem reading data file (it might not exist or might be corrupted)\n")); /// throw exception to handle
      }
   } catch (std::exception &ex) // hadles exception of not found file
   {
      std::cerr << ex.what() << std::endl;
      std::cerr << "Recovering dataset from web...\n" << std::endl;
      // reading dataframe from online NanoAOD
      ROOT::RDataFrame df_temp("Events", "root://eospublic.cern.ch//eos/opendata/cms/derived-data/"
                                         "AOD2NanoAODOutreachTool/Run2012BC_DoubleMuParked_Muons.root");
      // filter good muons as defined in the article and define useful variables in the dataframe
      auto df_set =
         df_temp
            .Define("goodmu", "(Muon_pt > 4.5 && abs(Muon_eta) < 1.2) || (Muon_pt > 3.5 && abs(Muon_eta) <1.4 && "
                              "abs(Muon_eta)>1.2) || (Muon_pt > 3. && abs(Muon_eta) <1.6 && abs(Muon_eta)>1.4)")
            .Filter("Sum(goodmu)>=2", "At least two muons")
            .Define("mu0", "Nonzero(goodmu)[0]")
            .Define("oppositeCharge", "goodmu && Muon_charge[mu0]*Muon_charge < 0")
            .Filter("Sum(oppositeCharge)>0", "Opposite charge")
            .Define("mu1", "Nonzero(oppositeCharge)[0]")
            .Define("Dimuon_FourVec", computeDiMuonFourVec,
                    {"Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "mu0", "mu1"})
            .Define("Dimuon_mass", computeDiMuonInvariantMass, {"Dimuon_FourVec"})
            .Define("Dimuon_pt", computeDiMuonPT, {"Dimuon_FourVec"})
            .Define("Dimuon_beta", computeDiMuonBeta, {"Dimuon_FourVec"})
            .Define("Dimuon_y", computeDiMuonRapidity, {"Dimuon_FourVec"})
            .Filter([](float x) { return x > 8.5 && x < 11.5; }, {"Dimuon_mass"},
                    {"Inviariant mass between 8.5 and 11.5"}); // Cut around the Ys;

      auto report = df_set.Report();
      std::cerr << "Dataframe read and formatted correctly. Cut report follows...\n" << std::endl;

      // Print cut-flow report
      report->Print();
      std::cerr << "Saving dataset in " + fname + " for future usage...\n" << std::endl;

      df_set.Snapshot("Events", fname); // save dataframe in a root file in order to avoid downloading it all the times

      std::cerr << "Data successfully stored\n" << std::endl;

      return df_set;
   } catch (...) // if it happens anything that is not caught
   {
      unknownErrorHandling(); // function defined in optionParse.C
   }
   return (*df);
}