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

#ifndef S1EventAction_h
#define S1EventAction_h 1

// Geant4 Headers
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "NP1Control.hh"
#include "G4THitsMap.hh"

class NP1RunAction;

using namespace std;

/// Event action class
///

class NP1EventAction : public G4UserEventAction
{
  public:
    NP1EventAction(NP1RunAction* runAction);
    virtual ~NP1EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddTotalEdep(G4double Edep) { fTotalEdep += Edep; }
    void AddPrimaryTrackLength(G4double TrackLenth) { fPrimaryTrackLength += TrackLenth; }
    void AddLetN(G4double LetN) { fLetN += LetN; }
    void AddLetD(G4double LetD) { fLetD += LetD; }
    void CountSecondary() { fNumberOfSecondaries ++; }

  private:
    NP1RunAction* 			fRunAction;
    G4double     			fTotalEdep;
    G4double				fPrimaryTrackLength;
    G4double     			fLetN;
    G4double     			fLetD;
    G4int     				fNumberOfSecondaries;

};

#endif

    
