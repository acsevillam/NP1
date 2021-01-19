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

// Geant4 Headers
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Sphere.hh"

// NP1 Headers
#include "NP1SVParameterisation.hh"

NP1SVParameterisation::NP1SVParameterisation(G4double aRmin, G4double aDr ):G4VPVParameterisation(),
fRMin(aRmin),
fDr(aDr)
{}

NP1SVParameterisation:: ~NP1SVParameterisation()
{}

void NP1SVParameterisation::ComputeTransformation(const G4int /*copyNo*/, G4VPhysicalVolume* /*physVol*/) const
{}
void NP1SVParameterisation::ComputeDimensions(G4Sphere& shell, const G4int copyNo,const G4VPhysicalVolume* /*physVol*/) const
{
	G4double rmin= fRMin + copyNo * fDr;
	G4double rmax= fRMin + (copyNo+1) * fDr;
	shell.SetInnerRadius(rmin);
	shell.SetOuterRadius(rmax);
}
