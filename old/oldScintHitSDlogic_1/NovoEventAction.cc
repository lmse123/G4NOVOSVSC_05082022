/*
 * G4NovoSVSC:
 * Event action
 * Source file
 * Author: Kyrre Skjerdal (kyrre.skjerdal@hvl.no) - author of master code
 * Author: Lena Setterdahl (lmse@hvl.no) - author of updated code
 */


/// \file NovoEventAction.cc
/// \brief Implementation of the NovoEventAction class


#include "NovoEventAction.hh"
#include "NovoRunAction.hh"
#include "NovoAnalysis.hh"
#include "NovoBSGatingHit.hh"
#include "NovoScintHit.hh"
#include "NovoPhotocatHit.hh"
#include "NovoUserEventInformation.hh"

#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4VAnalysisManager.hh"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "G4DataVector.hh"

#include <algorithm>
#include <vector>

NovoEventAction::NovoEventAction(NovoDetectorConstruction* det)
:	G4UserEventAction(),
	fEventID(0),
	fScintCollID(-1),
	fPCCollID(-1),
	fBSGatingCollID(-1),
	fVerbose(1),
	fTimestampsN(0),
	fTimestampsP(0),
	fForcedrawphotons(false),
	fForcenophotons(false),
	fDetector(det),
	// fScintHitPosXs(0), //set to 0?
	// fScintHitPosYs(0), //set to 0?
	// fScintHitPosZs(0), //set to 0?
	// fScintParticleIDs(0) //set to 0?
	fHitPosX_N(0),
	fHitPosX_P(0),
	fHitPosY_N(0),
	fHitPosY_P(0),
	fHitPosZ_N(0),
	fHitPosZ_P(0)
	//fRunAction(runAction)
{
}


NovoEventAction::~NovoEventAction()
{
}

void NovoEventAction::BeginOfEventAction(const G4Event* anEvent)
{
	G4EventManager::
    GetEventManager()->SetUserInformation(new NovoUserEventInformation);

	// Clear event vectors (position and time stamps and IDs) -?
	fHitPosX_N.clear(); fHitPosX_P.clear();
	fHitPosY_N.clear(); fHitPosY_P.clear();
	fHitPosZ_N.clear(); fHitPosZ_P.clear();
	fTimestampsN.clear(); fTimestampsP.clear();
	
  	fVerbose = 1; // Verbose ??
	G4SDManager* SDman = G4SDManager::GetSDMpointer(); // Sensitive detector manager
	// TODO: Get the ID number of the sensitive detector. (what is a collection?)
	if(fBSGatingCollID<0){
		fBSGatingCollID=SDman->GetCollectionID("BSGatingCollection");
	}
	if(fScintCollID<0){
		fScintCollID=SDman->GetCollectionID("scintCollection");
	}
	if(fPCCollID<0){
		fPCCollID=SDman->GetCollectionID("photocatHitCollection");
	}
	// her kan du lage en ntuple for lagrind av info. create ntuple, double
/*	G4int evNum = anEvent->GetEventID();
	//G4cout << "Event number: " << evNum << G4endl;
	if(evNum < 100){
		G4cout << "Event number: " << evNum << G4endl;
	} else if (evNum < 1000 && evNum%100 == 0){
		G4cout << "Event number: " << evNum << G4endl;
	} else if (evNum < 10000 && evNum%1000 == 0){
		G4cout << "Event number: " << evNum << G4endl;
	} else if(evNum%10000 == 0){
		G4cout << "Event number: " << evNum << G4endl;
	}
*/
}

