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
#include "G4UnitsTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4Material.hh"
#include "G4PVParameterised.hh"
#include "G4VPVParameterisation.hh"
#include "G4UserLimits.hh"

// NP1 Headers
#include "NP1DetectorConstruction.hh"
#include "NP1Materials.hh"
#include "NP1Control.hh"
#include "NP1SVParameterisation.hh"
#include "NP1PhantomVoxelParameterisation.hh"

NP1DetectorConstruction::NP1DetectorConstruction()
: G4VUserDetectorConstruction()
{

	const G4double ug = 1.e-6*g;
	const G4double ppm = ug/g;
	new G4UnitDefinition("microgram", "ug" , "Mass", ug);
	new G4UnitDefinition("ppm", "ppm","Concentration", ppm);
	new G4UnitDefinition("miligram/gram", "mg/g","Concentration", mg/g);
	new G4UnitDefinition("microgram/gram", "ug/g","Concentration", ug/g);

	fNP1DetectorMessenger = new NP1DetectorMessenger(this) ;

	fParticleMaterial = NP1Materials::GetInstance()->GetMaterial("G4_Gd");
	fParticleCoatingMaterial = NP1Materials::GetInstance()->GetMaterial("G4_Gd");
	fBoxMaterial = NP1Materials::GetInstance()->GetMaterial("G4_WATER");

	NP1Control::GetInstance()->SetShellsNo(100);
	fParticle_phys=0;
	fParticleCoating_phys=0;
	fParticleCoatingThickness=0*nm;
	fParticleRadius = 5*nm;
	NP1Control::GetInstance()->SetShellsFrame_rmin(fParticleRadius);
	fWaterVoxel_phys=0;
	fWaterVoxelEmpty_phys=0;
	fWaterBox_phys=0;
	fWaterPhantom_phys=0;
	fShell_phys=0;
	fShellsFrame_phys=0;
	fConcentration0=500*mg/g;
	fConcentration=500*mg/g;
	fWaterVoxel_dx= pow(fParticleMaterial->GetDensity()/fBoxMaterial->GetDensity()*4./3.*M_PI*pow(fParticleRadius,3)/fConcentration0,1/3.);
	NP1Control::GetInstance()->SetShellsFrame_rmax(fWaterVoxel_dx);
	fWaterPhantom_dx=1*mm;
	fVoxelsXNo=1;
	fVoxelsYNo=1;
	fVoxelsZNo=1;

}

NP1DetectorConstruction::~NP1DetectorConstruction()
{}

G4VPhysicalVolume* NP1DetectorConstruction::Construct()
{  

	// Cleanup old geometry

	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	// General Attributes
	G4VisAttributes* simpleInvisibleSVisAtt;
	simpleInvisibleSVisAtt= new G4VisAttributes(G4Colour(0.,0.0,0.5,0.025));
	simpleInvisibleSVisAtt->SetVisibility(true);

	//Elements
	G4Material* vacuum = NP1Materials::GetInstance()->GetMaterial("G4_Galactic");

	// Geometrical Volume =========================================================================================

	// World
	G4double world_size_X = (100/2.)*mm;
	G4VSolid* world_geo = new G4Box("world_geo", world_size_X, world_size_X, world_size_X);

	//Logical Volume ==============================================================================================

	// World
	G4LogicalVolume* world_log = new G4LogicalVolume(world_geo,vacuum,"world_log");
	world_log -> SetVisAttributes(simpleInvisibleSVisAtt);

	//Physics Volume  =============================================================================================

	G4VPhysicalVolume* world_phys =
			new G4PVPlacement(0,                    //no rotation
					G4ThreeVector(),       			//at (0,0,0)
					world_log,      				//its logical volume
					"world_phys",   				//its name
					0,                     			//its mother  volume
					false,                 			//no boolean operation
					0,                     			//copy number
					0);      			    		//overlaps checking

	SetupGeometry(world_log);

	//always return the physical World
	//
	return world_phys;

}

