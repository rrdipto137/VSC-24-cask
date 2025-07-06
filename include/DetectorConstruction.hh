/// \file photon-dose-sim/include/DetectorConstruction.hh
/// \brief Definition of the photon-dose-sim::DetectorConstruction class

#ifndef PhotonDoseSimDetectorConstruction_h
#define PhotonDoseSimDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4NistManager.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

namespace photon_dose_sim
{

    class DetectorConstruction : public G4VUserDetectorConstruction
    {
    public:
        DetectorConstruction() = default;
        ~DetectorConstruction() override = default;

        G4VPhysicalVolume* Construct() override;

        G4LogicalVolume* GetScoringVolume1() const { return fScoringVolume1; }
G4LogicalVolume* GetScoringVolume2() const { return fScoringVolume2; }
    protected:
        G4LogicalVolume* fScoringVolume1 = nullptr;
        G4LogicalVolume* fScoringVolume2 = nullptr;
    };

}


#endif
