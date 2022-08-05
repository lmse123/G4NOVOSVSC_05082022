/// \file NovoserEventInformation.cc
/// \brief Implementation of the NovoUserEventInformation class
//
//
#include "NovoUserEventInformation.hh"


NovoUserEventInformation::NovoUserEventInformation()
	:fHitCount(0),
	fPhotonCount_Scint(0),
	fPhotonCount_Ceren(0),
	fAbsorptionCount(0),
	fBoundaryAbsorptionCount(0),
	fTotE(0.),
	fEWeightPos(0.),
	fReconPos(0.),
	fConvPos(0.),
	fConvPosSet(false),
	fPosMax(0.),
	fEdepMax(0.) 
{
}


NovoUserEventInformation::~NovoUserEventInformation() 
{
}
