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

#ifndef NP1DetectorConstruction_h
#define NP1DetectorConstruction_h 1

// Geant4 Headers
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "NP1DetectorMessenger.hh"

using namespace std;

class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VBiasingOperator;

/// Detector construction class to define materials and geometry.

class NP1DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    NP1DetectorConstruction();
    virtual ~NP1DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    inline vector<G4LogicalVolume*> GetScoringVolumeVector() const { return fScoringVolumeVector; }
    void SetNanoParticleRadius(G4double aRadius);
    void SetNanoParticleCoatingThickness(G4double aThickness);
    void SetNanoParticleMaterial(G4String aMaterial);
    void SetNanoParticleCoatingMaterial(G4String aMaterial);
    void SetVoxelsXNo (G4int aXNo);
    inline G4int GetVoxelsXNo () { return fVoxelsXNo;};
    void SetVoxelsYNo (G4int aXNo);
    inline G4int GetVoxelsYNo () { return fVoxelsYNo;};
    void SetVoxelsZNo (G4int aXNo);
    inline G4int GetVoxelsZNo () { return fVoxelsZNo;};
    void SetWaterVoxel_dx (G4double aDx);
    inline G4double GetWaterVoxel_dx () { return fWaterVoxel_dx;};
    void SetWaterPhantom_dx (G4double aDx);
    inline G4double GetWaterPhantom_dx () { return fWaterPhantom_dx;};
    void SetConcentration0 (G4double aConcentration);
    inline G4double GetConcentration0 () { return fConcentration0;};
    void SetConcentration (G4double aConcentration);
    inline G4double GetConcentration () { return fConcentration;};
    inline void SetConcentrationTo0(){SetConcentration(fConcentration0);};


  private:
    void SetupGeometry(G4LogicalVolume* motherVolume);
	void SetupBiasing();
    vector<G4LogicalVolume*>  		fScoringVolumeVector;
	G4UImessenger* 	 				fNP1DetectorMessenger ;
	G4VPhysicalVolume* 				fParticle_phys;
	G4double						fParticleRadius;
	G4Material*						fParticleMaterial;
	G4VPhysicalVolume* 				fParticleCoating_phys;
	G4double						fParticleCoatingThickness;
	G4Material*						fParticleCoatingMaterial;
	G4VPhysicalVolume*				fShell_phys;
	G4VPhysicalVolume*				fShellsFrame_phys;
	G4double						fConcentration0;
	G4double						fConcentration;
	G4Material*						fBoxMaterial;
    G4int							fVoxelsXNo ;
    G4int							fVoxelsYNo ;
    G4int							fVoxelsZNo ;
    G4VPhysicalVolume* 				fWaterVoxel_phys;
    G4VPhysicalVolume* 				fWaterVoxelEmpty_phys;
    G4double						fWaterVoxel_dx ;
    G4VPhysicalVolume* 				fWaterBox_phys;
    G4double						fWaterPhantom_dx ;
    G4double						fWaterPhantom_dy ;
    G4double						fWaterPhantom_dz ;
    G4VPhysicalVolume* 				fWaterPhantom_phys;
	vector<G4int> 					fVoxelsID;

};

#endif
