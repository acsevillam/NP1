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

//Geant4 Headers
#include "G4SystemOfUnits.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"
#include "G4NistManager.hh"

// NP1 Headers
#include "NP1Materials.hh"

using namespace std ;

// Define Static Variables
NP1Materials* NP1Materials::instance = NULL ;


NP1Materials::NP1Materials()
{

}

NP1Materials::~NP1Materials()
{
}

NP1Materials* NP1Materials::GetInstance() {

	if (instance == NULL) instance =  new NP1Materials() ;
	return instance ;

}

void NP1Materials::Kill() {

	if(instance!=NULL){
		delete instance ;
		instance = NULL ;
	}

}

G4Material* NP1Materials::GetMaterial(const G4String materialName){

	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();

	if(fMaterialsCollection[materialName]==NULL){
		if(nist->FindOrBuildMaterial(materialName)!=NULL){
			fMaterialsCollection[materialName] = nist->FindOrBuildMaterial(materialName);
			G4cout<<"New NIST material "<<materialName<<" instantiated"<<G4endl;
		}else if(otherMaterials(materialName)!=NULL){
			fMaterialsCollection[materialName]=otherMaterials(materialName);
			G4cout<<"New other material "<<materialName<<" instantiated"<<G4endl;
		}
	}

	return fMaterialsCollection[materialName];
}

