/// \file NovoUserTrackInformation.cc
/// \brief Implementation of the NovoUserTrackInformation class
//
//
#include "NovoUserTrackInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//~ G4Allocator<NovoUserTrackInformation> aTrackInformationAllocator;

NovoUserTrackInformation::NovoUserTrackInformation()
  : fStatus(active),
	fReflections(0),
	fForcedraw(false) 
{
}


NovoUserTrackInformation::~NovoUserTrackInformation() 
{
}


void NovoUserTrackInformation::AddTrackStatusFlag(int s)
{
	if(s&active) //track is now active
		fStatus&=~inactive; //remove any flags indicating it is inactive
	else if(s&inactive) //track is now inactive
		fStatus&=~active; //remove any flags indicating it is active
	fStatus|=s; //add new flags
}

