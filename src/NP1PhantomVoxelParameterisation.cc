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
#include "Randomize.hh"

// NP1 Headers
#include "NP1PhantomVoxelParameterisation.hh"

NP1PhantomVoxelParameterisation::NP1PhantomVoxelParameterisation(G4int aVoxelsXNo, G4int aVoxelsYNo, G4int aVoxelsZNo, G4double aWaterVoxel_dx, vector<G4int> activeVoxels):G4VPVParameterisation(),
fVoxelsXNo(aVoxelsXNo),
fVoxelsYNo(aVoxelsYNo),
fVoxelsZNo(aVoxelsZNo),
fWaterVoxel_dx(aWaterVoxel_dx),
fActiveVoxels(activeVoxels)
{}

NP1PhantomVoxelParameterisation:: ~NP1PhantomVoxelParameterisation()
{}

void NP1PhantomVoxelParameterisation::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const
{

	G4int newCopyNo=fActiveVoxels[copyNo];

	G4double voxel_x=2*(newCopyNo%fVoxelsXNo)*fWaterVoxel_dx-(fVoxelsXNo-1)*fWaterVoxel_dx;
	G4double voxel_y=(2*(newCopyNo%(fVoxelsXNo*fVoxelsYNo)/fVoxelsXNo)-(fVoxelsYNo-1))*fWaterVoxel_dx;
	G4double voxel_z=(2*(newCopyNo/(fVoxelsXNo*fVoxelsYNo))-(fVoxelsZNo-1))*fWaterVoxel_dx;
	//G4cout<<copyNo<<"\t"<<voxel_x/fWaterVoxel_dx<<"\t"<<voxel_y/fWaterVoxel_dx<<"\t"<<voxel_z/fWaterVoxel_dx<<G4endl;
	physVol->SetTranslation(G4ThreeVector(voxel_x,voxel_y,voxel_z));

}
void NP1PhantomVoxelParameterisation::ComputeDimensions(G4Box& /*voxel*/, const G4int /*copyNo*/,const G4VPhysicalVolume* /*physVol*/) const
{

}