G4Material* NP1Materials::otherMaterials(const G4String materialName)
{
	G4Material * material = 0;
	G4double A, Z, d,fractionmass, pressure, temperature;
	G4String name,symbol;
	G4int ncomponents, natoms, nel;
	G4double universe_mean_density = 1.e-25*g/cm3;

	// General elements

	A = 157.25*g/mole;
	G4Element* Gd = new G4Element (name="Gadolinium", symbol="Gd", Z=64. , A);

	A = 1.01*g/mole;
	G4Element* H = new G4Element (name="Hydrogen", symbol="H", Z=1. , A);

	A = 14.01*g/mole;
	G4Element* N  = new G4Element(name="Nitrogen",symbol="N" , Z=7., A);

	A = 15.99*g/mole;
	G4Element* O  = new G4Element(name="Oxygen", symbol="O", Z=8., A);

	A = 174.96*g/mole;
	G4Element* Lu  = new G4Element(name="Lutetium", symbol="Lu", Z=71., A);

	A = 28.085*g/mole;
	G4Element* Si  = new G4Element(name="Silicon", symbol="Si", Z=14., A);

	A = 26.98*g/mole;
	G4Element* Al  = new G4Element(name="Aluminium", symbol="Al", Z=13., A);

	A = 63.546*g/mole;
	G4Element* Cu = new G4Element(name="Copper", symbol="Cu", Z=29., A);

	A = 208.98038*g/mole;
	G4Element* Bi = new G4Element(name="Bismuth", symbol="Bi", Z=83., A);

	A = 32.065*g/mole;
	G4Element* S = new G4Element(name="Sulfur", symbol="Cu", Z=16., A);

	A = 180.9479*g/mole;
	G4Element* Ta = new G4Element(name="Tantalum", symbol="Ta", Z=73., A);

	A = 39.948*g/mole;
	G4Element* Ar = new G4Element (name="Argon", symbol="Ar", Z=18., A);

	A = 12.011*g/mole;
	G4Element* C = new G4Element (name="Carbon", symbol="C", Z=6., A);

	A = 65.409*g/mole;
	G4Element *  Zn = new G4Element (name="Zinc", symbol="Zn", Z=30., A);

	A = 30.973761*g/mole;
	G4Element *  P  = new G4Element (name="Phosphorus", symbol="P",Z=15.,A);

	if (materialName=="HDAir")
	{
		d = (1.290*mg/cm3)*1000; // x1000 density
		G4Material* Air = new G4Material(name="HDAir",d,ncomponents=2);
		Air->	AddElement(N, fractionmass=70*perCent);
		Air->	AddElement(O, fractionmass=30*perCent);
		material=Air;
	}

	if (materialName=="SHDAir")
	{
		d = (1.290*mg/cm3)*1000000; // 10x6 density
		G4Material* Air = new G4Material(name="HDAir",d,ncomponents=2);
		Air->	AddElement(N, fractionmass=70*perCent);
		Air->	AddElement(O, fractionmass=30*perCent);
		material=Air;
	}

	if (materialName=="LSO")
	{
		d = (7.35*g/cm3);
		G4Material* LSO = new G4Material(name="LSO",d,ncomponents=3);
		LSO->	AddElement(Lu, natoms=2);
		LSO->	AddElement(Si, natoms=1);
		LSO->	AddElement(O, natoms=5);
		material=LSO;
	}

	if (materialName=="LuYaP")
	{
		d = (8.34*g/cm3);
		G4Material* LSO = new G4Material(name="LuYaP",d,ncomponents=3);
		LSO->	AddElement(Lu, natoms=1);
		LSO->	AddElement(Al, natoms=1);
		LSO->	AddElement(O, natoms=3);
		material=LSO;
	}

	if (materialName=="LDCu")
	{
		d = (8.933*g/cm3);
		G4Material* LDCooper = new G4Material(name="LDCu",d,ncomponents=1);
		LDCooper->AddElement(Cu, natoms=1);
		material=LDCooper;
	}

	if (materialName=="Bi2S3")
	{
		d = (6.56*g/cm3);
		G4Material* Bi2S3 = new G4Material(name="Bi2S3",d,ncomponents=2);
		Bi2S3->AddElement(Bi, natoms=2);
		Bi2S3->AddElement(S, natoms=3);
		material=Bi2S3;
	}

	if (materialName=="Ta2O5")
	{
		d = (8.2*g/cm3);
		G4Material* Ta2O5 = new G4Material(name="Ta2O5",d,ncomponents=2);
		Ta2O5->AddElement(Ta, natoms=2);
		Ta2O5->AddElement(O, natoms=5);
		material=Ta2O5;
	}


	if (materialName=="Vacuum")
	{
		d = universe_mean_density;
		G4Material* vacuum = new G4Material(name="Vacuum", Z=1., A=1.01*g/mole,d);
		material=vacuum;
	}

	if (materialName=="HDWater")
	{
		d = 1.000*g/cm3;
		G4Material* HDWater = new G4Material(name="HDWater", d, ncomponents=2);
		HDWater->AddElement(H, natoms=2);
		HDWater->AddElement(O, natoms=1);
		material=HDWater;
	}

	if (materialName=="HDGd")
	{
		d = 7.900*g/cm3;
		G4Material* HDGd = new G4Material(name="HDGd", d, ncomponents=1);
		HDGd->AddElement(Gd, natoms=1);
		material=HDGd;
	}

	if (materialName=="H2O")
	{
		d = 1.000*g/cm3;
		G4Material* H2O = new G4Material(name="H2O", d, ncomponents=2);
		H2O->AddElement(H, natoms=2);
		H2O->AddElement(O, natoms=1);
	}

	if (materialName=="Air")
	{
		d= 1.290*mg/cm3;
		pressure = 1*atmosphere;
		temperature = 293.16*kelvin;
		G4Material* Air = new G4Material(name="Air"  , d, ncomponents=2, kStateGas, temperature, pressure);
		Air->AddElement(N, fractionmass=0.7);
		Air->AddElement(O, fractionmass=0.3);
	}

	if (materialName=="LPAir")
	{
		d = (5e-6/1013.)*1.290*mg/cm3; // 5e-6 mbar is the usual beam pipe air pressure
		pressure = 1*atmosphere;
		temperature = 293.16*kelvin;
		G4Material* LPAir = new G4Material(name="LPAir"  , d, ncomponents=3, kStateGas, temperature, pressure);
		LPAir->AddElement(N, fractionmass=0.715);
		LPAir->AddElement(O, fractionmass=0.25);
		LPAir->AddElement(Ar, fractionmass=0.035);
	}

	if (materialName=="Polyprop")
	{
		d = 0.9*g/cm3;
		G4Material* Polyprop = new G4Material(name = "Polyprop", d, nel = 2);
		Polyprop->AddElement (C,3);
		Polyprop->AddElement (H,6);
	}

	if (materialName=="Polyprop")
	{
		d = 3.44*g/cm3;
		G4Material* Si3N4 = new G4Material(name = "Si3N4", d, nel = 2);
		Si3N4->AddElement (Si, natoms=3);
		Si3N4->AddElement (N, natoms=4);
	}

	if (materialName=="SiO2")
	{
		d = 2.5*g/cm3;
		G4Material* SiO2 = new G4Material(name = "SiO2", d, nel = 2);
		SiO2->AddElement (Si, natoms=1);
		SiO2->AddElement (O, natoms=2);
	}

	if (materialName=="SiO2")
	{
		d = 8.5*g/cm3;
		G4Material* Laiton = new G4Material(name = "Laiton", d, nel = 2);
		Laiton->AddElement (Cu,1);
		Laiton->AddElement (Zn,1);
	}

	if (materialName=="Cytoplasm1")
	{
		d = 1*g/cm3;
		G4Material* Cytoplasm1 = new G4Material(name="Cytoplasm1"  , d, ncomponents=2);
		Cytoplasm1->AddElement(H, fractionmass=0.112);
		Cytoplasm1->AddElement(O, fractionmass=0.888);
	}

	if (materialName=="Nucleoli")
	{
		// in g/cm3 (nucleoli are assumed to have the same chemical comp. as nucleus)
		d = 1*g/cm3;
		G4Material* Cytoplasm2 = new G4Material(name="Cytoplasm2"  , d, ncomponents=5);
		Cytoplasm2->AddElement(H, fractionmass=0.1064);
		Cytoplasm2->AddElement(O, fractionmass=0.745);
		Cytoplasm2->AddElement(C, fractionmass=0.0904);
		Cytoplasm2->AddElement(N, fractionmass=0.0321);
		Cytoplasm2->AddElement(P, fractionmass=0.0261);
	}

	if (materialName=="Cytoplasm3")
	{
		d = 1*g/cm3;
		G4Material* Cytoplasm3 = new G4Material(name="Cytoplasm3"  , d, ncomponents=2);
		Cytoplasm3->AddElement(H, fractionmass=0.112);
		Cytoplasm3->AddElement(O, fractionmass=0.888);
	}

	if (materialName=="Cytoplasm3")
	{
		d = 1*g/cm3;
		G4Material* Nucleus1 = new G4Material(name="Nucleus1"  , d, ncomponents=5);
		Nucleus1->AddElement(H, fractionmass=0.1064);
		Nucleus1->AddElement(O, fractionmass=0.745);
		Nucleus1->AddElement(C, fractionmass=0.0904);
		Nucleus1->AddElement(N, fractionmass=0.0321);
		Nucleus1->AddElement(P, fractionmass=0.0261);
	}

	if (materialName=="Nucleus2")
	{
		d = 1*g/cm3;
		G4Material* Nucleus2 = new G4Material(name="Nucleus2"  , d, ncomponents=5);
		Nucleus2->AddElement(H, fractionmass=0.1064);
		Nucleus2->AddElement(O, fractionmass=0.745);
		Nucleus2->AddElement(C, fractionmass=0.0904);
		Nucleus2->AddElement(N, fractionmass=0.0321);
		Nucleus2->AddElement(P, fractionmass=0.0261);
	}

	if (materialName=="Nucleus3")
	{
		d = 1*g/cm3;
		G4Material* Nucleus3 = new G4Material(name="Nucleus3"  , d, ncomponents=5);
		Nucleus3->AddElement(H, fractionmass=0.1064);
		Nucleus3->AddElement(O, fractionmass=0.745);
		Nucleus3->AddElement(C, fractionmass=0.0904);
		Nucleus3->AddElement(N, fractionmass=0.0321);
		Nucleus3->AddElement(P, fractionmass=0.0261);
	}

	// DISPLAY MATERIALS
	G4cout << G4endl << *(G4Material::GetMaterialTable()) << G4endl;


	return material;
}
