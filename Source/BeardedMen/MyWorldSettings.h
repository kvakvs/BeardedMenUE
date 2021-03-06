// Copyright 2016, Dmytro Lytovchenko

#pragma once

#include "GameFramework/WorldSettings.h"
#include "Game/WorldVolume.h"
#include "Game/World.h"

// always goes last
#include "MyWorldSettings.generated.h"

/**
 * 
 */
UCLASS()
class BEARDEDMEN_API AMyWorldSettings : public AWorldSettings
{
  GENERATED_BODY()

  AMyWorldSettings();
  virtual void BeginPlay() override;
  static pv::Region get_whole_region();

public:
  // Visible piece of world + some nearby
  std::unique_ptr<bm::VolumeType> volume_;
  std::unique_ptr<bm::World> world_;
};
