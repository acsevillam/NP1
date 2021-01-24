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
#include "NP1ActionInitialization.hh"
#include "NP1DetectorConstruction.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "QGSP_BIC_HP.hh"
#include "Randomize.hh"
#include "G4GenericBiasingPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4ScoringManager.hh"

// NP1 Headers
#include "NP1Control.hh"
#include "NP1PhysicsListFactory.hh"

namespace {
void PrintUsage() {
	G4cerr << " Usage: " << G4endl;
	G4cerr << " ./NP1 [-m macro ] "
			<< " [-v visualization {'on','off'}]"
			<< " [-vm vis_macro ]"
			<< " [-b biasing {'on','off'}]"
			<< " [-n numberOfEvent ]"
			<< "\n or\n ./NP1 [macro.mac]"
			<< G4endl;
}
}

int main(int argc,char** argv)
{

	// Detect interactive mode (if no arguments) and define UI session
	//
	G4UIExecutive* ui = 0;

	// Evaluate arguments
	//
	if ( argc > 10 ) {
		PrintUsage();
		return 1;
	}

	G4String macro("");
	G4String vis_macro("");
	G4String onOffBiasing("");
	G4String onOffVisualization("");
	G4int numberOfEvent(0);

	if (argc == 1) {
		ui = new G4UIExecutive(argc, argv);
		onOffVisualization="on";
	}
	for ( G4int i=1; i<argc; i=i+2 )
	{
		if ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
		else if (G4String(argv[i]) == "-v" ) {
			onOffVisualization=argv[i+1];
			if(onOffVisualization=="on"){
				ui = new G4UIExecutive(argc, argv);
			}
		}
		else if ( G4String(argv[i]) == "-vm" ) {
			if(!ui) ui = new G4UIExecutive(argc, argv);
			if(G4String(argv[i]) == "-vm") vis_macro = argv[i+1];
			onOffVisualization="on";
		}
		else if ( G4String(argv[i]) == "-b" ) onOffBiasing	= argv[i+1];
		else if ( G4String(argv[i]) == "-n" ) numberOfEvent = G4UIcommand::ConvertToInt(argv[i+1]);
		else{
			PrintUsage();
			return 1;
		}
	}

	if(macro == "") macro = "mac/init.mac";
	if(vis_macro == "") vis_macro = "mac/vis1.mac";
	if(onOffBiasing == "") onOffBiasing = "off";
	if(onOffVisualization == "") onOffVisualization = "off";

	// Construct Control and ControlMessenger
	NP1Control::GetInstance();

	// Choose the Random engine
	//
	CLHEP::RanluxEngine defaultEngine( 1234567, 4 );
	G4Random::setTheEngine( &defaultEngine );
	G4int seed = time( NULL );
	G4Random::setTheSeed( seed );

	// Construct the default run manager
	//
#ifdef G4MULTITHREADED
	G4MTRunManager* runManager = new G4MTRunManager;
	runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
#else
	G4RunManager* runManager = new G4RunManager;
#endif

	// Activate UI-command base scorer
	G4ScoringManager * scoringManager = G4ScoringManager::GetScoringManager();
	scoringManager->SetVerboseLevel(1);

	// Set mandatory initialization classes
	//
	// Detector construction
	runManager->SetUserInitialization(new NP1DetectorConstruction());

	// Physics list
	//G4VModularPhysicsList* physicsList = new NP1PhysicsListFactory();
	G4VModularPhysicsList* physicsList = new QGSP_BIC_HP();
	physicsList->SetDefaultCutValue(1*mm);
	//physicsList->SetCutValue(4*um,"e-");
	//physicsList->SetCutValue(4*um,"e+");
	//physicsList->SetCutValue(4*um,"proton");
	//physicsList->SetCutValue(0.01*mm,"gamma");
	physicsList->DumpCutValuesTable();

	G4GenericBiasingPhysics* biasingPhysics = new G4GenericBiasingPhysics();
	if ( onOffBiasing == "on" )
	{
		//biasingPhysics->NonPhysicsBiasAllCharged();
		biasingPhysics->Bias("gamma");
		//biasingPhysics->NonPhysicsBias("gamma");
		physicsList->RegisterPhysics(biasingPhysics);
		G4cout << "      ********************************************************* " << G4endl;
		G4cout << "      ********** processes are wrapped for biasing ************ " << G4endl;
		G4cout << "      ********************************************************* " << G4endl;
	}
	else
	{

		NP1Control::GetInstance()->SetCSBiasFactor(1);

		G4cout << "      ************************************************* " << G4endl;
		G4cout << "      ********** processes are not wrapped ************ " << G4endl;
		G4cout << "      ************************************************* " << G4endl;
	}

	runManager->SetUserInitialization(physicsList);

	// User action initialization
	runManager->SetUserInitialization(new NP1ActionInitialization());

	// Initialize visualization
	//
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();

	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	// Process macro or start UI session
	//

	if ( macro != "" )
	{
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command+macro);
	}

	if (onOffVisualization=="on"){
		// interactive mode
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command+vis_macro);
		ui->SessionStart();
		delete ui;
	}else{
		if ( numberOfEvent >= 0 ) runManager->BeamOn(numberOfEvent);
	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted
	// in the main() program !

	delete visManager;
	delete runManager;
}
