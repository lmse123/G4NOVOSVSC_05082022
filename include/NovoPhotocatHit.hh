/// \file NovoPhotocatHit.hh
/// \brief Definition of the NovoPhotoCatHit class
//
//
#ifndef NovoPhotocatHit_h
#define NovoPhotocatHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"
#include "G4DataVector.hh"

#include "tls.hh"
#include <vector>

class G4VTouchable;

class NovoPhotocatHit : public G4VHit
{
	public:
		NovoPhotocatHit();
		virtual ~NovoPhotocatHit();
		NovoPhotocatHit(const NovoPhotocatHit &right);

		const NovoPhotocatHit& operator=(const NovoPhotocatHit &right);
		G4int operator==(const NovoPhotocatHit &right) const;

		inline void *operator new(size_t);
		inline void operator delete(void *aHit);
 
		virtual void Draw();
		virtual void Print();

		inline void SetDrawit(G4bool b){fDrawit=b;}
		inline G4bool GetDrawit(){return fDrawit;}


		inline void IncPhotonCount(){fPhotons++;} // increase photon count 
		inline G4int GetPhotonCount(){return fPhotons;}

		inline void SetPhotocatNumber(G4int n) { fPhotocatNumber = n; }
		inline G4int GetPhotocatNumber() { return fPhotocatNumber; }

		inline void SetPhotocatPhysVol(G4VPhysicalVolume* physVol){this->fPhysVol=physVol;}
		inline G4VPhysicalVolume* GetPhotocatPhysVol(){return fPhysVol;}

		inline void          SetPhotocatPos(G4double x,G4double y,G4double z){fPos=G4ThreeVector(x,y,z);}
		inline G4ThreeVector GetPhotocatPos(){return fPos;}
		
		inline void         SetTime(G4double t){fTimestamps.push_back(t);} // Collect all time stamps of an event
		inline G4DataVector GetTimestamps(){return fTimestamps;}   // Return vector with all time stamps thus far accumulated in an event
		
		inline void     IncEdep(G4double de){fEdep += de;} // Accumulate energy deposited in detector
		inline G4double GetEdep(){return fEdep;}     // Return thus far accumulated energy deposited.
		
		inline void         SetPhotonE(G4double e){fPhotonE.push_back(e);}
		inline G4DataVector GetPhotonE(){return fPhotonE;}

		inline void         SetHitPosX(G4double x){fHitPosX.push_back(x);} // Append x hit position to x-coordinate vector
		inline G4DataVector GetHitPosX(){return fHitPosX;}         // Return vector all x hit positions
		
		inline void         SetHitPosY(G4double y){fHitPosY.push_back(y);} // Append y hit position to Y-coordinate vector
		inline G4DataVector GetHitPosY(){return fHitPosY;}         // Return vector all y hit positions
		
		inline void         SetHitPosZ(G4double z){fHitPosZ.push_back(z);} // Append z hit position to x-coordinate vector
		inline G4DataVector GetHitPosZ(){return fHitPosZ;}         // Return vector all z hit positions
		

	private:
		G4int fPhotocatNumber;
		G4int fPhotons;
		G4ThreeVector fPos;
		G4VPhysicalVolume* fPhysVol;
		G4bool fDrawit;
		G4DataVector fTimestamps;
		G4double fEdep;
		G4DataVector fPhotonE;
		G4DataVector fHitPosX; // vector of all x hit positions made in photocathode
		G4DataVector fHitPosY; // vector of all y hit positions made in photocathode
		G4DataVector fHitPosZ; // vector of all z hit positions made in photocathode
};

typedef G4THitsCollection<NovoPhotocatHit> NovoPhotocatHitsCollection;

extern G4ThreadLocal G4Allocator<NovoPhotocatHit>* NovoPhotocatHitAllocator;

inline void* NovoPhotocatHit::operator new(size_t)
{
	if(!NovoPhotocatHitAllocator)
		NovoPhotocatHitAllocator = new G4Allocator<NovoPhotocatHit>;
	return (void *) NovoPhotocatHitAllocator->MallocSingle();
}

inline void NovoPhotocatHit::operator delete(void *aHit)
{
	NovoPhotocatHitAllocator->FreeSingle((NovoPhotocatHit*) aHit);
}

#endif
