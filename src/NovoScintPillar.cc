/// \file NovoScintPillar.cc
/// \brief Implementation of the NovoScintPillar class
//
//
#include "NovoScintPillar.hh"

#include "globals.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"

G4LogicalVolume* NovoScintPillar::fScintPillarLog = NULL;

NovoScintPillar::NovoScintPillar(G4RotationMatrix *pRot,
				const G4ThreeVector &tlate,
				G4LogicalVolume *pMotherLogical,
				G4bool pMany,
				G4int pCopyNo,
				NovoDetectorConstruction* c)
				:	G4PVPlacement(pRot,tlate,
					new G4LogicalVolume(new G4Box("temp",1,1,1),
                                     G4Material::GetMaterial("Vacuum"),
                                     "temp",0,0,0),
					"Scintillator",pMotherLogical,pMany,pCopyNo),fConstructor(c)
{
	CopyValues();
	
	G4double pillarX = fScint_x/2.;
	G4double pillarY = fScint_y/2.;
	G4double pillarZ = fScint_z/2.;	
	fScintPosition = tlate;
	fScintPillar = new G4Box("PillarBox",pillarX, pillarY, pillarZ);
	
	fScintPillarLog = new G4LogicalVolume(fScintPillar,
                        G4Material::GetMaterial("Stilbene"),
                        "PillarLog", 0, 0, 0);
                        
    
    
    //~ fPMT_x = fScint_x;
    //~ fPMT_y = fScint_y;
    //~ fPMT_z = 2.0*cm;
    
    fPhotocath_x = fScint_x;
    fPhotocath_y = fScint_y;
    fPhotocath_z = 0.5*cm;
    
    // Make PMTs with photocathodes
    //~ fPMT = new G4Box("PMTBox", fPMT_x/2., fPMT_y/2., fPMT_z/2.);
    fPhotocathode = new G4Box("PhotocathodeBox", fPhotocath_x/2., fPhotocath_y/2., fPhotocath_z/2.);
    
    //~ fPMTLog = new G4LogicalVolume(fPMT, G4Material::GetMaterial("Glass"), "PMTLog");
    fPhotocathodeLog = new G4LogicalVolume(fPhotocathode, G4Material::GetMaterial("Silicon"), "PhotocathodeLog");
    
    G4double photocat_x = 0.;
    G4double photocat_y = 0.;
    G4double photocat_z = -fScint_z/2-fPhotocath_z/2;
    //~ G4double photocat_z = -fScint_z/2-fPhotocath_z/2;
    fPC1 = new G4PVPlacement(0,G4ThreeVector(photocat_x, photocat_y, photocat_z),
                                    fPhotocathodeLog,"Photocathode",
                                    fScintPillarLog,false,0);
										
	fPCPositions.push_back(G4ThreeVector(photocat_x,photocat_y,photocat_z));									
	fPCAbsPositions.push_back(G4ThreeVector(fScintPosition.x(),fScintPosition.y(),photocat_z));									
	
    G4RotationMatrix* rot = new G4RotationMatrix();
	//rot->rotateY(180*deg);
	photocat_z = fScint_z/2+fPhotocath_z/2;
	
	//~ photocat_x = 0.4*m;
    //~ photocat_y = 0.4*m;
    //~ photocat_z = 0.0*m;
	
	fPC2 = new G4PVPlacement(rot,G4ThreeVector(photocat_x, photocat_y, photocat_z),
                                    fPhotocathodeLog,"Photocathode",
                                    fScintPillarLog,false,1);
	
	fPCPositions.push_back(G4ThreeVector(photocat_x,photocat_y,photocat_z));
    //~ G4double pmtx = 0.;
    //~ G4double pmty = 0.;             
    //~ G4double pmtz = fScint_z/2. + fPMT_z/2.;
    //~ new G4PVPlacement(0,G4ThreeVector(pmtx,pmty,pmtz),fPMTLog,"PMT",
                        //~ fScintPillarLog,false,0);
                     
    //~ pmtz = -fScint_z/2 - fPMT_z/2.;
    //~ G4RotationMatrix* rot = new G4RotationMatrix();
	//~ rot->rotateY(180*deg);
	//~ new G4PVPlacement(rot,G4ThreeVector(pmtx,pmty,pmtz),fPMTLog,"PMT",
                        //~ fScintPillarLog,false,1);
                        
    SurfaceProperties();
    SetLogicalVolume(fScintPillarLog);
	
}

void NovoScintPillar::CopyValues()
{
	fScint_x=fConstructor->GetScintX();
	fScint_y=fConstructor->GetScintY();
	fScint_z=fConstructor->GetScintZ();
}

void NovoScintPillar::SurfaceProperties()
{
	//G4double ephoton[] = {7.0*eV, 7.14*eV};
	G4double ephoton[] = {3.18*eV, 3.20*eV};
	const G4int num = sizeof(ephoton)/sizeof(G4double);
	G4double photocath_EFF[]={0.95,0.95}; //Enables 'detection' of photons
	assert(sizeof(photocath_EFF) == sizeof(ephoton));
	G4double photocath_REFL[]={0.0,0.0}; //Enables 'detection' of photons
	assert(sizeof(photocath_EFF) == sizeof(ephoton));
	G4double photocath_ReR[]={1.92,1.92};
	assert(sizeof(photocath_ReR) == sizeof(ephoton));
	G4double photocath_ImR[]={1.69,1.69};
	assert(sizeof(photocath_ImR) == sizeof(ephoton));
	G4double speclobe[] = {0.0, 0.0};
    assert(sizeof(speclobe) == sizeof(ephoton));
    G4double specspike[] = {0.0, 0.0};
    assert(sizeof(specspike) == sizeof(ephoton));
    G4double backscatter[] = {0.0, 0.0};
    assert(sizeof(backscatter) == sizeof(pp));
    G4double rindex[] = {1.0, 1.0};
    assert(sizeof(rindex) == sizeof(ephoton));
    
	G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
	photocath_mt->AddProperty("EFFICIENCY",ephoton,photocath_EFF,num);
	photocath_mt->AddProperty("REFLECTIVITY",ephoton,photocath_REFL,num);
	//~ photocath_mt->AddProperty("REALRINDEX",ephoton,photocath_ReR,num);
	//~ photocath_mt->AddProperty("IMAGINARYRINDEX",ephoton,photocath_ImR,num);
	//~ photocath_mt->AddProperty("RINDEX",ephoton,rindex,num);
	photocath_mt->AddProperty("SPECULARLOBECONSTANT",ephoton,speclobe,num);
	photocath_mt->AddProperty("SPECULARSPIKECONSTANT",ephoton,specspike,num);
	photocath_mt->AddProperty("BACKSCATTERCONSTANT",ephoton,backscatter,num);
	
	G4OpticalSurface* photocath_opsurf=
    new G4OpticalSurface("photocath_opsurf",unified,polished,
                         dielectric_metal, 1.3*degree);
	photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);
	
	//new G4LogicalSkinSurface("photocath_surf",fPhotocathodeLog, photocath_opsurf);
	new G4LogicalBorderSurface("borderPC1Scint", this,
                               fPC1,
                               photocath_opsurf);
    new G4LogicalBorderSurface("borderPC2Scint", this,
                               fPC2,
                               photocath_opsurf);                           
	
}
