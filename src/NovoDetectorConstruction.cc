/// \file NovoDetectorConstruction.cc
/// \brief Implementation of the NovoDetectorConstruction class
//
//


#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"
#include "G4RotationMatrix.hh"

#include "NovoDetectorConstruction.hh"
//#include "NovoScintPillar.hh"
#include "NovoPhotocatSD.hh"
#include "NovoScintSD.hh"
#include "NovoBSGatingSD.hh"




NovoDetectorConstruction::NovoDetectorConstruction()
: fVacuumMPT(NULL), fStilbeneMPT(NULL), fEJ276MPT(NULL)/*, fScintPillar(NULL),fScintPillar2(NULL) */
{
  fWorldBox = NULL;
  fWorldLog = NULL;
  fWorldPhys = NULL;

  fScintillator = NULL;
  fScintillatorLog = NULL;
  //fScintillators = NULL;

  fPhotocathode = NULL;
  fPhotocathodeLog = NULL;
  // fPhotodetectors1 = NULL;
  //fPhotodetectors2 = NULL;

  //fPhotodetPositions = NULL;
  fLeadblockBox = NULL;
  fLeadblockLV = NULL;

  //fBSDetector = NULL;
  //fBSDetectorLog = NULL;

  fVacuum = fAir = fStilbene = fEJ276  = fEJ200 = NULL;

  fN = fO = fC = fH = NULL;

  SetDefaults();

  //fDetectorMessenger = new LXeDetectorMessenger(this);
}


NovoDetectorConstruction::~NovoDetectorConstruction()
{
}

