/// \file NovoBSGatingSD.cc
/// \brief Implementation of the NovoBSGatingSD class
//
//
#include "NovoBSGatingSD.hh"
#include "NovoBSGatingHit.hh"
//#include "NovoDetectorConstruction.hh"
//#include "NovoUserTrackInformation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"


NovoBSGatingSD::NovoBSGatingSD(G4String name)
  : G4VSensitiveDetector(name)
  	//fBSHitCollection(0)
{
  fBSHitCollection = NULL;
  collectionName.insert("BSGatingCollection");
}

// Deconstructor
NovoBSGatingSD::~NovoBSGatingSD()
{
}

///
// The hits collection(s) created by this sensitive detector must be set to the G4H0fThisEvent object (?)
void NovoBSGatingSD::Initialize(G4HCofThisEvent* hitsCE)
{
  fBSHitCollection = new NovoBSGatingHitsCollection(SensitiveDetectorName,collectionName[0]);
	//Store collection with event and keep ID
	static G4int hitCID = -1; //
	if(hitCID<0)
  {
	hitCID = GetCollectionID(0);
	}
	hitsCE->AddHitsCollection(hitCID, fBSHitCollection);
}


G4bool NovoBSGatingSD::ProcessHits(G4Step* aStep,G4TouchableHistory* )
{
	return false;
}

G4bool NovoBSGatingSD::ProcessHits_constStep(const G4Step* aStep,G4TouchableHistory* )
{
  G4int fVerbose = 0;
  // Hit information
  G4double edep = aStep->GetTotalEnergyDeposit(); // Hit energy
  G4int parentID = aStep->GetTrack()->GetParentID(); //ParentID
  G4String particleName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName(); // Particle name
  G4String preVol = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();
  G4String postVol = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();
	G4int n = fBSHitCollection->entries();

  // filter hits
  if         (n>0) return false;       // only need to know if the BSG has been hit once.
  if (parentID!=0) return false;       // Only save primary particles
  if    (edep==0.) return false;       // No edep so dont count as hit


  // initiate a hit object
  NovoBSGatingHit* bsgHit = new NovoBSGatingHit();
  fBSHitCollection->insert(bsgHit);
  bsgHit->SetParentID(parentID);

  if (fVerbose >0 ){
    G4cout << "NEW HIT --------------------------" << G4endl;
    G4cout << "fBSHitCollection entries:      " << n             << G4endl;
    G4cout << "parent ID: " << parentID             << G4endl;
    G4cout << "preVol:    " << preVol                   << G4endl;
    G4cout << "postVol:   " << postVol                   << G4endl;
    G4cout << "edep:      " << edep             << G4endl;
  }


  // // Is the hit a primary particle?
  // G4bool isPrimary = (parentID == 0 ) ? true : false;

  // //Get the average hit position
  // G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
	// G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
	// G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
	// pos/=2.;

  // // Find pre step volume
  // G4TouchableHistory* theTouchable = (G4TouchableHistory*)(thePrePoint->GetTouchable());
  // G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();

  // // Create hit and set attributes
  // NovoBSGatingHit* BSGatingHit = new NovoBSGatingHit(thePrePV);
	// BSGatingHit->SetEdep(edep); // 
	// BSGatingHit->SetPos(pos); // Hit position
	// BSGatingHit->SetIsPrimary(isPrimary); // Hit isPrimary
  // BSGatingHit->SetParticleName(particleName); // not used atm (22/07/2022)
  // // Add hit to hit collection
	// fBSHitCollection->insert(BSGatingHit);
	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NovoBSGatingSD::EndOfEvent(G4HCofThisEvent* ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NovoBSGatingSD::clear() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NovoBSGatingSD::DrawAll() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NovoBSGatingSD::PrintAll() {}
