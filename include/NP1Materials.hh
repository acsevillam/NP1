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


#ifndef NP1Materials_hh
#define NP1Materials_hh 1

// Geant4 Headers
#include "G4Material.hh"

using namespace std ;

typedef map <G4String,G4Material*> MaterialsCollection ;

class NP1Materials {

public:

	/// Static method returning an instance of Control.
	static NP1Materials* GetInstance() ;
	/// Static method killing the instance.
	static void Kill() ;
	G4Material* GetMaterial(const G4String materialName);
	G4Material* otherMaterials(const G4String materialName);

private:

	// Constructor
	NP1Materials();

	//Destructor
	virtual ~NP1Materials();

	// Singleton material instance.
	static NP1Materials* instance;

	/// Collection of materials.
	MaterialsCollection		fMaterialsCollection ;


};

#endif // NP1Materials_hh
