//
//  fitlp2.h
//  
//
//  Created by Giulio Cordova on 10/11/22.
//
//

#ifndef fitlp2_h
#define fitlp2_h



Double_t lorentzianPeak(Double_t *x, Double_t *par); //Funzione di fit del picco lorentziano

Double_t background(Double_t *x, Double_t *par); //Funzione di fit per il background

Double_t fitfunction(Double_t *x, Double_t *par); // funzione che esegue il fit

#endif /* fitlp2_h */
