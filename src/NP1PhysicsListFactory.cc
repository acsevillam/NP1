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

// Geant4 Headers

#include "G4SystemOfUnits.hh"
#include "G4StepLimiter.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmDNAPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4LossTableManager.hh"
#include "G4ProcessManager.hh"
#include "G4RegionStore.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

// NP1 Headers
#include "NP1PhysicsListFactory.hh"
#include "NP1PhysicsListMessenger.hh"
#include "NP1PhysicsList.hh"

NP1PhysicsListFactory::NP1PhysicsListFactory() : G4VModularPhysicsList()
{
	//G4LossTableManager::Instance();

	// Production cuts range
	defaultCutValue  = 1*um;
	fCutForGamma     = defaultCutValue;
	fCutForElectron  = defaultCutValue;
	fCutForPositron  = defaultCutValue;

	fStepMaxProcess= NULL;

	fMessenger = new NP1PhysicsListMessenger(this);

	SetVerboseLevel(1);

	// EM physics
	fEmName = G4String("custom");

	fEmPhysicsList = new NP1PhysicsList("custom");

	// Decay physics and all particles
	fDecPhysicsList = new G4DecayPhysics();

}


NP1PhysicsListFactory::~NP1PhysicsListFactory()
{
	delete fMessenger;
	delete fEmPhysicsList;
	delete fDecPhysicsList;
}

void NP1PhysicsListFactory::ConstructParticle()
{
	fDecPhysicsList->ConstructParticle();
}

void NP1PhysicsListFactory::ConstructProcess()
{
	// transportation
	AddTransportation();

	// electromagnetic physics list
	fEmPhysicsList->ConstructProcess();

	// decay physics list
	fDecPhysicsList->ConstructProcess();

	// step limitation (as a full process)
	AddStepMax();

}

void NP1PhysicsListFactory::AddPhysicsList(const G4String& name)
{
	if (verboseLevel>1) {
		G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
	}

	if (name == fEmName) return;

	if (name == "custom") {

		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new NP1PhysicsList("custom");

	} else if (name == "emstandard_opt0") {

		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics(1);

	} else if (name == "emstandard_opt1") {

		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics_option1();

	} else if (name == "emstandard_opt2") {

		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics_option2();

	} else if (name == "emstandard_opt3") {

		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics_option3();

	} else if (name == "emstandard_opt4") {

		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics_option4();

	} else if (name == "emlivermore") {

		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmLivermorePhysics();

	} else if (name == "empenelope") {

		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmPenelopePhysics();

	}
}

void NP1PhysicsListFactory::AddStepMax()
{
	// Step limitation seen as a process

	fStepMaxProcess = new G4StepLimiter();

	auto particleIterator=GetParticleIterator();
	particleIterator->reset();
	while ((*particleIterator)()){
		G4ParticleDefinition* particle = particleIterator->value();
		G4ProcessManager* pmanager = particle->GetProcessManager();

		if (fStepMaxProcess->IsApplicable(*particle) && pmanager)
		{
			pmanager ->AddDiscreteProcess(fStepMaxProcess);
		}
	}
}

void NP1PhysicsListFactory::SetCuts()
{

	if (verboseLevel >0){
		G4cout << "PhysicsList::SetCuts:";
		G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
	}

	SetCutValue(fCutForGamma, "gamma");
	SetCutValue(fCutForElectron, "e-");
	SetCutValue(fCutForPositron, "e+");

	if (verboseLevel>0) DumpCutValuesTable();

}

void NP1PhysicsListFactory::SetCutForGamma(G4double cut)
{
	fCutForGamma = cut;
	SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

void NP1PhysicsListFactory::SetCutForElectron(G4double cut)
{
	fCutForElectron = cut;
	SetParticleCuts(fCutForElectron, G4Electron::Electron());
}

void NP1PhysicsListFactory::SetCutForPositron(G4double cut)
{
	fCutForPositron = cut;
	SetParticleCuts(fCutForPositron, G4Positron::Positron());
}

