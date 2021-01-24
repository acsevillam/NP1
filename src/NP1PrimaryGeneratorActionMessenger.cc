/*
 * NanoParticle 1.1
 * Copyright (c) 2020
 * Universidad Nacional de Colombia
 * Servicio Geológico Colombiano
 * All Right Reserved.
 *
 * Developed by Ángela Londoño Tobón
 *              Andrés Camilo Sevilla Moreno
 *
 * Use and copying of these libraries and preparation of derivative works
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 *
 * Bogotá, Colombia.
 *
 */

#include "NP1PrimaryGeneratorActionMessenger.hh"

NP1PrimaryGeneratorActionMessenger::NP1PrimaryGeneratorActionMessenger(NP1PrimaryGeneratorAction* aNP1PrimaryGeneratorAction) {

	fNP1PrimaryGeneratorAction = aNP1PrimaryGeneratorAction ;

	fUIDirectoryCollection["NP1GPS"] = new G4UIdirectory("/NP1/source/") ;
	fUIDirectoryCollection["NP1GPS"] -> SetGuidance("General particle source commands") ;

	fUIcmdWithAStringCollection["SetSquareBeamTo"] = new G4UIcmdWithAString("/NP1/source/SetSquareBeamTo",this) ;
	fUIcmdWithAStringCollection["SetSquareBeamTo"] -> SetGuidance("Set square beam cross section to volume");
	fUIcmdWithAStringCollection["SetSquareBeamTo"] -> SetParameterName("Volume",false);
	fUIcmdWithAStringCollection["SetSquareBeamTo"] -> AvailableForStates(G4State_PreInit, G4State_Idle);

}

NP1PrimaryGeneratorActionMessenger::~NP1PrimaryGeneratorActionMessenger() {

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

}

void NP1PrimaryGeneratorActionMessenger::SetNewValue(G4UIcommand* command,G4String aValue ){

	if( command == fUIcmdWithAStringCollection["SetSquareBeamTo"] ) { fNP1PrimaryGeneratorAction->SetSquareBeamTo(aValue);}


}

