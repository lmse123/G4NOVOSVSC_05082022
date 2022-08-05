/// \file NovoTrackingAction.hh
/// \brief Definition of the NovoTrackingAction class
//
//
#ifndef NovoTrackingAction_h
#define LXeTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"


class NovoTrackingAction : public G4UserTrackingAction {

  public:

    NovoTrackingAction();
    virtual ~NovoTrackingAction() {};

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:


};

#endif

