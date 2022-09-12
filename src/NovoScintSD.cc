/// \file NovoScintSD.cc
/// \brief Implementation of the NovoScintSD class
//
//
#include "NovoScintSD.hh"
#include "NovoScintHit.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "Instrumentor.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NovoScintSD::NovoScintSD(G4String name)
  : G4VSensitiveDetector(name),
  	fScintCollection(0),
	fScintPositionsX(0),
	fScintPositionsY(0),
	fScintPositionsZ(0)
{
	PROFILE_FUNCTION(); // for profiling
	// fScintCollection = NULL; // TODO: delete, its old.
	collectionName.insert("scintCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NovoScintSD::~NovoScintSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NovoScintSD::SetScintPositions(const std::vector<G4ThreeVector>& positions)
{
	PROFILE_FUNCTION(); // for profiling
	for (G4int i=0; i<G4int(positions.size()); i++) {
		// G4cout << "i: " << i << G4endl;
		if(fScintPositionsX){
			fScintPositionsX->push_back(positions[i].x());
		}
		if(fScintPositionsY){
			fScintPositionsY->push_back(positions[i].y());
		}
		if(fScintPositionsZ){
			fScintPositionsZ->push_back(positions[i].z());
		}
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NovoScintSD::Initialize(G4HCofThisEvent* hitsCE)
{
	PROFILE_FUNCTION(); // for profiling
	fScintCollection = new NovoScintHitsCollection(SensitiveDetectorName,collectionName[0]);
	//A way to keep all the hits of this event in one place if needed
	static G4int hitsCID = -1;
	if(hitsCID<0){
		hitsCID = GetCollectionID(0);
	}
	hitsCE->AddHitsCollection( hitsCID, fScintCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool NovoScintSD::ProcessHits(G4Step* aStep,G4TouchableHistory* ){
	// G4double edep = aStep->GetTotalEnergyDeposit();
	// if(edep==0.) return false; //No edep so dont count as hit
  //
	// G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
	// G4TouchableHistory* theTouchable = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
	// G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();
  //
	// G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
  //
	// //Get the average position of the hit
	// G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
	// pos/=2.;
	// G4double globaltime = thePrePoint->GetGlobalTime();
	// NovoScintHit* scintHit = new NovoScintHit(thePrePV);
  //
	// scintHit->SetEdep(edep);
	// scintHit->SetPos(pos);
	// scintHit->SetTime(globaltime);
	// fScintCollection->insert(scintHit);
  //
	// return true;
	return false;
}

G4bool NovoScintSD::ProcessHits_constStep(const G4Step* aStep,G4TouchableHistory* ){
	PROFILE_FUNCTION(); // for profiling
	
	G4int    fVerbose = 0; // if > 0 --> print out hit information. 
	G4int    parentID = aStep->GetTrack()->GetParentID(); //ParentID = 0 = primary particle
	G4double     edep = aStep->GetTotalEnergyDeposit();
	G4String particleName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName(); // Particle name

	// if(parentID != 0){return false;} // if not a primary particle, skip.
	if (edep == 0)                      {return false;} // if no energy deposit, skip. 
	// if (particleName == "opticalphoton"){return false;}

	// get step information of interest
	G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
	G4String  volPost = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
	G4String   volPre = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
	G4int     pdgCode = aStep->GetTrack()->GetDynamicParticle()->GetPDGcode();


	// TODO: make a particle dictionary, or use GetPDGEncoding().
	G4int particleID = 999;
	// G4cout << "- NEW HIT - - - - - - - - - - - - - - - - - " << G4endl;
	// G4cout << "particleName: " << particleName << G4endl;
	if (particleName == "opticalphoton"){particleID = 1001;}
	if (particleName == "gamma"){particleID = 1; }
	if (particleName == "proton"){particleID = 2;} // ?
	if (particleName == "e-"){particleID = 3;} // ?
	
	// Has this scintillator been hit before? Find the already exisitng hit or create a new hit

	G4int scintNumber = aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber();
	// G4int scintNumber = aStep->GetPostStepPoint()->GetTouchable()->GetCopyNumber(); // this returns the wrong copy number when the post step volume is a photocathode instead of the scintillator
	G4int n = fScintCollection->entries();
	NovoScintHit* scintHit = NULL;
	for(G4int i=0;i<n;i++){
		if((*fScintCollection)[i]->GetScintNumber()==scintNumber){
			scintHit=(*fScintCollection)[i];
			if(fVerbose > 0){G4cout << "- Hit -> " << i << " <- in collection -" << G4endl;}
			break;
		}
	}
	if(scintHit==NULL){//this scint wasnt previously hit in this event
		if(fVerbose > 0){G4cout << "- NEW HIT - - - - - - - - - - - - - - - - - " << G4endl;}
		scintHit = new NovoScintHit(); //so create new hit
		fScintCollection->insert(scintHit);
		scintHit->SetScintNumber(scintNumber);
		scintHit->SetScintPos((*fScintPositionsX)[scintNumber], (*fScintPositionsY)[scintNumber], (*fScintPositionsZ)[scintNumber]);
		scintHit->SetParentID(parentID);       //
		scintHit->SetParticleID(particleID);   //
	}

	// ======= STORE / UPDATE HIT INFORMATION  ==========

	// store information on the secondaries
	const std::vector<const G4Track*>* secondaries = aStep->GetSecondaryInCurrentStep(); // returns the secondaries created in this specific step
	G4int optPhotonCount = 0; G4int electronCount = 0;
	G4int nSecondaries = secondaries->size();
	if (nSecondaries>0){
		for (G4int i = 0; i < nSecondaries ; i ++){
			const G4Track * sTrack = (*secondaries)[i];
			G4String name = sTrack->GetParticleDefinition()->GetParticleName();
			// G4ThreeVector mom = sTrack->GetMomentumDirection();
			// G4ThreeVector sPos = sTrack->GetPosition();
			// G4double theta = mom.theta();
			// G4double phi = mom.phi();   
			
			// scintHit->SetOpticalPhotonTheta(theta);
			// scintHit->SetOpticalPhotonPhi(phi);
			// // scintHit->SetOpticalPhotonPosition(sPos); // cannot store a vector in another vector
			// scintHit->SetOpticalPhotonX(sPos.x()); 
			// scintHit->SetOpticalPhotonY(sPos.y()); 
			// scintHit->SetOpticalPhotonZ(sPos.z()); 
			
			if (name == "opticalphoton" ){optPhotonCount++;} // count optical photons produced in the hit
			if (name == "e-" ){electronCount++;} // count electrons produced in the hit
		}
	}

	// Store info on optical photons
	if (particleName == "opticalphoton"){
		G4ThreeVector sPos = aStep->GetTrack()->GetPosition();
		G4ThreeVector mom = aStep->GetTrack()->GetMomentumDirection();
		G4double theta = mom.theta(); // scatterign angle - theta
		G4double phi = mom.phi();     // scatterign angle - phi
		G4double stepNr = aStep->GetTrack()->GetCurrentStepNumber();     // scatterign angle - phi
		
		scintHit->SetOpticalPhotonX(sPos.x()); 
		scintHit->SetOpticalPhotonY(sPos.y()); 
		scintHit->SetOpticalPhotonZ(sPos.z()); 
		scintHit->SetOpticalPhotonStepNr(stepNr); 
		scintHit->SetOpticalPhotonTheta(theta);
		scintHit->SetOpticalPhotonPhi(phi);
	}


	// Increase values of the event
	scintHit->IncEdep(edep);
	scintHit->IncElectronCount(electronCount);// IncElectronCount
	scintHit->IncOpticalPhotonCount(optPhotonCount);// IncOpticalPhotonCount


	if(fVerbose > 0){
	// info on secondaries produced
		G4cout << "scint #: " << scintNumber << G4endl;
		G4cout << "pre vol : " << volPre << G4endl;
		G4cout << "post vol : " << volPost << G4endl;
		G4cout << "parent ID: " << parentID << G4endl;
		G4cout << "trackID: " << aStep->GetTrack()->GetTrackID() << G4endl;
		G4cout << "current step number: " << aStep->GetTrack()->GetCurrentStepNumber() << G4endl;
		G4cout << "x,y,z: " << pos << G4endl;
		G4cout << "particleName: " << particleName << G4endl;
		G4cout << "PDGcode: " << aStep->GetTrack()->GetDynamicParticle()->GetPDGcode() << G4endl;
		// auto process = aStep->GetTrack()->GetCreatorProcess();
		// G4String processName = process->GetProcessName();
		// if (aStep->GetTrack()->GetCreatorProcess() && aStep->GetTrack()->GetCreatorProcess()->GetProcessName() != NULL){
		// // if (process && processName != NULL){
		// 	G4cout << "creator process: " << aStep->GetTrack()->GetCreatorProcess()->GetProcessName() << G4endl;
		// }
		// else G4cout << "creator process: none" << G4endl;
		G4cout << "edep: "<< edep << G4endl;
		G4cout << "incEdep: "<< scintHit->GetEdep() << G4endl; 
		G4cout << "opt.phot. count: " <<scintHit->GetOpticalPhotonCount() << G4endl; 
		G4cout << "opt.phot. (new): " <<optPhotonCount << G4endl; 
		G4cout << "electron. count: " <<electronCount << G4endl; 
		G4cout << "secondaries: "<< G4endl; 


		G4cout << "> n: " <<nSecondaries<< G4endl;
		if (nSecondaries>0){
			
			for (G4int i = 0; i < nSecondaries ; i ++){
				const G4Track * sTrack = (*secondaries)[i];
				G4String name = sTrack->GetParticleDefinition()->GetParticleName();
				// G4ThreeVector sPos = sTrack->GetStep()->GetPostStepPoint()->GetPosition(); // gives segmentation fault
				// G4ThreeVector sPos = sTrack->GetVertexPosition();
				G4ThreeVector sPos = sTrack->GetPosition();
				// G4ThreeVector mom = sTrack->GetVertexMomentumDirection();
				G4ThreeVector mom = sTrack->GetMomentumDirection();
				G4double theta = mom.theta();
				G4double phi = mom.phi();

				G4cout << "> name: " <<name << G4endl;
				G4cout << "> x,y,z: " << sPos << G4endl;
				G4cout << "> momentum: " << mom << G4endl;
				G4cout << "> theta: " << theta << " rad (" << theta*180/3.14 << " deg)" << G4endl;
				// G4cout << ">> atan(1)': " << atan(1) << " rad (" << atan(1)*180/3.14 << "deg)"<<G4endl; // test the math and unit conversion
				G4cout << "> phi: " << phi << " rad (" << phi*180/3.14 << " deg)" << G4endl;
			
				// G4cout << "> Px: " << -sin(theta)*cos(phi) <<  G4endl;
				// G4cout << "> Py: " << -sin(theta)*sin(phi) <<  G4endl;
				// G4cout << "> Pz: " << -cos(theta) <<  G4endl;
				// G4cout << "> angle': " << mom.angle()<<  G4endl; //?

				// if ((*secondaries)[i]->GetCreatorProcess() && (*secondaries)[i]->GetCreatorProcess()->GetProcessName() != NULL){
				// 	G4cout << "> creator process: " << (*secondaries)[i]->GetCreatorProcess()->GetProcessName() << G4endl;
				// }
				// else G4cout << "creator process: none" << G4endl;
			}
		}
	}
	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NovoScintSD::EndOfEvent(G4HCofThisEvent* ) 
{
	PROFILE_FUNCTION(); // for profiling
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NovoScintSD::clear() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NovoScintSD::DrawAll() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NovoScintSD::PrintAll() {}
