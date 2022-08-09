/// \file NovoBSGatingHit.hh
/// \brief Definition of the NovoBSGatingHit class
/// In progress....


#ifndef NovoBSGatingHit_h
#define NovoBSGatingHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"


#include "tls.hh"

class NovoBSGatingHit : public G4VHit
{
	public:
		NovoBSGatingHit(); //constructor
		NovoBSGatingHit(G4VPhysicalVolume* pVol); //constructor ??
		virtual ~NovoBSGatingHit(); //Deconstructor
		NovoBSGatingHit(const NovoBSGatingHit &right); //??
		const NovoBSGatingHit& operator=(const NovoBSGatingHit &right);
		G4int operator==(const NovoBSGatingHit &right) const;

		inline void *operator new(size_t);
		inline void operator delete(void *aHit);

		virtual void Draw();
		virtual void Print();

		// Setters and getters
		inline void SetEdep(G4double de) { fEdep = de; }  // set energy deposited in THIS hit
		inline void AddEdep(G4double de) { fEdep += de; } // Accumulate energy deposited thus far 
		inline G4double GetEdep() const { return fEdep; } // get energy deposited in THIS hit

		inline void SetPos(G4ThreeVector xyz) { fPos = xyz; } // set position (x,y,z) of THIS hit
		inline G4ThreeVector GetPos() const { return fPos; } // get position (x,y,z) of THIS hit

		inline const G4VPhysicalVolume * GetPhysV() { return fPhysVol; }

		inline G4bool 	GetIsPrimary () const {return fIsPrimary;} // is THIS hit made by a primary particle? 0: no, 1: yes
		inline void 	SetIsPrimary (G4bool isPrimary) {fIsPrimary=isPrimary;}

		inline G4String GetParticleName () const {return fParticleName;} // get name of particle of THIS hit
		inline void 	SetParticleName (G4String particleName) {fParticleName=particleName;}  // set name of particle of THIS hit


		inline G4int GetParentID () const {return fParentID;}               // 
		inline void 	SetParentID (G4int parentID) {fParentID=parentID;}  //

	private:
		G4double      fEdep;                 // energy deposited in the hit
		G4ThreeVector fPos;                  // position of the hit
		const G4VPhysicalVolume* fPhysVol;   // volume of the hit (when is this used?)
		G4bool 	      fIsPrimary;
		G4String	  fParticleName;         // name of the particle of the hit
		G4int	      fParentID;             // parent ID of the hit
};

typedef G4THitsCollection<NovoBSGatingHit> NovoBSGatingHitsCollection;

extern G4ThreadLocal G4Allocator<NovoBSGatingHit>* NovoBSGatingHitAllocator; //??

inline void* NovoBSGatingHit::operator new(size_t)
{
  if(!NovoBSGatingHitAllocator)
      NovoBSGatingHitAllocator = new G4Allocator<NovoBSGatingHit>;
  return (void *) NovoBSGatingHitAllocator->MallocSingle();
}

inline void NovoBSGatingHit::operator delete(void *aHit)
{
  NovoBSGatingHitAllocator->FreeSingle((NovoBSGatingHit*) aHit);
}


#endif
