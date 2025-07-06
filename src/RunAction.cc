#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

namespace photon_dose_sim
{

    RunAction::RunAction()
    {
        // add new units for dose
        //
        const G4double milligray = 1.e-3 * gray;
        const G4double microgray = 1.e-6 * gray;
        const G4double nanogray = 1.e-9 * gray;
        const G4double picogray = 1.e-12 * gray;

        new G4UnitDefinition("milligray", "milliGy", "Dose", milligray);
        new G4UnitDefinition("microgray", "microGy", "Dose", microgray);
        new G4UnitDefinition("nanogray", "nanoGy", "Dose", nanogray);
        new G4UnitDefinition("picogray", "picoGy", "Dose", picogray);

        // Register accumulable to the accumulable manager
        G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
        accumulableManager->RegisterAccumulable(fEdep0);
        accumulableManager->RegisterAccumulable(fEdep1);
        accumulableManager->RegisterAccumulable(fEdep2);
        accumulableManager->RegisterAccumulable(fEdep3);
    }


    void RunAction::BeginOfRunAction(const G4Run*)
    {
        // inform the runManager to save random number seed
        G4RunManager::GetRunManager()->SetRandomNumberStore(false);

        // reset accumulables to their initial values
        G4AccumulableManager* accumulableManager1 = G4AccumulableManager::Instance();
        accumulableManager1->Reset();

    }


    void RunAction::EndOfRunAction(const G4Run* run)
    {
      G4int nofEvents1 = run->GetNumberOfEvent();
        if (nofEvents1 == 0) return;

        // Merge accumulables
        G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
        accumulableManager->Merge();

        // Compute dose = total energy deposit in a run and its variance
        //
        G4double edep0 = fEdep0.GetValue();
        G4double edep1 = fEdep1.GetValue();

        G4double rms1 = edep1 - edep0 * edep0 / nofEvents1;
        if (rms1 > 0.) rms1 = std::sqrt(rms1); else rms1 = 0.;

        const auto detConstruction1 = static_cast<const DetectorConstruction*>(
            G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        G4double mass1 = detConstruction1->GetScoringVolume1()->GetMass();
        G4double dose1 = edep1 / mass1;
        G4double rmsDose1 = rms1 / mass1;

     
     
     
     
     
     
        G4int nofEvents2 = run->GetNumberOfEvent();
        if (nofEvents2 == 0) return;
   // Merge accumulables
        G4AccumulableManager* accumulableManager2 = G4AccumulableManager::Instance();
        accumulableManager2->Merge(); 

        // Compute dose = total energy deposit in a run and its variance
        //
        G4double edep2 = fEdep2.GetValue();
        G4double edep3 = fEdep3.GetValue();

        G4double rms2 = edep3 - edep2 * edep2 / nofEvents2;
        if (rms2 > 0.) rms2 = std::sqrt(rms2); else rms2 = 0.;

        const auto detConstruction2 = static_cast<const DetectorConstruction*>(
            G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        G4double mass2 = detConstruction2->GetScoringVolume2()->GetMass();
        G4double dose2 = edep3 / mass2;
        G4double rmsDose2 = rms2 / mass2;
        
        
        
        
        
        
        
        
        
        
        
        // Run conditions
        //  note: There is no primary generator action object for "master"
        //        run manager for multi-threaded mode.
        const auto generatorAction = static_cast<const PrimaryGeneratorAction*>(
            G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
        G4String runCondition;
        if (generatorAction)
        {
            const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
            runCondition += particleGun->GetParticleDefinition()->GetParticleName();
            runCondition += " of ";
            G4double particleEnergy = particleGun->GetParticleEnergy();
            runCondition += G4BestUnit(particleEnergy, "Energy");
        }

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
            << " The run consists of " << nofEvents1
            << G4endl
            << " Cumulated dose per run in Detector1, in scoring volume : "
            << G4BestUnit(dose2, "Dose") << " rms2 = " << G4BestUnit(rmsDose2, "Dose")
            << G4endl
            << " Cumulated dose per run in Detector2, in scoring volume : "
            << G4BestUnit(dose1, "Dose") << " rms1 = " << G4BestUnit(rmsDose1, "Dose")
            << G4endl
            << "------------------------------------------------------------"
            << G4endl
            << G4endl;
    }

    void RunAction::AddEdep1(G4double edep0)
    {
        fEdep0 += edep0;
        fEdep1 += edep0 * edep0;
    }

    void RunAction::AddEdep2(G4double edep2)
    {
        fEdep2 += edep2;
        fEdep3 += edep2 * edep2;
    }

}
