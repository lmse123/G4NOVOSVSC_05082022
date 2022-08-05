/// \file NovoPhysicsList.hh
/// \brief Definition of the NovoPhysicsList class
//
//
#ifndef NovoPhysicsList_h
#define NovoPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class NovoPhysicsList: public G4VModularPhysicsList
{
  public:

    NovoPhysicsList();
    virtual ~NovoPhysicsList();
  protected:
    // SetCuts()
    virtual void SetCuts();
    


};

#endif
