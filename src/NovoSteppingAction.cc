/// \file NovoSteppingAction.cc
/// \brief Implementation of the NovoSteppingAction class
//
//
#include "NovoSteppingAction.hh"
#include "NovoEventAction.hh"
#include "NovoPhotocatSD.hh"
#include "NovoBSGatingSD.hh"
#include "NovoUserTrackInformation.hh"
#include "NovoUserEventInformation.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "Instrumentor.hh"


NovoSteppingAction::NovoSteppingAction()
  : fOneStepPrimaries(false)
{
	PROFILE_FUNCTION(); // for profiling
  fExpectedNextStatus = Undefined;
}

NovoSteppingAction::~NovoSteppingAction() {}


void NovoSteppingAction::UserSteppingAction(const G4Step * theStep)
{
	PROFILE_FUNCTION(); // for profiling
  //================================ Kyrres code =============================

	G4Track* theTrack = theStep->GetTrack();

	if ( theTrack->GetCurrentStepNumber() == 1 ) fExpectedNextStatus = Undefined;

	NovoUserTrackInformation* trackInformation = (NovoUserTrackInformation*)theTrack->GetUserInformation();
	NovoUserEventInformation* eventInformation = (NovoUserEventInformation*)G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation();

	G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
	G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

	G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
	G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

	G4OpBoundaryProcessStatus boundaryStatus=Undefined;
	static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;

	//find the boundary process only once
	if(!boundary){
		G4ProcessManager* pm = theStep->GetTrack()->GetDefinition()->GetProcessManager();
		G4int nprocesses = pm->GetProcessListLength();
		G4ProcessVector* pv = pm->GetProcessList();
		G4int i;
		for( i=0;i<nprocesses;i++){
			if((*pv)[i]->GetProcessName()=="OpBoundary"){
				boundary = (G4OpBoundaryProcess*)(*pv)[i];
				break;
			}
		}
	}

// Primary tracks
	if(theTrack->GetParentID()==0){
		//This is a primary track

		G4TrackVector* fSecondary=fpSteppingManager->GetfSecondary();
		G4int tN2ndariesTot = fpSteppingManager->GetfN2ndariesAtRestDoIt()
							+ fpSteppingManager->GetfN2ndariesAlongStepDoIt()
							+ fpSteppingManager->GetfN2ndariesPostStepDoIt();

		//If we havent already found the conversion position and there were
		//secondaries generated, then search for it
		if(!eventInformation->IsConvPosSet() && tN2ndariesTot>0 ){
			for(size_t lp1=(*fSecondary).size()-tN2ndariesTot; lp1<(*fSecondary).size(); lp1++){
				const G4VProcess* creator=(*fSecondary)[lp1]->GetCreatorProcess();
				if(creator){
					G4String creatorName=creator->GetProcessName();
					if(creatorName=="phot"||creatorName=="compt"||creatorName=="conv"){
						//since this is happening before the secondary is being tracked
						//the Vertex position has not been set yet(set in initial step)
						eventInformation->SetConvPos((*fSecondary)[lp1]->GetPosition());
					}
				}
			}
		}

		if(fOneStepPrimaries&&thePrePV->GetName()=="Scintillator"){
			theTrack->SetTrackStatus(fStopAndKill);
		}
	}

	if(!thePostPV){//out of world
		fExpectedNextStatus=Undefined;
		return;
	}

	G4ParticleDefinition* particleType = theTrack->GetDefinition(); // Check the particle type
  //G4cout << "type: "<<theTrack->GetParticleDefinition()->GetParticleName() << G4endl; // Check particle name


// ----- my code (start) -----------------------------------------------------
/// --- BS Gating tracking (start) ---GetPreStepPoint
//
G4LogicalVolume* volume = theStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume(); // get volume of the current step
G4String volumeName = volume->GetName(); // get volume name
G4String particleName = theStep->GetTrack()->GetParticleDefinition()->GetParticleName(); // get particle name
//G4cout << volumeName << G4endl;
if (volumeName == "BS_detectorLV"){//if we are in BS detector volume
  G4SDManager* SDman = G4SDManager::GetSDMpointer(); //??
  G4String sdName="bsSD";
  NovoBSGatingSD* bsSD = (NovoBSGatingSD*)SDman->FindSensitiveDetector(sdName);
  if(bsSD)bsSD->ProcessHits_constStep(theStep,NULL);
  trackInformation->AddTrackStatusFlag(hitPC); // what does this do?
}

if (volumeName == "ScintillatorLV"){//if we are in scintillator detector volume
  G4SDManager* SDman = G4SDManager::GetSDMpointer(); //??
  G4String sdName="scintSD";
  NovoBSGatingSD* scint_SD = (NovoBSGatingSD*)SDman->FindSensitiveDetector(sdName);
  //if(scint_SD)scint_SD->ProcessHits_constStep(theStep,NULL);
  if(scint_SD)scint_SD->ProcessHits_constStep(theStep,NULL);
  trackInformation->AddTrackStatusFlag(hitPC); // what does this do?
}

//
/// --- BS Gating tracking (end) ---GetPreStepPoint

// ----- my code (end) -------------------------------------------------------



	if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){ // Only optical photons -------------------------------------

		//~ if(thePostPV->GetName()=="Photocathode"){
			//~ G4SDManager* SDman = G4SDManager::GetSDMpointer();
			//~ G4String sdName="pcSD";
			//~ NovoPhotocatSD* pcSD = (NovoPhotocatSD*)SDman->FindSensitiveDetector(sdName);
			//~ if(pcSD)pcSD->ProcessHits_constStep(theStep,NULL);
			//~ //trackInformation->AddTrackStatusFlag(hitPC);
		//~ }
		//~ //Optical photon only

		if(thePrePV->GetName()=="Scintillator"){
			//force drawing of photons in WLS slab
			trackInformation->SetForceDrawTrajectory(true);
		} else if(thePostPV->GetName()=="world"){
			//Kill photons entering world
			theTrack->SetTrackStatus(fStopAndKill);
		}
		//Was the photon absorbed by the absorption process
		if(thePostPoint->GetProcessDefinedStep()->GetProcessName() =="OpAbsorption"){
			eventInformation->IncAbsorption();
			trackInformation->AddTrackStatusFlag(absorbed);
		}

		//~ //Was the photon absorbed by the absorption process
		//~ if(thePostPoint->GetProcessDefinedStep()->GetProcessName() =="OpAbsorption"){
			//~ eventInformation->IncAbsorption();
			//~ trackInformation->AddTrackStatusFlag(absorbed);
		//~ }

		boundaryStatus=boundary->GetStatus();
		//~ G4cout << "boudaryStatus:" << boundaryStatus << G4endl;
		//Check to see if the partcile was actually at a boundary
		//Otherwise the boundary status may not be valid
		//Prior to Geant4.6.0-p1 this would not have been enough to check
		if(thePostPoint->GetStepStatus()==fGeomBoundary){
			//~ G4cout << "aaa" << G4endl;
			//~ if(fExpectedNextStatus==StepTooSmall){
				//~ if(boundaryStatus!=StepTooSmall){
					//~ G4ExceptionDescription ed;
					//~ ed << "NovoSteppingAction::UserSteppingAction(): "
						//~ << "No reallocation step after reflection!"
						//~ << G4endl;
					//~ G4Exception("NovoSteppingAction::UserSteppingAction()", "NovoExpl01", FatalException,ed,
						//~ "Something is wrong with the surface normal or geometry");
				//~ }
			//~ }
			fExpectedNextStatus=Undefined;
			//~ G4cout << "bbb" << G4endl;
			switch(boundaryStatus){

				case Absorption:
					//~ G4cout << "Absorption" << G4endl;
					trackInformation->AddTrackStatusFlag(boundaryAbsorbed);
					eventInformation->IncBoundaryAbsorption();
					break;
				case Detection: //Note, this assumes that the volume causing detection
                      //is the photocathode because it is the only one with
                      //non-zero efficiency
				{
					//~ G4cout << "Detection" << G4endl;
					//Triger sensitive detector manually since photon is
					//absorbed but status was Detection
					G4SDManager* SDman = G4SDManager::GetSDMpointer();
					G4String sdName="pcSD";
					NovoPhotocatSD* pcSD = (NovoPhotocatSD*)SDman->FindSensitiveDetector(sdName);
					if(pcSD)pcSD->ProcessHits_constStep(theStep,NULL);
					trackInformation->AddTrackStatusFlag(hitPC);
					break;
				}
				case FresnelReflection:
				case TotalInternalReflection:
				case LambertianReflection:
				case LobeReflection:
				case SpikeReflection:
				case BackScattering:
					//~ G4cout << "Reflection etc" << G4endl;
					trackInformation->IncReflections();
					fExpectedNextStatus=StepTooSmall;
					break;
				default:
					//~ G4cout <<"default" << G4endl;
					break;
			}

		}
	} // END of optical photons

}
