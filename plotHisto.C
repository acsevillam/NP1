// ROOT Headers
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"

#include <map>
#include <iostream>

using namespace std;

void plotHisto(){

	gStyle->SetOptStat(0);


	// Draw histos filled by Geant4 simulation
	//
	map<TString,TFile*> FileCollection;
	map<TString,TFile*>::iterator FileCollectionIt;

	//FileCollection["1"] 		= new TFile("data/run1.root");
	//FileCollection["2"] 		= new TFile("data/run2.root");
	FileCollection["Ag"] 		= new TFile("data/gamma_Ag_40keV.root");
	FileCollection["Au"]		= new TFile("data/gamma_Au_40keV.root");
	//FileCollection["Bi2S3"]	= new TFile("data/gamma_Bi2S3_40keV.root");
	//FileCollection["Pb"]		= new TFile("data/gamma_Pb_40keV.root");
	//FileCollection["Pt"]		= new TFile("data/gamma_Pt_40keV.root");
	//FileCollection["Ta2O5"]	= new TFile("data/gamma_Ta2O5_40keV.root");

	map<TString,Int_t> ColorCollection;

	ColorCollection["1"] 		= kBlue;
	ColorCollection["2"] 		= kRed;
	ColorCollection["Ag"] 		= kBlue;
	ColorCollection["Au"] 		= kRed;
	ColorCollection["Bi2S3"] 	= kMagenta;
	ColorCollection["Pb"] 		= kCyan;
	ColorCollection["Pt"] 		= kGreen;
	ColorCollection["Ta2O5"]	= kOrange;

	map<Int_t,TString> VariableNameCollection;
	map<Int_t,TString>::iterator VariableNameCollectionIt;

	VariableNameCollection[1]	="DepthEnergyDepositByElectronsPerGoodEvent";
	VariableNameCollection[2]	="DepthEnergyDepositByElectronsPerGoodEvent2";
	VariableNameCollection[3]	="DepthDoseByElectronsPerGoodEvents";
	VariableNameCollection[4]	="DepthDoseByElectronsPerGoodEvents2";
	VariableNameCollection[5]	="EnergyDepositByElectronsPerGoodEvent";
	VariableNameCollection[6]	="DoseByElectronsPerGoodEvent";
	VariableNameCollection[7]	="NumberOfElectronsPerGoodEvent";
	VariableNameCollection[8]	="SecondariesSpectrumAtVertex";
	VariableNameCollection[9]	="SecondariesSpectrumAtNPSurface";
	VariableNameCollection[10]	="SecondariesTrackLength";
	VariableNameCollection[11]	="SecondariesRange";

	map<Int_t,double_t> NFactorCollection;

	NFactorCollection[1] 		= 1.;
	NFactorCollection[2] 		= 1.;
	NFactorCollection[3] 		= 1.;
	NFactorCollection[4] 		= 1.;
	NFactorCollection[5] 		= 0;
	NFactorCollection[6] 		= 0;
	NFactorCollection[7] 		= 0;
	NFactorCollection[8]		= 0;
	NFactorCollection[9]		= 0;
	NFactorCollection[10]		= 0;
	NFactorCollection[11]		= 0;

	TCanvas* c1 = new TCanvas("c1", "c1");
	c1->Divide(6,2);

	for(VariableNameCollectionIt = VariableNameCollection.begin() ; VariableNameCollectionIt != VariableNameCollection.end(); ++VariableNameCollectionIt){

		c1->cd(VariableNameCollectionIt->first);

		TLegend* l = new TLegend(0.65, 0.75, 0.85, 0.85) ;

		map<TString,TH1D*> h;
		TString HistogramName("histograms/");
		HistogramName+=VariableNameCollectionIt->second.Data();

		Int_t MaximunBinContent = 0;

		for (FileCollectionIt = FileCollection.begin() ; FileCollectionIt != FileCollection.end(); ++FileCollectionIt){

			h[FileCollectionIt->first] = static_cast<TH1D*>(FileCollectionIt->second->Get(HistogramName.Data()));
			h[FileCollectionIt->first]->SetLineColor(ColorCollection[FileCollectionIt->first]);
			if(NFactorCollection[VariableNameCollectionIt->first]!=0){
			h[FileCollectionIt->first]->Scale(NFactorCollection[VariableNameCollectionIt->first]);
			h[FileCollectionIt->first]->Draw("HIST same");
			}else{
				h[FileCollectionIt->first]->DrawNormalized("HIST same");
			}

			l->AddEntry(h[FileCollectionIt->first], FileCollectionIt->first.Data(), "L") ;
			l->Draw() ;

		}

	}
	c1->Print("data/h1.png");

}  
