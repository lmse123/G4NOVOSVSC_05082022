/// \file NovoPhotocatHit.cc
/// \brief Implementation of the NovoPhotocatHit class
//
//

#include "NovoPhotocatHit.hh"

#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

G4ThreadLocal G4Allocator<NovoPhotocatHit>* NovoPhotocatHitAllocator=0;

NovoPhotocatHit::NovoPhotocatHit()
  : fPhotocatNumber(-1),
	fPhotons(0),
	fPhysVol(0),
	fDrawit(false) ,
	fTimestamps(0),
	fEdep(0),
	fPhotonE(0)
{
}


NovoPhotocatHit::~NovoPhotocatHit()
{
}

NovoPhotocatHit::NovoPhotocatHit(const NovoPhotocatHit &right) : G4VHit()
{
	fPhotocatNumber=right.fPhotocatNumber;
	fPhotons=right.fPhotons;
	fPhysVol=right.fPhysVol;
	fDrawit=right.fDrawit;
}

const NovoPhotocatHit& NovoPhotocatHit::operator=(const NovoPhotocatHit &right)
{
	fPhotocatNumber = right.fPhotocatNumber;
	fPhotons=right.fPhotons;
	fPhysVol=right.fPhysVol;
	fDrawit=right.fDrawit;
	return *this;
}

G4int NovoPhotocatHit::operator==(const NovoPhotocatHit &right) const
{
  return (fPhotocatNumber==right.fPhotocatNumber);
}

void NovoPhotocatHit::Draw(){
	if(fDrawit&&fPhysVol){ //ReDraw only the PMTs that have hit counts > 0
		//Also need a physical volume to be able to draw anything
		G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
		if(pVVisManager){//Make sure that the VisManager exists
			G4VisAttributes attribs(G4Colour(1.,0.,0.));
			attribs.SetForceSolid(true);
			G4RotationMatrix rot;
			if(fPhysVol->GetRotation())//If a rotation is defined use it
				rot=*(fPhysVol->GetRotation());
			G4Transform3D trans(rot,fPhysVol->GetTranslation());//Create transform
			pVVisManager->Draw(*fPhysVol,attribs,trans);//Draw it
		}
	}
}

void NovoPhotocatHit::Print() 
{
}



