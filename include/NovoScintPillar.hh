/// \file NovoScintPillar.hh
/// \brief Definition of the NovoScintPillar class
//
//

#ifndef NovoScintPillar_H
#define NovoScintPillar_H 1

class NovoDetectorConstruction;

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"

#include "NovoDetectorConstruction.hh"



class NovoScintPillar : public G4PVPlacement
{
  public:

    NovoScintPillar(G4RotationMatrix *pRot,
               const G4ThreeVector &tlate,
               G4LogicalVolume *pMotherLogical,
               G4bool pMany,
               G4int pCopyNo,
               NovoDetectorConstruction* c);
    std::vector<G4ThreeVector> GetPhotcatPositions() {return fPCPositions;}
    std::vector<G4ThreeVector> GetAbsPhotcatPositions() {return fPCAbsPositions;}
    G4ThreeVector GetScintPosition() {return fScintPosition;}
    
    G4LogicalVolume* GetLogPhotocat() {return fPhotocathodeLog;}
    G4LogicalVolume* GetLogScint() {return fScintPillarLog;}

  private:

    void CopyValues();
	void SurfaceProperties();
	
    NovoDetectorConstruction* fConstructor;

	G4Box* fScintPillar;
	//~ G4Box* fPMT;
	G4Box* fPhotocathode;
	

    static G4LogicalVolume* fScintPillarLog;
   //~ G4LogicalVolume* fPMTLog;
	G4LogicalVolume* fPhotocathodeLog;
	G4VPhysicalVolume* fPC1;
	G4VPhysicalVolume* fPC2;
	

    G4double fScint_x;
    G4double fScint_y;
    G4double fScint_z;
    
    //~ G4double fPMT_x;
    //~ G4double fPMT_y;
    //~ G4double fPMT_z;
    
    G4double fPhotocath_x;
    G4double fPhotocath_y;
    G4double fPhotocath_z;
    
    // Sensitive Detectors positions
    std::vector<G4ThreeVector> fPCPositions;
    std::vector<G4ThreeVector> fPCAbsPositions;
    G4ThreeVector fScintPosition;
};

#endif
