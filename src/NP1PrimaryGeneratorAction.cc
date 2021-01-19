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
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Box.hh"

// NP1 Headers
#include "NP1PrimaryGeneratorAction.hh"

NP1PrimaryGeneratorAction::NP1PrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction()
{

	fNP1PrimaryGeneratorActionMessenger = new NP1PrimaryGeneratorActionMessenger(this) ;
	G4cout<<"01 - Primary Generator action have started !!!"<<G4endl;
	fParticleSource = new G4GeneralParticleSource();
}

NP1PrimaryGeneratorAction::~NP1PrimaryGeneratorAction()
{
  delete fParticleSource;
}

void NP1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  fParticleSource->GeneratePrimaryVertex(anEvent);

}

void NP1PrimaryGeneratorAction::SetSquareBeamTo(G4String aPhysicalVolumeName){

	G4VPhysicalVolume* volume_phys = G4PhysicalVolumeStore::GetInstance()->GetVolume(aPhysicalVolumeName);
	G4Box* volume_geo =(G4Box*) volume_phys->GetLogicalVolume()->GetSolid();

	fParticleSource->GetCurrentSource()->GetPosDist()->SetHalfX(volume_geo->GetXHalfLength());
	fParticleSource->GetCurrentSource()->GetPosDist()->SetHalfY(volume_geo->GetYHalfLength());
}

