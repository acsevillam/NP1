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

#ifndef NP1ActionInitialization_h
#define NP1ActionInitialization_h 1

// Geant4 Headers
#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class NP1ActionInitialization : public G4VUserActionInitialization
{
  public:
    NP1ActionInitialization();
    virtual ~NP1ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif

    