void NovoEventAction::EndOfEventAction(const G4Event* anEvent)
{
	NovoUserEventInformation* eventInformation = (NovoUserEventInformation*)anEvent->GetUserInformation(); 

	NovoBSGatingHitsCollection* bsHC = 0;
	NovoScintHitsCollection* scintHC = 0;
	NovoPhotocatHitsCollection* pcHC = 0;
	G4HCofThisEvent* hitsCE = anEvent->GetHCofThisEvent(); //returns the pointers to the G4HCofThisEvent (hits collections of this event)
	auto analysisManager = G4AnalysisManager::Instance();

	// Get hit collections of sensitive detectors (SDs)
	if(hitsCE){
		if(fBSGatingCollID >= 0){
			bsHC = (NovoBSGatingHitsCollection*)(hitsCE->GetHC(fBSGatingCollID));
		}
		if(fScintCollID >= 0){
			scintHC = (NovoScintHitsCollection*)(hitsCE->GetHC(fScintCollID));
		}
		if(fPCCollID >=0 ){
			pcHC = (NovoPhotocatHitsCollection*)(hitsCE->GetHC(fPCCollID));
		}
	}

	// Process the event's scintillator (scint) hit collection
	if(scintHC){
		// energy weighted position
		// max energy deposition
		int n_hit = scintHC->entries();
		G4ThreeVector  eWeightPos(0.);
		G4double edep;
		G4double edepMax=0;

		for(int i=0;i<n_hit;i++){ //gather info on hits in scintillator
			edep=(*scintHC)[i]->GetEdep();
			eventInformation->IncEDep(edep); //sum up the edep
			eWeightPos += (*scintHC)[i]->GetPos()*edep;//calculate energy weighted pos
			if(edep>edepMax){
				edepMax=edep;//store max energy deposit
				G4ThreeVector posMax=(*scintHC)[i]->GetPos();
				eventInformation->SetPosMax(posMax,edep);
			}
		}
		if(eventInformation->GetEDep()==0.){
			if(fVerbose>0)G4cout<<"No hits in the scintillator this event."<<G4endl;
		}
		else{
			//Finish calculation of energy weighted position
			eWeightPos/=eventInformation->GetEDep();
			eventInformation->SetEWeightPos(eWeightPos);
			if(fVerbose>0){
				// G4cout << "\tscintNumber: " << scintNumber<< G4endl;
				G4cout << "\tEnergy weighted position of hits in Scintillator : " << eWeightPos/mm << G4endl;
				G4cout << "\tTotal energy deposition in scintillator : " << eventInformation->GetEDep() / MeV << " (MeV)" << G4endl;
			}
			analysisManager->FillH1(4, eventInformation->GetEDep());

			// Fill the ntuple
			// G4int scintNumber = (*scintHC)[i]->GetScintNumber(); // where does this go?
			// analysisManager->FillNtupleDColumn(scintNumber, 20, eventInformation->GetEDep());

		}
	}

	// Process the event's backscatter (bs) gating hit collection
	if(bsHC){
		// Get number of hits in the hit collection
		// TODO: get information of interest. 
		// Fill ntuple
		// TODO: ...

	}

	// Process the event's photocathode (pc) hit collection
	if(pcHC){
		// Get number of hits in the hit collection
		G4int nHitPcs = pcHC->entries();
		if(nHitPcs > 0){
			//NovoRunAction::AddNevScint(1);
			// A photocathode hit:
			// Here, there is essentially one hit per photocathode that has been hit
			// one hit contains info on the accumulated values in the respective cathode, like total energy deposit.
			// The hit also carries info on the individual and actual hits that have occured in the cathode, like arrival time of an optical photon.
			// This information is stored in vector format, e.g., arrival times of optical photons at the cathode = [t1,t2,t3,...]
			for(G4int i = 0; i < nHitPcs; i++){
				G4double photoncount = (*pcHC)[i]->GetPhotonCount(); //number of optical photon hits in photocathode
				G4int pcnumber = (*pcHC)[i]->GetPhotocatNumber();
				G4int ntupleNo = pcnumber/2;
				G4cout <<"ntupleNo: " <<ntupleNo<< ", pcnumber: "<< pcnumber  << G4endl;
				G4ThreeVector pos = (*pcHC)[i]->GetPhotocatPos(); // x,y,z, coordinates of photocathode
				G4double edep = (*pcHC)[i]->GetEdep(); // total energy deposited in photocathode
				std::vector<G4double> hitPosX = (*pcHC)[i]->GetHitPosX(); // vector of all x hit positions in photocathode
				std::vector<G4double> hitPosY = (*pcHC)[i]->GetHitPosY(); // vector of all x hit positions in photocathode
				std::vector<G4double> hitPosZ = (*pcHC)[i]->GetHitPosZ(); // vector of all x hit positions in photocathode
				
				std::vector<G4double> timestamps = (*pcHC)[i]->GetTimestamps(); // arrival times at photocathode
				sort(timestamps.begin(), timestamps.end());
				// photocathode with neg. (N) z position 
				if(pcnumber%2 == 0){
					// N
					// TODO: ...
					
					analysisManager->FillNtupleDColumn(ntupleNo, 0, pos.x());
					analysisManager->FillNtupleDColumn(ntupleNo, 2, pos.y());
					analysisManager->FillNtupleDColumn(ntupleNo, 4, pos.z());
					analysisManager->FillNtupleDColumn(ntupleNo, 6, photoncount); 
					analysisManager->FillNtupleDColumn(ntupleNo, 8, edep); 
					fTimestampsN = timestamps;
					// Ntuple filled with hit positions in NovoRunAction.cc
					fHitPosX_N = hitPosX; // hit coordinates
					fHitPosY_N = hitPosY;
					fHitPosZ_N = hitPosZ;
					// Fill histogram for XY-hit distribution 
					for (G4int h = 0; h<fHitPosX_N.size() ; h++){
						analysisManager->FillH2(3,fHitPosX_N.at(h), fHitPosY_N.at(h));
					}
				}
				// photocathode with pos. (P) z position 
				else if(pcnumber%2 != 0){
					// P
					// TODO: ...
					
					// Fill ntuple
					analysisManager->FillNtupleDColumn(ntupleNo, 1, pos.x());
					analysisManager->FillNtupleDColumn(ntupleNo, 3, pos.y());
					analysisManager->FillNtupleDColumn(ntupleNo, 5, pos.z());
					analysisManager->FillNtupleDColumn(ntupleNo, 7, photoncount); 
					analysisManager->FillNtupleDColumn(ntupleNo, 9, edep); 
					fTimestampsP = timestamps; // Arrival times at photocathode
					// Ntuple filled with hit positions in NovoRunAction.cc
					fHitPosX_P = hitPosX; // 11
					fHitPosY_P = hitPosY; // 13
					fHitPosZ_P = hitPosZ; // 15
					for (G4int h = 0; h<fHitPosX_P.size() ; h++){
						analysisManager->FillH2(4,fHitPosX_P.at(h), fHitPosY_P.at(h));
					}
				}
			}
		}
	}
	// Add new rows to ntuples
	// scintillator ntuples
	for(G4int i = 0; i < fDetector->GetNScint(); i++){
		analysisManager->AddNtupleRow(i); // add a new row to every ntuple related to a scint bar. 
	}
	// backscatter gating ntuple
	//analysisManager->AddNtupleRow(n_BSGating_tupleNo);
}


