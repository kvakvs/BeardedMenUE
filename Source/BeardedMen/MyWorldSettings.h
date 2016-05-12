// Copyright 2016, Dmytro Lytovchenko

#pragma once

#include "GameFramework/WorldSettings.h"

#include "WorldVolume.h"
#include <memory>

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
public:
    // Visible piece of world + some nearby
    std::unique_ptr<bm::VolumeType> volume_;
};
