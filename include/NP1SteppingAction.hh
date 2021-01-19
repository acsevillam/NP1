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

#ifndef NP1SteppingAction_h
#define NP1SteppingAction_h 1

// Geant4 Headers
#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "G4VPhysicalVolume.hh"

using namespace std;

class NP1EventAction;
class NP1TrackingAction;
class G4LogicalVolume;

/// Stepping action class
///

class NP1SteppingAction : public G4UserSteppingAction
{
  public:
    NP1SteppingAction(NP1EventAction* eventAction, NP1TrackingAction* trackingAction);
    virtual ~NP1SteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    NP1EventAction*  			fEventAction;
    NP1TrackingAction*  		fTrackingAction;
    vector<G4LogicalVolume*> 	fScoringVolumeVector;

};

#endif
