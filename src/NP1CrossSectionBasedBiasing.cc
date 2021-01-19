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
/// \file GB01/src/NP1CrossSectionBasedBiasing.cc
/// \brief Implementation of the NP1CrossSectionBasedBiasing class
//
#include "NP1ChangeCrossSection.hh"
#include "G4BiasingProcessInterface.hh"

#include "NP1CrossSectionBasedBiasing.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NP1CrossSectionBasedBiasing::NP1CrossSectionBasedBiasing()
  : G4VBiasingOperator("TestManyExponentialTransform")
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NP1CrossSectionBasedBiasing::AddParticle(G4String particleName)
{
  const G4ParticleDefinition* particle =
    G4ParticleTable::GetParticleTable()->FindParticle( particleName );
  
  if ( particle == 0 )
    {
      G4ExceptionDescription ed;
      ed << "Particle `" << particleName << "' not found !" << G4endl;
      G4Exception("NP1CrossSectionBasedBiasing::AddParticle(...)","exNP1.02",JustWarning,ed);
      return;
    }
  
  NP1ChangeCrossSection* optr = new NP1ChangeCrossSection(particleName);
  fParticlesToBias.push_back( particle );
  fBOptrForParticle[ particle ] = optr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VBiasingOperation*
NP1CrossSectionBasedBiasing::
ProposeOccurenceBiasingOperation(const G4Track* track,
                                 const G4BiasingProcessInterface* callingProcess)
{
  // -- examples of limitations imposed to apply the biasing:
  // -- limit application of biasing to primary particles only:
  if ( track->GetTrackID() != 1 ) return 0;
  
  if ( fCurrentOperator ) return fCurrentOperator->
                            GetProposedOccurenceBiasingOperation(track, callingProcess);
  else                    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NP1CrossSectionBasedBiasing::StartTracking( const G4Track* track )
{
  // -- fetch the underneath biasing operator, if any, for the current particle type:
  const G4ParticleDefinition* definition = track->GetParticleDefinition();
  std::map < const G4ParticleDefinition*, NP1ChangeCrossSection* > :: iterator
    it = fBOptrForParticle.find( definition );
  fCurrentOperator = 0;
  if ( it != fBOptrForParticle.end() ) fCurrentOperator = (*it).second;

  // -- reset count for number of biased interactions:
  fnInteractions = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void 
NP1CrossSectionBasedBiasing::
OperationApplied( const G4BiasingProcessInterface*               callingProcess, 
                  G4BiasingAppliedCase                              biasingCase,
                  G4VBiasingOperation*                occurenceOperationApplied, 
                  G4double                        weightForOccurenceInteraction,
                  G4VBiasingOperation*               finalStateOperationApplied, 
                  const G4VParticleChange*               particleChangeProduced )
{
  // -- count number of biased interactions:
  fnInteractions++;

  // -- inform the underneath biasing operator that a biased interaction occured:
  if ( fCurrentOperator ) fCurrentOperator->ReportOperationApplied( callingProcess,
                                                                    biasingCase,
                                                                    occurenceOperationApplied,
                                                                    weightForOccurenceInteraction,
                                                                    finalStateOperationApplied,
                                                                    particleChangeProduced );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
