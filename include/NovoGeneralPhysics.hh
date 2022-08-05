/// \file NovoGeneralPhysics.hh
/// \brief Definition of the NovoGeneralPhysics class
//
//
#ifndef NovoGeneralPhysics_h
#define NovoGeneralPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"

class NovoGeneralPhysics : public G4VPhysicsConstructor
{
  public:

    NovoGeneralPhysics(const G4String& name = "general");
    virtual ~NovoGeneralPhysics();

    // This method will be invoked in the Construct() method.
    // each particle type will be instantiated
    virtual void ConstructParticle();
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type
    virtual void ConstructProcess();
    
  private:
    G4VPhysicsConstructor* decPhysicsList;
	G4VPhysicsConstructor* radDecayPhysicsList;
	G4VPhysicsConstructor* ionBinaryCascadePhysicsList;
	G4VPhysicsConstructor* hadronElasticPhysicsList;
	G4VPhysicsConstructor* stoppingPhysicsList;
	G4VPhysicsConstructor* hadronQGSPBICPhysicsList;

};

#endif

