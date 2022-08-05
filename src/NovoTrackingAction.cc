/// \file NovoTrackingAction.cc
/// \brief Implementation of the NovoTrackingAction class
//
//
//#include "LXeTrajectory.hh"
#include "NovoTrackingAction.hh"
#include "NovoUserTrackInformation.hh"
#include "NovoDetectorConstruction.hh"
#include "NovoAnalysis.hh"
//#include "LXeRecorderBase.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"


NovoTrackingAction::NovoTrackingAction()
{}


void NovoTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
	//Let this be up to the user via vis.mac
	//  fpTrackingManager->SetStoreTrajectory(true);

	//Use custom trajectory class
	//fpTrackingManager->SetTrajectory(new LXeTrajectory(aTrack));

	//This user track information is only relevant to the photons
	fpTrackingManager->SetUserTrackInformation(new NovoUserTrackInformation);

	/*  const G4VProcess* creator = aTrack->GetCreatorProcess();
	if(creator)
		G4cout<<creator->GetProcessName()<<G4endl;
	*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NovoTrackingAction::PostUserTrackingAction(const G4Track* aTrack){
	auto analysisManager = G4AnalysisManager::Instance();
	NovoUserTrackInformation* trackInformation = (NovoUserTrackInformation*)aTrack->GetUserInformation();
	if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){
		if(trackInformation->GetTrackStatus()&hitPC){
			//G4cout << "Reflections for this track: " << trackInformation->GetReflectionCount() << G4endl;
			analysisManager->FillH1(0, trackInformation->GetReflectionCount());
		}
	}
}	


