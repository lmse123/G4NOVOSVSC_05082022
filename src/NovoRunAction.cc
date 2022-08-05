/*
 * G4NovoSVSC:
 * Run action
 * Source file
 * Author: Kyrre Skjerdal (kyrre.skjerdal@hvl.no) - author of master code
 * Author: Lena Setterdahl (lmse@hvl.no) - author of updated code
 */


/// \file NovoRunAction.cc
/// \brief Implementation of the NovoRunAction class

#include "NovoRunAction.hh"
#include "NovoPrimaryGeneratorAction.hh"
#include "NovoAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include <string>

NovoRunAction::NovoRunAction(NovoDetectorConstruction* det, NovoEventAction* ev)
:	G4UserRunAction(),
	fNevScint(0),
	fDetector(det),
	fEventAction(ev)
{
	//auto analysisManager = G4AnalysisManager::Instance();
	// Create analysis manager
	// The choice of analysis technology is done via selectin of a namespace
	// in NovoAnalysis.hh
	auto analysisManager = G4AnalysisManager::Instance();
	G4cout << "Using " << analysisManager->GetType() << G4endl;

	// Create directories
	analysisManager->SetHistoDirectoryName("histograms");
	analysisManager->SetNtupleDirectoryName("ntuples");
	analysisManager->SetVerboseLevel(1);

	// HISTOGRAMS 
	// ======================================================
	//
	// TODO: ...
	analysisManager->CreateH1("SourceDirX", "Source X-direction", 100, -1, 1); // H1: 0
	analysisManager->CreateH1("SourceDirY", "Source Y-direction", 100, -1, 1); // H1: 1
	analysisManager->CreateH1("SourceDirZ", "Source Z-direction", 100, -1, 1); // H1: 2
	analysisManager->CreateH1("ESource", "Energy of source particle", 100, 0.0*MeV, 2.5*MeV, "MeV", "E"); // H1: 3
	analysisManager->CreateH1("EDepScint", "Energy distribution in scintillator", 1400,0.0*MeV, 1.4*MeV, "MeV", "E"); // H1:4
	// analysisManager->CreateH1(); // H1:13
	// analysisManager->CreateH1(); // H1:14
	// analysisManager->CreateH1(); // H1:15

	// source 
	analysisManager->CreateH2("XYsource", "XY-dist of source", 100, -5.0*cm, 5.0*cm, 100, 5.0*cm, 25.0*cm, "cm", "cm"); //H2: 0
	analysisManager->CreateH2("XZsource", "XZ-dist of source", 100, -5.0*cm, 5.0*cm, 100, -5.0*cm, 5.0*cm, "cm", "cm"); //H2: 1
	analysisManager->CreateH2("YZsource", "YZ-dist of source", 100, 5.0*cm, 25.0*cm, 100, -5.0*cm, 5.0*cm, "cm", "cm"); //H2: 2

	// photocathode hit distribution

	G4int nbinsx  =    12; G4int nbinsy  =    12;
	G4double xlow = -6*mm; G4double ylow = -6*mm;
	G4double xup  = +6*mm; G4double yup  = +6*mm;
	
	analysisManager->CreateH2("photocath_XY_hit_position_N", "Photocathode N - XY-hit distribution",nbinsx,xlow,xup,nbinsy,ylow,yup ,"mm","mm"); // H2:3
	analysisManager->CreateH2("photocath_XY_hit_position_P", "Photocathode P - XY-hit distribution",nbinsx,xlow,xup,nbinsy,ylow,yup ,"mm","mm"); // H2:4

	// scintillator 

	
	// NTUPLES                                           
	// ======================================================

	// SCINTILLATOR NTUPLES 
	// ------------------------------------------------------
	// a(nScintX x nScintY) matrix of scintillators
	G4int nScintX = fDetector->GetNScintX(); // number of scintillators in the x-direction
	G4int nScintY = fDetector->GetNScintY(); // number of scintillators in the y-direction
	G4int ntupleNo = 0;
	analysisManager->SetFirstNtupleId(0);
	analysisManager->SetFirstNtupleColumnId(0);
	// loop over all scintillators
	for(G4int x = 0; x < nScintX; x++){
		for(G4int y = 0; y < nScintY; y++){
			G4String ntuplename = "Ntuple-"+ std::to_string(x) + '-' + std::to_string(y);
			G4String ntupletitle = "Event info " + std::to_string(x) + ' ' + std::to_string(y);
			analysisManager->CreateNtuple(ntuplename,ntupletitle);
			// photocathode related information
			analysisManager->CreateNtupleDColumn(ntupleNo,"PosX_N"); // 0
			analysisManager->CreateNtupleDColumn(ntupleNo,"PosX_P"); // 1
			analysisManager->CreateNtupleDColumn(ntupleNo,"PosY_N"); // 2
			analysisManager->CreateNtupleDColumn(ntupleNo,"PosY_P"); // 3
			analysisManager->CreateNtupleDColumn(ntupleNo,"PosZ_N"); // 4
			analysisManager->CreateNtupleDColumn(ntupleNo,"PosZ_P"); // 5
			analysisManager->CreateNtupleDColumn(ntupleNo,"PhotonCount_N"); // 6
			analysisManager->CreateNtupleDColumn(ntupleNo,"PhotonCount_P"); // 7
			analysisManager->CreateNtupleDColumn(ntupleNo,"eDep_N"); // 8
			analysisManager->CreateNtupleDColumn(ntupleNo,"eDep_P"); // 9
			analysisManager->CreateNtupleDColumn(ntupleNo,"HitPosX_N", fEventAction->GetHitPosX_N()); // 10 
			analysisManager->CreateNtupleDColumn(ntupleNo,"HitPosX_P", fEventAction->GetHitPosX_P()); // 11
			analysisManager->CreateNtupleDColumn(ntupleNo,"HitPosY_N", fEventAction->GetHitPosY_N()); // 12
			analysisManager->CreateNtupleDColumn(ntupleNo,"HitPosY_P", fEventAction->GetHitPosY_P()); // 13
			analysisManager->CreateNtupleDColumn(ntupleNo,"HitPosZ_N", fEventAction->GetHitPosZ_N()); // 14
			analysisManager->CreateNtupleDColumn(ntupleNo,"HitPosZ_P", fEventAction->GetHitPosZ_P()); // 15
			analysisManager->CreateNtupleDColumn(ntupleNo,"TimestampsVec_N", fEventAction->GetTimestamps_N()); // 16
			analysisManager->CreateNtupleDColumn(ntupleNo,"TimestampsVec_P", fEventAction->GetTimestamps_P()); // 17
			// scintillator related information
			analysisManager->CreateNtupleDColumn(ntupleNo,"ParentID"); // 18
			analysisManager->CreateNtupleDColumn(ntupleNo,"ParticleID"); // 19
			analysisManager->CreateNtupleDColumn(ntupleNo,"eDep"); // 20 - Total energy deposited in scintillator in an event
			analysisManager->CreateNtupleDColumn(ntupleNo,"HitPosX");//,fEventAction->GetScintHitPosXs()); // 21
			analysisManager->CreateNtupleDColumn(ntupleNo,"HitPosY");//,fEventAction->GetScintHitPosYs()); // 22
			analysisManager->CreateNtupleDColumn(ntupleNo,"HitPosZ");//,fEventAction->GetScintHitPosZs()); // 23 
			analysisManager->CreateNtupleDColumn(ntupleNo,"ElectronCount"); // 24
			analysisManager->CreateNtupleDColumn(ntupleNo,"OpticalPhotonCount"); // 25
			analysisManager->CreateNtupleDColumn(ntupleNo,"OpticalPhotonTheta",fEventAction->GetOpticalPhotonTheta()); // 26
			analysisManager->CreateNtupleDColumn(ntupleNo,"OpticalPhotonPhi",fEventAction->GetOpticalPhotonPhi()); // 27
			analysisManager->CreateNtupleDColumn(ntupleNo,"OpticalPhotonHitPosXs",fEventAction->GetOpticalPhotonHitPosXs()); // 28
			analysisManager->CreateNtupleDColumn(ntupleNo,"OpticalPhotonHitPosYs",fEventAction->GetOpticalPhotonHitPosYs()); // 29
			analysisManager->CreateNtupleDColumn(ntupleNo,"OpticalPhotonHitPosZs",fEventAction->GetOpticalPhotonHitPosZs()); // 30
		
			// TODO: more columns ....
			analysisManager->FinishNtuple(ntupleNo);
			ntupleNo++;
		}
		
	}
	// Create scintillator + photocathode tuple(s)
	// TODO: N tuples. 

	
	// Create tuple for backscatter gating detector. 
	
}



NovoRunAction::~NovoRunAction()
{
	delete G4AnalysisManager::Instance();
}

void NovoRunAction::BeginOfRunAction(const G4Run*)
{
	// inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();

	// reset accumulables to their initial values
	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->Reset();

	// Open ouput file
	G4String filename = "NovoOutput";
	analysisManager->OpenFile(filename);

}

void NovoRunAction::EndOfRunAction(const G4Run* run)
{
	G4int nofEvents = run->GetNumberOfEvent();
	if ( nofEvents == 0) return;

	// Print
	//
	if (IsMaster()) {
		G4cout
			<< G4endl
			<< "--------------------End of Global Run-----------------------";
	}
	else {
		G4cout
			<< G4endl
			<< "--------------------End of Local Run------------------------";
	}
	G4cout
		<< G4endl
		<< " The run consists of " << nofEvents << " events"
		<< " where " << fNevScint.GetValue() << " detected scintillation photons in both photodetectors. "
		<< G4endl;

	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->Write();
	analysisManager->CloseFile();

}

void NovoRunAction::AddNevScint(G4int n)
{
	fNevScint += n;
}
