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

// NP1 Headers
#include "NP1EventAction.hh"
#include "NP1TrackingAction.hh"
#include "NP1RunAction.hh"
#include "NP1Analysis.hh"

using namespace std;

NP1TrackingAction::NP1TrackingAction(NP1EventAction* eventAction)
:G4UserTrackingAction(),
 fEventAction(eventAction),
 fSecondaryCountFlag(false)
{}

NP1TrackingAction::~NP1TrackingAction() {

}

void NP1TrackingAction::PreUserTrackingAction (const G4Track* /*aTrack*/){
	fSecondaryCountFlag=false;
}

void NP1TrackingAction::PostUserTrackingAction (const G4Track* aTrack){

	if(!fSecondaryCountFlag) return;

	// Analysis manager

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	G4String CreatorProcessName;
	G4String CreatorModelName ;

	if(aTrack -> GetCreatorProcess ()!=0){
		CreatorProcessName = aTrack -> GetCreatorProcess() -> GetProcessName() ;
		CreatorModelName = aTrack ->GetCreatorModelName();
		if(CreatorModelName=="Undefined"){
			CreatorModelName="u_"+CreatorProcessName;
		}
	}else{
		CreatorProcessName = "gun" ;
		CreatorModelName = "gun";
	}

	G4double KineticEnergyAtVertex 		= aTrack->GetVertexKineticEnergy();
	G4double RAtVertex 					= aTrack->GetVertexPosition().getR();
	G4String ParticleName 				= aTrack->GetParticleDefinition()-> GetParticleName();
	G4double TrackLength		 		= aTrack->GetTrackLength();
	G4double R		 					= aTrack->GetPosition().getR();


	analysisManager->FillH1(9,KineticEnergyAtVertex);
	analysisManager->FillH1(11,TrackLength);
	analysisManager->FillH1(12,R);

	if(analysisManager->GetNtupleActivation(0)==true){
		analysisManager->FillNtupleDColumn(0,0,KineticEnergyAtVertex);
		analysisManager->FillNtupleDColumn(0,1,RAtVertex);
		analysisManager->FillNtupleSColumn(0,2,CreatorProcessName);
		analysisManager->FillNtupleSColumn(0,3,CreatorModelName);
		analysisManager->FillNtupleSColumn(0,4,ParticleName);
		analysisManager->FillNtupleDColumn(0,5,TrackLength);
		analysisManager->FillNtupleDColumn(0,6,R);
		analysisManager->AddNtupleRow(0);
	}

	fEventAction->CountSecondary();

}
