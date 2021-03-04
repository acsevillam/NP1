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

#ifndef NP1RunAction_h
#define NP1RunAction_h 1

// Geant4 Headers
#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

// NP1 Headers
#include "NP1DetectorConstruction.hh"
#include "NP1Control.hh"

using namespace std;

/// Run action class
///

class NP1RunAction : public G4UserRunAction
{

  public:
    NP1RunAction();
    virtual ~NP1RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void CreateHistos();
    void CreateNTuples();

    inline void CountEdepEvent(){ fEDepEvents++; };
    inline void AddTotalEdep(G4double Edep) {fTotalEdep+=Edep; fTotalEdep2+=Edep*Edep;}
    inline void CountGoodEvent(G4double goodEvents){ fGoodEvents += goodEvents; };

private:
    G4Accumulable<G4int>    			fEDepEvents;
    G4Accumulable<G4double>    			fGoodEvents;
    G4Accumulable<G4double>     		fTotalEdep;
    G4Accumulable<G4double>	   			fTotalEdep2;

};

#endif // NP1RunAction_h