G4ThreeVector NovoEventAction::CalculatePosition(G4ThreeVector vec)
{
	G4ThreeVector pos(0., 0., 0.);
	if (fDetector->GetScintZ() == 30.0*cm){
		G4double p0 = 1.02500e-02;
		G4double p1 = 3.38410e+00;
		pos.setX(vec.x());
		pos.setY(vec.y());
		pos.setZ(p0 + vec.z()*p1);
	}
	if (fDetector->GetScintZ() == 20.0*cm){
		G4double p0 = -6.18240e-02;
		G4double p1 = 7.26294e+00;
		pos.setX(vec.x());
		pos.setY(vec.y());
		pos.setZ(p0 + vec.z()*p1);
	}
	return pos;
}

G4ThreeVector NovoEventAction::CalculatePositionSigmoid(G4ThreeVector vec)
{
	G4ThreeVector pos(0., 0., 0.);
	//~ G4cout << "fDetector->GetScintZ(): " << fDetector->GetScintZ()/cm << G4endl;
	if (fDetector->GetScintZ() == 30.0*cm){
		G4double p0 = 4.53401e+00;
		G4double p1 = 1.01916e-02;
		G4double z = (p0*vec.z())/(1+std::abs(p1*vec.z()));
		pos.setX(vec.x());
		pos.setY(vec.y());
		pos.setZ(z);
	}
	if (fDetector->GetScintZ() == 20.0*cm){
		G4double p0 = 1.36367e+01;
		G4double p1 = -8.50906e-02;
		G4double z = (p0*vec.z())/(1+std::abs(p1*vec.z()));
		pos.setX(vec.x());
		pos.setY(vec.y());
		pos.setZ(z);
	}
	return pos;
}

G4ThreeVector NovoEventAction::CalculatePositionTime(G4ThreeVector vec)
{
	G4ThreeVector pos(0., 0., 0.);
	if (fDetector->GetScintZ() == 30.0*cm){
		G4double p0 = -1.21310e-02;
		G4double p1 = 2.31182e+00;
		pos.setX(vec.x());
		pos.setY(vec.y());
		pos.setZ(p0 + vec.z()*p1);
	}
	if (fDetector->GetScintZ() == 20.0*cm){
		G4double p0 = 6.45101e-02;
		G4double p1 = 4.68424e+00;
		pos.setX(vec.x());
		pos.setY(vec.y());
		pos.setZ(p0 + vec.z()*p1);
	}
	return pos;
}

G4ThreeVector NovoEventAction::CalculatePositionTimeSigmoid(G4ThreeVector vec)
{
	G4ThreeVector pos(0., 0., 0.);
	//~ G4cout << "fDetector->GetScintZ(): " << fDetector->GetScintZ()/cm << G4endl;
	if (fDetector->GetScintZ() == 30.0*cm){
		G4double p0 = 2.89218e+00;
		G4double p1 = 5.09484e-03;
		G4double z = (p0*vec.z())/(1+std::abs(p1*vec.z()));
		pos.setX(vec.x());
		pos.setY(vec.y());
		pos.setZ(z);
	}
	if (fDetector->GetScintZ() == 20.0*cm){
		G4double p0 = 7.97598e+00;
		G4double p1 = 4.41081e-02;
		G4double z = (p0*vec.z())/(1+std::abs(p1*vec.z()));
		pos.setX(vec.x());
		pos.setY(vec.y());
		pos.setZ(z);
	}
	return pos;
}
