/*
 * NanoParticle 1.0
 * Copyright (c) 2017 Escuela Colombiana de Carreras Industriales - ECCI
 * All Right Reserved.
 *
 * Developed by Andrés Camilo Sevilla
 *
 * Use and copying of these libraries and preparation of derivative works
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 *
 * Bogotá, Colombia.
 *
 */

// Geant4 Headers
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4ScoringManager.hh"

// NP1 Headers
#include "NP1PrimaryGeneratorAction.hh"
#include "NP1RunAction.hh"
#include "NP1Analysis.hh"
#include "NP1Control.hh"

class G4AccumulableManager;

using namespace std;

NP1RunAction::NP1RunAction(): G4UserRunAction(),
		fEDepEvents(0),
		fGoodEvents(0),
		fTotalEdep(0.),
		fTotalEdep2(0.),
		fSecondariesEdep(0.),
		fSecondariesEdep2(0.),
		fNumberOfSecondaries(0),
		fNumberOfSecondaries2(0),
		fTrackLength(0.),
		fTrackLength2(0.)
{ 

	// add new units for dose
	//
	const G4double milligray = 1.e-3*gray;
	const G4double microgray = 1.e-6*gray;
	const G4double nanogray  = 1.e-9*gray;
	const G4double picogray  = 1.e-12*gray;

	new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
	new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
	new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
	new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);

	new G4UnitDefinition("keV/um", "keV/um" , "LET", keV/um);

	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	// Register accumulable to the accumulable manager
	accumulableManager->RegisterAccumulable(fEDepEvents);
	accumulableManager->RegisterAccumulable(fGoodEvents);
	accumulableManager->RegisterAccumulable(fTotalEdep);
	accumulableManager->RegisterAccumulable(fTotalEdep2);
	accumulableManager->RegisterAccumulable(fSecondariesEdep);
	accumulableManager->RegisterAccumulable(fSecondariesEdep2);
	accumulableManager->RegisterAccumulable(fNumberOfSecondaries);
	accumulableManager->RegisterAccumulable(fNumberOfSecondaries2);
	accumulableManager->RegisterAccumulable(fTrackLength);
	accumulableManager->RegisterAccumulable(fTrackLength2);

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetActivation(true);
	G4cout << "Using " << analysisManager->GetType() << G4endl;
	analysisManager->SetVerboseLevel(0);
	analysisManager->SetFileName("data/NP1");

	// Create Histograms an n-Tuples
	CreateNTuples();
	CreateHistos();

}

NP1RunAction::~NP1RunAction()
{
	delete G4AccumulableManager::Instance();
	delete G4AnalysisManager::Instance();
}

void NP1RunAction::BeginOfRunAction(const G4Run*)
{ 

	// reset accumulables to their initial values
	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->Reset();

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	if(analysisManager->IsActive()){
		// Open an output file
		analysisManager->OpenFile();
	}

}

