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

#ifndef NP1Control_h
#define NP1Control_h 1

// NP1 Headers
#include "NP1ControlMessenger.hh"
#include "G4UserTrackingAction.hh"
#include "G4UserSteppingAction.hh"
#include "G4VSensitiveDetector.hh"

class NP1ControlMessenger ;

class NP1Control {

public:
    /// Static method returning an instance of Control.
    static NP1Control* GetInstance() ;
    /// Static method killing the instance.
    static void Kill() ;

    void Setup(int argc,char** argv);

    inline void SetShellsNo (const G4int aShellsNo) { fShellsNo=aShellsNo;};
    inline G4int GetShellsNo () { return fShellsNo;};

    inline void SetShellsFrame_rmin (G4double anRmin) { fShellsFrame_rmin=anRmin;};
    inline G4double GetShellsFrame_rmin () { return fShellsFrame_rmin;};

    inline void SetShellsFrame_rmax (G4double anRmax) { fShellsFrame_rmax=anRmax;};
    inline G4double GetShellsFrame_rmax () { return fShellsFrame_rmax;};

    inline void SetCSBiasFactor (G4double aFactor) { fCSBiasFactor=aFactor;};
    inline G4double GetCSBiasFactor () { return fCSBiasFactor;};


    inline G4UserTrackingAction* GetTrackingAction() const { return fTrackingAction;}

    inline void SetTrackingAction(G4UserTrackingAction* aTrackingAction){fTrackingAction=aTrackingAction;}

    inline G4UserSteppingAction* GetSteppingAction() const { return fSteppingAction;}

    inline void SetSteppingAction(G4UserSteppingAction* aSteppingAction){fSteppingAction=aSteppingAction;}

    inline G4UserSteppingAction* GetSensitiveDetectorAction() const{ return fSensitiveDetectorAction;}

    inline void SetSensitiveVolumeAction(G4UserSteppingAction* aSensitiveDetectorAction){fSensitiveDetectorAction=aSensitiveDetectorAction;}

private:
    // Constructor
	NP1Control();
	// Destructor
	virtual ~NP1Control();

    // Singleton control instance.
    static NP1Control*			instance ;

    NP1ControlMessenger*		fNP1ControlMessenger ;

    G4int						fShellsNo ;
    G4double					fShellsFrame_rmin ;
    G4double					fShellsFrame_rmax ;
    G4double					fCSBiasFactor ;

    G4UserTrackingAction*		fTrackingAction ;
    G4UserSteppingAction*    	fSteppingAction ;
    G4UserSteppingAction*    	fSensitiveDetectorAction ;

};

#endif // NP1Control_h
