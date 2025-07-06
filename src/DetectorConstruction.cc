#include "DetectorConstruction.hh"
#include "G4Tubs.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4ExtrudedSolid.hh"
#include "G4VisAttributes.hh"
#include "G4CutTubs.hh"
#include "G4SubtractionSolid.hh"
namespace photon_dose_sim
{
    G4VPhysicalVolume* DetectorConstruction::Construct()
    {
         G4NistManager* nist = G4NistManager::Instance();

        // world properties
        G4Material* worldMaterial = nist->FindOrBuildMaterial("G4_Galactic");
        G4bool checkOverlaps = false;

        G4int worldSizeX = 5*m;
        G4int worldSizeY = 5*m;
        G4int worldSizeZ = 5*m;


        // define worlds
        auto solidWorld = new G4Box("World",
            worldSizeX,                 // default is mm (it seems like)
            worldSizeY,
            worldSizeZ);

        auto logicWorld = new G4LogicalVolume(solidWorld,
            worldMaterial, "World");

        auto physWorld = new G4PVPlacement(nullptr,  // no rotation
            G4ThreeVector(),                           // at (0,0,0)
            logicWorld,                                // its logical volume
            "World",                                   // its name
            nullptr,                                   // its mother  volume
            false,                                     // no boolean operation
            0,                                         // copy number
            checkOverlaps);                            // overlaps checking

        
        

   
       // First Detector
     G4Material*detector1Material = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
     
   
        G4double detector1SizeX = 0.4 ;
        G4double detector1SizeY = 0.4 ;
        G4double detector1SizeZ = 0.4 * cm;
        G4ThreeVector detector1Pos = G4ThreeVector(0, 0, -5 * cm);

        auto solidDetector1 = new G4Box("Detector1",
            detector1SizeX,
            detector1SizeY,
            detector1SizeZ);

        auto logicDetector1 = new G4LogicalVolume(solidDetector1,
            detector1Material,
            "Detector1");

        new G4PVPlacement(nullptr,    // rotation
            detector1Pos,              // position
            logicDetector1,           // its logical volume
            "Detector1",              // its name
            logicWorld,               // its mother  volume
            false,                    // boolean operation
            0,                        // copy number
            checkOverlaps);           // overlaps checking

      
// Materials
    G4Material* steelMaterial = nist->FindOrBuildMaterial("G4_Fe");
    G4Material* heliumMaterial = nist->FindOrBuildMaterial("G4_He");
    G4Material* concreteMaterial = nist->FindOrBuildMaterial("G4_CONCRETE");
    G4Material* airMaterial = nist->FindOrBuildMaterial("G4_AIR");
    
    // World dimensions
   G4double worldSizeX2 = 10 * m;
    G4double worldSizeY2 = 10 * m;
    G4double worldSizeZ2= 10 * m;
    
    auto solidWorld2 = new G4Box("World", worldSizeX2/2, worldSizeY2/2, worldSizeZ2/2);
    auto logicWorld2 = new G4LogicalVolume(solidWorld, airMaterial, "World");
    new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);
    
    
     G4double caskHeight = 4.973 * m;
    G4double caskRadius = 1.715 * m / 2;
    G4double containerHeight = 5.809 * m;
    G4double containerRadius = 3.378 * m / 2;
     G4double annularWidth = 0.070 * m;
    
    
   // Outer Concrete Layer (Hollow)

auto solidOuter = new G4Tubs("Outer", 0, containerRadius, containerHeight/2, 0, 360*deg);
auto solidInner = new G4Tubs("Inner", 0, caskRadius, containerHeight/2 + 1*mm, 0, 360*deg); // Inner void
auto solidContainer = new G4SubtractionSolid("Container", solidOuter, solidInner);

auto logicContainer = new G4LogicalVolume(solidContainer, concreteMaterial, "Container");
new G4PVPlacement(nullptr, G4ThreeVector(), logicContainer, "Container", logicWorld, false, 0, true);
logicContainer->SetVisAttributes(new G4VisAttributes(G4Colour(1.0, 0.0, 0.0))); // Orange

// Inner Storage Cask (Helium-filled)

auto solidCask = new G4Tubs("Cask", 0, caskRadius, caskHeight/2, 0, 360*deg);
auto logicCask = new G4LogicalVolume(solidCask, heliumMaterial, "Cask");
new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), logicCask, "Cask", logicContainer, false, 0, true);
logicCask->SetVisAttributes(new G4VisAttributes(G4Colour(0.0, 0.0, 1.0))); // green (Helium gas)

    
    // Annular Channel (Gap between container and cask)
   
    G4double annularInnerRadius = caskRadius;
    G4double annularOuterRadius = caskRadius + annularWidth;
    auto solidAnnularChannel = new G4Tubs("AnnularChannel", annularInnerRadius, annularOuterRadius, caskHeight/2, 0, 360*deg);
    auto logicAnnularChannel = new G4LogicalVolume(solidAnnularChannel, airMaterial, "AnnularChannel");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), logicAnnularChannel, "AnnularChannel", logicContainer, false, 0, true);
    logicAnnularChannel->SetVisAttributes(new G4VisAttributes(G4Colour(0.0, 1.0, 0.0))); // Green (Annular gap) 
    
   

