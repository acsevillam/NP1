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

#ifndef NP1ControlMessenger_h
#define NP1ControlMessenger_h 1

// C++ Headers
#include <vector>
#include <map>

// Geant4 Headers
#include "G4UImessenger.hh"
#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

// NP1 Headers
#include "NP1Control.hh"

using namespace std;

typedef map <G4String,G4UIdirectory*>         			UIDirectoryCollection ;
typedef map <G4String,G4UIcmdWithAString*>    			UIcmdWithAStringCollection ;
typedef map <G4String,G4UIcmdWithABool*>    			UIcmdWithABoolCollection ;
typedef map <G4String,G4UIcmdWithADoubleAndUnit*>    	UIcmdWithADoubleAndUnitCollection ;

class NP1Control ;
class G4UIdirectory ;
class G4UIcmdWithAString ;
class G4UIcmdWithABool ;
class G4UIcmdWithADoubleAndUnit ;

class NP1ControlMessenger: public G4UImessenger {

public:

	NP1ControlMessenger(NP1Control* aNP1Control );
	virtual ~NP1ControlMessenger();

	void SetNewValue(G4UIcommand* command, G4String aValue) ;

private:

	NP1Control*								fNP1Control ;

	UIDirectoryCollection					fUIDirectoryCollection ;
	UIcmdWithAStringCollection				fUIcmdWithAStringCollection ;
	UIcmdWithABoolCollection				fUIcmdWithABoolCollection ;
	UIcmdWithADoubleAndUnitCollection		fUIcmdWithADoubleAndUnitCollection;


};

#endif // NP1ControlMessenger_h
