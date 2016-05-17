// Copyright 2016, Dmytro Lytovchenko
#pragma once

#include "GameFramework/Actor.h"

#include <stdint.h>
#include <vector>

#include "AI/Goal.h"
#include "AI/Action.h"

#include "AnimateObject.generated.h"

namespace bm {
    class EntityComponent;
    class BrainsComponent;
    class BodyComponent;
    class WorkerComponent;

#define AS_COMPONENT(Lowercase, Capitalized) \
    virtual bm::Capitalized##Component* as_##Lowercase() { \
        return nullptr;  \
    } \
    virtual const bm::Capitalized##Component* as_##Lowercase() const { \
        return nullptr;  \
    }
} // ns bm

UCLASS()
class BEARDEDMEN_API AAnimateObject : public AActor
{
    GENERATED_BODY()
	
public:	
    // Sets default values for this actor's properties
    AAnimateObject() {}
    AAnimateObject(bm::World *wo);

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
	
    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

public:
    AS_COMPONENT(entity, Entity)
    AS_COMPONENT(brains, Brains)
    AS_COMPONENT(body, Body)
    AS_COMPONENT(worker, Worker)

    bm::World* get_world() { return world_; }

    // Override this for descendants to change available actions (or script it)
    virtual const bm::ai::ActionDefVec& ai_get_all_actions() const;

private:
    bm::World *world_;
};
