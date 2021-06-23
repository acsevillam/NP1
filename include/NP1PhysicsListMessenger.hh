//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software 
// shall cite the following Geant4-DNA collaboration publication:
// Med. Phys. 37 (2010) 4692-4708
// The Geant4-DNA web site is available at http://geant4-dna.org
// 
// If you use this example, please cite the following publication:
// Rad. Prot. Dos. 133 (2009) 2-11

#ifndef NP1PhysicsListMessenger_h
#define NP1PhysicsListMessenger_h 1

#include "G4UImessenger.hh"

class NP1PhysicsListFactory;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;

class NP1PhysicsListMessenger: public G4UImessenger
{
  public:
  
    NP1PhysicsListMessenger(NP1PhysicsListFactory* );
   ~NP1PhysicsListMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    NP1PhysicsListFactory* fPhysicsList;
    
    G4UIdirectory*             fPhysDir;        
    G4UIcmdWithADoubleAndUnit* fGammaCutCmd;
    G4UIcmdWithADoubleAndUnit* fElectCutCmd;
    G4UIcmdWithADoubleAndUnit* fProtoCutCmd;    
    G4UIcmdWithADoubleAndUnit* fAllCutCmd;    
    G4UIcmdWithAString*        fListCmd;
    
};

#endif // NP1PhysicsListMessenger_h

