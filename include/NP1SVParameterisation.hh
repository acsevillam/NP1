/*
 * NanoParticle 1.0
 * Copyright (c) 2020 Universidad Nacional de Colombia
 * All Right Reserved.
 *
 * Developed by Ángela María Londoño Tobón
 *              Andrés Camilo Sevilla Moreno
 *
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

#ifndef NP1SVParameterisation_h
#define NP1SVParameterisation_h 1

// Geant4 Headers
#include "G4VPVParameterisation.hh"

class NP1SVParameterisation : public G4VPVParameterisation
{
public:
	NP1SVParameterisation(G4double fRMin, G4double fDr );
	~NP1SVParameterisation();
	void ComputeTransformation (const G4int copyNo,G4VPhysicalVolume* physVol) const;
	void ComputeDimensions (G4Sphere& shell, const G4int copyNo,const G4VPhysicalVolume* physVol) const;

private:
	G4double	fRMin;
	G4double	fDr;
};

#endif // NP1SVParameterisation_h
