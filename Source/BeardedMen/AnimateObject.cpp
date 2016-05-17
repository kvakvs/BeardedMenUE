// Copyright 2016, Dmytro Lytovchenko

#include "BeardedMen.h"
#include "AnimateObject.h"

AAnimateObject::AAnimateObject(bm::World *wo)
{
    // Set this actor to call Tick() every frame. 
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    world_ = wo;
}

// Called when the game starts or when spawned
void AAnimateObject::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AAnimateObject::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

const bm::ai::ActionDefVec& AAnimateObject::ai_get_all_actions() const
{
    static bm::ai::ActionDefVec _tmp;
    return _tmp;
}