void NP1RunAction::EndOfRunAction(const G4Run* run)
{

	// Merge accumulables
	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->Merge();

	G4int nofEvents = run->GetNumberOfEvent();
	if (nofEvents == 0) return;

	G4double biasFactor= NP1Control::GetInstance()->GetCSBiasFactor();

	// Compute energy deposited events
	G4int eDepEvents = G4int(fEDepEvents.GetValue());

	// Compute good events
	G4int goodEvents = G4int(fGoodEvents.GetValue())/biasFactor;

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	if(analysisManager->IsActive()){
		// Scale histograms
		if(goodEvents>0){
			analysisManager->ScaleH1(0,1./goodEvents);
			analysisManager->ScaleH1(1,1./goodEvents);
			analysisManager->ScaleH1(2,1./goodEvents);
			analysisManager->ScaleH1(3,1./goodEvents);
		}

		// Save histograms and ntuples
		analysisManager->Write();
		analysisManager->CloseFile();
	}

	// Total energy deposit in a run and its variance
	//
	G4double TotalEdep=0., TotalEdep2=0., rmsTotalEdep=0.;

	TotalEdep=fTotalEdep.GetValue();
	TotalEdep2=fTotalEdep2.GetValue()+fSecondariesEdep2.GetValue()*(1/biasFactor-1);

	if(eDepEvents>0) rmsTotalEdep = TotalEdep2 - TotalEdep*TotalEdep/eDepEvents;
	if (rmsTotalEdep > 0.) rmsTotalEdep = std::sqrt(rmsTotalEdep);

	// Secondaries energy deposit in a run and its variance
	//
	G4double SecondariesEdep=0., SecondariesEdep2=0., rmsSecondariesEdep=0.;

	SecondariesEdep=fSecondariesEdep.GetValue()/biasFactor;
	SecondariesEdep2=fSecondariesEdep2.GetValue()/biasFactor;

	if(goodEvents>0) rmsSecondariesEdep = SecondariesEdep2/goodEvents - std::pow(SecondariesEdep/goodEvents,2);
	if (rmsSecondariesEdep > 0.) rmsSecondariesEdep = std::sqrt(rmsSecondariesEdep/(goodEvents-1));

	// Compute number of secondaries = number of secondaries in a run and its variance
	//
	G4double NumberOfSecondaries=0, NumberOfSecondaries2=0;
	G4double rmsNumberOfSecondaries=0.;

	NumberOfSecondaries  = G4double(fNumberOfSecondaries.GetValue())/biasFactor;
	NumberOfSecondaries2 = G4double(fNumberOfSecondaries2.GetValue())/biasFactor;

	if(goodEvents>0) rmsNumberOfSecondaries = NumberOfSecondaries2/goodEvents - std::pow(NumberOfSecondaries/goodEvents,2);
	if (rmsNumberOfSecondaries > 0.) rmsNumberOfSecondaries = std::sqrt(rmsNumberOfSecondaries/(goodEvents-1));

	// Total track length in a run and its variance
	//
	G4double TrackLength=0., TrackLength2=0., rmsTrackLength=0.;

	TrackLength=fTrackLength.GetValue()/biasFactor;
	TrackLength2=fTrackLength2.GetValue()/biasFactor;

	if(eDepEvents>0) rmsTrackLength = TrackLength2 - TrackLength*TrackLength/eDepEvents;
	if (rmsTrackLength > 0.) rmsTrackLength = std::sqrt(rmsTrackLength);


	G4LogicalVolume* waterBox_log = G4LogicalVolumeStore::GetInstance()->GetVolume("waterBox_log");
	G4double mass = waterBox_log->GetMass();
	G4Box* waterBox_geo= (G4Box*) waterBox_log->GetSolid();
	G4double volume = waterBox_geo->GetCubicVolume();
	G4double waterBox_dx = waterBox_geo->GetXHalfLength();
	G4double waterBox_dy = waterBox_geo->GetYHalfLength();
	G4double waterBox_dz = waterBox_geo->GetZHalfLength();

	// Print
	//
	if (IsMaster()) {
		G4cout
		<< G4endl
		<< "--------------------End of Global Run-----------------------"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(1)  Bias factor : "
		<< biasFactor
		<< G4endl;

		G4cout
		<< G4endl
		<< "(2)  Sensitive volume dimensions : "
		<< G4BestUnit(volume,"Volume")<< "\t"<< "(" << G4BestUnit(waterBox_dx,"Length") << "," << G4BestUnit(waterBox_dy,"Length") << ","  << G4BestUnit(waterBox_dz,"Length") <<")"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(3)  Total events per run : "
		<< nofEvents
		<< G4endl;

		G4cout
		<< G4endl
		<< "(4)  Number of eDep events per event : "
		<< G4double(eDepEvents)/G4double(nofEvents)
		<< G4endl;

		G4cout
		<< G4endl
		<< "(5)  Number of good events per event: "
		<< G4double(goodEvents)/G4double(nofEvents)
		<< G4endl
		<< "     [Total number of good events / Total events per run (3)]"
		<< G4endl;
		G4cout
		<< G4endl
		<< "(6)  Cumulated total Edep per event in water box volume: "
		<< G4BestUnit(TotalEdep/nofEvents,"Energy") << " rms = " << G4BestUnit(rmsTotalEdep/nofEvents,"Energy")
		<< G4endl
		<< "     [Total Edep in water box / Total events per run (3)]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(7)  Cumulated total dose per event, in water box volume: "
		<< G4BestUnit(TotalEdep/nofEvents/mass,"Dose") << " rms = " << G4BestUnit(rmsTotalEdep/nofEvents/mass,"Dose")
		<< G4endl
		<< "     [Total Edep in water box / mass of water box / Total events per run (3)]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(8)  Cumulated track length per event, in water box volume: "
		<< G4BestUnit(TrackLength/nofEvents,"Length") << " rms = " << G4BestUnit(rmsTrackLength/nofEvents,"Length")
		<< G4endl
		<< "     [Track length / Total events per run (3)]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(9)  LET average in water box volume: "
		<< G4BestUnit(TotalEdep/TrackLength,"LET") << " rms = " << G4BestUnit((rmsTotalEdep/TotalEdep+rmsTrackLength/TrackLength)*TotalEdep/TrackLength,"LET")
		<< G4endl
		<< "     [Cumulated total Edep per event in water box volume (6) / Track length per event in water box volume  (8)]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(10)  Cumulated total Edep per good event, in water box volume : "
		<< G4BestUnit(TotalEdep/goodEvents,"Energy") << " rms = " << G4BestUnit(rmsTotalEdep/goodEvents,"Energy")
		<< G4endl
		<< "     [Total Edep in water box / mass of water box / (Total good events per event (4) * Total events per run (1))]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(11)  Cumulated total dose per good event, in water box volume : "
		<< G4BestUnit(TotalEdep/goodEvents/mass,"Dose") << " rms = " << G4BestUnit(rmsTotalEdep/goodEvents/mass,"Dose")
		<< G4endl
		<< "     [Total Edep in water box / mass of water box / (Total good events per event (4) * Total events per run (1))]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(12)  Cumulated secondaries Edep per event, in water box volume : "
		<< G4BestUnit(SecondariesEdep/nofEvents,"Energy") << " rms = " << G4BestUnit(rmsSecondariesEdep/nofEvents,"Energy")
		<< G4endl
		<< "     [Secondaries Edep in water box / Total events per run (3)]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(13)  Cumulated secondaries dose per event, in water box volume : "
		<< G4BestUnit(SecondariesEdep/nofEvents/mass,"Dose") << " rms = " << G4BestUnit(rmsSecondariesEdep/nofEvents/mass,"Dose")
		<< G4endl
		<< "     [Secondaries Edep in water box / mass of water box / Total events per run (3)]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(14)  Cumulated secondaries Edep per good event, in water box volume : "
		<< G4BestUnit(SecondariesEdep/goodEvents,"Energy") << " rms = " << G4BestUnit(rmsSecondariesEdep/goodEvents,"Energy")
		<< G4endl
		<< "     [Secondaries Edep in water box / Total good events per event (4) * Total events per run (3)]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(15) Cumulated secondaries dose per good event in water box volume : "
		<< G4BestUnit(SecondariesEdep/goodEvents/mass,"Dose") << " rms = " << G4BestUnit(rmsSecondariesEdep/goodEvents/mass,"Dose")
		<< G4endl
		<< "     [Secondaries Edep in water box / mass of water box / Total good events per event (4) * Total events per run (3)]"
		<< G4endl;


		G4cout
		<< G4endl
		<< "(16) Secondaries to total dose ratio: "
		<< SecondariesEdep/TotalEdep << " rms = " << ((rmsSecondariesEdep/SecondariesEdep)+(rmsTotalEdep/TotalEdep))*(SecondariesEdep/TotalEdep)
		<< G4endl
		<< "     [Secondaries Edep in water box / Total Edep in water box]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(17) Cumulated secondaries per good event, in scoring volume : "
		<< NumberOfSecondaries/goodEvents << " rms = " << rmsNumberOfSecondaries/goodEvents
		<< G4endl;

		G4cout
		<< G4endl
		<< "------------------------------------------------------------"
		<< G4endl;

	}

	// Activate UI-command base scorer
	G4ScoringManager * scoringManager = G4ScoringManager::GetScoringManager();
	scoringManager->SetVerboseLevel(1);

}

