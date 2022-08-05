/// \file NovoUserTrackInformation.hh
/// \brief Definition of the NovoUserTrackInformation class
//
#include "G4VUserTrackInformation.hh"
#include "globals.hh"

#include "G4Allocator.hh"

#ifndef NovoUserTrackInformation_h
#define NovoUserTrackInformation_h 1

enum NovoTrackStatus { active=1, hitPC=2, absorbed=4, boundaryAbsorbed=8, inactive=14};

/*NovoTrackStatus:
  active: still being tracked
  hitPMT: stopped by being detected in a PMT
  absorbed: stopped by being absorbed with G4OpAbsorbtion
  boundaryAbsorbed: stopped by being aborbed with G4OpAbsorbtion
  inactive: track is stopped for some reason
   -This is the sum of all stopped flags so can be used to remove stopped flags
 
*/

class NovoUserTrackInformation : public G4VUserTrackInformation
{
	public:

		NovoUserTrackInformation();
		virtual ~NovoUserTrackInformation();
		
		//~ inline void *operator new(size_t);
		//~ inline void operator delete(void *aTrackInfo);

		//Sets the track status to s (does not check validity of flags)
		void SetTrackStatusFlags(int s){fStatus=s;}
		//Does a smart add of track status flags (disabling old flags that conflict)
		//If s conflicts with itself it will not be detected
		void AddTrackStatusFlag(int s);
 
		int GetTrackStatus()const {return fStatus;}
 
		void IncReflections(){fReflections++;}
		G4int GetReflectionCount()const {return fReflections;}

		void SetForceDrawTrajectory(G4bool b){fForcedraw=b;}
		G4bool GetForceDrawTrajectory(){return fForcedraw;}

		inline virtual void Print() const{};

	private:

		int fStatus;
		G4int fReflections;
		G4bool fForcedraw;
};

//~ extern G4Allocator<NovoUserTrackInformation> aTrackInformationAllocator;

//~ inline void* NovoUserTrackInformation::operator new(size_t){ 
	//~ void* aTrackInfo;
	//~ aTrackInfo = (void*)aTrackInformationAllocator.MallocSingle();
	//~ return aTrackInfo;
//~ }

//~ inline void NovoUserTrackInformation::operator delete(void *aTrackInfo){ 
	//~ aTrackInformationAllocator.FreeSingle((NovoUserTrackInformation*)aTrackInfo);
//~ }

#endif
