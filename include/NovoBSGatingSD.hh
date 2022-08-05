/// \file NovoBSGatingSD.hh
/// \brief Definition of the NovoBSGatingSD class
//
//
#ifndef NovoBSGatingSD_h
#define NovoBSGatingSD_h 1 //TODO ??

#include "NovoBSGatingHit.hh"

#include "G4DataVector.hh"
#include "G4VSensitiveDetector.hh"

//#include <vector> //Needed to store position vector


class G4Step;
class G4HCofThisEvent;

class NovoBSGatingSD : public G4VSensitiveDetector
{

	public:

		NovoBSGatingSD(G4String name); // Constructor
		virtual ~NovoBSGatingSD(); // Deconstructor

		virtual void Initialize(G4HCofThisEvent*); //Invoked at the beginning of each event.
		virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory*); //Constructs one or more G4VHit objects. Invoked by G4SteppingManager when a step is composed in the G4LogicalVolume
		virtual G4bool ProcessHits_constStep(const G4Step* aStep,G4TouchableHistory*); //A version of processHits that keeps aStep constant
		virtual void EndOfEvent(G4HCofThisEvent* );
		virtual void clear();
		void DrawAll();
		void PrintAll();

	private:
		//NovoPhotocatHitsCollection* fPhotocatHitCollection;
		NovoBSGatingHitsCollection* fBSHitCollection; // create a hit colletion. What do i want to store?
};

#endif
