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

#include "NP1ControlMessenger.hh"

NP1ControlMessenger::NP1ControlMessenger(NP1Control* aNP1Control) {

	fNP1Control = aNP1Control ;

	fUIDirectoryCollection["NP1Control"] = new G4UIdirectory("/NP1/Control/") ;
	fUIDirectoryCollection["NP1Control"] -> SetGuidance("Control commands") ;

}

NP1ControlMessenger::~NP1ControlMessenger() {

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

void NP1ControlMessenger::SetNewValue(G4UIcommand* /*command*/,G4String /*aValue*/ ){

}

