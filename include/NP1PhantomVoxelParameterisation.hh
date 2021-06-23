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

#ifndef NP1PhantomVoxelParameterisation_h
#define NP1PhantomVoxelParameterisation_h 1

// C++ Headers
#include <vector>
#include <map>

// Geant4 Headers
#include "G4VPVParameterisation.hh"

class G4LogicalVolume;

using namespace std;

class NP1PhantomVoxelParameterisation : public G4VPVParameterisation
{
public:
	NP1PhantomVoxelParameterisation(G4int aVoxelsXNo, G4int aVoxelsYNo, G4int aVoxelsZNo, G4double aWaterVoxel_dx, vector<G4int> activeVoxels);
	~NP1PhantomVoxelParameterisation();
	void ComputeTransformation (const G4int copyNo,G4VPhysicalVolume* physVol) const;
	void ComputeDimensions (G4Box& voxel, const G4int copyNo,const G4VPhysicalVolume* physVol) const;

private:
	G4int 					fVoxelsXNo;
	G4int 					fVoxelsYNo;
	G4int 					fVoxelsZNo;
	G4double				fWaterVoxel_dx;
	vector<G4int> 			fActiveVoxels;
};

#endif // NP1PhantomVoxelParameterisation_h
