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

//NP1 Headers
#include "NP1RunAction.hh"
#include "NP1TrackingAction.hh"
#include "NP1ActionInitialization.hh"
#include "NP1PrimaryGeneratorAction.hh"
#include "NP1EventAction.hh"
#include "NP1SteppingAction.hh"
//#include "NP1SensitiveVolumeAction.hh"

NP1ActionInitialization::NP1ActionInitialization()
 : G4VUserActionInitialization()
{}

NP1ActionInitialization::~NP1ActionInitialization()
{}

void NP1ActionInitialization::BuildForMaster() const
{
  SetUserAction(new NP1RunAction());
}

void NP1ActionInitialization::Build() const
{

  SetUserAction(new NP1PrimaryGeneratorAction);

  NP1RunAction* runAction = new NP1RunAction();
  SetUserAction(runAction);

  NP1EventAction* eventAction = new NP1EventAction(runAction);
  SetUserAction(eventAction);

  NP1Control* theControl = NP1Control::GetInstance();

  NP1TrackingAction* trackingAction = new NP1TrackingAction(eventAction);
  SetUserAction(trackingAction);
  theControl->SetTrackingAction(trackingAction);

  NP1SteppingAction* steppingAction = new NP1SteppingAction(eventAction,trackingAction);
  SetUserAction(steppingAction);
  theControl->SetSteppingAction(steppingAction);

  /*NP1SensitiveVolumeAction* sensitiveVolumeAction = new NP1SensitiveVolumeAction(eventAction,trackingAction);
  SetUserAction(sensitiveVolumeAction);
  theControl->SetSensitiveVolumeAction(sensitiveVolumeAction);*/
}
