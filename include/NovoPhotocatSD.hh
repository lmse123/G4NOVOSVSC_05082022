/// \file NovoPhotocatSD.hh
/// \brief Definition of the NovoPhotocatSD class
//
//
#ifndef NovoPhotocatSD_h
#define NovoPhottoSD_h 1

#include "G4DataVector.hh"
#include "NovoPhotocatHit.hh"
#include "G4VSensitiveDetector.hh"

#include <vector>


class G4Step;
class G4HCofThisEvent;

class NovoPhotocatSD : public G4VSensitiveDetector
{

	public:

		NovoPhotocatSD(G4String name);
		virtual ~NovoPhotocatSD();
 
		virtual void Initialize(G4HCofThisEvent* );
		virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
		G4bool ProcessHits_constStep(const G4Step*, G4TouchableHistory* ); //A version of processHits that keeps aStep constant
		virtual void EndOfEvent(G4HCofThisEvent* );
		virtual void clear();
		void DrawAll();
		void PrintAll();
 
		//Initialize the arrays to store photocathode possitions
		inline void InitPhotocathodes(G4int nPCs) // what is nPCs used for?
		{
			if(fPCPositionsX)delete fPCPositionsX;
			if(fPCPositionsY)delete fPCPositionsY;
			if(fPCPositionsZ)delete fPCPositionsZ;
			fPCPositionsX=new G4DataVector();
			fPCPositionsY=new G4DataVector();
			fPCPositionsZ=new G4DataVector();
			//G4cout << "InitPhotocathodes: " << fPCPositionsX->size() << G4endl; 
		}

		//Store a photocathode position
		void SetPCPositions(const std::vector<G4ThreeVector>& positions);

	private:

		NovoPhotocatHitsCollection* fPhotocatHitCollection;

		G4DataVector* fPCPositionsX;
		G4DataVector* fPCPositionsY;
		G4DataVector* fPCPositionsZ;

};

#endif
