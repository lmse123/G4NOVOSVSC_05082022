/// \file NovoBSGatingHit.cc
/// \brief Implementation of the NovoBSGatingHit class
//
//

#include "NovoBSGatingHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"


G4ThreadLocal G4Allocator<NovoBSGatingHit>* NovoBSGatingHitAllocator=0;

NovoBSGatingHit::NovoBSGatingHit()
:	fEdep(0.),
	//fTime(0),
	fPos(0.),
	fPhysVol(0),
	fIsPrimary(false),
	fParentID(0)
{
}


NovoBSGatingHit::NovoBSGatingHit(G4VPhysicalVolume* pVol)
: fPhysVol(pVol)
{

}


NovoBSGatingHit::~NovoBSGatingHit()
{
}

const NovoBSGatingHit& NovoBSGatingHit::operator=(const NovoBSGatingHit &right)
{
	fEdep = right.fEdep;
  fPos = right.fPos;
  fPhysVol = right.fPhysVol;
  fIsPrimary = right.fIsPrimary;
  fParentID = right.fParentID;
  return *this;
}

G4int NovoBSGatingHit::operator==(const NovoBSGatingHit&) const
{
  return false;
  //returns false because there currently isnt need to check for equality yet
}

void NovoBSGatingHit::Draw()
{
}

void NovoBSGatingHit::Print()
{
}
