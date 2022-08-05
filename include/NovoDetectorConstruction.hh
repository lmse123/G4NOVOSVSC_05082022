/// \file NovoDetectorConstruction.hh
/// \brief Definition of the NovoDetectorConstruction class
//
//
#ifndef NovoDetectorConstruction_H
#define NovoDetectorConstruction_H 1

//class NovoScintPillar;
class NovoPhotocatSD;
class NovoScintSD;
class NovoBSGatingSD;

#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"
#include "G4MaterialPropertiesTable.hh"

//#include "NovoPhotocatSD.hh"
#include "NovoScintPillar.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4PVPlacement;


class NovoDetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		NovoDetectorConstruction();
		virtual ~NovoDetectorConstruction();

		virtual G4VPhysicalVolume* Construct();
		virtual void ConstructSDandField();

		// Getters
		G4double GetScintX(){return fScint_x;}
		G4double GetScintY(){return fScint_y;}
		G4double GetScintZ(){return fScint_z;}
		G4int GetNScintX(){return fNScintX;}
		G4int GetNScintY(){return fNScintY;}
		G4int GetNScint(){return fNScint;}

		// Set geomentical dimensions
		void SetDimensions(G4ThreeVector dim);
		void SetDefaults();

	private:
		void DefineMaterials();
		G4VPhysicalVolume* ConstructDetector();

		// World
		G4Box* fWorldBox;
		G4LogicalVolume* fWorldLog;
		G4VPhysicalVolume* fWorldPhys;

		// Backscatter (BS) gating detector
		G4Tubs* fBSDetector;
	  G4LogicalVolume* fBSDetectorLog;
	  G4VPhysicalVolume* fBSDetectorPhys;

		// Scintillator
		G4Box* fScintillator;
		// G4Tubs* fScintillator;
		G4LogicalVolume* fScintillatorLog;

		// Photocathode
		G4Box* fPhotocathode;
		G4LogicalVolume* fPhotocathodeLog;
		//~ G4VPhysicalVolume* fPC1;
		//~ G4VPhysicalVolume* fPC2;

		// Lead blocks
		G4Box* fLeadblockBox;
		G4LogicalVolume* fLeadblockLV;

		// Materials and elements
		G4Element* fC;
		G4Element* fH;
		G4Element* fN;
		G4Element* fO;
		G4Element* fCe;
		G4Element* fBr;
		G4Material* fVacuum;
		G4Material* fAir;
		G4Material* fCeBr3;
		G4Material* fStilbene;
		G4Material* fEJ276;
		G4Material* fEJ200;
		//G4Material* fGlass;
		//G4Material* fAl;
		G4Material* fSilicon;
		G4Material* fLead;

		// Geometry
		G4double fScint_x;
		G4double fScint_y;
		G4double fScint_z;

		G4double fPhotocath_x;
		G4double fPhotocath_y;
		G4double fPhotocath_z;

		G4MaterialPropertiesTable* fVacuumMPT;
		G4MaterialPropertiesTable* fStilbeneMPT;
		G4MaterialPropertiesTable* fEJ276MPT;
		G4MaterialPropertiesTable* fEJ200MPT;
		//G4MaterialPropertiesTable* fGlassMPT;

		G4int fNScintX;
		G4int fNScintY;
		G4int fNScint;

		//~ NovoScintPillar* fScintPillar;
		//~ NovoScintPillar* fScintPillar2;
		//~ std::vector<NovoScintPillar*> fScintPillars;
		std::vector<G4PVPlacement*> fScintPillars;
		std::vector<G4PVPlacement*> fPC1;
		std::vector<G4PVPlacement*> fPC2;

		std::vector<G4PVPlacement*> fLeadblocks;

		std::vector<G4ThreeVector> fPCPositions;
		std::vector<G4ThreeVector> fScintPositions;

		G4Cache<NovoPhotocatSD*> fPhotcat_SD;
		G4Cache<NovoScintSD*> fScint_SD;
		G4Cache<NovoBSGatingSD*> fBS_SD;

};


#endif