void NovoDetectorConstruction::DefineMaterials()
{
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4double fractionmass;

  G4NistManager* manager = G4NistManager::Instance();

  //***Elements
  fH = new G4Element("H", "H", z=1., a=1.01*g/mole);
  fC = new G4Element("C", "C", z=6., a=12.01*g/mole);
  fN = new G4Element("N", "N", z=7., a= 14.01*g/mole);
  fO = new G4Element("O"  , "O", z=8., a= 16.00*g/mole);
  fCe = new G4Element("Ce","Ce",z=58., a= 140*g/mole);
  fBr = new G4Element("Br","Br",z=35., a= 80*g/mole);

  // Vacuum
  fVacuum = new G4Material("Vacuum",z=1.,a=1.01*g/mole,
  density=universe_mean_density,kStateGas,0.1*kelvin,
  1.e-19*pascal);

  //Air
  fAir = new G4Material("Air", density= 1.29*mg/cm3, 2);
  fAir->AddElement(fN, 75*perCent);
  fAir->AddElement(fO, 25*perCent);
  //fAir = manager->FindOrBuildMaterial("G4_AIR");

  //CeBr3
  fCeBr3 = new G4Material("CeBr3", density = 5.2*g/cm3, 2);
  fCeBr3->AddElement(fCe, 36.889*perCent);
  fCeBr3->AddElement(fBr, 63.111*perCent);

  G4double vacuumEnergy[]={2.0*eV,7.0*eV,7.14*eV};
  const G4int vacuumNum = sizeof(vacuumEnergy)/sizeof(G4double);
  G4double vacuumRIND[]={1.,1.,1.};
  assert(sizeof(vacuumRIND) == sizeof(vacuumEnergy));
  fVacuumMPT = new G4MaterialPropertiesTable();
  fVacuumMPT->AddProperty("RINDEX", vacuumEnergy, vacuumRIND, vacuumNum);
  fVacuum->SetMaterialPropertiesTable(fVacuumMPT);
  fAir->SetMaterialPropertiesTable(fVacuumMPT); // Give air the same MPT

  //Stilbene
  fStilbene = new G4Material("Stilbene", density=1.22*g/cm3, 2);
  fStilbene->AddElement(fH, 12);
  fStilbene->AddElement(fC, 14);

  //G4double scintEnergy[] = { 2.0*eV, 4.85*eV };
  //G4double scintEnergy[] = { 2.0*eV, 3.18*eV };

  G4double scintEnergy[] = {3.10*eV, 3.18*eV, 3.31*eV}; //Energy
  const G4int scintNum = sizeof(scintEnergy)/sizeof(G4double);
  //~ G4double scintRIND[]  = { 1.757 , 1.757 };
  //~ G4double scintRIND[]  = { 1.64 , 1.64 };
  G4double scintRIND[]  = {1.64, 1.64 , 1.64 }; //Refraction index
  assert(sizeof(scintRIND) == sizeof(scintEnergy));
  //~ G4double scintABSL[]  = { 100.*cm, 100.*cm };
  G4double scintABSL[]  = {100.*cm, 100.*cm, 100.*cm }; //Absorbtion length

  assert(sizeof(scintABSL) == sizeof(scintEnergy));
  //~ G4double scintFAST[]  = {0.000008, 1.0};
  //~ G4double scintFAST[]  = {0.23, 0.54, 0.23};
  //~ G4double scintFAST[]  = {0.0, 1.0, 0.0}; //Probability distribution
  G4double scintFAST[]  = {0.165, 0.67, 0.165}; //Probability distribution
  assert(sizeof(scintFAST) == sizeof(scintEnergy));

  G4double scintSLOW[]  = {0.165, 0.67, 0.165}; //Probability distribution
  assert(sizeof(scintSLOW) == sizeof(scintEnergy));

  G4double pEnergy[] = {0.1*MeV, 1.0*MeV, 100.0*MeV};
  G4double pYIELD[] = {1400, 14000., 1400000.};
  assert(sizeof(pYIELD) == sizeof(pEnergy));
  const G4int pNum = sizeof(pEnergy)/sizeof(G4double);


  fStilbeneMPT = new G4MaterialPropertiesTable();
  fStilbeneMPT->AddProperty("RINDEX", scintEnergy, scintRIND, scintNum);
  fStilbeneMPT->AddProperty("ABSLENGTH", scintEnergy, scintABSL, scintNum);
  fStilbeneMPT->AddProperty("FASTCOMPONENT", scintEnergy, scintFAST, scintNum);
  fStilbeneMPT->AddProperty("SLOWCOMPONENT", scintEnergy, scintSLOW, scintNum);

  fStilbeneMPT->AddProperty("PROTONSCINTILLATIONYIELD", pEnergy, pYIELD, pNum);
  fStilbeneMPT->AddProperty("ELECTRONSCINTILLATIONYIELD", pEnergy, pYIELD, pNum);
  fStilbeneMPT->AddProperty("IONSCINTILLATIONYIELD", pEnergy, pYIELD, pNum);
  //~ fStilbeneMPT->AddConstProperty("SCINTILLATIONYIELD", 14000./MeV);
  fStilbeneMPT->AddConstProperty("RESOLUTIONSCALE", 1.);
  fStilbeneMPT->AddConstProperty("FASTTIMECONSTANT", 3.5*ns); // Decay time
  fStilbeneMPT->AddConstProperty("SLOWTIMECONSTANT", 350.*ns); // Decay time
  fStilbeneMPT->AddConstProperty("YIELDRATIO", 1.0); // Proportion of scintillation light in fast component

  //fStilbeneMPT->AddConstProperty("RINDEX", 1.64);
  fStilbene->SetMaterialPropertiesTable(fStilbeneMPT);


  //EJ-276
  fEJ276 = new G4Material("EJ276", density=1.096*g/cm3, 2);
  fEJ276->AddElement(fH, fractionmass=0.072);
  fEJ276->AddElement(fC, fractionmass=0.928);

  G4double scintEnergyEJ276[] = {2.362*eV, 2.48*eV, 2.5433*eV, 2.61*eV, 2.6807*eV, 2.755*eV, 2.8339*eV, 2.917*eV, 2.952*eV, 2.98757*eV, 3.024*eV, 3.06134*eV, 3.0996*eV}; //Energy
  //~ G4double scintEnergyEJ276[] = {2.36*eV, 2.48*eV, 2.54*eV, 2.61*eV, 2.68*eV, 2.75*eV, 2.83*eV, 2.9*eV, 3.00*eV, 3.09*eV, 3.1*eV}; //Energy
  const G4int scintNumEJ276 = sizeof(scintEnergyEJ276)/sizeof(G4double);

  G4double scintFASTEJ276[]  ={0.00019486, 0.00974279, 0.01948558, 0.04481684, 0.07794232, 0.10717069, 0.15588465, 0.19485561, 0.15588465, 0.11691348, 0.07794232, 0.0387116, 0.00019486}; //Probability distribution
  assert(sizeof(scintFASTEJ276) == sizeof(scintEnergyEJ276));

  G4double riej276 = 1.64;
  G4double scintRINDEJ276[]  = {riej276, riej276, riej276, riej276, riej276, riej276, riej276, riej276, riej276, riej276, riej276, riej276, riej276}; //Refraction index
  assert(sizeof(scintRINDEJ276) == sizeof(scintEnergyEJ276));

  G4double abslengthej276 = 100.*cm;
  G4double scintABSLEJ276[]  = {abslengthej276, abslengthej276, abslengthej276, abslengthej276, abslengthej276, abslengthej276, abslengthej276, abslengthej276, abslengthej276, abslengthej276, abslengthej276, abslengthej276, abslengthej276}; //Absorbtion length
  assert(sizeof(scintABSLEJ276) == sizeof(scintEnergyEJ276));
  G4double photonyield = 8600.; // /1MeVe^-
  G4double pYIELDEJ276[] = {photonyield*0.1, photonyield*1.0, photonyield*100.};
  //~ G4double pYIELDEJ276[] = {700, 7000., 700000.};
  assert(sizeof(pYIELDEJ276) == sizeof(pEnergy));
  const G4int pNumEJ276 = sizeof(pEnergy)/sizeof(G4double);

  // Material property table of EJ276
  G4double yieldratio, fast_yield, fast_rise, fast_decay, med_yield, med_rise, med_decay, slow_yield, slow_rise, slow_decay;
  G4int n_scint_components = 3; // options: 2 (fast +slow/medium) or 3 (fast + medium + slow)
  G4String scint_2nd_comp = "slow"; // second scint component. Options: "medium" and "slow"
  // rise times based in other EJ scintillators which have rise times ≈ 1 ns.
  fast_rise  = 1*ns;  // ?
  med_rise   = 1*ns;  // ?
  slow_rise  = 1*ns;  // ?
  // EJ276 datasheet
  fast_decay = 13*ns; //ns
  med_decay  = 35*ns; //ns
  slow_decay = 270*ns; //ns
  // Grodzicka Kobylka 2020:
  fast_decay = 4*ns;  // ns, from Grodzicka Kobylka 2020
  med_decay  = 16*ns; // from Grodzicka Kobylka 2020
  G4double slow_decay_1 = 98*ns;  // from Grodzicka Kobylka 2020
  G4double slow_decay_2 = 690*ns;  // from Grodzicka Kobylka 2020
  slow_decay= slow_decay_1;
  fast_yield = 0.71; 
  med_yield  = 0.12;
  slow_yield = 1-fast_yield-med_yield;
  G4double slow_yield_1  = 0.8;
  G4double slow_yield_2  = 0.9;

  fast_yield = 1; 
  med_yield  = 1;
  slow_yield = 1;
  

  fEJ276MPT = new G4MaterialPropertiesTable();
  fEJ276MPT->AddProperty("RINDEX", scintEnergyEJ276, scintRINDEJ276, scintNumEJ276);
  fEJ276MPT->AddProperty("ABSLENGTH", scintEnergyEJ276, scintABSLEJ276, scintNumEJ276);

  fEJ276MPT->AddProperty("PROTONSCINTILLATIONYIELD", pEnergy, pYIELDEJ276, pNumEJ276);
  fEJ276MPT->AddProperty("ELECTRONSCINTILLATIONYIELD", pEnergy, pYIELDEJ276, pNumEJ276);
  fEJ276MPT->AddProperty("IONSCINTILLATIONYIELD", pEnergy, pYIELDEJ276, pNumEJ276);
  fEJ276MPT->AddConstProperty("RESOLUTIONSCALE", 1.);
  // Two scintillation components: fast + slow/med.
  if (n_scint_components == 2){
    fEJ276MPT->AddProperty("FASTCOMPONENT", scintEnergyEJ276, scintFASTEJ276, scintNumEJ276); // energy spectrum for the fast component
    fEJ276MPT->AddProperty("SLOWCOMPONENT", scintEnergyEJ276, scintFASTEJ276, scintNumEJ276); // energy spectrum for the slow component
    // Fast scintillation component
    fEJ276MPT->AddConstProperty("FASTTIMECONSTANT", fast_decay); 
    fEJ276MPT->AddConstProperty("FASTSCINTILLATIONRISETIME", fast_rise); 
    if (scint_2nd_comp == "medium"){
    // Medium scintillation component
    fEJ276MPT->AddConstProperty("SLOWTIMECONSTANT", med_decay); 
    fEJ276MPT->AddConstProperty("SLOWSCINTILLATIONRISETIME", med_rise); 
    }
    else if (scint_2nd_comp == "slow"){
    // Long (slow) scintillation component
    fEJ276MPT->AddConstProperty("SLOWTIMECONSTANT", slow_decay); 
    fEJ276MPT->AddConstProperty("SLOWSCINTILLATIONRISETIME", slow_rise); 
    // fEJ276MPT->AddConstProperty("SLOWTIMECONSTANT", 270.*ns); // Decay time
    // fEJ276MPT->AddConstProperty("SLOWSCINTILLATIONRISETIME", 35.*ns); //Rise time - Ilker "remove" 28/04/2022
    //~ fEJ276MPT->AddConstProperty("SLOWTIMECONSTANT", 350.*ns); // Decay time
    // YIELDRATIO: proportion of scintillation light in fast component. Used when considering two scintillation components. 
    }
    fEJ276MPT->AddConstProperty("YIELDRATIO", fast_yield); // use if two scintillation components; fast + slow/medium 
    // fEJ276MPT->AddConstProperty("YIELDRATIO", 1); // only fast
  }
  // Three scintillation components: fast + med. + slow
  if(n_scint_components == 3){
    fEJ276MPT->AddProperty("ELECTRONSCINTILLATIONYIELD1", pEnergy, pYIELDEJ276, pNumEJ276);
    fEJ276MPT->AddProperty("ELECTRONSCINTILLATIONYIELD2", pEnergy, pYIELDEJ276, pNumEJ276);
    fEJ276MPT->AddProperty("ELECTRONSCINTILLATIONYIELD3", pEnergy, pYIELDEJ276, pNumEJ276);
    fEJ276MPT->AddConstProperty("SCINTILLATIONYIELD1", fast_yield); // relative amount of photons produced in each component.
    fEJ276MPT->AddConstProperty("SCINTILLATIONYIELD2", med_yield); 
    fEJ276MPT->AddConstProperty("SCINTILLATIONYIELD3", slow_yield); 
    fEJ276MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", fast_decay); 
    fEJ276MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT2", med_decay); 
    fEJ276MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT3", slow_decay); 
    fEJ276MPT->AddConstProperty("SCINTILLATIONRISETIME1", fast_rise); 
    fEJ276MPT->AddConstProperty("SCINTILLATIONRISETIME2", med_rise); 
    fEJ276MPT->AddConstProperty("SCINTILLATIONRISETIME3", slow_rise); 
  }
  fEJ276->SetMaterialPropertiesTable(fEJ276MPT);
  G4cout << "=============================================" << G4endl;
  G4cout << "EJ276 Material Properties Table:" << G4endl;
  G4cout << "=============================================" << G4endl;
  switch(n_scint_components){
    case(2): G4cout << "Mode: Fast + " << scint_2nd_comp << G4endl; break;
    case(3): G4cout << "Mode: fast + medium + slow" << G4endl; break;
    default: G4cout << "Mode: " << n_scint_components<< G4endl;
  }
  
  fEJ276MPT->DumpTable();
  G4cout << "=============================================" << G4endl;
  //EJ-200
  fEJ200 = new G4Material("EJ200", density=1.023*g/cm3, 2);
  fEJ200->AddElement(fH, fractionmass=0.0847);
  fEJ200->AddElement(fC, fractionmass=0.9153);


  G4double scintEnergyEJ200[] = {2.4796*eV, 2.9172*eV, 3.1387*eV}; //Energy
  const G4int scintNumEJ200 = sizeof(scintEnergyEJ200)/sizeof(G4double);
  G4double scintRINDEJ200[]  = {1.58, 1.58 , 1.58 }; //Refraction index
  assert(sizeof(scintRINDEJ200) == sizeof(scintEnergyEJ200));
  //~ G4double scintABSL[]  = { 100.*cm, 100.*cm };
  G4double scintABSLEJ200[]  = {380.*cm, 380.*cm, 380.*cm }; //Absorbtion length
  assert(sizeof(scintABSLEJ200) == sizeof(scintEnergyEJ200));
  G4double scintFASTEJ200[]  = {0., 1.0, 0.}; //Probability distribution
  assert(sizeof(scintFASTEJ200) == sizeof(scintEnergyEJ200));

  G4double scintSLOWEJ200[]  = {0., 1., 0.}; //Probability distribution
  assert(sizeof(scintSLOWEJ200) == sizeof(scintEnergyEJ200));

  G4double pEnergyEJ200[] = {0.1*MeV, 1.0*MeV, 100.0*MeV};
  G4double pYIELDEJ200[] = {1000, 10000., 1000000.};
  assert(sizeof(pYIELDEJ200) == sizeof(pEnergyEJ200));
  const G4int pNumEJ200 = sizeof(pEnergyEJ200)/sizeof(G4double);

  fEJ200MPT = new G4MaterialPropertiesTable();
  fEJ200MPT->AddProperty("RINDEX", scintEnergyEJ200, scintRINDEJ200, scintNumEJ200);
  fEJ200MPT->AddProperty("ABSLENGTH", scintEnergyEJ200, scintABSLEJ200, scintNumEJ200);
  fEJ200MPT->AddProperty("FASTCOMPONENT", scintEnergyEJ200, scintFASTEJ200, scintNumEJ200);
  fEJ200MPT->AddProperty("SLOWCOMPONENT", scintEnergyEJ200, scintSLOWEJ200, scintNumEJ200);

  fEJ200MPT->AddProperty("PROTONSCINTILLATIONYIELD", pEnergyEJ200, pYIELDEJ200, pNumEJ200);
  fEJ200MPT->AddProperty("ELECTRONSCINTILLATIONYIELD", pEnergyEJ200, pYIELDEJ200, pNumEJ200);
  fEJ200MPT->AddProperty("IONSCINTILLATIONYIELD", pEnergyEJ200, pYIELDEJ200, pNumEJ200);
  //~ fStilbeneMPT->AddConstProperty("SCINTILLATIONYIELD", 14000./MeV);
  fEJ200MPT->AddConstProperty("RESOLUTIONSCALE", 1.);
  fEJ200MPT->AddConstProperty("FASTTIMECONSTANT", 2.1*ns); // Decay time
  fEJ200MPT->AddConstProperty("FASTSCINTILLATIONRISETIME", 0.9*ns); //Rise time
  fEJ200MPT->AddConstProperty("SLOWTIMECONSTANT", 210.*ns); // Decay time
  fEJ200MPT->AddConstProperty("SLOWSCINTILLATIONRISETIME", 0.9*ns); //Rise time
  fEJ200MPT->AddConstProperty("YIELDRATIO", 1.0); // Proportion of scintillation light in fast component

  //fStilbeneMPT->AddConstProperty("RINDEX", 1.64);
  fEJ200->SetMaterialPropertiesTable(fEJ200MPT);



  //Silicon
  //fSilicon = new G4Material("Silicon", z=14, a=28.086*g/mole, density=2.329*g/cm3);
  fSilicon = manager->FindOrBuildMaterial("G4_Si");

  //Lead
  fLead = manager->FindOrBuildMaterial("G4_Pb");

}