void NP1DetectorConstruction::SetupGeometry(G4LogicalVolume* motherVolume){


	// Geometrical Volume =========================================================================================

	// Nano-particle coating
	G4double particleCoating_rmax = fParticleRadius;
	G4VSolid* particleCoating_geo = new G4Orb("particleCoating_geo", particleCoating_rmax);

	// Nano-particle
	G4double particle_rmax = fParticleRadius-fParticleCoatingThickness;
	G4VSolid* particle_geo = new G4Orb("particle_geo", particle_rmax);

	// Shells frame
	G4double shellsFrame_rmin = particleCoating_rmax;
	G4double shellsFrame_rmax = NP1Control::GetInstance()->GetShellsFrame_rmax();
	G4VSolid* shellsFrame_geo = new G4Sphere("shellsFrame_geo", shellsFrame_rmin, shellsFrame_rmax, 0, 360*deg, 0,180*deg);

	// Shell
	G4int NbOfShells = NP1Control::GetInstance()->GetShellsNo();
	G4double shell_rmin = shellsFrame_rmin;
	G4double shell_dr = (shellsFrame_rmax-shellsFrame_rmin)/NbOfShells;
	G4VSolid* shell_geo = new G4Sphere("shell_geo",shell_rmin,shell_rmin+shell_dr,0,360*deg,0*deg,180*deg);

	// Water voxel
	G4VSolid* waterVoxel_geo = new G4Box("waterVoxel_geo", fWaterVoxel_dx, fWaterVoxel_dx, fWaterVoxel_dx);

	// Water box
	G4double waterBox_dx = fVoxelsXNo*fWaterVoxel_dx;
	G4double waterBox_dy = fVoxelsYNo*fWaterVoxel_dx;
	G4double waterBox_dz = fVoxelsZNo*fWaterVoxel_dx;
	G4VSolid* waterBox_geo = new G4Box("waterBox_geo", waterBox_dx, waterBox_dy, waterBox_dz);

	// Water phantom
	G4double waterPhantom_dx;
	if(fWaterPhantom_dx>=fVoxelsXNo*fWaterVoxel_dx) waterPhantom_dx = fWaterPhantom_dx;
	else waterPhantom_dx = fVoxelsXNo*fWaterVoxel_dx;
	G4double waterPhantom_dy;
	if(fWaterPhantom_dx>=fVoxelsYNo*fWaterVoxel_dx) waterPhantom_dy = fWaterPhantom_dx;
	else waterPhantom_dy = fVoxelsYNo*fWaterVoxel_dx;
	G4double waterPhantom_dz;
	if(fWaterPhantom_dx>=fVoxelsZNo*fWaterVoxel_dx) waterPhantom_dz = fWaterPhantom_dx;
	else waterPhantom_dz = fVoxelsZNo*fWaterVoxel_dx;

	G4VSolid* waterPhantom_geo = new G4Box("waterPhantom_geo", waterPhantom_dx, waterPhantom_dy, waterPhantom_dz);

	//Logical Volume ==============================================================================================


	// Visualization attributes

	G4VisAttributes* invisibleSVisAtt;
	invisibleSVisAtt= new G4VisAttributes(G4Colour(0.,0.5,0.5,0.2));
	invisibleSVisAtt->SetVisibility(true);

	// Nano-particle coating
	G4LogicalVolume* particleCoating_log = new G4LogicalVolume(particleCoating_geo,fParticleMaterial,"particleCoating_log");

	G4VisAttributes* simpleParticleCoatingSVisAtt;
	simpleParticleCoatingSVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));
	simpleParticleCoatingSVisAtt->SetVisibility(true);

	particleCoating_log -> SetVisAttributes(simpleParticleCoatingSVisAtt);

	// Nano-particle
	G4LogicalVolume* particle_log = new G4LogicalVolume(particle_geo,fParticleCoatingMaterial,"particle_log");

	G4VisAttributes* simpleParticleSVisAtt;
	simpleParticleSVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0,1.0));
	simpleParticleSVisAtt->SetVisibility(true);

	particle_log -> SetVisAttributes(simpleParticleSVisAtt);

	// Shells frame
	G4LogicalVolume * shellsFrame_log = new G4LogicalVolume(shellsFrame_geo,fBoxMaterial, "shellsFrame_log");

	G4VisAttributes* simpleShellsFrameSVisAtt;
	simpleShellsFrameSVisAtt= new G4VisAttributes(G4Colour(0.,0.5,0.5,0.2));
	simpleShellsFrameSVisAtt->SetVisibility(false);

	shellsFrame_log -> SetVisAttributes(simpleShellsFrameSVisAtt);

	// Shell
	G4LogicalVolume* shell_log = new G4LogicalVolume(shell_geo, fBoxMaterial, "shell_log");

	G4VisAttributes* simpleShellSVisAtt;
	simpleShellSVisAtt= new G4VisAttributes(G4Colour(0.,0.5,0.5,0.01));
	simpleShellSVisAtt->SetVisibility(false);

	shell_log->SetVisAttributes(simpleShellSVisAtt);

	fScoringVolumeVector.push_back(shell_log);

	G4UserLimits *l = new G4UserLimits();
	// Sets a max Step length in the layer and layer frame :
	G4double maxStep = 10*nm;
	l->SetMaxAllowedStep(maxStep);
	shell_log->SetUserLimits(l);

	// Water voxel
	G4LogicalVolume * waterVoxel_log = new G4LogicalVolume(waterVoxel_geo, fBoxMaterial, "waterVoxel_log");

	G4VisAttributes* simpleWaterVoxelVisAtt;
	simpleWaterVoxelVisAtt= new G4VisAttributes(G4Colour(0.,0.5,0.5,0.2));
	simpleWaterVoxelVisAtt->SetVisibility(false);

	waterVoxel_log -> SetVisAttributes(simpleWaterVoxelVisAtt);

	fScoringVolumeVector.push_back(waterVoxel_log);

	// Water box
	G4LogicalVolume * waterBox_log = new G4LogicalVolume(waterBox_geo, fBoxMaterial, "waterBox_log");

	G4VisAttributes* simpleWaterBoxVisAtt;
	simpleWaterBoxVisAtt= new G4VisAttributes(G4Colour(0.,0.5,0.5,0.2));
	simpleWaterBoxVisAtt->SetVisibility(true);

	waterBox_log -> SetVisAttributes(simpleWaterBoxVisAtt);

	fScoringVolumeVector.push_back(waterBox_log);

	// Water phantom
	G4LogicalVolume * waterPhantom_log = new G4LogicalVolume(waterPhantom_geo, fBoxMaterial, "waterPhantom_log");

	G4VisAttributes* simpleWaterPhantomVisAtt;
	simpleWaterPhantomVisAtt= new G4VisAttributes(G4Colour(0.,0.5,0.5,0.2));
	simpleWaterPhantomVisAtt->SetVisibility(true);

	waterPhantom_log -> SetVisAttributes(simpleWaterPhantomVisAtt);

	//Physics Volume  =============================================================================================

	// Nano-particle
	fParticle_phys =
			new G4PVPlacement(0,                     		//no rotation
					G4ThreeVector(),       					//at (0,0,0)
					particle_log,     						//its logical volume
					"particle_phys",  						//its name
					particleCoating_log, 					//its mother  volume
					false,                 					//no boolean operation
					0,                     					//copy number
					true);      			     			//overlaps checking

	// Nano-particle coating
	fParticleCoating_phys =
			new G4PVPlacement(0,                     		//no rotation
					G4ThreeVector(),       					//at (0,0,0)
					particleCoating_log,     				//its logical volume
					"particleCoating_phys",  				//its name
					waterVoxel_log,							//its mother  volume
					false,									//no boolean operation
					0,										//copy number
					true);									//overlaps checking

	// Shell
	G4VPVParameterisation* shell_param = new NP1SVParameterisation(shell_rmin, shell_dr);
	fShell_phys = new G4PVParameterised("shell_phys",
			shell_log,
			shellsFrame_log,
			kZAxis,
			NbOfShells,
			shell_param
	);

	// Shells frame
	fShellsFrame_phys =new G4PVPlacement(0,                	//no rotation
			G4ThreeVector(),                                //at (0,0,0)
			shellsFrame_log,                     		    //its logical volume
			"shellsFrame_phys",                             //its name
			waterVoxel_log,                 				//its mother  volume
			false,                                          //no boolean operation
			0,                                              //copy number
			true);                                          //overlaps checking

	// Apply concentration
	G4int voxelsNo=0;
	fVoxelsID.clear();
	for(G4int i=0; i<fVoxelsXNo*fVoxelsYNo*fVoxelsZNo; i++){

		G4double relativeConcentration = fConcentration/fConcentration0;
		if(relativeConcentration>1) relativeConcentration=1;

		G4double trial = G4UniformRand();
		if(trial<relativeConcentration){
			fVoxelsID.push_back(i);
			voxelsNo++;
		}
	}

	// Water voxel
	if(voxelsNo>0){
		G4VPVParameterisation* voxel_param = new NP1PhantomVoxelParameterisation(fVoxelsXNo, fVoxelsYNo, fVoxelsZNo, fWaterVoxel_dx,fVoxelsID);
		fWaterVoxel_phys = new G4PVParameterised("waterVoxel_phys",
				waterVoxel_log,
				waterBox_log,
				kXAxis,
				voxelsNo,
				voxel_param
		);
	}

	// Water box
	fWaterBox_phys = new G4PVPlacement(0,                   //no rotation
			G4ThreeVector(),                                //at (0,0,0)
			waterBox_log,                   	 	 		//its logical volume
			"waterBox_phys",                      			//its name
			waterPhantom_log,              					//its mother  volume
			false,                                          //no boolean operation
			0,                                              //copy number
			true);                                          //overlaps checking

	// Water phantom
	fWaterPhantom_phys = new G4PVPlacement(0,               //no rotation
			G4ThreeVector(),                                //at (0,0,0)
			waterPhantom_log,                   	 	 	//its logical volume
			"waterPhantom_phys",                      		//its name
			motherVolume,                 					//its mother  volume
			false,                                          //no boolean operation
			0,                                              //copy number
			true);                                          //overlaps checking

}

