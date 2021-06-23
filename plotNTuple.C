// ROOT Headers
#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TLegend.h"

void plotNTuple(){
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  
  // Draw histos filled by Geant4 simulation 
  //   

  // Open file filled by Geant4 simulation 
  TFile*  F = new TFile("data/NP1.root") ;

  // Create a canvas and divide it into 2x2 pads
  TCanvas* c1 = new TCanvas("c1", "c1", 20, 20, 1000, 1000);
  c1->Divide(2,2);
  
  // Get ntuple
  //TNtuple* ntuple = (TNtuple*)f.Get("B4");
  TNtuple* ntuple1 = static_cast<TNtuple*>(F->Get("histograms/SecondariesProduction"));

  // Draw Eabs histogram in the pad 1
  c1->cd(1);
  ntuple1->Draw("Eabs");
  
  // Draw Labs histogram in the pad 2
  c1->cd(2);
  ntuple1->Draw("Labs");
  
  // Draw Egap histogram in the pad 3
  // with logaritmic scale for y  ?? how to do this?
  c1->cd(3);
  gPad->SetLogy(1);
  ntuple1->Draw("Egap");
  
  // Draw Lgap histogram in the pad 4
  // with logaritmic scale for y  ?? how to do this?
  c1->cd(4);
  gPad->SetLogy(1);
  ntuple1->Draw("Egap");
}  
