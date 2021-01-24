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
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TrackingManager.hh"

// NP1 Headers
#include "NP1EventAction.hh"
#include "NP1TrackingAction.hh"
#include "NP1SteppingAction.hh"
#include "NP1Analysis.hh"
#include "NP1DetectorConstruction.hh"
#include "NP1Control.hh"
#include "G4VPhysicalVolume.hh"

using namespace std;

NP1SteppingAction::NP1SteppingAction(NP1EventAction* eventAction,NP1TrackingAction* trackingAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fTrackingAction(trackingAction),
  fScoringVolumeVector(0)
{}

NP1SteppingAction::~NP1SteppingAction()
{}

void NP1SteppingAction::UserSteppingAction(const G4Step* aStep)
{

	G4String LogicalVolumeAtVertexName = aStep->GetTrack()->GetLogicalVolumeAtVertex()->GetName();

	if (fScoringVolumeVector.empty()) {
		const NP1DetectorConstruction* detectorConstruction
		= static_cast<const NP1DetectorConstruction*>
		(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
		fScoringVolumeVector = detectorConstruction->GetScoringVolumeVector();
	}

	// get volume of the current step
	G4LogicalVolume* currentVolume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

	G4bool IsScoringVolume = false;

	// check if we are in scoring volume
	for (unsigned i=0; i < fScoringVolumeVector.size(); i++) {
		if (currentVolume == fScoringVolumeVector[i]) IsScoringVolume = true;
	}

	if(!IsScoringVolume) return;

	G4int TrackID						= aStep->GetTrack()->GetTrackID();
	G4String ParticleName				= aStep->GetTrack()->GetDefinition()->GetParticleName();
	G4double TotalEnergyDeposit 		= aStep->GetTotalEnergyDeposit();
	G4double StepLength			 		= aStep->GetStepLength();
	G4double KineticEnergy				= aStep->GetPreStepPoint()->GetKineticEnergy();
	G4double mass						= currentVolume->GetMass();
	G4double TotalDose					= TotalEnergyDeposit/mass;
	G4double Position_r					= aStep->GetPreStepPoint()->GetPosition().r();
	G4double CopyNo						= aStep->GetTrack()->GetTouchableHandle()->GetVolume(0)->GetCopyNo();

	if(!(TotalEnergyDeposit>0. || TotalDose>0. || aStep->GetNumberOfSecondariesInCurrentStep()>0)) return;

	// analysis manager
	G4AnalysisManager* man = G4AnalysisManager::Instance();

	if(LogicalVolumeAtVertexName=="particle_log" || LogicalVolumeAtVertexName=="particleCoating_log"){

		if(TotalEnergyDeposit>0.){
			man->FillH1(0,Position_r,TotalEnergyDeposit);
			man->FillH1(1,CopyNo,TotalEnergyDeposit);
		}
		if(TotalDose>0.) {
			man->FillH1(2,Position_r,TotalDose);
			man->FillH1(3,CopyNo,TotalDose);
		}
		if(!fTrackingAction->GetSecondaryCountFlag()){
			if(KineticEnergy>0.) man->FillH1(8,KineticEnergy);
			fTrackingAction->SetSecondaryCountFlag(true);
		}

	}

	if(LogicalVolumeAtVertexName=="particle_log" || LogicalVolumeAtVertexName=="particleCoating_log"){
		if(TotalEnergyDeposit>0.) fEventAction->AddSecondariesEdep(TotalEnergyDeposit);

	}
	if(TotalEnergyDeposit>0.){
		fEventAction->AddTotalEdep(TotalEnergyDeposit);
		if(TrackID==1) fEventAction->AddTrackLength(StepLength);
	}


}

