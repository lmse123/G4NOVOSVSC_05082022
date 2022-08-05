
/// \file NovoOpticalPhysics.cc
/// \brief Implementation of the NovoOpticalPhysics class
//


#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

#include "NovoOpticalPhysics.hh"


NovoOpticalPhysics::NovoOpticalPhysics(const G4String& name)
    : G4VPhysicsConstructor(name)
{
	fWLSProcess                = NULL;
	fScintProcess              = NULL;
	fCerenkovProcess           = NULL;
	fBoundaryProcess           = NULL;
	fAbsorptionProcess         = NULL;
	fRayleighScattering        = NULL;
	fMieHGScatteringProcess    = NULL;
}



NovoOpticalPhysics::~NovoOpticalPhysics() { }

#include "G4OpticalPhoton.hh"

void NovoOpticalPhysics::ConstructParticle()
{
	G4OpticalPhoton::OpticalPhotonDefinition();
}

#include "G4ProcessManager.hh"

void NovoOpticalPhysics::ConstructProcess()
{
	G4cout << "NovoOpticalPhysics:: Add Optical Physics Processes"
           << G4endl;
	fWLSProcess = new G4OpWLS();

	fScintProcess = new G4Scintillation();
	//fScintProcess->SetScintillationYieldFactor(1.);
	//fScintProcess->SetTrackSecondariesFirst(true);

	fCerenkovProcess = new G4Cerenkov();
	fCerenkovProcess->SetMaxNumPhotonsPerStep(300);
	fCerenkovProcess->SetTrackSecondariesFirst(true);

	fAbsorptionProcess      = new G4OpAbsorption();
	fRayleighScattering     = new G4OpRayleigh();
	fMieHGScatteringProcess = new G4OpMieHG();
	fBoundaryProcess        = new G4OpBoundaryProcess();

	G4ProcessManager* pManager =
                G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

	if (!pManager) {
		std::ostringstream o;
		o << "Optical Photon without a Process Manager";
		G4Exception("WLSOpticalPhysics::ConstructProcess()","",
                  FatalException,o.str().c_str());
	}

	pManager->AddDiscreteProcess(fAbsorptionProcess);

	pManager->AddDiscreteProcess(fRayleighScattering);
	pManager->AddDiscreteProcess(fMieHGScatteringProcess);

	pManager->AddDiscreteProcess(fBoundaryProcess);

	fWLSProcess->UseTimeProfile("delta");
	fWLSProcess->UseTimeProfile("exponential");

	pManager->AddDiscreteProcess(fWLSProcess);

	fScintProcess->SetScintillationYieldFactor(1.);
	fScintProcess->SetScintillationExcitationRatio(0.0);
	fScintProcess->SetTrackSecondariesFirst(true);
	fScintProcess->SetScintillationByParticleType(false);

	// Use Birks Correction in the Scintillation process

	G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
	fScintProcess->AddSaturation(emSaturation);
	G4String scintstr = "";
	auto particleIterator=GetParticleIterator();
	particleIterator->reset();
	while ( (*particleIterator)() ){

		G4ParticleDefinition* particle = particleIterator->value();
		G4String particleName = particle->GetParticleName();

		pManager = particle->GetProcessManager();
		if (!pManager) {
			std::ostringstream o;
			o << "Particle " << particleName << "without a Process Manager";
			G4Exception("WLSOpticalPhysics::ConstructProcess()","",
                    FatalException,o.str().c_str());
		}	

		if(fCerenkovProcess->IsApplicable(*particle)){
			pManager->AddProcess(fCerenkovProcess);
			pManager->SetProcessOrdering(fCerenkovProcess,idxPostStep);
		}
		if(fScintProcess->IsApplicable(*particle)){
			scintstr += particle->GetParticleName();
			scintstr += " ";
			pManager->AddProcess(fScintProcess);
			pManager->SetProcessOrderingToLast(fScintProcess,idxAtRest);
			pManager->SetProcessOrderingToLast(fScintProcess,idxPostStep);
		}

	}
	G4cout << "Scintillation set for " <<  scintstr << G4endl; 
}

void NovoOpticalPhysics::SetNbOfPhotonsCerenkov(G4int maxNumber)
{
	fCerenkovProcess->SetMaxNumPhotonsPerStep(maxNumber);
}