void NP1RunAction::CreateNTuples(){

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetNtupleMerging(true);

	// Creating ntuples
	analysisManager->SetFirstNtupleId(0);

	// id = 0
	analysisManager->CreateNtuple("SecondariesProduction", "Variables related to secondaries production");
	analysisManager->SetNtupleActivation(0,false);
	analysisManager->CreateNtupleDColumn(0,"KineticEnergyAtVertex");
	analysisManager->CreateNtupleDColumn(0,"RAtVertex");
	analysisManager->CreateNtupleSColumn(0,"CreatorProcessName");
	analysisManager->CreateNtupleSColumn(0,"CreatorModelName");
	analysisManager->CreateNtupleSColumn(0,"ParticleName");
	analysisManager->CreateNtupleDColumn(0,"TrackLength");
	analysisManager->CreateNtupleDColumn(0,"R");
	analysisManager->FinishNtuple();

	// id = 1
	analysisManager->CreateNtuple("Source", "Variables related to source");
	analysisManager->SetNtupleActivation(1,false);
	analysisManager->CreateNtupleDColumn(1,"KineticEnergyAtVertex");
	analysisManager->CreateNtupleSColumn(1,"ParticleName");
	analysisManager->CreateNtupleDColumn(1,"XAtVertex");
	analysisManager->CreateNtupleDColumn(1,"YAtVertex");
	analysisManager->CreateNtupleDColumn(1,"ZAtVertex");
	analysisManager->CreateNtupleDColumn(1,"RAtVertex");
	analysisManager->FinishNtuple();

}

