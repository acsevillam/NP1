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
#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

// NP1 Headers
#include "NP1EventAction.hh"
#include "NP1Analysis.hh"
#include "NP1RunAction.hh"
#include "NP1Control.hh"

NP1EventAction::NP1EventAction(NP1RunAction* runAction):
G4UserEventAction(),
fRunAction(runAction),
fTotalEdep(0.),
fNumberOfSecondaries(0)
{}

NP1EventAction::~NP1EventAction()
{}

void NP1EventAction::BeginOfEventAction(const G4Event* anEvent)
{    

	G4int nEvents = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed() ;
	if(nEvents>10){
		G4int 	fraction 	= G4int(nEvents/100) ;
		if(anEvent->GetEventID()%fraction == 0)
			G4cout<<"("<<(anEvent->GetEventID()/(nEvents*1.0)*100)<<" %)"<<G4endl ;

	}else {
		G4int 	fraction 	= G4int(nEvents/1) ;
		if(anEvent->GetEventID()%fraction == 0)
			G4cout<<"("<<(anEvent->GetEventID()/(nEvents*1.0)*100)<<" %)"<<G4endl ;
	}

	G4double KineticEnergyAtVertex 		= anEvent->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy()/keV;
	G4String ParticleName		 		= anEvent->GetPrimaryVertex()->GetPrimary()->GetParticleDefinition()->GetParticleName();
	G4double XAtVertex 					= anEvent->GetPrimaryVertex()->GetPosition().x();
	G4double YAtVertex 					= anEvent->GetPrimaryVertex()->GetPosition().y();
	G4double ZAtVertex 					= anEvent->GetPrimaryVertex()->GetPosition().z();
	G4double RAtVertex 					= anEvent->GetPrimaryVertex()->GetPosition().r();

	// Analysis manager
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	if(analysisManager->GetNtupleActivation(1)==true){
		analysisManager->FillNtupleDColumn(1,0,KineticEnergyAtVertex);
		analysisManager->FillNtupleSColumn(1,1,ParticleName);
		analysisManager->FillNtupleDColumn(1,2,XAtVertex);
		analysisManager->FillNtupleDColumn(1,3,YAtVertex);
		analysisManager->FillNtupleDColumn(1,4,ZAtVertex);
		analysisManager->FillNtupleDColumn(1,5,RAtVertex);
		analysisManager->AddNtupleRow(1);
	}

	fTotalEdep=0.;
	fNumberOfSecondaries=0;

}

void NP1EventAction::EndOfEventAction(const G4Event* /*anEvent*/)
{   

	if(!(fTotalEdep>0. || fNumberOfSecondaries>0)) return;

	G4LogicalVolume* waterBox_log = G4LogicalVolumeStore::GetInstance()->GetVolume("waterBox_log");
	G4double mass = waterBox_log->GetMass();

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	// accumulate statistics in run action
	// Analysis manager

	fRunAction->CountGoodEvent(1.0);

	if(fTotalEdep>0.){
		fRunAction->AddTotalEdep(fTotalEdep);
		fRunAction->CountEdepEvent();

		//G4cout<<"\t"<<G4BestUnit(fTotalEdep,"Energy")<<G4endl;
		analysisManager->FillH1(4,fTotalEdep);
		analysisManager->FillH1(5,fTotalEdep/mass);
	}
	if(fNumberOfSecondaries>0.){
		analysisManager->FillH1(6,fNumberOfSecondaries);
	}

}
