
/// \file NovoPhysicsListSingular.hh
/// \brief Definition of the NovoPhysicsListSingular class

#ifndef NovoPhysicsListSingular_h
#define NovoPhysicsListSingular_h 1

#include "globals.hh"
#include "G4VUserPhysicsList.hh"
#include "G4VPhysicsConstructor.hh"

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;

class NovoPhysicsListSingular : public G4VUserPhysicsList
{
	public:

		NovoPhysicsListSingular();
		virtual ~NovoPhysicsListSingular();

	public:

		virtual void ConstructParticle();
		virtual void ConstructProcess();

		virtual void SetCuts();

		//these methods Construct physics processes and register them
		void ConstructDecay();
		void ConstructEM();
		void ConstructOp();

		//~ //for the Messenger 
		void SetVerbose(G4int);
		void SetNbOfPhotonsCerenkov(G4int);
 
	private:

		//~ OpNovicePhysicsListMessenger* fMessenger;
		
		G4VPhysicsConstructor* radDecayPhysicsList;
		G4VPhysicsConstructor* ionBinaryCascadePhysicsList;
		G4VPhysicsConstructor* hadronElasticPhysicsList;
		G4VPhysicsConstructor* stoppingPhysicsList;
		G4VPhysicsConstructor* hadronQGSPBICPhysicsList;

		static G4ThreadLocal G4int fVerboseLevel;
		static G4ThreadLocal G4int fMaxNumPhotonStep;

		static G4ThreadLocal G4Cerenkov* fCerenkovProcess;
		static G4ThreadLocal G4Scintillation* fScintillationProcess;
		static G4ThreadLocal G4OpAbsorption* fAbsorptionProcess;
		static G4ThreadLocal G4OpRayleigh* fRayleighScatteringProcess;
		static G4ThreadLocal G4OpMieHG* fMieHGScatteringProcess;
		static G4ThreadLocal G4OpBoundaryProcess* fBoundaryProcess;
};


#endif