void NP1DetectorConstruction::ConstructSDandField()
{}

void NP1DetectorConstruction::SetNanoParticleRadius(G4double aRadius){

	G4GeometryManager* geoman = G4GeometryManager::GetInstance() ;

	fParticleRadius=aRadius;

	// Open geometry for the physical volume to be modified ...
	//

	geoman->OpenGeometry(fParticleCoating_phys);

	// Setting particle radius
	G4Orb* particle_geo = (G4Orb*) this->fParticle_phys->GetLogicalVolume()->GetSolid();
	particle_geo->SetRadius(aRadius-fParticleCoatingThickness);

	// Setting particle coating radius
	NP1Control::GetInstance()->SetShellsFrame_rmin(fParticleRadius);
	G4Orb* particleCoating_geo = (G4Orb*) this->fParticleCoating_phys->GetLogicalVolume()->GetSolid();
	particleCoating_geo->SetRadius(aRadius);

	// Setting voxel side length
	G4Box* waterVoxel_geo = (G4Box*) this->fWaterVoxel_phys->GetLogicalVolume()->GetSolid();
	waterVoxel_geo->SetXHalfLength(fWaterVoxel_dx);
	waterVoxel_geo->SetYHalfLength(fWaterVoxel_dx);
	waterVoxel_geo->SetZHalfLength(fWaterVoxel_dx);

	// Setting water box side length
	fWaterVoxel_dx= pow(fParticleMaterial->GetDensity()/fBoxMaterial->GetDensity()*4./3.*M_PI*pow(fParticleRadius,3)/fConcentration,1/3.);
	NP1Control::GetInstance()->SetShellsFrame_rmax(fWaterVoxel_dx);
	G4Box* waterBox_geo = (G4Box*) this->fWaterBox_phys->GetLogicalVolume()->GetSolid();
	waterBox_geo->SetXHalfLength(fWaterVoxel_dx*fVoxelsXNo);
	waterBox_geo->SetYHalfLength(fWaterVoxel_dx*fVoxelsYNo);
	waterBox_geo->SetZHalfLength(fWaterVoxel_dx*fVoxelsZNo);

	// Setting water phantom side length
	G4Box* waterPhantom_geo = (G4Box*) this->fWaterPhantom_phys->GetLogicalVolume()->GetSolid();

	G4double waterPhantom_dx;
	if(fWaterPhantom_dx>=fVoxelsXNo*fWaterVoxel_dx) waterPhantom_dx = fWaterPhantom_dx;
	else waterPhantom_dx = fVoxelsXNo*fWaterVoxel_dx;
	G4double waterPhantom_dy;
	if(fWaterPhantom_dx>=fVoxelsYNo*fWaterVoxel_dx) waterPhantom_dy = fWaterPhantom_dx;
	else waterPhantom_dy = fVoxelsYNo*fWaterVoxel_dx;
	G4double waterPhantom_dz;
	if(fWaterPhantom_dx>=fVoxelsZNo*fWaterVoxel_dx) waterPhantom_dz = fWaterPhantom_dx;
	else waterPhantom_dz = fVoxelsZNo*fWaterVoxel_dx;

	waterPhantom_geo->SetXHalfLength(waterPhantom_dx);
	waterPhantom_geo->SetYHalfLength(waterPhantom_dy);
	waterPhantom_geo->SetZHalfLength(waterPhantom_dz);


	// Setting voxels postition inside water box
	fWaterBox_phys->GetLogicalVolume()->ClearDaughters();

	// Apply concentration
	G4int voxelsNo=0;
	fVoxelsID.clear();
	for(G4int i=0; i<fVoxelsXNo*fVoxelsYNo*fVoxelsZNo; i++){

		G4double relativeConcentration = fConcentration/fConcentration0;
		if(relativeConcentration>1) relativeConcentration=1;

		G4double trial = G4UniformRand();
		if(trial<relativeConcentration){
			fVoxelsID.push_back(i);
			voxelsNo++;
		}
	}

	// Water voxel
	if(voxelsNo>0){
		G4VPVParameterisation* voxel_param = new NP1PhantomVoxelParameterisation(fVoxelsXNo, fVoxelsYNo, fVoxelsZNo, fWaterVoxel_dx,fVoxelsID);
		fWaterVoxel_phys = new G4PVParameterised("waterVoxel_phys",
				fWaterVoxel_phys->GetLogicalVolume(),
				fWaterBox_phys->GetLogicalVolume(),
				kXAxis,
				voxelsNo,
				voxel_param
		);
	}

	// Setting shells frame radius
	NP1Control::GetInstance()->SetShellsFrame_rmax(fWaterVoxel_dx);
	G4double shellsFrame_rmin =fParticleRadius;
	G4double shellsFrame_rmax = NP1Control::GetInstance()->GetShellsFrame_rmax();

	G4Sphere* shellsFrame_geo = (G4Sphere*) this->fShellsFrame_phys->GetLogicalVolume()->GetSolid();
	shellsFrame_geo->SetOuterRadius(shellsFrame_rmax);

	// Setting shell radius
	fShellsFrame_phys->GetLogicalVolume()->ClearDaughters();

	G4int NbOfShells = NP1Control::GetInstance()->GetShellsNo();
	G4double shell_rmin = shellsFrame_rmin;
	G4double shell_dr = (shellsFrame_rmax-shellsFrame_rmin)/NbOfShells;

	G4VPVParameterisation* shell_param = new NP1SVParameterisation(shell_rmin, shell_dr);
	fShell_phys = new G4PVParameterised("shell_phys",
			fShell_phys->GetLogicalVolume(),
			fShellsFrame_phys->GetLogicalVolume(),
			kZAxis,
			NbOfShells,
			shell_param
	);

	// Close geometry for the portion modified ...
	//
	geoman->CloseGeometry(fParticleCoating_phys);

	G4RunManager::GetRunManager()->ReinitializeGeometry();

}

