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

    void CountGoodEvent(){ fGoodEvents++; };
    void CountEdepEvent(){ fEDepEvents++; };
    inline void AddTotalEdep(G4double Edep) {fTotalEdep+=Edep; fTotalEdep2+=Edep*Edep;}
    inline void SetTotalEdepLimits(G4double EdepMin,G4double EdepMax){fTotalEdepMin=EdepMin; fTotalEdepMax=EdepMax;}
    inline void AddSecondariesEdep(G4double Edep) {fSecondariesEdep+=Edep; fSecondariesEdep2+=Edep*Edep;}
    inline void SetSecondariesEdepLimits(G4double EdepMin,G4double EdepMax){fSecondariesEdepMin=EdepMin; fSecondariesEdepMax=EdepMax;}
    inline void CountSecondaries(G4double numberOfSecondaries) { fNumberOfSecondaries += numberOfSecondaries; fNumberOfSecondaries2 += numberOfSecondaries*numberOfSecondaries;  };
    inline void SetSecondariesNoLimits(G4double NumberOfSecondariesMin,G4double NumberOfSecondariesMax){fNumberOfSecondariesMin=NumberOfSecondariesMin; fNumberOfSecondariesMax=NumberOfSecondariesMax;}
    inline void AddTrackLength(G4double trackLength) {fTrackLength+=trackLength; fTrackLength2+=trackLength*trackLength;}
    inline void SetTrackLengthLimits(G4double trackLengthMin,G4double trackLengthMax){fTrackLengthMin=trackLengthMin; fTrackLengthMax=trackLengthMax;}

private:
    G4Accumulable<G4int>    			fEDepEvents;
    G4Accumulable<G4int>    			fGoodEvents;
    G4Accumulable<G4double>     		fTotalEdep;
    G4Accumulable<G4double>	   			fTotalEdep2;
    G4double							fTotalEdepMin;
    G4double							fTotalEdepMax;
    G4Accumulable<G4double>     		fSecondariesEdep;
    G4Accumulable<G4double>	   			fSecondariesEdep2;
    G4double							fSecondariesEdepMin;
    G4double							fSecondariesEdepMax;
    G4Accumulable<G4int> 				fNumberOfSecondaries;
    G4Accumulable<G4int>				fNumberOfSecondaries2;
    G4double							fNumberOfSecondariesMin;
    G4double							fNumberOfSecondariesMax;
    G4Accumulable<G4double>				fTrackLength;
    G4Accumulable<G4double>				fTrackLength2;
    G4double							fTrackLengthMin;
    G4double							fTrackLengthMax;

};

#endif // NP1RunAction_h