// Define square fuel cell size
G4double fuelCellSize = 0.2 * m; // Approximate size of each square cell
G4double fuelRegionRadius = caskRadius * 0.85;

// Define the pattern (number of cells per row)
std::vector<int> cellsPerRow = {2, 4, 6, 6, 4, 2}; // Matches the given pattern

// Define square fuel cell shape
G4VSolid* solidFuelCell = new G4Box("FuelCell", fuelCellSize/2, fuelCellSize/2, caskHeight/2);
G4LogicalVolume* logicFuelCell = new G4LogicalVolume(solidFuelCell, steelMaterial, "FuelCell");
//logicFuelCell->SetVisAttributes(new G4VisAttributes(G4Colour(0.6, 0.0, 0.0))); // Purple

// Arrange fuel cells in the desired pattern within the circular cask
G4double startY = -((cellsPerRow.size() - 1) * fuelCellSize) / 2; // Centering Y

for (size_t row = 0; row < cellsPerRow.size(); row++) {
    G4int numCells = cellsPerRow[row];
    G4double startX = -((numCells - 1) * fuelCellSize) / 2; // Centering X for each row

    for (G4int col = 0; col < numCells; col++) {
        G4double xPos = startX + col * fuelCellSize;
        G4double yPos = startY + row * fuelCellSize;

        // Ensure cells stay within the circular boundary
        if (std::sqrt(xPos * xPos + yPos * yPos) + fuelCellSize/2 < fuelRegionRadius) {
            new G4PVPlacement(nullptr, G4ThreeVector(xPos, yPos, 0), logicFuelCell, "FuelCell", logicCask, false, row * 10 + col, true);
        }
    }
}

        
        
        
        
        
        
        // Second Detector (45 cm apart)
       G4Material*detector2Material = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
        
       /* G4double borontrifluorideDensity = 2.76* g/cm3;
        G4Material*detector2Material =new G4Material("borontrifluoride", borontrifluorideDensity, 2);
        
        detector2Material->AddElement(nist->FindOrBuildElement("B"), 15.94 * perCent);
	detector2Material->AddElement(nist->FindOrBuildElement("F"), 84.06* perCent); */
        
        
        G4double detector2SizeX = 0.4*cm ;
        G4double detector2SizeY = 0.4*cm;
        G4double detector2SizeZ = 0.4* cm;
        G4ThreeVector detector2Pos2 = G4ThreeVector(0, 0, 50 * cm); // 50 cm apart in the z-direction

        auto solidDetector2 = new G4Box("Detector2",
            detector2SizeX,
            detector2SizeY,
            detector2SizeZ);

        auto logicDetector2 = new G4LogicalVolume(solidDetector2,
            detector2Material,
            "Detector2");

        new G4PVPlacement(nullptr,    // rotation
            detector2Pos2,             // position
            logicDetector2,           // its logical volume
            "Detector2",              // its name
            logicWorld,               // its mother  volume
            false,                    // boolean operation
            1,                        // copy number (different than the first detector)
            checkOverlaps);           // overlaps checking

        

        // Set scoring volumes
        fScoringVolume1= logicDetector2;
         fScoringVolume2= logicDetector1;
        
       
        return physWorld;
    }
}

