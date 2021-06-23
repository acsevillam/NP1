// ROOT Headers
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TText.h"

#include <map>
#include <iostream>
#include <string>

using namespace std;

void plot_06(){

	gStyle->SetOptStat(0);

	// Draw histos filled by Geant4 simulation
	//
	map<TString,TFile*> FileCollection;
	map<TString,TFile*>::iterator FileCollectionIt;

	//FileCollection["Ag"] 		= new TFile("data/2018-04-01/gamma_Ag_40keV.root");
	FileCollection["Au"]			= new TFile("data/2018-04-01/gamma_Au_40keV.root");
	FileCollection["Bi2S3"]		= new TFile("data/2018-04-01/gamma_Bi2S3_40keV.root");
	FileCollection["Pb"]			= new TFile("data/2018-04-01/gamma_Pb_40keV.root");
	FileCollection["Pt"]			= new TFile("data/2018-04-01/gamma_Pt_40keV.root");
	FileCollection["Ta2O5"]		= new TFile("data/2018-04-01/gamma_Ta2O5_40keV.root");

	map<TString,Int_t> ColorCollection;

	//ColorCollection["Ag"] 		= kBlue;
	ColorCollection["Au"] 		= kRed;
	ColorCollection["Bi2S3"] 	= kMagenta;
	ColorCollection["Pb"] 		= kCyan;
	ColorCollection["Pt"] 		= kGreen;
	ColorCollection["Ta2O5"]		= kOrange;

	map<Int_t,TString> VariableNameCollection;

	VariableNameCollection[1]	="EnergyDepositByElectronsPerGoodEvent";

	TCanvas* c1 = new TCanvas("c1", "c1", 400, 600);
	c1->Divide(1,1);

	map<TString,TH1D*> h;
	TString HistogramName("histograms/");

	// --- SecondariesTrackLength ---

	c1->cd(1);
	c1->cd(1)->SetLeftMargin(0.20);
	c1->cd(1)->SetLogy(false);
	c1->cd(1)->SetGrid(true,true);
	c1->cd(1)->SetTicks(true,true);

	TString HistogramName1 = HistogramName+VariableNameCollection[1];

	Int_t n = FileCollection.size();
	Double_t x[n][n], y[n][n];
	TText* t1;

	Int_t i, j;

	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			x[i][j] = j;
			y[i][j] = 0;
		}
	}

	i = 0;
	for (FileCollectionIt = FileCollection.begin() ; FileCollectionIt != FileCollection.end(); ++FileCollectionIt){
		h[FileCollectionIt->first] = static_cast<TH1D*>(FileCollectionIt->second->Get(HistogramName1.Data()));
		y[i][i] = h[FileCollectionIt->first]->GetEntries()/1E8;
		i++;
	}

	Double_t min_x=-0.5, max_x=n-0.5;

	TMultiGraph *mg1 = new TMultiGraph();
	TGraph *g1[n];

	i=0;
	for (FileCollectionIt = FileCollection.begin() ; FileCollectionIt != FileCollection.end(); ++FileCollectionIt){
		g1[i]=new TGraph (n, x[i], y[i]);
		g1[i]->SetFillColor(ColorCollection[FileCollectionIt->first]);
		mg1->Add(g1[i]);
		i++;
	}

	mg1->Draw("AB");

	mg1->GetHistogram()->GetXaxis()->Set(n,min_x,max_x);
	mg1->GetXaxis()->SetRangeUser(min_x,max_x);
	mg1->GetXaxis()->SetLimits(min_x,max_x);
	mg1->GetXaxis()->SetLabelSize(0.06);
	mg1->GetXaxis()->SetTitleFont();
	mg1->GetXaxis()->SetLabelFont();
	mg1->GetYaxis()->SetTitleSize(0.04);
	mg1->GetYaxis()->SetLabelSize(0.04);
	mg1->GetYaxis()->SetTitle("Good events per incident photon");
	mg1->GetYaxis()->SetTitleFont();
	mg1->GetYaxis()->SetLabelFont();


	j=0;
	for (FileCollectionIt = FileCollection.begin() ; FileCollectionIt != FileCollection.end(); ++FileCollectionIt){
		mg1->GetHistogram()->GetXaxis()->SetBinLabel(j+1,FileCollectionIt->first);
		j++;
	}

	gPad->Modified();

	// --- End SecondariesTrackLength ---

	c1->Print("data/plot_06.png");

}  
