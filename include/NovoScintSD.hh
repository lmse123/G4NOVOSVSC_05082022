/// \file NovoScintSD.hh
/// \brief Definition of the NovoScintSD class
//
//
#ifndef NovoScintSD_h
#define NovoScintSD_h 1

#include "G4DataVector.hh"
#include "NovoScintHit.hh"
#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class NovoScintSD : public G4VSensitiveDetector
{
  public:

    NovoScintSD(G4String name);
    virtual ~NovoScintSD();

    virtual void Initialize(G4HCofThisEvent* );
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
    virtual G4bool ProcessHits_constStep(const G4Step* aStep,G4TouchableHistory* );
    virtual void EndOfEvent(G4HCofThisEvent* );
    virtual void clear();
    virtual void DrawAll();
    virtual void PrintAll();

    //Initialize the arrays to store scintillator possitions
		inline void InitScintillators(G4int nPCs)
		{
			if(fScintPositionsX)delete fScintPositionsX;
			if(fScintPositionsY)delete fScintPositionsY;
			if(fScintPositionsZ)delete fScintPositionsZ;
			fScintPositionsX=new G4DataVector();
			fScintPositionsY=new G4DataVector();
			fScintPositionsZ=new G4DataVector();
		}

    //Store a photocathode position
		void SetScintPositions(const std::vector<G4ThreeVector>& positions);

  private:

    NovoScintHitsCollection* fScintCollection;
    G4DataVector* fScintPositionsX;
		G4DataVector* fScintPositionsY;
		G4DataVector* fScintPositionsZ;

};

#endif
