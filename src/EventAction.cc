#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

namespace photon_dose_sim
{


EventAction::EventAction(RunAction* runAction)
: fRunAction1(runAction),fRunAction2(runAction)
{}


void EventAction::BeginOfEventAction(const G4Event*)
{
  fEdep0 = 0.;
   fEdep2 = 0.;
}


void EventAction::EndOfEventAction(const G4Event*)
{
  // accumulate statistics in run action
  fRunAction1->AddEdep1(fEdep0);
  fRunAction2->AddEdep2(fEdep2);
}


}
