/*
 * G4NovoSVSC: 
 * Primary generator action
 * Header file
 * Author: Kyrre Skjerdal (kyrre.skjerdal@hvl.no)
 */

/// \file NovoPrimaryGeneratorAction.hh
/// \brief Definition of the NovoPrimaryGeneratorAction class

#ifndef NovoPrimaryGeneratorAction_h
#define NovoPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"


class G4GeneralParticleSource;
class G4Event;
class NovoDetectorConstruction;

class NovoPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    NovoPrimaryGeneratorAction(NovoDetectorConstruction*);    
   ~NovoPrimaryGeneratorAction();

  public:

    virtual void GeneratePrimaries(G4Event*);

    void   ResetEbeamCumul() {fEbeamCumul = 0.;}
    G4double GetEbeamCumul() const {return fEbeamCumul;}
   
  private:

    void SetDefaultPrimaryParticle();

    G4GeneralParticleSource*   fParticleGun;
    NovoDetectorConstruction*      fDetector;    
    G4double                   fEbeamCumul;      
};


#endif
