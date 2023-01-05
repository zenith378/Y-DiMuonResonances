#include "TCanvas.h"
#include "SpectrumPlot.h"
#include "TLine.h"
#include "TPavesText.h"
#include "TPaveLabel.h"
#include <iostream>
#include <filesystem>
#include "TSystem.h"

int main()
{
   // create canvas
   TCanvas *c1 = new TCanvas("c1", "Canvas Example", 200, 10, 600, 480);

   TPad *pad1 = new TPad("pad1", "This is pad1", 0.05, 0.52, 0.95, 0.97);
   TPad *pad2 = new TPad("pad2", "This is pad2", 0.05, 0.02, 0.95, 0.47);
   pad1->SetFillColor(11);
   pad2->SetFillColor(11);
   pad1->Draw();
   pad2->Draw();

   pad2->cd();
   TPad *pad21 = new TPad("pad21", "First subpad of pad2", 0.02, 0.05, 0.48, 0.95, 17, 3);
   TPad *pad22 = new TPad("pad22", "Second subpad of pad2", 0.52, 0.05, 0.98, 0.95, 17, 3);
   pad21->Draw();
   pad22->Draw();

   // the option of this canvas are implemented just for fun, it's not really important what's in here
   pad1->cd();
   float xt1 = 0.5;
   float yt1 = 0.1;
   TText *t1 = new TText(0.5, yt1, "CrossY");
   t1->SetTextAlign(22);
   t1->SetTextSize(0.05);
   t1->Draw();
   TLine *line1 = new TLine(0.05, 0.05, 0.80, 0.70);
   line1->SetLineWidth(8);
   line1->SetLineColor(2);
   line1->Draw();
   line1->DrawLine(0.6, 0.1, 0.9, 0.9);
   TLine *line2 = new TLine(0.05, 0.70, 0.50, 0.10);
   line2->SetLineWidth(4);
   line2->SetLineColor(5);
   line2->Draw();

   pad21->cd();
   TText *t21 = new TText(0.05, 0.8, "Giulio Cordova");
   t21->SetTextSize(0.1);
   t21->Draw();
   float xp2 = 0.5;
   float yp2 = 0.4;
   TPavesText *paves = new TPavesText(0.1, 0.1, xp2, yp2);
   paves->AddText("Jon Snow King in the North");
   paves->AddText("Sam didn't eat the elven bread");
   paves->AddText("Kessel run in less than 12 parsec");
   paves->SetFillColor(43);
   paves->Draw();
   pad22->cd();
   TText *t22 = new TText(0.05, 0.8, "Matilde Carminati");
   t22->SetTextSize(0.1);
   t22->Draw();
   float xlc = 0.01;
   float ylc = 0.01;
   TPaveLabel *label = new TPaveLabel(xlc, ylc, xlc + 0.8, ylc + 0.1, "The Answer is 42");
   label->SetFillColor(24);
   label->Draw();
   c1->Update();

   // Actual test
   SavePlot(c1, "prova.pdf");
   if (!gSystem->AccessPathName("./Plots/prova.pdf")) { // if filesystem cannot access pathname
      std::cerr << "File not created" << std::endl;
      exit(1);
   }
   return 0;
}