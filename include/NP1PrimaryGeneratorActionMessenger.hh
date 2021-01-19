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

#ifndef NP1PrimaryGeneratorActionMessenger_h
#define NP1PrimaryGeneratorActionMessenger_h 1

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
#include "NP1PrimaryGeneratorAction.hh"

using namespace std;

typedef map <G4String,G4UIdirectory*>         			UIDirectoryCollection ;
typedef map <G4String,G4UIcmdWithAString*>    			UIcmdWithAStringCollection ;
typedef map <G4String,G4UIcmdWithABool*>    			UIcmdWithABoolCollection ;
typedef map <G4String,G4UIcmdWithADoubleAndUnit*>    	UIcmdWithADoubleAndUnitCollection ;

class NP1PrimaryGeneratorAction ;
class G4UIdirectory ;
class G4UIcmdWithAString ;
class G4UIcmdWithABool ;
class G4UIcmdWithADoubleAndUnit ;

class NP1PrimaryGeneratorActionMessenger: public G4UImessenger {

public:

	NP1PrimaryGeneratorActionMessenger(NP1PrimaryGeneratorAction* aNP1PrimaryGeneratorAction );
	virtual ~NP1PrimaryGeneratorActionMessenger();

	void SetNewValue(G4UIcommand* command, G4String aValue) ;

private:

	NP1PrimaryGeneratorAction*				fNP1PrimaryGeneratorAction ;

	UIDirectoryCollection					fUIDirectoryCollection ;
	UIcmdWithAStringCollection				fUIcmdWithAStringCollection ;
	UIcmdWithABoolCollection				fUIcmdWithABoolCollection ;
	UIcmdWithADoubleAndUnitCollection		fUIcmdWithADoubleAndUnitCollection;


};

#endif // NP1PrimaryGeneratorActionMessenger_h
