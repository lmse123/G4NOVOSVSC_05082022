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
#include "G4DataVector.hh"


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

		inline void     SetEdep(G4double de) { fEdep = de; }
		inline void     IncEdep(G4double de) { fEdep += de; }
		inline G4double GetEdep() const { return fEdep; }

		inline void     SetTime(G4double t) { fTime = t; }
		inline G4double GetTime() const { return fTime; }

		// inline void SetPos(G4ThreeVector xyz) { fPos = xyz; } // replaced by SetScintPos
		// inline G4ThreeVector GetPos() const { return fPos; }  // replaced by GetScintPos 

		inline const G4VPhysicalVolume * GetPhysV() { return fPhysVol; }

		inline G4bool 	GetIsPrimary () const {return fIsPrimary;}
		inline void 	SetIsPrimary (G4bool isPrimary) {fIsPrimary=isPrimary;}
		
		inline G4String GetParticleName () const {return fParticleName;}
		inline void 	SetParticleName (G4String particleName) {fParticleName=particleName;}

		inline G4int GetParticleID () const {return fParticleID;}
		inline void  SetParticleID (G4int particleID) {fParticleID=particleID;}

		inline G4int GetParentID () const {return fParentID;}
		inline void  SetParentID (G4int parentID) {fParentID=parentID;}


		inline void  SetScintNumber(G4int n) { fScintNumber = n; }
		inline G4int GetScintNumber() { return fScintNumber; }

		inline void          SetScintPos(G4double x,G4double y,G4double z){fScintPos=G4ThreeVector(x,y,z);}
		inline G4ThreeVector GetScintPos(){return fScintPos;}

		inline void     SetElectronCount(G4int n) { fElectronCount = n; }
		inline void     IncElectronCount(G4int n) { fElectronCount += n; }
		inline G4int GetElectronCount() const { return fElectronCount; }

		inline void     SetOpticalPhotonCount(G4int n) { fOpticalPhotonCount = n; }
		inline void     IncOpticalPhotonCount(G4int n) { fOpticalPhotonCount += n; }
		inline G4int GetOpticalPhotonCount() const { return fOpticalPhotonCount; }

		inline void         SetOpticalPhotonTheta(G4double theta){fOpticalPhotonTheta.push_back(theta);} // Append thetas 
		inline G4DataVector GetOpticalPhotonTheta(){return fOpticalPhotonTheta;}         // Return vector all thetas
		
		inline void         SetOpticalPhotonPhi(G4double phi){fOpticalPhotonPhi.push_back(phi);} // Append z hit position to x-coordinate vector
		inline G4DataVector GetOpticalPhotonPhi(){return fOpticalPhotonPhi;}         // Return vector all z hit positions
		
		// inline void         SetOpticalPhotonPosition(G4double pos){fOpticalPhotonPosition.push_back(pos);} // Append z hit position to x-coordinate vector
		// inline G4DataVector GetOpticalPhotonPosition(){return fOpticalPhotonPosition;}         // Return vector all z hit positions
		
		inline void         SetOpticalPhotonX(G4double x){fOpticalPhotonX.push_back(x);} // Append x hit position to x-coordinate vector
		inline G4DataVector GetOpticalPhotonX(){return fOpticalPhotonX;}         // Return vector all x hit positions
		
		inline void         SetOpticalPhotonY(G4double y){fOpticalPhotonY.push_back(y);} // Append y hit position to Y-coordinate vector
		inline G4DataVector GetOpticalPhotonY(){return fOpticalPhotonY;}         // Return vector all y hit positions
		
		inline void         SetOpticalPhotonZ(G4double z){fOpticalPhotonZ.push_back(z);} // Append z hit position to x-coordinate vector
		inline G4DataVector GetOpticalPhotonZ(){return fOpticalPhotonZ;}         // Return vector all z hit positions
		

	private:

		G4double      fEdep;                       // Accumulated energy by hits in an event
		G4double      fTime;                       // Time of hit (what is the use of this?) 
		G4ThreeVector fScintPos;                   // x,y,z position of the hit scintillator, previously named fPos. 
		const         G4VPhysicalVolume* fPhysVol; // the volume the hit occurs ? 
		G4bool 	      fIsPrimary;                  // replace this by parent ID
		G4String	  fParticleName;               //
		G4int	      fParticleID;                 // Particle type (self defined)
		G4int	      fParentID;                   // Replacement for fIsPrimary
		G4int	      fScintNumber;                // Scintillator ID (in case of multiple scintillators)
		G4int	      fElectronCount;              // N electrons produced in the scintillator in an event
		G4int	      fOpticalPhotonCount;         // N optical photons produced in the scintillator in an event
		G4DataVector  fOpticalPhotonTheta;         // emission angle (theta) of optical photon at production
		G4DataVector  fOpticalPhotonPhi;           // emission angle (phi) of optical photon at production
		// G4DataVector  fOpticalPhotonPosition;      // production origin of optical photon (i think) OBS: not possible to store a vector in another vector
		G4DataVector  fOpticalPhotonX;             //
		G4DataVector  fOpticalPhotonY;             //
		G4DataVector  fOpticalPhotonZ;             //
		

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
