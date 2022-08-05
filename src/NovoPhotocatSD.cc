/// \file NovoPhotocatSD.cc
/// \brief Implementation of the NovoPhotocatSD class
//
//
#include "NovoPhotocatSD.hh"
#include "NovoPhotocatHit.hh"
#include "NovoDetectorConstruction.hh"
#include "NovoUserTrackInformation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

NovoPhotocatSD::NovoPhotocatSD(G4String name)
  : G4VSensitiveDetector(name),
	fPhotocatHitCollection(0),
	fPCPositionsX(0),
	fPCPositionsY(0),
	fPCPositionsZ(0)
{
	collectionName.insert("photocatHitCollection");
}


NovoPhotocatSD::~NovoPhotocatSD()
{
}

void NovoPhotocatSD::SetPCPositions(const std::vector<G4ThreeVector>& positions)
{
	for (G4int i=0; i<G4int(positions.size()); i++) {
		// G4cout << "i: " << i << G4endl;
		if(fPCPositionsX){
			fPCPositionsX->push_back(positions[i].x());
			//G4cout << "positions[i].x(): " << positions[i].x() << G4endl;
			//G4cout << "positions[i].x(): " <<  (*fPCPositionsX)[i] << G4endl;
		}
		if(fPCPositionsY){
			fPCPositionsY->push_back(positions[i].y());
			//G4cout << "positions[i].y(): " << positions[i].y() << G4endl;
			//G4cout << "positions[i].y(): " <<  (*fPCPositionsY)[i] << G4endl;
		}
		if(fPCPositionsZ){
			fPCPositionsZ->push_back(positions[i].z());
			//fPCPositionsZ->push_back(1234);
			//G4cout << "positions[i].z(): " << positions[i].z() << G4endl;
			//G4cout << "positions[i].z(): " <<  (*fPCPositionsZ)[i] << G4endl;
			//G4cout << "fPCPositionsZ: " << fPCPositionsZ->at(i) << G4endl;
		}
	}
}

void NovoPhotocatSD::Initialize(G4HCofThisEvent* hitsCE){
	fPhotocatHitCollection = new NovoPhotocatHitsCollection(SensitiveDetectorName,collectionName[0]);
	//Store collection with event and keep ID
	static G4int hitCID = -1;
	if(hitCID<0){
		hitCID = GetCollectionID(0);
	}
	hitsCE->AddHitsCollection( hitCID, fPhotocatHitCollection );
}


G4bool NovoPhotocatSD::ProcessHits(G4Step* aStep,G4TouchableHistory* ){


	return false;
}

//Generates a hit and uses the postStepPoint's mother volume replica number
//PostStepPoint because the hit is generated manually when the photon is
//absorbed by the photocathode

G4bool NovoPhotocatSD::ProcessHits_constStep(const G4Step* aStep, G4TouchableHistory* )
{

	//need to know if this is an optical photon
	if(aStep->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()){
		return false;
	}
	//G4cout << "Optical photon :)" << G4endl;
	G4double time = aStep->GetTrack()->GetGlobalTime();
	G4double edep = aStep->GetTotalEnergyDeposit();

	// get pre/post step position. Will this sometimes be outside the volume? yes, if pre.
	G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
	// G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
	G4double posX = pos.x();
	G4double posY = pos.y();
	G4double posZ = pos.z();
	//~ G4int pcNumber=
		//~ aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber();
	G4int pcNumber= aStep->GetPostStepPoint()->GetTouchable()->GetCopyNumber();
	//~ G4cout << "Copy number " << pcNumber << G4endl;
	//~ G4cout << "Pos: " << (*fPCPositionsX)[pcNumber] << " " << (*fPCPositionsY)[pcNumber] << " " << (*fPCPositionsZ)[pcNumber] << G4endl;
	//~ G4ThreeVector trans = aStep->GetPostStepPoint()->GetTouchable()->GetTranslation();
	//~ G4VPhysicalVolume* physVol= aStep->GetPostStepPoint()->GetTouchable()->GetVolume(pcNumber);
	//~ G4cout << "physVol: " << physVol->GetName() << G4endl;
	
	//Find the correct hit collection
	G4int n=fPhotocatHitCollection->entries();
	NovoPhotocatHit* hit=NULL;
	for(G4int i=0;i<n;i++){
		if((*fPhotocatHitCollection)[i]->GetPhotocatNumber()==pcNumber){
			hit=(*fPhotocatHitCollection)[i];
			break;
		}
	}
	if(hit==NULL){//this pc wasnt previously hit in this event
		hit = new NovoPhotocatHit(); //so create new hit
		hit->SetPhotocatNumber(pcNumber);
		//~ hit->SetPhotocatPhysVol(physVol);
		fPhotocatHitCollection->insert(hit);
		hit->SetPhotocatPos((*fPCPositionsX)[pcNumber], (*fPCPositionsY)[pcNumber], (*fPCPositionsZ)[pcNumber]);
	}
	//G4cout << "(*fPCPositionsZ)[pcNumber]: " <<  (*fPCPositionsZ)[pcNumber] << G4endl;
	hit->IncPhotonCount(); //increment hit for the selected pc
	hit->SetTime(time);
	hit->IncEdep(edep);
	hit->SetHitPosX(posX);
	hit->SetHitPosY(posY);
	hit->SetHitPosZ(posZ);
	hit->SetPhotonE(aStep->GetTrack()->GetKineticEnergy());
	//~ G4cout << "PhotocatSD:\t ReplicaNumber : " << pcNumber << " CopyNumber: " << pcNumber2 << " z-translation: " << trans.z() << G4endl;

	return true;
}

void NovoPhotocatSD::EndOfEvent(G4HCofThisEvent* )
{
}

void NovoPhotocatSD::clear()
{
}

void NovoPhotocatSD::DrawAll()
{
}

void NovoPhotocatSD::PrintAll()
{
}
