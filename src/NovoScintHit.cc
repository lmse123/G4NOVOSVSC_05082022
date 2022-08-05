/// \file NovoScintHit.cc
/// \brief Implementation of the NovoScintHit class
//
//
#include "NovoScintHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"


G4ThreadLocal G4Allocator<NovoScintHit>* NovoScintHitAllocator=0;

NovoScintHit::NovoScintHit()
:	fScintNumber(-1),
	fEdep(0.),
	fTime(0),
	fScintPos(0.),
	fPhysVol(0),
	fIsPrimary(false),
  fElectronCount(0),
  fOpticalPhotonCount(0),
  fOpticalPhotonTheta(0),
  fOpticalPhotonPhi(0),
  // fOpticalPhotonPosition(0)
  fOpticalPhotonX(0),
  fOpticalPhotonY(0),
  fOpticalPhotonZ(0)
{
}


NovoScintHit::NovoScintHit(G4VPhysicalVolume* pVol)
: fPhysVol(pVol)
{
}


NovoScintHit::~NovoScintHit()
{
}

const NovoScintHit& NovoScintHit::operator=(const NovoScintHit &right)
{
  fScintNumber = right.fScintNumber;
  fEdep = right.fEdep;
  fScintPos = right.fScintPos;
  fTime = right.fTime;
  fPhysVol = right.fPhysVol; 
  fIsPrimary = right.fIsPrimary;
  fElectronCount = right.fElectronCount;
  fOpticalPhotonCount = right.fOpticalPhotonCount;
  fOpticalPhotonTheta = right.fOpticalPhotonTheta;
  fOpticalPhotonPhi = right.fOpticalPhotonPhi;
  // fOpticalPhotonPosition = right.fOpticalPhotonPosition;
  fOpticalPhotonX = right.fOpticalPhotonX;
  fOpticalPhotonY = right.fOpticalPhotonY;
  fOpticalPhotonZ = right.fOpticalPhotonZ;
  return *this;
}

G4int NovoScintHit::operator==(const NovoScintHit &right) const
{
   return (fScintNumber==right.fScintNumber);
}

void NovoScintHit::Draw()
{
}

void NovoScintHit::Print()
{
}
