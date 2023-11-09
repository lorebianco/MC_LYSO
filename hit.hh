#ifndef HIT_HH
#define HIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

#include "G4UnitsTable.hh"

#include <iomanip>


class MyHit : public G4VHit
{
public:
    MyHit();
    ~MyHit();

    // operators
    MyHit& operator=(const MyHit&) = default;
    G4bool operator==(const MyHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // Set methods
    void SetTrackID(G4int track) { fTrackID = track; }
    void SetOpticalPhotonTime (G4double t) {fOpticalPhotonTime = t; }
    void SetOpticalPhotonPos (G4ThreeVector xyz) { fOpticalPhotonPos = xyz; }
    void SetDetectorPos (G4ThreeVector xyz) {fDetectorPos = xyz; }
    void SetDetectorChannel(G4int ch)      { fCh = ch; }

    // Get methods
    G4int GetTrackID() const     { return fTrackID; }
    G4double GetOpticalPhotonTime() const   { return fOpticalPhotonTime; }
    G4ThreeVector GetOpticalPhotonPos() const     { return fOpticalPhotonPos; }
    G4ThreeVector GetDetectorPos() const { return fDetectorPos; }
    G4int GetDetectorChannel() const { return fCh; }

  private:
    G4int fTrackID = -1;
    G4double fOpticalPhotonTime;
    G4ThreeVector fOpticalPhotonPos, fDetectorPos;
    G4int         fCh = -1;
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

#endif
