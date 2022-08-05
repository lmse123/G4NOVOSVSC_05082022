/// \file NovoGeneralPhysics.cc
/// \brief Implementation of the NovoGeneralPhysics class
//
//
#include "NovoGeneralPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>
#include "G4Decay.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "G4Geantino.hh"
#include "G4ChargedGeantino.hh"

#include "G4GenericIon.hh"

#include "G4Proton.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4StoppingPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4DecayPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NovoGeneralPhysics::NovoGeneralPhysics(const G4String& name)
                     :  G4VPhysicsConstructor(name) 
{
	decPhysicsList = new G4DecayPhysics();
	radDecayPhysicsList = new G4RadioactiveDecayPhysics();
	ionBinaryCascadePhysicsList = new G4IonBinaryCascadePhysics();
	hadronElasticPhysicsList = new G4HadronElasticPhysicsHP();
	stoppingPhysicsList = new G4StoppingPhysics();
	hadronQGSPBICPhysicsList = new G4HadronPhysicsQGSP_BIC_HP();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NovoGeneralPhysics::~NovoGeneralPhysics() 
{
  //fDecayProcess = NULL;
	delete decPhysicsList;
	delete radDecayPhysicsList;
	delete ionBinaryCascadePhysicsList;
	delete hadronElasticPhysicsList;
	delete stoppingPhysicsList;
	delete hadronQGSPBICPhysicsList;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



void NovoGeneralPhysics::ConstructParticle()
{
  // pseudo-particles
  //~ G4Geantino::GeantinoDefinition();
  //~ G4ChargedGeantino::ChargedGeantinoDefinition();

  //~ G4GenericIon::GenericIonDefinition();
  decPhysicsList -> ConstructParticle();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NovoGeneralPhysics::ConstructProcess()
{
	decPhysicsList->ConstructProcess();
	radDecayPhysicsList->ConstructProcess();
	ionBinaryCascadePhysicsList->ConstructProcess();
	hadronElasticPhysicsList->ConstructProcess();
	stoppingPhysicsList->ConstructProcess();
	hadronQGSPBICPhysicsList->ConstructProcess();
}
