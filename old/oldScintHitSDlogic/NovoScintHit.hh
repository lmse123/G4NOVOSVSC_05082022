/// \file NovoScintHit.hh
/// \brief Definition of the NovoScintHit class


#ifndef NovoScintHit_h
#define NovoScintHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"


#include "tls.hh"

class NovoScintHit : public G4VHit
{
	public:
		NovoScintHit();
		NovoScintHit(G4VPhysicalVolume* pVol);
		virtual ~NovoScintHit();
		NovoScintHit(const NovoScintHit &right);
		const NovoScintHit& operator=(const NovoScintHit &right);
		G4int operator==(const NovoScintHit &right) const;

		inline void *operator new(size_t);
		inline void operator delete(void *aHit);

		virtual void Draw();
		virtual void Print();

		inline void SetEdep(G4double de) { fEdep = de; }
		inline void AddEdep(G4double de) { fEdep += de; }

		inline G4double GetEdep() const { return fEdep; }

		inline void SetTime(G4double t) { fTime = t; }
		inline G4double GetTime() const { return fTime; }

		inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
		inline G4ThreeVector GetPos() const { return fPos; }

		inline const G4VPhysicalVolume * GetPhysV() { return fPhysVol; }

		inline G4bool 	GetIsPrimary () const {return fIsPrimary;}
		inline void 	SetIsPrimary (G4bool isPrimary) {fIsPrimary=isPrimary;}
		
		inline G4String GetParticleName () const {return fParticleName;}
		inline void 	SetParticleName (G4String particleName) {fParticleName=particleName;}

		inline G4int GetParticleID () const {return fParticleID;}
		inline void 	SetParticleID (G4int particleID) {fParticleID=particleID;}

		inline G4int GetParentID () const {return fParentID;}
		inline void 	SetParentID (G4int parentID) {fParentID=parentID;}


		inline void SetScintNumber(G4int n) { fScintNumber = n; }
		inline G4int GetScintNumber() { return fScintNumber; }

	private:
		G4double      fEdep;                       // Accumulated energy by hits in an event
		G4double      fTime;                       // Time of hit (what is the use of this?) 
		G4ThreeVector fPos;                        // x,y,z position of the hit
		const         G4VPhysicalVolume* fPhysVol; // the volume the hit occurs ? 
		G4bool 	      fIsPrimary;                  // replace this by parent ID
		G4String	  fParticleName;               //
		G4int	      fParticleID;                 // Particle type (self defined)
		G4int	      fParentID;                   // Replacement for fIsPrimary
		G4int	      fScintNumber;                // Scintillator ID (in case of multiple scintillators)
		

};

typedef G4THitsCollection<NovoScintHit> NovoScintHitsCollection;

extern G4ThreadLocal G4Allocator<NovoScintHit>* NovoScintHitAllocator;

inline void* NovoScintHit::operator new(size_t)
{
  if(!NovoScintHitAllocator)
      NovoScintHitAllocator = new G4Allocator<NovoScintHit>;
  return (void *) NovoScintHitAllocator->MallocSingle();
}

inline void NovoScintHit::operator delete(void *aHit)
{
  NovoScintHitAllocator->FreeSingle((NovoScintHit*) aHit);
}

#endif
