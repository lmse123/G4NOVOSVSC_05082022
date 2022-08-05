/// \file NovoSteppingAction.hh
/// \brief Definition of the NovoSteppingAction class
//
#ifndef NovoSteppingAction_H
#define NovoSteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include "G4OpBoundaryProcess.hh"


class NovoEventAction;
class NovoTrackingAction;

class NovoSteppingAction : public G4UserSteppingAction
{
  public:

    NovoSteppingAction();
    virtual ~NovoSteppingAction();
    virtual void UserSteppingAction(const G4Step*);

    void SetOneStepPrimaries(G4bool b){fOneStepPrimaries=b;}
    G4bool GetOneStepPrimaries(){return fOneStepPrimaries;}
 
  private:

    G4bool fOneStepPrimaries;

    G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif

