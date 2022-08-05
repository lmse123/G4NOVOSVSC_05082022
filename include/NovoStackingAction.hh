/// \file NovoStackingAction.hh
/// \brief Definition of the NovoStackingAction class
//
//
#ifndef NovoStackingAction_H
#define NovoStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class NovoStackingAction : public G4UserStackingAction
{
  public:

    NovoStackingAction();
    virtual ~NovoStackingAction();
 
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();
 
  private:
};

#endif
