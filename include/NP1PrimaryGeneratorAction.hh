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

#ifndef NP1PrimaryGeneratorAction_h
#define NP1PrimaryGeneratorAction_h 1

// Geant4 Headers
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"

// NP1 Headers
#include "NP1PrimaryGeneratorActionMessenger.hh"

class G4Event;
class G4Box;
class NP1PrimaryGeneratorActionMessenger;

class NP1PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    NP1PrimaryGeneratorAction();
    virtual ~NP1PrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
    inline const G4GeneralParticleSource* GetParticleSource() const {return fParticleSource; } ;

    void SetSquareBeamTo(G4String aPhysicalVolumeName);
  
  private:
    NP1PrimaryGeneratorActionMessenger*		fNP1PrimaryGeneratorActionMessenger ;
	G4GeneralParticleSource* 	fParticleSource;
};


#endif // NP1PrimaryGeneratorAction_h
