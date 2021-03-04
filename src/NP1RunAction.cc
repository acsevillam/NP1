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
		fTotalEdep2(0.)
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

	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	// Register accumulable to the accumulable manager
	accumulableManager->RegisterAccumulable(fEDepEvents);
	accumulableManager->RegisterAccumulable(fGoodEvents);
	accumulableManager->RegisterAccumulable(fTotalEdep);
	accumulableManager->RegisterAccumulable(fTotalEdep2);

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

	G4int nofEvents = run->GetNumberOfEvent();
	if (nofEvents == 0) return;

	// Compute energy deposited event
	G4double eDepEvents = G4double(fEDepEvents.GetValue());

	// Compute good events
	G4double goodEvents = G4double(fGoodEvents.GetValue());

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	// Merge accumulables
	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->Merge();


	// Total energy deposit in a run and its variance
	//
	G4double TotalEdep=0., TotalEdep2=0., rmsTotalEdep=0.;

	TotalEdep=fTotalEdep.GetValue();
	TotalEdep2=fTotalEdep2.GetValue();

	if(eDepEvents>0) rmsTotalEdep = TotalEdep2 - TotalEdep*TotalEdep/eDepEvents;
	if (rmsTotalEdep > 0.) rmsTotalEdep = std::sqrt(rmsTotalEdep);


	G4LogicalVolume* waterBox_log = G4LogicalVolumeStore::GetInstance()->GetVolume("waterBox_log");
	G4double mass = waterBox_log->GetMass();

	// Print
	//
	if (IsMaster()) {
		G4cout
		<< G4endl
		<< "--------------------End of Global Run-----------------------"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(1)  Total events per run : "
		<< G4endl
		<< "     " << nofEvents
		<< G4endl;

		G4cout
		<< G4endl
		<< "(2)  Number of edep events per event: "
		<< G4endl
		<< "     " << G4double(eDepEvents)/G4double(nofEvents)
		<< G4endl
		<< "     [Total number of good events / Total events per run (1)]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(3)  Number of good events per event: "
		<< G4endl
		<< "     " << G4double(goodEvents)/G4double(nofEvents)
		<< G4endl
		<< "     [Total number of good events / Total events per run (1)]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(4)  Cumulated total Edep per event in water box volume: "
		<< G4endl
		<< "     " << G4BestUnit(TotalEdep/nofEvents,"Energy") << " +/- " << G4BestUnit(rmsTotalEdep/nofEvents,"Energy")
		<< G4endl
		<< "     [Total Edep in water box / Total events per run (1)]"
		<< G4endl
		<< "     " << G4BestUnit(analysisManager->GetH1(0)->sum_all_bin_heights()*keV/nofEvents,"Energy")
		<< G4endl
		<< "     [Radial energy deposit histo integral / Total events per run (1) ]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(5)  Cumulated total dose per event, in water box volume: "
		<< G4endl
		<< "     " << G4BestUnit(TotalEdep/nofEvents/mass,"Dose") << " +/- " << G4BestUnit(rmsTotalEdep/nofEvents/mass,"Dose")
		<< G4endl
		<< "     [Total Edep in water box per event (4) / mass of water box ]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(6)  Cumulated total Edep per good event in water box volume: "
		<< G4endl
		<< "     " << G4BestUnit(analysisManager->GetH1(4)->mean()*keV,"Energy") << " +/- " << G4BestUnit(analysisManager->GetH1(4)->rms()*keV,"Energy")
		<< G4endl
		<< "     [Energy deposit per event histo mean and rms]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(7)  Cumulated total dose per good event, in water box volume: "
		<< G4endl
		<< "     " << G4BestUnit(analysisManager->GetH1(4)->mean()*keV/mass,"Dose") << " +/- " << G4BestUnit(analysisManager->GetH1(4)->rms()*keV/mass,"Dose")
		<< G4endl
		<< "     [Cumulated total Edep per good event in water box volume (6) / mass of water box]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "(8) Cumulated secondaries per good event, in scoring volume : "
		<< G4endl
		<< "     " << analysisManager->GetH1(6)->mean() << " +/- " << analysisManager->GetH1(6)->rms()
		<< G4endl
		<< "     [Number of secondaries per event histo mean and rms]"
		<< G4endl;

		G4cout
		<< G4endl
		<< "------------------------------------------------------------"
		<< G4endl;

	}

	if(analysisManager->IsActive()){
		if (IsMaster()) {
			analysisManager->GetH1(0)->scale(1./nofEvents);
			analysisManager->GetH1(1)->scale(1./nofEvents);
			analysisManager->GetH1(2)->scale(1./nofEvents);
			analysisManager->GetH1(3)->scale(1./nofEvents);
		}

		// Save histograms and ntuples
		analysisManager->Write();
		analysisManager->CloseFile();
	}

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
	analysisManager->CreateH1("DepthEnergyDepositByElectronsPerGoodEvent","Depth energy deposit by electrons per event", waterSpheresNo, 0, waterSphereArray_rmax-waterSphereArray_rmin,"um");
	analysisManager->SetH1Activation(0,false);
	analysisManager->SetH1XAxisTitle(0,"Distance from nano-particle center [um]");
	analysisManager->SetH1YAxisTitle(0,"Edep in "+G4String(G4BestUnit(waterSphereArray_dr,"Length"))+" shell per event [keV]");
	// id = 1
	analysisManager->CreateH1("DepthEnergyDepositByElectronsPerGoodEvent2","Depth energy deposit by electrons per event", waterSpheresNo, 0, waterSpheresNo);
	analysisManager->SetH1Activation(1,false);
	analysisManager->SetH1XAxisTitle(1,"Distance from nano-particle center [No.]");
	analysisManager->SetH1YAxisTitle(1,"Edep in "+G4String(G4BestUnit(waterSphereArray_dr,"Length"))+" shell per event [keV]");
	// id = 2
	analysisManager->CreateH1("DepthDoseByElectronsPerGoodEvents","Depth dose by electrons per event", waterSpheresNo, 0, waterSphereArray_rmax-waterSphereArray_rmin,"um");
	analysisManager->SetH1Activation(2,false);
	analysisManager->SetH1XAxisTitle(2,"Distance from nano-particle center [um]");
	analysisManager->SetH1YAxisTitle(2,"DoseDep in "+G4String(G4BestUnit(waterSphereArray_dr,"Length"))+" shell of water per event [keV]");
	// id = 3
	analysisManager->CreateH1("DepthDoseByElectronsPerGoodEvents2","Depth dose by electrons per event", waterSpheresNo, 0, waterSpheresNo);
	analysisManager->SetH1Activation(3,false);
	analysisManager->SetH1XAxisTitle(3,"Distance from nano-particle center [No.]");
	analysisManager->SetH1YAxisTitle(3,"DoseDep in "+G4String(G4BestUnit(waterSphereArray_dr,"Length"))+" shell of water per event [keV]");
	// id = 4
	analysisManager->CreateH1("NumberOfSecondariesPerGoodEvent","Number of secondaries generated per good event distribution", 20, 1, 20);
	analysisManager->SetH1Activation(4,false);
	// id = 5
	analysisManager->CreateH1("EnergyDepositPerGoodEvent","Energy deposit per good event distribution", 1000, 0., 40*keV, "keV");
	analysisManager->SetH1Activation(5,false);
	// id = 6
	analysisManager->CreateH1("DosePerGoodEvent","Dose per good event distribution", 1000, 0, 1E-3*gray, "microGy");
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
