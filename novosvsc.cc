#include "NovoDetectorConstruction.hh"
#include "NovoActionInitialization.hh"
//#include "NovoPhysicsList.hh"
//#include "NovoPhysicsListSingular.hh"

#include "G4RunManager.hh"
#include "G4PhysListFactory.hh"
#include "G4UImanager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "QGSP_BIC_HP.hh"
#include "FTFP_BERT.hh"

#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "Randomize.hh"
#include "time.h"

int main(int argc,char** argv)
{
	// Detect interactive mode (if no arguments) and define UI session
	//

	G4cout << "main start" << G4endl;
	G4UIExecutive* ui = 0;
	G4cout << "aaaahhh!" << G4endl;
	if ( argc == 1 ) {
		ui = new G4UIExecutive(argc, argv);
	}

	G4cout << "before runManager" << G4endl;
	G4RunManager* runManager = new G4RunManager;

	G4cout << "before detector" << G4endl;
	NovoDetectorConstruction* detector = new NovoDetectorConstruction();
	runManager->SetUserInitialization(detector);
	G4cout << "after detector" << G4endl;
	
	//choose the Random engine
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
	//set random seed with system time
	G4bool isFixedSeed = true;
	G4long seed;
	if (isFixedSeed == true) {
	 seed = 1661692541;
	}
	else {
	 seed = time(NULL);
	}

	CLHEP::HepRandom::setTheSeed(seed);
	G4cout << " ======================================================" << G4endl;
	G4cout << "Choosing random engine ... " << G4endl;
	G4cout << "setting random seed with system time ...: " << G4endl;
	G4cout << "seed = " << seed << G4endl;
	G4cout << "isFixedSeed = " << isFixedSeed << G4endl;
	G4cout << " ======================================================" << G4endl;

	// Physics list
	//NovoPhysicsList* physicsList = new NovoPhysicsList();
	//runManager->SetUserInitialization(physicsList);
	G4VModularPhysicsList* physicsList = new QGSP_BIC_HP;
	//~ G4VModularPhysicsList* physicsList = new FTFP_BERT;
	physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());

	// versions earlier than 10.7
	G4bool OpticalPhysicsON = true;
	if (OpticalPhysicsON == true ){
		G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
		G4OpticalParameters* opticalParameters = G4OpticalParameters::Instance();
		opticalParameters->SetScintYieldFactor(1.0); // what is this?
		opticalParameters->SetCerenkovTrackSecondariesFirst(false);
		opticalParameters->SetScintTrackSecondariesFirst(false);
		opticalParameters->SetScintByParticleType(true);
		opticalParameters->SetScintEnhancedTimeConstants(true); // false = two scint components, true = three scint components
		// opticalParameters->SetScintEnhancedTimedTimeConstants(true);        
		opticalParameters->SetScintFiniteRiseTime(true);        // If a non-zero rise time is wanted, set the optical parameter setFiniteRiseTime to true, 
														        // and set the material constant property SCINTILLATIONRISETIME1 to the desired value.

		// opticalPhysics->SetScintillationYieldFactor(0.5);
		// // opticalPhysics->SetScintillationYieldFactor(1.);
		// //~ opticalPhysics->SetScintillationExcitationRatio(0.0);
		// opticalPhysics->SetTrackSecondariesFirst(kCerenkov, false);
		// opticalPhysics->SetTrackSecondariesFirst(kScintillation, false);
		// opticalPhysics->SetScintillationByParticleType(true);
		// opticalPhysics->SetFiniteRiseTime(true);
		// //~ opticalPhysics->GetScintillationProcess()->SetScintillationByParticleType(false);
		// //~ opticalPhysics->SetScintillationByParticleType(false);

		physicsList->RegisterPhysics(opticalPhysics); // test to fix energy spect.
	}


	physicsList->SetVerboseLevel(0);
	runManager->SetUserInitialization(physicsList);

	//~ G4PhysListFactory* physListFactory = new G4PhysListFactory();
	//~ G4VUserPhysicsList* physicsList =  physListFactory->GetReferencePhysList("QGSP_BERT_HP");
	//~ G4VUserPhysicsList* physicsList =  physListFactory->GetReferencePhysList("QGSP_BIC_HP");
	//~ runManager->SetUserInitialization(physicsList);

	//NovoPhysicsList* physList = new NovoPhysicsList;
	//runManager->SetUserInitialization(physList);

	runManager->SetUserInitialization(new NovoActionInitialization(detector));

	//runManager->SetUserAction(new NovoPrimaryGeneratorAction());

	//runManager->Initialize();
	// Initialize visualization
	//
	G4VisManager* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();

	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	// Process macro or start UI session
	//
	if ( ! ui ) {

		// batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}
	else {
		// interactive mode
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		ui->SessionStart();
		delete ui;
	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted
	// in the main() program !

	delete visManager;
	delete runManager;

}