void NP1DetectorConstruction::SetNanoParticleCoatingThickness(G4double aThickness){

	G4GeometryManager* geoman = G4GeometryManager::GetInstance() ;

	fParticleCoatingThickness=aThickness;

	// Open geometry for the physical volume to be modified ...
	//
	geoman->OpenGeometry(fParticleCoating_phys);

	G4Orb* particle_geo = (G4Orb*) this->fParticle_phys->GetLogicalVolume()->GetSolid();
	particle_geo->SetRadius(fParticleRadius-aThickness);

	// Close geometry for the portion modified ...
	//
	geoman->CloseGeometry(fParticleCoating_phys);

	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void NP1DetectorConstruction::SetNanoParticleMaterial(G4String aMaterial){

	G4GeometryManager* geoman = G4GeometryManager::GetInstance() ;

	// Open geometry for the physical volume to be modified ...
	//
	geoman->OpenGeometry(fParticle_phys);

	this->fParticle_phys->GetLogicalVolume()->SetMaterial(NP1Materials::GetInstance()->GetMaterial(aMaterial));
	this->fParticleMaterial=NP1Materials::GetInstance()->GetMaterial(aMaterial);

	// Close geometry for the portion modified ...
	//
	geoman->CloseGeometry(fParticle_phys);

	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void NP1DetectorConstruction::SetNanoParticleCoatingMaterial(G4String aMaterial){

	G4GeometryManager* geoman = G4GeometryManager::GetInstance() ;

	// Open geometry for the physical volume to be modified ...
	//
	geoman->OpenGeometry(fParticleCoating_phys);

	this->fParticleCoating_phys->GetLogicalVolume()->SetMaterial(NP1Materials::GetInstance()->GetMaterial(aMaterial));
	this->fParticleCoatingMaterial=NP1Materials::GetInstance()->GetMaterial(aMaterial);

	// Close geometry for the portion modified ...
	//
	geoman->CloseGeometry(fParticleCoating_phys);

	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	G4RunManager::GetRunManager()->ReinitializeGeometry();

}

void NP1DetectorConstruction::SetWaterVoxel_dx(G4double aDx){

	G4GeometryManager* geoman = G4GeometryManager::GetInstance() ;

	fWaterVoxel_dx=aDx/2.+fParticleRadius;


	// Open geometry for the physical volume to be modified ...
	//
	geoman->OpenGeometry(fWaterBox_phys);

	// Setting voxel side length
	G4Box* waterVoxel_geo = (G4Box*) this->fWaterVoxel_phys->GetLogicalVolume()->GetSolid();
	waterVoxel_geo->SetXHalfLength(fWaterVoxel_dx);
	waterVoxel_geo->SetYHalfLength(fWaterVoxel_dx);
	waterVoxel_geo->SetZHalfLength(fWaterVoxel_dx);

	// Setting water box side length
	NP1Control::GetInstance()->SetShellsFrame_rmax(fWaterVoxel_dx);
	G4Box* waterBox_geo = (G4Box*) this->fWaterBox_phys->GetLogicalVolume()->GetSolid();
	waterBox_geo->SetXHalfLength(fWaterVoxel_dx*fVoxelsXNo);
	waterBox_geo->SetYHalfLength(fWaterVoxel_dx*fVoxelsYNo);
	waterBox_geo->SetZHalfLength(fWaterVoxel_dx*fVoxelsZNo);

	// Setting water phantom side length
	G4Box* waterPhantom_geo = (G4Box*) this->fWaterPhantom_phys->GetLogicalVolume()->GetSolid();

	G4double waterPhantom_dx;
	if(fWaterPhantom_dx>=fVoxelsXNo*fWaterVoxel_dx) waterPhantom_dx = fWaterPhantom_dx;
	else waterPhantom_dx = fVoxelsXNo*fWaterVoxel_dx;
	G4double waterPhantom_dy;
	if(fWaterPhantom_dx>=fVoxelsYNo*fWaterVoxel_dx) waterPhantom_dy = fWaterPhantom_dx;
	else waterPhantom_dy = fVoxelsYNo*fWaterVoxel_dx;
	G4double waterPhantom_dz;
	if(fWaterPhantom_dx>=fVoxelsZNo*fWaterVoxel_dx) waterPhantom_dz = fWaterPhantom_dx;
	else waterPhantom_dz = fVoxelsZNo*fWaterVoxel_dx;

	waterPhantom_geo->SetXHalfLength(waterPhantom_dx);
	waterPhantom_geo->SetYHalfLength(waterPhantom_dy);
	waterPhantom_geo->SetZHalfLength(waterPhantom_dz);


	// Setting voxels postition inside water box
	fWaterBox_phys->GetLogicalVolume()->ClearDaughters();

	// Apply concentration
	G4int voxelsNo=0;
	fVoxelsID.clear();
	for(G4int i=0; i<fVoxelsXNo*fVoxelsYNo*fVoxelsZNo; i++){

		G4double relativeConcentration = fConcentration/fConcentration0;
		if(relativeConcentration>1) relativeConcentration=1;

		G4double trial = G4UniformRand();
		if(trial<relativeConcentration){
			fVoxelsID.push_back(i);
			voxelsNo++;
		}
	}

	// Water voxel
	if(voxelsNo>0){
		G4VPVParameterisation* voxel_param = new NP1PhantomVoxelParameterisation(fVoxelsXNo, fVoxelsYNo, fVoxelsZNo, fWaterVoxel_dx,fVoxelsID);
		fWaterVoxel_phys = new G4PVParameterised("waterVoxel_phys",
				fWaterVoxel_phys->GetLogicalVolume(),
				fWaterBox_phys->GetLogicalVolume(),
				kXAxis,
				voxelsNo,
				voxel_param
		);
	}

	// Setting shells frame radius
	NP1Control::GetInstance()->SetShellsFrame_rmax(fWaterVoxel_dx);
	G4double shellsFrame_rmin =fParticleRadius;
	G4double shellsFrame_rmax = NP1Control::GetInstance()->GetShellsFrame_rmax();

	G4Sphere* shellsFrame_geo = (G4Sphere*) this->fShellsFrame_phys->GetLogicalVolume()->GetSolid();
	shellsFrame_geo->SetOuterRadius(shellsFrame_rmax);

	// Setting shell radius
	fShellsFrame_phys->GetLogicalVolume()->ClearDaughters();

	G4int NbOfShells = NP1Control::GetInstance()->GetShellsNo();
	G4double shell_rmin = shellsFrame_rmin;
	G4double shell_dr = (shellsFrame_rmax-shellsFrame_rmin)/NbOfShells;

	G4VPVParameterisation* shell_param = new NP1SVParameterisation(shell_rmin, shell_dr);
	fShell_phys = new G4PVParameterised("shell_phys",
			fShell_phys->GetLogicalVolume(),
			fShellsFrame_phys->GetLogicalVolume(),
			kZAxis,
			NbOfShells,
			shell_param
	);

	// Close geometry for the portion modified ...
	//
	geoman->CloseGeometry(fWaterBox_phys);

	G4RunManager::GetRunManager()->ReinitializeGeometry();

}


void NP1DetectorConstruction::SetVoxelsXNo(G4int aXNo){

	G4GeometryManager* geoman = G4GeometryManager::GetInstance() ;

	fVoxelsXNo=aXNo;

	// Open geometry for the physical volume to be modified ...
	//
	geoman->OpenGeometry(fWaterBox_phys);

	// Setting water box side length
	G4Box* waterBox_geo = (G4Box*) this->fWaterBox_phys->GetLogicalVolume()->GetSolid();
	waterBox_geo->SetXHalfLength(fWaterVoxel_dx*fVoxelsXNo);
	waterBox_geo->SetYHalfLength(fWaterVoxel_dx*fVoxelsYNo);
	waterBox_geo->SetZHalfLength(fWaterVoxel_dx*fVoxelsZNo);

	// Setting water phantom side length
	G4Box* waterPhantom_geo = (G4Box*) this->fWaterPhantom_phys->GetLogicalVolume()->GetSolid();

	G4double waterPhantom_dx;
	if(fWaterPhantom_dx>=fVoxelsXNo*fWaterVoxel_dx) waterPhantom_dx = fWaterPhantom_dx;
	else waterPhantom_dx = fVoxelsXNo*fWaterVoxel_dx;
	G4double waterPhantom_dy;
	if(fWaterPhantom_dx>=fVoxelsYNo*fWaterVoxel_dx) waterPhantom_dy = fWaterPhantom_dx;
	else waterPhantom_dy = fVoxelsYNo*fWaterVoxel_dx;
	G4double waterPhantom_dz;
	if(fWaterPhantom_dx>=fVoxelsZNo*fWaterVoxel_dx) waterPhantom_dz = fWaterPhantom_dx;
	else waterPhantom_dz = fVoxelsZNo*fWaterVoxel_dx;

	waterPhantom_geo->SetXHalfLength(waterPhantom_dx);
	waterPhantom_geo->SetYHalfLength(waterPhantom_dy);
	waterPhantom_geo->SetZHalfLength(waterPhantom_dz);

	// Setting voxels postition inside water box
	fWaterBox_phys->GetLogicalVolume()->ClearDaughters();

	// Apply concentration
	G4int voxelsNo=0;
	fVoxelsID.clear();
	for(G4int i=0; i<fVoxelsXNo*fVoxelsYNo*fVoxelsZNo; i++){

		G4double relativeConcentration = fConcentration/fConcentration0;
		if(relativeConcentration>1) relativeConcentration=1;

		G4double trial = G4UniformRand();
		if(trial<relativeConcentration){
			fVoxelsID.push_back(i);
			voxelsNo++;
		}
	}

	// Water voxel
	if(voxelsNo>0){
		G4VPVParameterisation* voxel_param = new NP1PhantomVoxelParameterisation(fVoxelsXNo, fVoxelsYNo, fVoxelsZNo, fWaterVoxel_dx,fVoxelsID);
		fWaterVoxel_phys = new G4PVParameterised("waterVoxel_phys",
				fWaterVoxel_phys->GetLogicalVolume(),
				fWaterBox_phys->GetLogicalVolume(),
				kXAxis,
				voxelsNo,
				voxel_param
		);
	}

	// Close geometry for the portion modified ...
	//
	geoman->CloseGeometry(fWaterBox_phys);

	G4RunManager::GetRunManager()->ReinitializeGeometry();

}

void NP1DetectorConstruction::SetVoxelsYNo(G4int aYNo){

	G4GeometryManager* geoman = G4GeometryManager::GetInstance() ;

	fVoxelsYNo=aYNo;

	// Open geometry for the physical volume to be modified ...
	//
	geoman->OpenGeometry(fWaterBox_phys);

	// Setting water box side length
	G4Box* waterBox_geo = (G4Box*) this->fWaterBox_phys->GetLogicalVolume()->GetSolid();
	waterBox_geo->SetXHalfLength(fWaterVoxel_dx*fVoxelsXNo);
	waterBox_geo->SetYHalfLength(fWaterVoxel_dx*fVoxelsYNo);
	waterBox_geo->SetZHalfLength(fWaterVoxel_dx*fVoxelsZNo);

	// Setting water phantom side length
	G4Box* waterPhantom_geo = (G4Box*) this->fWaterPhantom_phys->GetLogicalVolume()->GetSolid();

	G4double waterPhantom_dx;
	if(fWaterPhantom_dx>=fVoxelsXNo*fWaterVoxel_dx) waterPhantom_dx = fWaterPhantom_dx;
	else waterPhantom_dx = fVoxelsXNo*fWaterVoxel_dx;
	G4double waterPhantom_dy;
	if(fWaterPhantom_dx>=fVoxelsYNo*fWaterVoxel_dx) waterPhantom_dy = fWaterPhantom_dx;
	else waterPhantom_dy = fVoxelsYNo*fWaterVoxel_dx;
	G4double waterPhantom_dz;
	if(fWaterPhantom_dx>=fVoxelsZNo*fWaterVoxel_dx) waterPhantom_dz = fWaterPhantom_dx;
	else waterPhantom_dz = fVoxelsZNo*fWaterVoxel_dx;

	waterPhantom_geo->SetXHalfLength(waterPhantom_dx);
	waterPhantom_geo->SetYHalfLength(waterPhantom_dy);
	waterPhantom_geo->SetZHalfLength(waterPhantom_dz);

	// Setting voxels postition inside water box
	fWaterBox_phys->GetLogicalVolume()->ClearDaughters();

	// Apply concentration
	G4int voxelsNo=0;
	fVoxelsID.clear();
	for(G4int i=0; i<fVoxelsXNo*fVoxelsYNo*fVoxelsZNo; i++){

		G4double relativeConcentration = fConcentration/fConcentration0;
		if(relativeConcentration>1) relativeConcentration=1;

		G4double trial = G4UniformRand();
		if(trial<relativeConcentration){
			fVoxelsID.push_back(i);
			voxelsNo++;
		}
	}

	// Water voxel
	if(voxelsNo>0){
		G4VPVParameterisation* voxel_param = new NP1PhantomVoxelParameterisation(fVoxelsXNo, fVoxelsYNo, fVoxelsZNo, fWaterVoxel_dx,fVoxelsID);
		fWaterVoxel_phys = new G4PVParameterised("waterVoxel_phys",
				fWaterVoxel_phys->GetLogicalVolume(),
				fWaterBox_phys->GetLogicalVolume(),
				kXAxis,
				voxelsNo,
				voxel_param
		);
	}

	// Close geometry for the portion modified ...
	//
	geoman->CloseGeometry(fWaterBox_phys);

	G4RunManager::GetRunManager()->ReinitializeGeometry();

}


void NP1DetectorConstruction::SetVoxelsZNo(G4int aZNo){

	G4GeometryManager* geoman = G4GeometryManager::GetInstance() ;

	fVoxelsZNo=aZNo;

	// Open geometry for the physical volume to be modified ...
	//
	geoman->OpenGeometry(fWaterBox_phys);

	// Setting water box side length
	G4Box* waterBox_geo = (G4Box*) this->fWaterBox_phys->GetLogicalVolume()->GetSolid();
	waterBox_geo->SetXHalfLength(fWaterVoxel_dx*fVoxelsXNo);
	waterBox_geo->SetYHalfLength(fWaterVoxel_dx*fVoxelsYNo);
	waterBox_geo->SetZHalfLength(fWaterVoxel_dx*fVoxelsZNo);

	// Setting water phantom side length
	G4Box* waterPhantom_geo = (G4Box*) this->fWaterPhantom_phys->GetLogicalVolume()->GetSolid();

	G4double waterPhantom_dx;
	if(fWaterPhantom_dx>=fVoxelsXNo*fWaterVoxel_dx) waterPhantom_dx = fWaterPhantom_dx;
	else waterPhantom_dx = fVoxelsXNo*fWaterVoxel_dx;
	G4double waterPhantom_dy;
	if(fWaterPhantom_dx>=fVoxelsYNo*fWaterVoxel_dx) waterPhantom_dy = fWaterPhantom_dx;
	else waterPhantom_dy = fVoxelsYNo*fWaterVoxel_dx;
	G4double waterPhantom_dz;
	if(fWaterPhantom_dx>=fVoxelsZNo*fWaterVoxel_dx) waterPhantom_dz = fWaterPhantom_dx;
	else waterPhantom_dz = fVoxelsZNo*fWaterVoxel_dx;

	waterPhantom_geo->SetXHalfLength(waterPhantom_dx);
	waterPhantom_geo->SetYHalfLength(waterPhantom_dy);
	waterPhantom_geo->SetZHalfLength(waterPhantom_dz);

	// Setting voxels postition inside water box
	fWaterBox_phys->GetLogicalVolume()->ClearDaughters();

	// Apply concentration
	G4int voxelsNo=0;
	fVoxelsID.clear();
	for(G4int i=0; i<fVoxelsXNo*fVoxelsYNo*fVoxelsZNo; i++){

		G4double relativeConcentration = fConcentration/fConcentration0;
		if(relativeConcentration>1) relativeConcentration=1;

		G4double trial = G4UniformRand();
		if(trial<relativeConcentration){
			fVoxelsID.push_back(i);
			voxelsNo++;
		}
	}

	// Water voxel
	if(voxelsNo>0){
		G4VPVParameterisation* voxel_param = new NP1PhantomVoxelParameterisation(fVoxelsXNo, fVoxelsYNo, fVoxelsZNo, fWaterVoxel_dx,fVoxelsID);
		fWaterVoxel_phys = new G4PVParameterised("waterVoxel_phys",
				fWaterVoxel_phys->GetLogicalVolume(),
				fWaterBox_phys->GetLogicalVolume(),
				kXAxis,
				voxelsNo,
				voxel_param
		);
	}

	// Close geometry for the portion modified ...
	//
	geoman->CloseGeometry(fWaterBox_phys);

	G4RunManager::GetRunManager()->ReinitializeGeometry();

}

void NP1DetectorConstruction::SetConcentration0(G4double aConcentration){

	G4GeometryManager* geoman = G4GeometryManager::GetInstance() ;

	fConcentration0=aConcentration;
	fWaterVoxel_dx= pow(fParticleMaterial->GetDensity()/fBoxMaterial->GetDensity()*4./3.*M_PI*pow(fParticleRadius,3)/fConcentration0,1/3.);

	// Open geometry for the physical volume to be modified ...
	//
	geoman->OpenGeometry(fWaterBox_phys);

	// Setting voxel side length
	G4Box* waterVoxel_geo = (G4Box*) this->fWaterVoxel_phys->GetLogicalVolume()->GetSolid();
	waterVoxel_geo->SetXHalfLength(fWaterVoxel_dx);
	waterVoxel_geo->SetYHalfLength(fWaterVoxel_dx);
	waterVoxel_geo->SetZHalfLength(fWaterVoxel_dx);

	// Setting water box side length
	fWaterVoxel_dx= pow(fParticleMaterial->GetDensity()/fBoxMaterial->GetDensity()*4./3.*M_PI*pow(fParticleRadius,3)/fConcentration,1/3.);
	NP1Control::GetInstance()->SetShellsFrame_rmax(fWaterVoxel_dx);
	G4Box* waterBox_geo = (G4Box*) this->fWaterBox_phys->GetLogicalVolume()->GetSolid();
	waterBox_geo->SetXHalfLength(fWaterVoxel_dx*fVoxelsXNo);
	waterBox_geo->SetYHalfLength(fWaterVoxel_dx*fVoxelsYNo);
	waterBox_geo->SetZHalfLength(fWaterVoxel_dx*fVoxelsZNo);

	// Setting water phantom side length
	G4Box* waterPhantom_geo = (G4Box*) this->fWaterPhantom_phys->GetLogicalVolume()->GetSolid();

	G4double waterPhantom_dx;
	if(fWaterPhantom_dx>=fVoxelsXNo*fWaterVoxel_dx) waterPhantom_dx = fWaterPhantom_dx;
	else waterPhantom_dx = fVoxelsXNo*fWaterVoxel_dx;
	G4double waterPhantom_dy;
	if(fWaterPhantom_dx>=fVoxelsYNo*fWaterVoxel_dx) waterPhantom_dy = fWaterPhantom_dx;
	else waterPhantom_dy = fVoxelsYNo*fWaterVoxel_dx;
	G4double waterPhantom_dz;
	if(fWaterPhantom_dx>=fVoxelsZNo*fWaterVoxel_dx) waterPhantom_dz = fWaterPhantom_dx;
	else waterPhantom_dz = fVoxelsZNo*fWaterVoxel_dx;

	waterPhantom_geo->SetXHalfLength(waterPhantom_dx);
	waterPhantom_geo->SetYHalfLength(waterPhantom_dy);
	waterPhantom_geo->SetZHalfLength(waterPhantom_dz);

	// Setting voxels postition inside water box
	fWaterBox_phys->GetLogicalVolume()->ClearDaughters();

	// Apply concentration
	G4int voxelsNo=0;
	fVoxelsID.clear();
	for(G4int i=0; i<fVoxelsXNo*fVoxelsYNo*fVoxelsZNo; i++){

		G4double relativeConcentration = fConcentration/fConcentration0;
		if(relativeConcentration>1) relativeConcentration=1;

		G4double trial = G4UniformRand();
		if(trial<relativeConcentration){
			fVoxelsID.push_back(i);
			voxelsNo++;
		}
	}

	// Water voxel
	if(voxelsNo>0){
		G4VPVParameterisation* voxel_param = new NP1PhantomVoxelParameterisation(fVoxelsXNo, fVoxelsYNo, fVoxelsZNo, fWaterVoxel_dx,fVoxelsID);
		fWaterVoxel_phys = new G4PVParameterised("waterVoxel_phys",
				fWaterVoxel_phys->GetLogicalVolume(),
				fWaterBox_phys->GetLogicalVolume(),
				kXAxis,
				voxelsNo,
				voxel_param
		);
	}

	// Setting shells frame radius
	NP1Control::GetInstance()->SetShellsFrame_rmax(fWaterVoxel_dx);
	G4double shellsFrame_rmin =fParticleRadius;
	G4double shellsFrame_rmax = NP1Control::GetInstance()->GetShellsFrame_rmax();

	G4Sphere* shellsFrame_geo = (G4Sphere*) this->fShellsFrame_phys->GetLogicalVolume()->GetSolid();
	shellsFrame_geo->SetOuterRadius(shellsFrame_rmax);

	// Setting shell radius
	fShellsFrame_phys->GetLogicalVolume()->ClearDaughters();

	G4int NbOfShells = NP1Control::GetInstance()->GetShellsNo();
	G4double shell_rmin = shellsFrame_rmin;
	G4double shell_dr = (shellsFrame_rmax-shellsFrame_rmin)/NbOfShells;

	G4VPVParameterisation* shell_param = new NP1SVParameterisation(shell_rmin, shell_dr);
	fShell_phys = new G4PVParameterised("shell_phys",
			fShell_phys->GetLogicalVolume(),
			fShellsFrame_phys->GetLogicalVolume(),
			kZAxis,
			NbOfShells,
			shell_param
	);

	// Close geometry for the portion modified ...
	//
	geoman->CloseGeometry(fWaterBox_phys);

	G4RunManager::GetRunManager()->ReinitializeGeometry();

}

void NP1DetectorConstruction::SetConcentration(G4double aConcentration){

	G4GeometryManager* geoman = G4GeometryManager::GetInstance() ;

	fConcentration=aConcentration;

	// Open geometry for the physical volume to be modified ...
	//
	geoman->OpenGeometry(fWaterBox_phys);

	// Setting voxels postition inside water box
	fWaterBox_phys->GetLogicalVolume()->ClearDaughters();

	// Apply concentration
	G4int voxelsNo=0;
	fVoxelsID.clear();
	for(G4int i=0; i<fVoxelsXNo*fVoxelsYNo*fVoxelsZNo; i++){

		G4double relativeConcentration = fConcentration/fConcentration0;
		if(relativeConcentration>1) relativeConcentration=1;

		G4double trial = G4UniformRand();
		if(trial<relativeConcentration){
			fVoxelsID.push_back(i);
			voxelsNo++;
		}
	}

	// Water voxel
	if(voxelsNo>0){
		G4VPVParameterisation* voxel_param = new NP1PhantomVoxelParameterisation(fVoxelsXNo, fVoxelsYNo, fVoxelsZNo, fWaterVoxel_dx,fVoxelsID);
		fWaterVoxel_phys = new G4PVParameterised("waterVoxel_phys",
				fWaterVoxel_phys->GetLogicalVolume(),
				fWaterBox_phys->GetLogicalVolume(),
				kXAxis,
				voxelsNo,
				voxel_param
		);
	}

	// Close geometry for the portion modified ...
	//
	geoman->CloseGeometry(fWaterBox_phys);

	G4RunManager::GetRunManager()->ReinitializeGeometry();

}
