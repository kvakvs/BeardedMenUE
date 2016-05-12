// Copyright 2016, Dmytro Lytovchenko

#include "BeardedMen.h"
#include "MyWorldSettings.h"
#include "Util/Vec.h"
#include "PolyVox/Region.h"
namespace pv = PolyVox;

AMyWorldSettings::AMyWorldSettings()
{
  pv::Region reg(
    bm::Vec3i(0, 0, 0),
    bm::Vec3i(bm::WORLDSZ_X, bm::WORLDSZ_Y, bm::WORLDSZ_Z)
  );
  volume_ = std::make_unique<bm::VolumeType>(reg);
  bm::populate::populate_voxels(reg, *volume_);
}

void AMyWorldSettings::BeginPlay()
{
}
