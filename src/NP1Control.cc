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
#include "G4UImanager.hh"

// NP1 Headers
#include "NP1Control.hh"
#include "G4SystemOfUnits.hh"

using namespace std ;

// Define Static Variables
NP1Control* NP1Control::instance = NULL;

NP1Control::NP1Control() {
	fNP1ControlMessenger = new NP1ControlMessenger(this) ;
	fShellsNo = 0;
	fShellsFrame_rmin = 0.;
	fShellsFrame_rmax = 0.;

	fTrackingAction = NULL ;
	fSteppingAction = NULL ;
	fSensitiveDetectorAction = NULL ;
}

NP1Control::~NP1Control()
{ }

NP1Control* NP1Control::GetInstance() {

	if (instance == NULL) instance =  new NP1Control() ;
	return instance ;

}

void NP1Control::Kill() {

	if(instance!=NULL){
		delete instance ;
		instance = NULL ;
	}
}

void NP1Control::Setup(int argc,char** argv){

	if (argc>2) {  // batch mode
		// command line contains name of the macro to execute
		G4String command = "/control/execute " ;
		G4String fileName = argv[1] ;
		G4UImanager::GetUIpointer()->ApplyCommand(command+fileName) ;
	}

}
