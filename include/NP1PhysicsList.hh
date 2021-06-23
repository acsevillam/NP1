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

#ifndef NP1PhysicsList_h
#define NP1PhysicsList_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class NP1PhysicsList : public G4VPhysicsConstructor
{
public: 
  NP1PhysicsList(const G4String& name = "standard");
 ~NP1PhysicsList();

public: 
 
 // This method is dummy for physics
 virtual void ConstructParticle() {};

  // This method will be invoked in the Construct() method.
  // each physics process will be instantiated and
  // registered to the process manager of each particle type 
  virtual void ConstructProcess();
};

#endif // NP1PhysicsList

