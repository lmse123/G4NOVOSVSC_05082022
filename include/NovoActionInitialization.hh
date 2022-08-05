/*
 * G4NovoSVSC
 * Action initialization
 * Header file
 * Author: Kyrre Skjerdal (kyrre.skjerdal@hvl.no)
 */

/// \file NovoActionInitialization.hh
/// \brief Definition of the NovoActionInitialization class


#ifndef NovoActionInitialization_h
#define NovoActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class NovoDetectorConstruction;

class NovoActionInitialization : public G4VUserActionInitialization
{
	public:
	    NovoActionInitialization(NovoDetectorConstruction* det);
	    virtual ~NovoActionInitialization();

		virtual void BuildForMaster() const;
		virtual void Build() const;

	private:
		NovoDetectorConstruction* fDetector;
};

#endif
