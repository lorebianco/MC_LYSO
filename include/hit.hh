/**
 * @file hit.hh
 * @brief Declaration of the class @ref MyHit
 */
#ifndef HIT_HH
#define HIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
#include "G4UnitsTable.hh"

/**
 * @brief Concrete class of G4VHit, representing a hit in the
 * @ref MySensitiveDetector.
 */
class MyHit : public G4VHit
{
public:
    MyHit() = default; /**< @brief Constructor of the class.*/
    ~MyHit() override = default; /**< @brief Destructor of the class.*/

    // Operators
    MyHit& operator=(const MyHit&) = default; /**< @brief Assignment operator, default implementation.*/
    G4bool operator==(const MyHit&) const; /**< @brief Equality operator, compares two MyHit objects for equality.*/

    // Operator overloads for memory management
    inline void* operator new(size_t); /**< @brief Overloaded new operator for memory allocation: it uses the @ref MyHitAllocator for fast memory handling.*/
    inline void operator delete(void*); /**< @brief Overloaded delete operator for memory deallocation: it uses the @ref MyHitAllocator for fast memory handling.*/

    // Set methods
    inline void SetDetectionTime(G4double t) { fDetectionTime = t; } /**< @brief Set the detection time of the optical photon.*/
    inline void SetDetectorPosition(G4ThreeVector xyz) { fDetectorPosition = xyz; } /**< @brief Set the position (center) of the SiPM hit by the optical photon.*/
    inline void SetDetectorChannel(G4int ch) { fCh = ch; } /**< @brief Set the channel of the SiPM hit by the optical photon.*/

    // Get methods
    inline G4double GetDetectionTime() const { return fDetectionTime; } /**< @brief Get the detection time of the optical photon.*/
    inline G4ThreeVector GetDetectorPosition() const { return fDetectorPosition; } /**< @brief Get the position (center) of the SiPM hit by the optical photon.*/
    inline G4int GetDetectorChannel() const { return fCh; } /**< @brief Get the channel of the SiPM hit by the optical photon.*/

private:
    G4double fDetectionTime; /**< @brief Time of detection of the optical photon.*/
    G4ThreeVector fDetectorPosition; /**< @brief Position (center) of the SiPM hit by the optical photon.*/
    G4int fCh; /**< @brief Channel of the SiPM hit by the optical photon.*/
};

/** @brief Concrete hit collection class for @ref MyHit.
 *
 * It is instantiated from G4THitsCollection, a template class derived from 
 * G4VHitsCollection (basically a vector collection of hits).
 */
using MyHitsCollection = G4THitsCollection<MyHit>;

/** @brief G4Allocator for @ref MyHit objects. 
 *
 * G4Allocator is a class for fast allocation of objects to the heap.
 */
extern G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator;


// Implementation of the new operator for MyHit by leveraging MyHitAllocator
inline void* MyHit::operator new(size_t)
{
    // Create a new G4Allocator if it does not exist
    if (!MyHitAllocator)
            MyHitAllocator = new G4Allocator<MyHit>;

    // Allocate memory for a single MyHit object
    return (void*)MyHitAllocator->MallocSingle();
}

// Implementation of the delete operator for MyHit by leveraging MyHitAllocator
inline void MyHit::operator delete(void* hit)
{
    // Free the memory for a single MyHit object
    MyHitAllocator->FreeSingle(static_cast<MyHit*>(hit));
}

#endif  // HIT_HH
