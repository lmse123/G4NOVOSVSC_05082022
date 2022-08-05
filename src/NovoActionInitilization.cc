/*
* G4Rangeverifcation: 
 * Action initialization
 * Source file
 * Author: Kyrre Skjerdal (kyrre.skjerdal@hvl.no)
 */

/// \file NovoActionInitialization.cc
/// \brief Implementation of the NovoActionInitialization class

#include "NovoActionInitialization.hh"
#include "NovoPrimaryGeneratorAction.hh"
#include "NovoRunAction.hh"
#include "NovoEventAction.hh"
#include "NovoSteppingAction.hh"
#include "NovoStackingAction.hh"
#include "NovoTrackingAction.hh"

// *** Constructor ***
NovoActionInitialization::NovoActionInitialization(NovoDetectorConstruction* det)
:	G4VUserActionInitialization(),
	fDetector(det)
{
}

// *** Destructor ***
NovoActionInitialization::~NovoActionInitialization()
{
}


// *** Build for master ***
void NovoActionInitialization::BuildForMaster() const
{
	//NovoRunAction* runAction = new NovoRunAction(fDetector, eventAction);
	//SetUserAction(runAction);
}

// *** Build ***
void NovoActionInitialization::Build() const
{
	
	NovoPrimaryGeneratorAction *primaryGeneratorAction = new NovoPrimaryGeneratorAction(fDetector);
	SetUserAction(primaryGeneratorAction);
	
	
	
	NovoEventAction* eventAction = new NovoEventAction(fDetector);
	SetUserAction(eventAction);
	
	NovoRunAction* runAction = new NovoRunAction(fDetector, eventAction);
	SetUserAction(runAction);
	
	NovoSteppingAction* steppingAction = new NovoSteppingAction();
	SetUserAction(steppingAction);
	
	NovoStackingAction* stackingAction = new NovoStackingAction();
	SetUserAction(stackingAction);

	NovoTrackingAction* trackingAction = new NovoTrackingAction();
	SetUserAction(trackingAction);
	
}

