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

// NP1 Headers
#include "NP1DetectorMessenger.hh"

NP1DetectorMessenger::NP1DetectorMessenger(NP1DetectorConstruction* aNP1DetectorConstruction){

	fNP1DetectorConstruction = aNP1DetectorConstruction ;

	fUIDirectoryCollection["NP1Geometry"] = new G4UIdirectory("/NP1/geometry/") ;
	fUIDirectoryCollection["NP1Geometry"] -> SetGuidance("Geometry construction commands") ;

	fUIcmdWithADoubleAndUnitCollection["SetNanoParticleRadius"] = new G4UIcmdWithADoubleAndUnit("/NP1/geometry/SetNanoParticleRadius",this) ;
	fUIcmdWithADoubleAndUnitCollection["SetNanoParticleRadius"] -> SetGuidance("Set the nano-particle radius");
	fUIcmdWithADoubleAndUnitCollection["SetNanoParticleRadius"] -> SetParameterName("Radius",false);
	fUIcmdWithADoubleAndUnitCollection["SetNanoParticleRadius"] -> SetUnitCategory("Length");
	fUIcmdWithADoubleAndUnitCollection["SetNanoParticleRadius"] -> AvailableForStates(G4State_PreInit, G4State_Idle);

	fUIcmdWithADoubleAndUnitCollection["SetNanoParticleCoatingThickness"] = new G4UIcmdWithADoubleAndUnit("/NP1/geometry/SetNanoParticleCoatingThickness",this) ;
	fUIcmdWithADoubleAndUnitCollection["SetNanoParticleCoatingThickness"] -> SetGuidance("Set the nano-particle coating thickness");
	fUIcmdWithADoubleAndUnitCollection["SetNanoParticleCoatingThickness"] -> SetParameterName("Thickness",false);
	fUIcmdWithADoubleAndUnitCollection["SetNanoParticleCoatingThickness"] -> SetUnitCategory("Length");
	fUIcmdWithADoubleAndUnitCollection["SetNanoParticleCoatingThickness"] -> AvailableForStates(G4State_PreInit, G4State_Idle);

	fUIcmdWithAStringCollection["SetNanoParticleMaterial"] = new G4UIcmdWithAString("/NP1/geometry/SetNanoParticleMaterial",this) ;
	fUIcmdWithAStringCollection["SetNanoParticleMaterial"] -> SetGuidance("Set the nano-particle material");
	fUIcmdWithAStringCollection["SetNanoParticleMaterial"] -> SetParameterName("Material",false);
	fUIcmdWithAStringCollection["SetNanoParticleMaterial"] -> SetCandidates("G4_Au G4_Ag G4_Pt G4_Pb G4_Gd Ta2O5 Bi2S3");
	fUIcmdWithAStringCollection["SetNanoParticleMaterial"] -> AvailableForStates(G4State_PreInit, G4State_Idle);

	fUIcmdWithAStringCollection["SetNanoParticleCoatingMaterial"] = new G4UIcmdWithAString("/NP1/geometry/SetNanoParticleCoatingMaterial",this) ;
	fUIcmdWithAStringCollection["SetNanoParticleCoatingMaterial"] -> SetGuidance("Set the nano-particle coating material");
	fUIcmdWithAStringCollection["SetNanoParticleCoatingMaterial"] -> SetParameterName("Material",false);
	fUIcmdWithAStringCollection["SetNanoParticleCoatingMaterial"] -> SetCandidates("G4_Au G4_Ag G4_Gd Dextran_70 PEG_3500 TiO2");
	fUIcmdWithAStringCollection["SetNanoParticleCoatingMaterial"] -> AvailableForStates(G4State_PreInit, G4State_Idle);

	fUIcmdWithADoubleAndUnitCollection["SetWaterVoxelSideLength"] = new G4UIcmdWithADoubleAndUnit("/NP1/geometry/SetWaterVoxelSideLength",this) ;
	fUIcmdWithADoubleAndUnitCollection["SetWaterVoxelSideLength"] -> SetGuidance("Set the water cube voxel side length");
	fUIcmdWithADoubleAndUnitCollection["SetWaterVoxelSideLength"] -> SetParameterName("Length",false);
	fUIcmdWithADoubleAndUnitCollection["SetWaterVoxelSideLength"] -> SetUnitCategory("Length");
	fUIcmdWithADoubleAndUnitCollection["SetWaterVoxelSideLength"] -> AvailableForStates(G4State_PreInit, G4State_Idle);

	fUIcmdWithAIntCollection["SetVoxelsXNo"] = new G4UIcmdWithAnInteger("/NP1/geometry/SetVoxelsXNo",this) ;
	fUIcmdWithAIntCollection["SetVoxelsXNo"] -> SetGuidance("Set number of voxels on the X axis");
	fUIcmdWithAIntCollection["SetVoxelsXNo"] -> SetParameterName("XNo",false);
	fUIcmdWithAIntCollection["SetVoxelsXNo"] -> AvailableForStates(G4State_PreInit, G4State_Idle);

	fUIcmdWithAIntCollection["SetVoxelsYNo"] = new G4UIcmdWithAnInteger("/NP1/geometry/SetVoxelsYNo",this) ;
	fUIcmdWithAIntCollection["SetVoxelsYNo"] -> SetGuidance("Set number of voxels on the Y axis");
	fUIcmdWithAIntCollection["SetVoxelsYNo"] -> SetParameterName("YNo",false);
	fUIcmdWithAIntCollection["SetVoxelsYNo"] -> AvailableForStates(G4State_PreInit, G4State_Idle);

	fUIcmdWithAIntCollection["SetVoxelsZNo"] = new G4UIcmdWithAnInteger("/NP1/geometry/SetVoxelsZNo",this) ;
	fUIcmdWithAIntCollection["SetVoxelsZNo"] -> SetGuidance("Set number of voxels on the Z axis");
	fUIcmdWithAIntCollection["SetVoxelsZNo"] -> SetParameterName("ZNo",false);
	fUIcmdWithAIntCollection["SetVoxelsZNo"] -> AvailableForStates(G4State_PreInit, G4State_Idle);

	fUIcmdWithADoubleAndUnitCollection["SetConcentration0"] = new G4UIcmdWithADoubleAndUnit("/NP1/geometry/SetConcentration0",this) ;
	fUIcmdWithADoubleAndUnitCollection["SetConcentration0"] -> SetGuidance("Set reference particles concentration in the medium");
	fUIcmdWithADoubleAndUnitCollection["SetConcentration0"] -> SetParameterName("Concentration",false);
	fUIcmdWithADoubleAndUnitCollection["SetConcentration0"] -> SetUnitCategory("Concentration");
	fUIcmdWithADoubleAndUnitCollection["SetConcentration0"] -> AvailableForStates(G4State_PreInit, G4State_Idle);

	fUIcmdWithADoubleAndUnitCollection["SetConcentration"] = new G4UIcmdWithADoubleAndUnit("/NP1/geometry/SetConcentration",this) ;
	fUIcmdWithADoubleAndUnitCollection["SetConcentration"] -> SetGuidance("Set particles concentration in the medium");
	fUIcmdWithADoubleAndUnitCollection["SetConcentration"] -> SetParameterName("Concentration",false);
	fUIcmdWithADoubleAndUnitCollection["SetConcentration"] -> SetUnitCategory("Concentration");
	fUIcmdWithADoubleAndUnitCollection["SetConcentration"] -> AvailableForStates(G4State_PreInit, G4State_Idle);

	fUIcmdWithoutParameter["SetConcentrationTo0"] = new G4UIcmdWithoutParameter("/NP1/geometry/SetConcentrationTo0",this) ;

}


