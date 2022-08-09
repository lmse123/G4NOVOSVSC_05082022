/// \file NovoScintSD.hh
/// \brief Definition of the NovoScintSD class
//
//
#ifndef NovoScintSD_h
#define NovoScintSD_h 1

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

  private:

    NovoScintHitsCollection* fScintCollection;

};

#endif
