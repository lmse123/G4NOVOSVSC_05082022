/// \file NovoEMPhysics.cc
/// \brief Implementation of the NovoEMPhysics class
//
//
#include "NovoEMPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4Gamma.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"

#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4ProcessManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NovoEMPhysics::NovoEMPhysics(const G4String& name)
               :  G4VPhysicsConstructor(name)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NovoEMPhysics::~NovoEMPhysics() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



void NovoEMPhysics::ConstructParticle()
{
  // gamma
  G4Gamma::GammaDefinition();
 
  // electron
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



void NovoEMPhysics::ConstructProcess()
{
  G4PhotoElectricEffect* fPhotoEffect =
    new G4PhotoElectricEffect();
  G4ComptonScattering* fComptonEffect =
    new G4ComptonScattering();
  G4GammaConversion* fPairProduction =
    new G4GammaConversion();
 
    // Electron physics
  G4eMultipleScattering* fElectronMultipleScattering =
    new G4eMultipleScattering();
  G4eIonisation* fElectronIonisation =
    new G4eIonisation();
  G4eBremsstrahlung* fElectronBremsStrahlung =
    new G4eBremsstrahlung();
 
    //Positron physics
  G4eMultipleScattering* fPositronMultipleScattering =
    new G4eMultipleScattering();
  G4eIonisation* fPositronIonisation =
    new G4eIonisation();
  G4eBremsstrahlung* fPositronBremsStrahlung =
    new G4eBremsstrahlung();
  G4eplusAnnihilation* fAnnihilation =
    new G4eplusAnnihilation();

  G4ProcessManager* pManager = 0;

  // Gamma Physics
  pManager = G4Gamma::Gamma()->GetProcessManager();
  pManager->AddDiscreteProcess(fPhotoEffect);
  pManager->AddDiscreteProcess(fComptonEffect);
  pManager->AddDiscreteProcess(fPairProduction);

  // Electron Physics
  pManager = G4Electron::Electron()->GetProcessManager();

  pManager->AddProcess(fElectronMultipleScattering, -1, 1, 1);
  pManager->AddProcess(fElectronIonisation,         -1, 2, 2);
  pManager->AddProcess(fElectronBremsStrahlung,     -1, 3, 3);  

  //Positron Physics
  pManager = G4Positron::Positron()->GetProcessManager();
 
  pManager->AddProcess(fPositronMultipleScattering, -1, 1, 1);
  pManager->AddProcess(fPositronIonisation,         -1, 2, 2);
  pManager->AddProcess(fPositronBremsStrahlung,     -1, 3, 3);  
  pManager->AddProcess(fAnnihilation,                0,-1, 4);  
  
  //protons
  pManager = G4Proton::Proton()->GetProcessManager();
  pManager->AddProcess(new G4hBremsstrahlung, -1, -3, 3);
  pManager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
  
  
  //Neutrons
  pManager = G4Neutron::Neutron()->GetProcessManager();
  pManager->AddProcess(new G4hBremsstrahlung, -1, -3, 3);
  pManager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
  pManager->AddProcess(new G4hIonisation(), -1, 2, 2);

}
