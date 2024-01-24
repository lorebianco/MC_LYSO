#include "hit.hh"

MyHit::MyHit()
{}



MyHit::~MyHit()
{}


// Use the G4Allocator
G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator = nullptr;


G4bool MyHit::operator==(const MyHit& right) const
{
  return ( this == &right ) ? true : false;
}
