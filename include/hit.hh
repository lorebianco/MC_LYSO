#ifndef HIT_HH
#define HIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
#include "G4UnitsTable.hh"


class MyHit : public G4VHit
{
public:
    MyHit();
    ~MyHit();

    // Operators
    MyHit& operator=(const MyHit&) = default;
    G4bool operator==(const MyHit&) const;
    inline void* operator new(size_t);
    inline void operator delete(void*);

    // Set methods
    inline void SetDetection(G4bool detected) { isDetected = detected; }
    inline void SetOpticalPhotonTime(G4double t) { fOpticalPhotonTime = t; }
    inline void SetOpticalPhotonPos(G4ThreeVector xyz) { fOpticalPhotonPos = xyz; }
    inline void SetDetectorPos(G4ThreeVector xyz) { fDetectorPos = xyz; }
    inline void SetDetectorChannel(G4int ch) { fCh = ch; }

    // Get methods
    inline G4bool GetDetection() const { return isDetected; } 
    inline G4double GetOpticalPhotonTime() const { return fOpticalPhotonTime; }
    inline G4ThreeVector GetOpticalPhotonPos() const { return fOpticalPhotonPos; }
    inline G4ThreeVector GetDetectorPos() const { return fDetectorPos; }
    inline G4int GetDetectorChannel() const { return fCh; }

private:
    G4bool isDetected;
    G4double fOpticalPhotonTime;
    G4ThreeVector fOpticalPhotonPos;
    G4ThreeVector fDetectorPos;
    G4int         fCh;
};


using MyHitsCollection = G4THitsCollection<MyHit>;


extern G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator;


inline void* MyHit::operator new(size_t)
{
    if(!MyHitAllocator)
        MyHitAllocator = new G4Allocator<MyHit>;
    return (void *) MyHitAllocator->MallocSingle();
}




inline void MyHit::operator delete(void *hit)
{
    MyHitAllocator->FreeSingle((MyHit*) hit);
}

#endif  // HIT_HH
