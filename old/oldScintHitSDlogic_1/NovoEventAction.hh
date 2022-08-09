/*
 * G4NovoSVSC:
 * Event action: Actions happening at the beginning and end of each event.
 * Header file
 * Author: Kyrre Skjerdal (kyrre.skjerdal@hvl.no) - author of master code
 * Author: Lena Setterdahl (lmse@hvl.no) - author of updated code
 */


/// \file NovoEventAction.hh
/// \brief Definition of the NovoEventAction class


#ifndef NovoEventAction_h
#define NovoEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4Accumulable.hh"
#include "G4ThreeVector.hh"
#include "NovoDetectorConstruction.hh"
#include <vector>

//class NovoDetectorConstruction;
class NovoRunAction;
using std::array;
using std::vector;
/// Event action class
///

class NovoEventAction : public G4UserEventAction
{
	public:
		NovoEventAction(NovoDetectorConstruction* det);
		virtual ~NovoEventAction();
		virtual void BeginOfEventAction(const G4Event* event);
		virtual void EndOfEventAction(const G4Event* event);

		void SetEventVerbose(G4int v){fVerbose=v;}

		//void SetPCThreshold(G4int t){fPCThreshold=t;}

		void SetForceDrawPhotons(G4bool b){fForcedrawphotons=b;}
		void SetForceDrawNoPhotons(G4bool b){fForcenophotons=b;}

		// Inline getter methods
		// TODO: ... (uncomment in turn/when needed)
		inline G4int GetEventID() const {return fEventID;}
		inline std::vector<G4double>& GetTimestamps_N() {return fTimestampsN;}
		inline std::vector<G4double>& GetTimestamps_P() {return fTimestampsP;}

		// Getters for scintillator related members
		// inline std::vector<G4double> &GetScintHitPosXs(){return fScintHitPosXs; }
		// inline std::vector<G4double> &GetScintHitPosYs(){return fScintHitPosYs; }
		// inline std::vector<G4double> &GetScintHitPosZs(){return fScintHitPosZs; }
		// inline std::vector<G4String> &GetScintParticleNames(){return fScintParticleNames; }
		// inline std::vector<G4int> &GetScintParticleIDs(){return fScintParticleIDs; }

		// Getters for photocathode related members
		inline std::vector<G4double>& GetHitPosX_N() {return fHitPosX_N;}
		inline std::vector<G4double>& GetHitPosX_P() {return fHitPosX_P;}
		inline std::vector<G4double>& GetHitPosY_N() {return fHitPosY_N;}
		inline std::vector<G4double>& GetHitPosY_P() {return fHitPosY_P;}
		inline std::vector<G4double>& GetHitPosZ_N() {return fHitPosZ_N;}
		inline std::vector<G4double>& GetHitPosZ_P() {return fHitPosZ_P;}

	private:
		//~ NovoDetectorConstruction* fDetector;
		//~ NovoRunAction* fRunAction;
		G4int fEventID;

		//G4int fSaveThreshold;

		G4int fScintCollID; //where are these three used?
		G4int fPCCollID;
		G4int fBSGatingCollID;

		G4int fVerbose;
		std::vector<G4double> fTimestampsN;
		std::vector<G4double> fTimestampsP;

		//G4int fPCThreshold;

		G4bool fForcedrawphotons;
		G4bool fForcenophotons;
		G4ThreeVector CalculatePosition(G4ThreeVector vec);
		G4ThreeVector CalculatePositionSigmoid(G4ThreeVector vec);
		G4ThreeVector CalculatePositionTime(G4ThreeVector vec);
		G4ThreeVector CalculatePositionTimeSigmoid(G4ThreeVector vec);
		NovoDetectorConstruction *fDetector;
		//NovoRunAction* fRunAction;


		// Event info for scintillator SDs
		// std::vector<G4double> fScintHitPosXs; // array of vectors corresponding to hit x-coordinates in scintillator (EJ276)
		// std::vector<G4double> fScintHitPosYs; // array of vectors corresponding to hit y-coordinates in scintillator (EJ276)
		// std::vector<G4double> fScintHitPosZs; // array of vectors corresponding to hit z-coordinates in scintillator (EJ276)
		// std::vector<G4String> fScintParticleNames; // array of names corresponding to coordinates HitPosX,Y,Z
		// std::vector<G4int> fScintParticleIDs; // array of names corresponding to coordinates HitPosX,Y,Z
		
		// Event info for photocathode SDs
		std::vector<G4double> fHitPosX_N; // vector of hit x-coordinates in photocathode N
		std::vector<G4double> fHitPosX_P; // vector of hit x-coordinates in photocathode P
		std::vector<G4double> fHitPosY_N; // vector of hit y-coordinates in photocathode N
		std::vector<G4double> fHitPosY_P; // vector of hit y-coordinates in photocathode P
		std::vector<G4double> fHitPosZ_N; // vector of hit z-coordinates in photocathode N
		std::vector<G4double> fHitPosZ_P; // vector of hit z-coordinates in photocathode P
		
		
};


#endif