void NP1RunAction::CreateHistos(){

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	// Get required values to define histogram limits
	G4double waterSphereArray_rmin = NP1Control::GetInstance()->GetShellsFrame_rmin();
	G4double waterSphereArray_rmax = NP1Control::GetInstance()->GetShellsFrame_rmax();
	G4int waterSpheresNo = NP1Control::GetInstance()->GetShellsNo();
	G4double waterSphereArray_dr	= waterSphereArray_rmax/waterSpheresNo;

	// Creating histos
	analysisManager->SetFirstHistoId(0);

	// id = 0
	analysisManager->CreateH1("DepthEnergyDepositByElectronsPerGoodEvent","Depth energy deposit by electrons per good event", waterSpheresNo, waterSphereArray_rmin, waterSphereArray_rmax,"nm");
	analysisManager->SetH1Activation(0,false);
	analysisManager->SetH1XAxisTitle(0,"Distance from nano-particle center [nm]");
	analysisManager->SetH1YAxisTitle(0,"Edep in "+G4String(G4BestUnit(waterSphereArray_dr,"Length"))+" shell per good event [keV]");
	// id = 1
	analysisManager->CreateH1("DepthEnergyDepositByElectronsPerGoodEvent2","Depth energy deposit by electrons per good event", waterSpheresNo, 0, waterSpheresNo);
	analysisManager->SetH1Activation(1,false);
	analysisManager->SetH1XAxisTitle(1,"Distance from nano-particle center [nm]");
	analysisManager->SetH1YAxisTitle(1,"Edep in "+G4String(G4BestUnit(waterSphereArray_dr,"Length"))+" shell per good event [keV]");
	// id = 2
	analysisManager->CreateH1("DepthDoseByElectronsPerGoodEvents","Depth dose by electrons per good event", waterSpheresNo, waterSphereArray_rmin, waterSphereArray_rmax,"nm");
	analysisManager->SetH1Activation(2,false);
	analysisManager->SetH1XAxisTitle(2,"Distance from nano-particle center [nm]");
	analysisManager->SetH1YAxisTitle(2,"DoseDep in "+G4String(G4BestUnit(waterSphereArray_dr,"Length"))+" shell of water per good event [keV]");
	// id = 3
	analysisManager->CreateH1("DepthDoseByElectronsPerGoodEvents2","Depth dose by electrons per good event", waterSpheresNo, 0, waterSpheresNo);
	analysisManager->SetH1Activation(3,false);
	analysisManager->SetH1XAxisTitle(3,"Distance from nano-particle center [nm]");
	analysisManager->SetH1YAxisTitle(3,"DoseDep in "+G4String(G4BestUnit(waterSphereArray_dr,"Length"))+" shell of water per good event [keV]");
	// id = 4
	analysisManager->CreateH1("SecondariesEnergyDepositPerGoodEvent","Secondaries energy deposit per good event", 1000, 0., 40*keV, "keV");
	analysisManager->SetH1Activation(4,false);
	// id = 5
	analysisManager->CreateH1("SecondariesDosePerGoodEvent","Secondaries dose per good event", 1000, 0, 1E4*gray, "microGy");
	analysisManager->SetH1Activation(5,false);
	// id = 6
	analysisManager->CreateH1("NumberOfSecondariesPerGoodEvent","Number of secondaries generated per good event", 10, 1, 10);
	analysisManager->SetH1Activation(6,false);
	// id = 7
	analysisManager->CreateH1("SecondariesSpectrumAtVertex","Secondaries' energy distribution at vertex", 1000, 0, 40*keV,"keV");
	analysisManager->SetH1Activation(7,false);
	// id = 8
	analysisManager->CreateH1("SecondariesSpectrumAtNPSurface","Secondaries' energy distribution at nano-particle surface", 1000, 0, 40*keV,"keV");
	analysisManager->SetH1Activation(8,false);
	// id = 9
	analysisManager->CreateH1("SecondariesTrackLength","Secondaries' track length", 1000, waterSphereArray_rmin, waterSphereArray_rmax*100,"um");
	analysisManager->SetH1Activation(9,false);
	// id = 10
	analysisManager->CreateH1("SecondariesRange","Secondaries' range", 1000, waterSphereArray_rmin, waterSphereArray_rmax*100,"um");
	analysisManager->SetH1Activation(10,false);
}
