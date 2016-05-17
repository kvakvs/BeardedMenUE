// Copyright 2016, Dmytro Lytovchenko

#pragma once

#include "GameFramework/Actor.h"

#include "Util/Vec.h"
#include "AnimateObject.h"
#include "Tab/ModelId.h"
#include "Game/Co/Body.h"
#include "Game/Co/Brains.h"
#include "Game/Co/Ent.h"
#include "AI/Action.h"

#include "Man.generated.h"

UCLASS()
class BEARDEDMEN_API AMan : public AAnimateObject
{
    GENERATED_BODY()
	
public:	
    // Sets default values for this actor's properties
    AMan();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
	
    // Called every frame
    virtual void Tick(float dt) override;

public:
    //AMan(bm::World *wo, const bm::Vec3i &pos)
    //    : AAnimateObject(wo),
    //      entity_(),
    //      body_(BodyType::BeardedMan),
    //      brains_(this)
    //{
    //    entity_->set_parent(this);
    //}

    virtual bm::EntityComponent* as_entity() override { return &entity_; }
    virtual const bm::EntityComponent* as_entity() const override { return &entity_; }

    virtual bm::BrainsComponent* as_brains() override { return &brains_; }
    virtual const bm::BrainsComponent* as_brains() const override { return &brains_; }

    virtual bm::BodyComponent* as_body() override { return &body_; }
    virtual const bm::BodyComponent* as_body() const override { return &body_; }

    // Initializes static vector of actions and returns constref
    virtual const bm::ai::ActionDefVec& ai_get_all_actions() const override;

private:
    bm::EntityComponent entity_;
    bm::BrainsComponent brains_;
    bm::BodyComponent   body_;
};
