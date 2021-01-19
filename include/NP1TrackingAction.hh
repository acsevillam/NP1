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

#ifndef NP1TrackingAction_h
#define NP1TrackingAction_h 1

// Geant4 Headers
#include "G4UserTrackingAction.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

class NP1EventAction;

using namespace std;

/// Tracking action class
///

class NP1TrackingAction : public G4UserTrackingAction{

public :

	NP1TrackingAction(NP1EventAction* eventAction);
	virtual ~NP1TrackingAction();

	// method from the base class
	virtual void PreUserTrackingAction (const G4Track* aTrack);
	virtual void PostUserTrackingAction (const G4Track* aTrack);

	inline void SetSecondaryCountFlag(G4bool aFlag){fSecondaryCountFlag=aFlag;}
	inline G4bool GetSecondaryCountFlag(){return fSecondaryCountFlag;}

private:
    NP1EventAction*  fEventAction;
    G4bool fSecondaryCountFlag;

};

#endif // NP1TrackingAction_h
