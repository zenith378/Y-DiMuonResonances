//
//  df_set.h
//
//
//  Created by Giulio Cordova on 21/11/22.
//

#ifndef df_set_h
#define df_set_h

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx" //per usare ROOT::VecOps::RVec<T>
#include "Math/Vector4Dfwd.h"
#include "Math/Vector4D.h" //per usare PtEtaPhiMVector e le sue funzioni
#include "TMath.h"
#include "TFile.h"


using namespace ROOT::VecOps;

ROOT::Math::PtEtaPhiMVector computeFourVec(RVec<float>& pt, RVec<float>& eta, RVec<float>& phi, RVec<float>& mass);

float computeFourVecInvariantMass(ROOT::Math::PtEtaPhiMVector& fourvec);

float computeFourVecPT(ROOT::Math::PtEtaPhiMVector& fourvec);

float computeFourVecRapidity(ROOT::Math::PtEtaPhiMVector& fourvec);

float computeFourVecBeta(ROOT::Math::PtEtaPhiMVector& fourvec);

ROOT::RDataFrame df_set();


#endif /* Cuts_h */
