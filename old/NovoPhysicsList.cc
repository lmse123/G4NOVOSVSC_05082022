
/// \file NovoPhysicsList.cc
/// \brief Implementation of the NovoPhysicsList class
//
//
#include "NovoPhysicsList.hh"

#include "NovoGeneralPhysics.hh"
#include "NovoEMPhysics.hh"
#include "NovoOpticalPhysics.hh"

#include "G4OpticalPhysics.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4Scintillation.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NovoPhysicsList::NovoPhysicsList() : G4VModularPhysicsList()
{
	// default cut value  (1.0mm)
	defaultCutValue = 1.0*mm;

	// General Physics
	NovoGeneralPhysics* gen = new NovoGeneralPhysics("general");
	RegisterPhysics( gen );

	// EM Physics
	RegisterPhysics( new NovoEMPhysics("standard EM"));

	// Optical Phyiscs
	RegisterPhysics( new NovoOpticalPhysics("optical"));

	//~ // Optical Physics
	//~ G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  
	//~ RegisterPhysics( opticalPhysics );//
	//~ opticalPhysics->Configure(kScintillation, true);
	//~ opticalPhysics->SetWLSTimeProfile("delta");
	
	//~ opticalPhysics->SetScintillationYieldFactor(1.0);
	//~ opticalPhysics->SetScintillationExcitationRatio(0.0);

	//~ opticalPhysics->SetMaxNumPhotonsPerStep(100);
	//~ opticalPhysics->SetMaxBetaChangePerStep(10.0);

	//~ opticalPhysics->SetTrackSecondariesFirst(kCerenkov,true);
	//~ opticalPhysics->SetTrackSecondariesFirst(kScintillation,true);
	//~ G4Scintillation* scint = opticalPhysics->GetScintillationProcess();
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NovoPhysicsList::~NovoPhysicsList() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NovoPhysicsList::SetCuts(){
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();
}

