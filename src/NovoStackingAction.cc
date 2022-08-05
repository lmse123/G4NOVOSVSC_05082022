/// \file NovoStackingAction.cc
/// \brief Implementation of the NovoStackingAction class
//
//
#include "NovoStackingAction.hh"
#include "NovoUserEventInformation.hh"
#include "NovoSteppingAction.hh"

#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"



NovoStackingAction::NovoStackingAction() {}



NovoStackingAction::~NovoStackingAction() {}



G4ClassificationOfNewTrack NovoStackingAction::ClassifyNewTrack(const G4Track * aTrack){
 
  NovoUserEventInformation* eventInformation=
    (NovoUserEventInformation*)G4EventManager::GetEventManager()
    ->GetConstCurrentEvent()->GetUserInformation();
 
  //Count what process generated the optical photons
  if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){
    // particle is optical photon
    if(aTrack->GetParentID()>0){
      // particle is secondary
      if(aTrack->GetCreatorProcess()->GetProcessName()=="Scintillation")
        eventInformation->IncPhotonCount_Scint();
      else if(aTrack->GetCreatorProcess()->GetProcessName()=="Cerenkov")
        eventInformation->IncPhotonCount_Ceren();
    }
  }
  else{
  }
  return fUrgent;
}


void NovoStackingAction::NewStage() {}



void NovoStackingAction::PrepareNewEvent() {}

