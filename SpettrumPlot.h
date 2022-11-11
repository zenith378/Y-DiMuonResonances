//
//  SpettrumPlot.h
//  
//
//  Created by Giulio Cordova on 10/11/22.
//
//

#ifndef SpettrumPlot_h
#define SpettrumPlot_h

using namespace ROOT::VecOps;

float computeInvariantMass(RVec<float>& pt, RVec<float>& eta, RVec<float>& phi, RVec<float>& mass);
//compute invariant mass

void SpettrumPlot(); //fa il magic trick


#endif /* SpettrumPlot_h */
