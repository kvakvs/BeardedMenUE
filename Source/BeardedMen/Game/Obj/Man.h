#pragma once

#include "Tab/ModelId.h"
#include "Game/Component.h"
#include "Game/Co/Body.h"
//#include "Game/Co/Worker.h"
#include "Game/Co/Brains.h"
#include "Game/Co/Ent.h"
#include "AI/Action.h"

namespace bm {

// TODO: evict from this file, make own file
class BeardedMan: public AnimateObject
{
public:
    BeardedMan(World *wo, const Vec3i &pos)
        : AnimateObject(wo),
          entity_(this, pos, ModelId::BeardedMan),
          body_(BodyType::BeardedMan),
          brains_(this)
    {
    }
    virtual EntityComponent* as_entity() override { return &entity_; }
    virtual const EntityComponent* as_entity() const override {return &entity_;}

    virtual BrainsComponent* as_brains() override { return &brains_; }
    virtual const BrainsComponent* as_brains() const override {return &brains_;}

    virtual BodyComponent* as_body() override { return &body_;   }
    virtual const BodyComponent* as_body() const override { return &body_;   }
    //virtual WorkerComponent* as_worker() override { return &worker_; }

    // Filter list of available actions which satisfy one of conditions
    //virtual ai::ActionVec ai_choose_actions(const ai::Goal& g) const override;

    // Initializes static vector of actions and returns constref
    virtual const ai::ActionDefVec& ai_get_all_actions() const override;

private:
    EntityComponent entity_;
    BrainsComponent brains_;
    BodyComponent   body_;
    //WorkerComponent worker_;
};

} // ns bm