G4VPhysicalVolume* NovoDetectorConstruction::Construct()
{
  SetDefaults();
  DefineMaterials();
  return ConstructDetector();
}

G4VPhysicalVolume* NovoDetectorConstruction::ConstructDetector()
{

  G4double worldX = 1.0*m;
  G4double worldY = 1.0*m;
  G4double worldZ = 1.0*m;

  fWorldBox = new G4Box("worldBox", worldX/2., worldY/2., worldZ/2.);
  fWorldLog = new G4LogicalVolume(fWorldBox, G4Material::GetMaterial("Air"), "worldLog",0,0,0);
  fWorldPhys = new G4PVPlacement(0,G4ThreeVector(),fWorldLog,"world",0,false,0);

  fWorldLog->SetVisAttributes(G4VisAttributes::GetInvisible());

  // Backscatter (BS) gating detector
  G4double BS_innerRadius = 0.*cm;
  G4double BS_outerRadious = 25.4/2*mm;
  G4double BS_hz = 25.4*mm;
  G4double BS_startAgnle = 0.*deg;
  G4double BS_spanningAngle = 360.*deg;
  // G4double BS_dist_to_bar = 40*mm;
  G4double BS_dist_to_bar = 2*mm;
  G4double BS_dist_from_end_to_pmt =5*cm; //position along the bar, 0 = end
  G4double BS_xpos = fScint_x/2+BS_dist_to_bar+BS_hz/2;
  // G4double BS_zpos = -1*cm; //position along the bar, 0 = center
  // G4double BS_zpos = -3*cm; //position along the bar, 0 = center
  // G4double BS_zpos = -5*cm; //position along the bar, 0 = center
  // G4double BS_zpos = -7*cm; //position along the bar, 0 = center
  // G4double BS_zpos = -9*cm; //position along the bar, 0 = center
  G4double BS_zpos = 0*cm; //position along the bar, 0 = center
  // G4double BS_zpos = 1*cm; //position along the bar, 0 = center
  // G4double BS_zpos = 3*cm; //position along the bar, 0 = center
  // G4double BS_zpos = 5*cm; //position along the bar, 0 = center
  // G4double BS_zpos = 7*cm; //position along the bar, 0 = center
  // G4double BS_zpos = 9*cm; //position along the bar, 0 = center
  // G4double BS_zpos = fScint_z/2-BS_dist_from_end_to_pmt; //position along the bar in terms of distance from bar end
  G4double BS_rotation = 90*deg;
    G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
    rotationMatrix->rotateY(BS_rotation);
  fBSDetector = new G4Tubs("BS_Detector",BS_innerRadius,BS_outerRadious, BS_hz/2,BS_startAgnle, BS_spanningAngle);
  fBSDetectorLog = new G4LogicalVolume(fBSDetector, fCeBr3,"BS_detectorLV");
  fBSDetectorPhys = new G4PVPlacement(rotationMatrix,G4ThreeVector(BS_xpos,0,BS_zpos),fBSDetectorLog,"BS_detector",fWorldLog,false,0);

  // Scintillator pillar (alternative 1). OBS: update fScintillator type in DetectorConstruction.hh to Box is alt.1
  fScintillator = new G4Box("Scintillator", fScint_x/2., fScint_y/2., fScint_z/2.);
  fScintillatorLog = new G4LogicalVolume(fScintillator, G4Material::GetMaterial("EJ276"), "ScintillatorLV", 0,0,0);

  // // Scintillator cylinder (alternative 2)
  // G4double scintInnerRadius = 0.*mm;
  // G4double scintOuterRadius = 5.*mm;
  // G4double scintHz = fScint_z/2.;
  // G4double scintStartAngle = 0.*deg; 
  // G4double scintSpanningAngle = 360.*deg;
  // fScintillator = new G4Tubs("ScintillatorTube",scintInnerRadius, scintOuterRadius, scintHz, scintStartAngle,scintSpanningAngle);
  // fScintillatorLog = new G4LogicalVolume(fScintillator, G4Material::GetMaterial("EJ276"), "ScintillatorLV", 0,0,0);

  // Photocathodes
  fPhotocathode = new G4Box("Photocathode", fPhotocath_x/2., fPhotocath_y/2., fPhotocath_z/2.);
  fPhotocathodeLog = new G4LogicalVolume(fPhotocathode, fSilicon, "PhotocathodeLV");

  G4double xposStart = -((double)(fNScintX/2.) - (fScint_x/2.)/cm)*cm;
  G4double xpos = xposStart;
  //~ G4cout << "xpos: " << xpos << " " << fNScintX/2. << " " << fScint_x/cm << G4endl;

  // TODO: what is happening here in this loop?
  G4double ypos = -((double)(fNScintY/2.) - (fScint_y/2.)/cm)*cm;
  G4double zpos = 0.0*cm;
  G4double photocat_z = 0.0;
  int copyNo = 0; // scintillator number
  int copyPC = 0; // photocathode number
  for(auto i = 0; i < fNScintY; i++){
    for(auto j = 0; j < fNScintX; j++){
      fScintPositions.push_back(G4ThreeVector(xpos, ypos, zpos));
      fScintPillars.push_back(new G4PVPlacement(0, G4ThreeVector(xpos, ypos, zpos), fScintillatorLog, "Scintillator", fWorldLog, false, copyNo, true));
      photocat_z = zpos-(fScint_z/2)-fPhotocath_z/2;
      fPCPositions.push_back(G4ThreeVector(xpos, ypos, photocat_z));
      //~ fPC1.push_back(
      //~ new G4PVPlacement(0, G4ThreeVector(0., 0., photocat_z), fPhotocathodeLog,"Photocathode", fScintillatorLog,false, copyPC));
      fPC1.push_back(new G4PVPlacement(0, G4ThreeVector(xpos, ypos, photocat_z), fPhotocathodeLog,"Photocathode", fWorldLog,false, copyPC));
      photocat_z = zpos+(fScint_z/2)+fPhotocath_z/2;
      fPCPositions.push_back(G4ThreeVector(xpos, ypos, photocat_z));
      copyPC++;
      //~ fPC2.push_back(
      //~ new G4PVPlacement(0, G4ThreeVector(0., 0., photocat_z), fPhotocathodeLog,"Photocathode", fScintillatorLog,false, copyPC));
      fPC2.push_back(new G4PVPlacement(0, G4ThreeVector(xpos, ypos, photocat_z), fPhotocathodeLog,"Photocathode", fWorldLog,false, copyPC));

      xpos += fScint_x;
      copyNo++;
      copyPC++;
    }
    ypos += fScint_y;
    xpos = xposStart;
  }
  //Surface properties for the pillar
  G4OpticalSurface* scintWrap = new G4OpticalSurface("ScintWrap");

  // unified:
  scintWrap->SetType(dielectric_dielectric);
  scintWrap->SetModel(unified);
  scintWrap->SetSigmaAlpha(12*degree);
  scintWrap->SetFinish(groundbackpainted);
  //~ scintWrap->SetFinish(groundteflonair);

  // glisur:
  //~ scintWrap->SetType(dielectric_dielectric);
  //~ scintWrap->SetModel(glisur);
  //~ scintWrap->SetSigmaAlpha(1.0);
  //~ scintWrap->SetFinish(groundbackpainted);

  // LUT:
  //~ scintWrap->SetType(dielectric_LUTDAVIS);
  //~ scintWrap->SetModel(DAVIS);
  //~ scintWrap->SetFinish(RoughTeflon_LUT);


  scintWrap->DumpInfo();

  //G4double pp[] = {2.0*eV, 3.5*eV};
  G4double pp[] = {1.84*eV, 2.00*eV, 4.08*eV, 4.85*eV};
  const G4int num = sizeof(pp)/sizeof(G4double);
  //G4double reflectivity[] = {1.0, 1.0};
  G4double refl = 0.99;
  G4double reflectivity[] = {refl, refl, refl, refl};
  assert(sizeof(reflectivity) == sizeof(pp));
  G4double efficiency[] = {0.0, 0.0, 0.0, 0.0};
  assert(sizeof(efficiency) == sizeof(pp));
  G4double speclobe[] = {1.0, 1.0, 1.0, 1.0};
  assert(sizeof(speclobe) == sizeof(pp));
  G4double specspike[] = {0.0, 0.0, 0.0, 0.0};
  assert(sizeof(specspike) == sizeof(pp));
  G4double backscatter[] = {0.0, 0.0, 0.0, 0.0};
  assert(sizeof(backscatter) == sizeof(pp));
  G4double rindex[] = {1.0, 1.0, 1.0, 1.0};
  assert(sizeof(rindex) == sizeof(pp));


  G4MaterialPropertiesTable* scintWrapProperty
  = new G4MaterialPropertiesTable();

  scintWrapProperty->AddProperty("REFLECTIVITY",pp,reflectivity,num);
  scintWrapProperty->AddProperty("EFFICIENCY",pp,efficiency,num);
  scintWrapProperty->AddProperty("SPECULARLOBECONSTANT",pp,speclobe,num);
  scintWrapProperty->AddProperty("SPECULARSPIKECONSTANT",pp,specspike,num);
  scintWrapProperty->AddProperty("BACKSCATTERCONSTANT",pp,backscatter,num);
  scintWrapProperty->AddProperty("RINDEX",pp,rindex,num);
  scintWrap->SetMaterialPropertiesTable(scintWrapProperty);


  G4double ephoton[] = {2.4796*eV, 2.9172*eV, 3.1387*eV};
  const G4int num_pc = sizeof(ephoton)/sizeof(G4double);
  //G4double photocath_EFF[]={1.0,1.0, 1.0}; //Enables 'detection' of photons
  G4double photocath_EFF[]={0.26,0.26, 0.26}; //Enables 'detection' of photons
  assert(sizeof(photocath_EFF) == sizeof(ephoton));
  G4double photocath_REFL[]={0.00,0.00, 0.00}; //Enables 'detection' of photons
  assert(sizeof(photocath_EFF) == sizeof(ephoton));
  //G4double photocath_ReR[]={1.92,1.92, 1.92};
  //assert(sizeof(photocath_ReR) == sizeof(ephoton));
  //G4double photocath_ImR[]={1.69,1.69, 1.69};
  //assert(sizeof(photocath_ImR) == sizeof(ephoton));
  G4double speclobe_pc[] = {0.0, 0.0, 0.0};
  assert(sizeof(speclobe) == sizeof(ephoton));
  G4double specspike_pc[] = {0.0, 0.0, 0.0};
  assert(sizeof(specspike) == sizeof(ephoton));
  G4double backscatter_pc[] = {0.0, 0.0, 0.0};
  assert(sizeof(backscatter) == sizeof(pp));
  //G4double rindex_PC[] = {1.0, 1.0, 1.0};
  //assert(sizeof(rindex) == sizeof(ephoton));

  G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
  photocath_mt->AddProperty("EFFICIENCY",ephoton,photocath_EFF,num_pc);
  photocath_mt->AddProperty("REFLECTIVITY",ephoton,photocath_REFL,num_pc);
  //~ photocath_mt->AddProperty("REALRINDEX",ephoton,photocath_ReR,num);
  //~ photocath_mt->AddProperty("IMAGINARYRINDEX",ephoton,photocath_ImR,num);
  //~ photocath_mt->AddProperty("RINDEX",ephoton,rindex,num);
  photocath_mt->AddProperty("SPECULARLOBECONSTANT",ephoton,speclobe_pc,num_pc);
  photocath_mt->AddProperty("SPECULARSPIKECONSTANT",ephoton,specspike_pc,num_pc);
  photocath_mt->AddProperty("BACKSCATTERCONSTANT",ephoton,backscatter_pc,num_pc);

  G4OpticalSurface* photocath_opsurf=
  new G4OpticalSurface("photocath_opsurf",unified,polished,
  dielectric_metal, 1.3*degree);
  photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);


  for(auto i = 0; i < fNScint; i++){
    new G4LogicalBorderSurface("ScintWrap", fScintPillars[i], fWorldPhys, scintWrap);
    new G4LogicalBorderSurface("borderPC1Scint", fScintPillars[i], fPC1[i], photocath_opsurf);
    new G4LogicalBorderSurface("borderPC2Scint", fScintPillars[i], fPC2[i], photocath_opsurf);
  }
  //new G4LogicalSkinSurface("ScintWrap", fScintPillar->GetLogScint(), scintWrap);
  return fWorldPhys;
}

