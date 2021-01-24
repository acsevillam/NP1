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

#ifndef NP1PhysicsListFactory_h
#define NP1PhysicsListFactory_h 1

#include "G4VModularPhysicsList.hh"

class G4VPhysicsConstructor;
class G4StepLimiter;
class NP1PhysicsListMessenger;

class NP1PhysicsListFactory: public G4VModularPhysicsList
{
public:

  NP1PhysicsListFactory();
  virtual ~NP1PhysicsListFactory();

  void ConstructParticle();
    
  void SetCuts();
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);
        
  void AddPhysicsList(const G4String& name);
  void ConstructProcess();
    
  void AddStepMax();       
  G4StepLimiter* GetStepMaxProcess() {return fStepMaxProcess;};

private:

  G4double fCutForGamma;
  G4double fCutForElectron;
  G4double fCutForPositron;

  G4String               fEmName;
  G4VPhysicsConstructor* fEmPhysicsList;
  G4VPhysicsConstructor* fDecPhysicsList;
    
  G4StepLimiter* fStepMaxProcess;

  NP1PhysicsListMessenger* fMessenger;
};

#endif // NP1PhysicsListFactory_h

