// Copyright 2016, Dmytro Lytovchenko

#pragma once

#include "GameFramework/Actor.h"

#include <point_multimap.hpp>

#include "Game/GameDefs.h"
#include "Tab/ModelId.h"

#include "InanimateObject.generated.h"

UCLASS()
class BEARDEDMEN_API AInanimateObject : public AActor
{
    GENERATED_BODY()

public:	
    // Sets default values for this actor's properties
    AInanimateObject();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
	
    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

public:
    bm::InanimateType type_;
    bm::ModelId       model_;

    void set_type(bm::InanimateType t) { type_ = t; }
};


namespace bm {
    using SpatialInanimateMap = spatial::point_multimap<3, Array3i, AInanimateObject *>;
} // ns bm
