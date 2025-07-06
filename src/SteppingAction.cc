#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

namespace photon_dose_sim
{

    SteppingAction::SteppingAction(EventAction* eventAction)
        : fEventAction1(eventAction), fEventAction2(eventAction)
    {
    }

    void SteppingAction::UserSteppingAction(const G4Step* step)
    {
        if (!fScoringVolume1) {
            const auto detConstruction1 = static_cast<const DetectorConstruction*>(
                G4RunManager::GetRunManager()->GetUserDetectorConstruction());
            fScoringVolume1 = detConstruction1->GetScoringVolume1();
        }

        // get volume of the current step
        G4LogicalVolume* volume1
            = step->GetPreStepPoint()->GetTouchableHandle()
            ->GetVolume()->GetLogicalVolume();

        // check if we are in scoring volume 1
        if (volume1 == fScoringVolume1) {
            // collect energy deposited in this step
            G4double edepStep1 = step->GetTotalEnergyDeposit();
            fEventAction1->AddEdep1(edepStep1);
        }

        if (!fScoringVolume2) {
            const auto detConstruction2 = static_cast<const DetectorConstruction*>(
                G4RunManager::GetRunManager()->GetUserDetectorConstruction());
            fScoringVolume2 = detConstruction2->GetScoringVolume2();
        }

        // get volume of the current step for the second volume
        G4LogicalVolume* volume2
            = step->GetPreStepPoint()->GetTouchableHandle()
            ->GetVolume()->GetLogicalVolume();

        // check if we are in scoring volume 2
        if (volume2 == fScoringVolume2) {
            // collect energy deposited in this step for the second volume
            G4double edepStep2 = step->GetTotalEnergyDeposit();
            fEventAction2->AddEdep2(edepStep2);
        }
    }

}

