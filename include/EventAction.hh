//
#ifndef PhotonDoseSimEventAction_h
#define PhotonDoseSimEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

/// Event action class
///

namespace photon_dose_sim
{

class RunAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    void AddEdep1(G4double edep0) { fEdep0 += edep0; }
     void AddEdep2(G4double edep2) { fEdep2 += edep2; }

  private:
    RunAction* fRunAction1 = nullptr;
    RunAction* fRunAction2 = nullptr;
    G4double   fEdep0 = 0.;
    G4double   fEdep2 = 0.;
};

}


#endif