NP1DetectorMessenger::~NP1DetectorMessenger(){

	// UI Directory Collection
	for(size_t i = 0; i < fUIDirectoryCollection.size(); i++){
		delete fUIDirectoryCollection[i] ;
	}

	// UI cmd With a String Collection
	for(size_t i = 0; i < fUIcmdWithAStringCollection.size(); i++){
		delete fUIcmdWithAStringCollection[i] ;
	}

	// UI cmd With a Bool Collection
	for(size_t i = 0; i < fUIcmdWithABoolCollection.size(); i++){
		delete fUIcmdWithABoolCollection[i] ;
	}

	// UI cmd With a Double and Unit Collection
	for(size_t i = 0; i < fUIcmdWithADoubleAndUnitCollection.size(); i++){
		delete fUIcmdWithADoubleAndUnitCollection[i] ;
	}

	// UI cmd With a Int Collection
	for(size_t i = 0; i < fUIcmdWithAIntCollection.size(); i++){
		delete fUIcmdWithAIntCollection[i] ;
	}
}

void NP1DetectorMessenger::SetNewValue(G4UIcommand* command,G4String aValue ){

	if( command == fUIcmdWithADoubleAndUnitCollection["SetNanoParticleRadius"] ) { fNP1DetectorConstruction->SetNanoParticleRadius(fUIcmdWithADoubleAndUnitCollection["SetNanoParticleRadius"]->GetNewDoubleValue(aValue)); }
	if( command == fUIcmdWithADoubleAndUnitCollection["SetNanoParticleCoatingThickness"] ) { fNP1DetectorConstruction->SetNanoParticleCoatingThickness(fUIcmdWithADoubleAndUnitCollection["SetNanoParticleCoatingThickness"]->GetNewDoubleValue(aValue)); }
	if( command == fUIcmdWithAStringCollection["SetNanoParticleMaterial"] ) { fNP1DetectorConstruction->SetNanoParticleMaterial(aValue);}
	if( command == fUIcmdWithAStringCollection["SetNanoParticleCoatingMaterial"] ) { fNP1DetectorConstruction->SetNanoParticleCoatingMaterial(aValue); }
	if( command == fUIcmdWithADoubleAndUnitCollection["SetWaterVoxelSideLength"] ) { fNP1DetectorConstruction->SetWaterVoxel_dx(fUIcmdWithADoubleAndUnitCollection["SetWaterVoxelSideLength"]->GetNewDoubleValue(aValue)); }
	if( command == fUIcmdWithAIntCollection["SetVoxelsXNo"] ) { fNP1DetectorConstruction->SetVoxelsXNo(fUIcmdWithAIntCollection["SetVoxelsXNo"]->GetNewIntValue(aValue)); }
	if( command == fUIcmdWithAIntCollection["SetVoxelsYNo"] ) { fNP1DetectorConstruction->SetVoxelsYNo(fUIcmdWithAIntCollection["SetVoxelsYNo"]->GetNewIntValue(aValue)); }
	if( command == fUIcmdWithAIntCollection["SetVoxelsZNo"] ) { fNP1DetectorConstruction->SetVoxelsZNo(fUIcmdWithAIntCollection["SetVoxelsZNo"]->GetNewIntValue(aValue)); }
	if( command == fUIcmdWithADoubleAndUnitCollection["SetConcentration0"] ) { fNP1DetectorConstruction->SetConcentration0(fUIcmdWithADoubleAndUnitCollection["SetConcentration0"]->GetNewDoubleValue(aValue)); }
	if( command == fUIcmdWithADoubleAndUnitCollection["SetConcentration"] ) { fNP1DetectorConstruction->SetConcentration(fUIcmdWithADoubleAndUnitCollection["SetConcentration"]->GetNewDoubleValue(aValue)); }
	if( command == fUIcmdWithoutParameter["SetConcentrationTo0"] ) { fNP1DetectorConstruction->SetConcentrationTo0(); }

}

