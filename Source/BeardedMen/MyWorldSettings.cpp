#include "BeardedMen.h"
#include "MyWorldSettings.h"
#include "Util/Vec.h"
#include "PolyVox/Region.h"
namespace pv = PolyVox;

AMyWorldSettings::AMyWorldSettings()
{
  volume_ = std::make_unique<bm::VolumeType>(get_whole_region());
}

pv::Region AMyWorldSettings::get_whole_region() {
  return pv::Region(
    bm::Vec3i(0, 0, 0),
    bm::Vec3i(bm::WORLDSZ_X, bm::WORLDSZ_Y, bm::WORLDSZ_Z)
  );
}

void AMyWorldSettings::BeginPlay()
{
  auto reg = get_whole_region();
  bm::populate::populate_voxels(reg, *volume_);
}