void NovoDetectorConstruction::ConstructSDandField()
{
  // Construct backscatter detector (BS)
  if(!fBS_SD.Get()){ // create fBS_SD if it doesnt already exist
    NovoBSGatingSD* bs_SD = new NovoBSGatingSD("bsSD");
    fBS_SD.Put(bs_SD);
  }
  G4SDManager::GetSDMpointer()->AddNewDetector(fBS_SD.Get());  //?
  SetSensitiveDetector(fBSDetectorLog, fBS_SD.Get()); // Assign logical volume to SD



  if(!fScintillatorLog) return; //??
  if (!fPhotcat_SD.Get()) {
    //Created here so it exists as pmts are being placed
    G4cout << "Construction pcSD" << G4endl;
    NovoPhotocatSD* pc_SD = new NovoPhotocatSD("pcSD");
    fPhotcat_SD.Put(pc_SD);

    pc_SD->InitPhotocathodes(fNScint*2); //let pcSD know # of photocathodes
    pc_SD->SetPCPositions(fPCPositions);
    //~ G4cout << "DetConst: " << fScintPillars[i]->GetAbsPhotcatPositions()[0] << G4endl;
    //~ G4cout << "DetConst: " << fScintPillars[i]->GetAbsPhotcatPositions()[1] << G4endl;
  }
  G4SDManager::GetSDMpointer()->AddNewDetector(fPhotcat_SD.Get());
  //sensitive detector is not actually on the photocathode.
  //processHits gets done manually by the stepping action.
  //It is used to detect when photons hit and get absorbed&detected at the
  //boundary to the photocathode (which doesnt get done by attaching it to a
  //logical volume.
  //It does however need to be attached to something or else it doesnt get
  //reset at the begining of events

  SetSensitiveDetector(fPhotocathodeLog, fPhotcat_SD.Get());


  // Scint SD

  if (!fScint_SD.Get()) {
    G4cout << "Construction scintSD" << G4endl;
    NovoScintSD* scint_SD = new NovoScintSD("scintSD");
    fScint_SD.Put(scint_SD);

    scint_SD->InitScintillators(fNScint); // let scint_SD know # of scintillators
    scint_SD->SetScintPositions(fScintPositions);
  }
  G4SDManager::GetSDMpointer()->AddNewDetector(fScint_SD.Get());
  SetSensitiveDetector(fScintillatorLog, fScint_SD.Get());
}

void NovoDetectorConstruction::SetDimensions(G4ThreeVector dims) {
  this->fScint_x=dims[0];
  this->fScint_y=dims[1];
  this->fScint_z=dims[2];
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void NovoDetectorConstruction::SetDefaults() {

  //Resets to default values
  fNScintX = 1;
  fNScintY = 1;
  fNScint = fNScintX*fNScintY;

  fScint_x = 1.0*cm;
  fScint_y = 1.0*cm;
  // fScint_z = 10.0*cm; // Decided to try out shorter bars. (Not sure why)
  fScint_z = 20.0*cm;

  fPhotocath_x = fScint_x;
  fPhotocath_y = fScint_y;
  fPhotocath_z = 0.5*cm;
}
