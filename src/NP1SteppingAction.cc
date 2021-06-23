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
#include "G4EmCalculator.hh"

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

	// Track
	G4Track * theTrack = aStep -> GetTrack();
	// Track ID
	G4int trackID = aStep -> GetTrack() -> GetTrackID();
	// Particle definition
	G4ParticleDefinition* particleDef = theTrack->GetDefinition();
	// Particle name
	G4String ParticleName					= particleDef->GetParticleName();
	// Atomic Number
	G4int Z								    = particleDef-> GetAtomicNumber();
	// Atomic Mass
	G4int A									= particleDef-> GetAtomicMass();
    // Get particle PDG code
    G4int pdg = particleDef ->GetPDGEncoding();
	// Mass
	G4double mass							= currentVolume->GetMass();
	// Total energy deposit
	G4double TotalEnergyDeposit 	    	= aStep->GetTotalEnergyDeposit()*aStep->GetTrack()->GetWeight();
	// Total dose
	G4double TotalDose						= TotalEnergyDeposit/mass;
	// Pre-step kinetic energy
	G4double kinEPre = aStep -> GetPreStepPoint() -> GetKineticEnergy();
	// Post-step kinetic energy
	G4double kinEPost = aStep -> GetPostStepPoint() -> GetKineticEnergy();
	// step Length
	G4double stepLength = aStep -> GetStepLength();
	// Water sphere array
	G4double waterSphereArray_rmin 			= NP1Control::GetInstance()->GetShellsFrame_rmin();
	// Position radial
	G4double Position_r						= aStep->GetPreStepPoint()->GetPosition().r()-waterSphereArray_rmin;
	// Copy number
	G4double CopyNo							= aStep->GetTrack()->GetTouchableHandle()->GetVolume(0)->GetCopyNo();

	if(!(TotalEnergyDeposit>0. || TotalDose>0. || aStep->GetNumberOfSecondariesInCurrentStep()>0)) return;

	if ( !(Z==0 && A==1) ){ // All but not neutrons

		if (pdg !=22 && pdg !=11) // not gamma and electrons
		{

			// Step average kinetic energy
			G4double kinEMean = (kinEPre + kinEPost) * 0.5;
			// Material
			G4Material * mat = aStep -> GetPreStepPoint() -> GetMaterial();

			// Get the secondary particles in current step
			G4Step fstep = *theTrack -> GetStep();

			// Store all the secondary particles in current step
			const std::vector<const G4Track*> * secondary = fstep.GetSecondaryInCurrentStep();

			size_t secondariesSize = (*secondary).size();
			G4double secondariesEDep = 0.;

			// Get secondary electrons energy deposited
			if (secondariesSize)
			{
				for (size_t numsec = 0; numsec< secondariesSize ; numsec ++)
				{
					G4int secondaryPDGCode=(*secondary)[numsec]->GetDefinition()->GetPDGEncoding();

					if(secondaryPDGCode == 11)
					{
						// Calculate the energy deposit of secondary electrons in current step
						secondariesEDep += (*secondary)[numsec]->GetKineticEnergy();
					}
				}

			}

			// ICRU stopping power calculation
			G4EmCalculator emCal;

			// use the mean kinetic energy of ions in a step to calculate ICRU stopping power
			G4double Lsn = emCal.ComputeElectronicDEDX(kinEMean, particleDef, mat);

			fEventAction->AddLetN((secondariesEDep+TotalEnergyDeposit) * Lsn);
			fEventAction->AddLetD(secondariesEDep+TotalEnergyDeposit);

		}
	}

	// analysis manager
	G4AnalysisManager* man = G4AnalysisManager::Instance();

	if(LogicalVolumeAtVertexName=="particle_log" || LogicalVolumeAtVertexName=="particleCoating_log"){

		if(!fTrackingAction->GetSecondaryCountFlag()){
			if(kinEPre>0.) man->FillH1(10,kinEPre);
			fTrackingAction->SetSecondaryCountFlag(true);
		}

	}

	fEventAction->AddTotalEdep(TotalEnergyDeposit);

	if(trackID==1){
		fEventAction->AddPrimaryTrackLength(stepLength);
	}

	if(TotalEnergyDeposit>0. && (currentVolume->GetName()=="shell_log" )){
		man->FillH1(0,Position_r,TotalEnergyDeposit/keV);
		man->FillH1(1,CopyNo,TotalEnergyDeposit/keV);
	}

	if(TotalDose>0. && currentVolume->GetName()=="shell_log") {
		man->FillH1(2,Position_r,TotalDose);
		man->FillH1(3,CopyNo,TotalDose);
	}



}

