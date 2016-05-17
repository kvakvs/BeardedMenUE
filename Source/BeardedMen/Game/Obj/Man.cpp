// Copyright 2016, Dmytro Lytovchenko

#include "BeardedMen.h"
#include "Game/Obj/Man.h"


// Sets default values
AMan::AMan()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    brains_.set_parent((AAnimateObject *)this);
    entity_.set_parent((AAnimateObject *)this);
    body_.set_body_type(bm::BodyType::BeardedMan);
}

// Called when the game starts or when spawned
void AMan::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMan::Tick(float dt)
{
    Super::Tick(dt);
}

const bm::ai::ActionDefVec &AMan::ai_get_all_actions() const
{
    static bool is_initialized = false;
    static bm::ai::ActionDefVec bm_actions;

    if (not is_initialized) {
        //bm_actions = ai_load_actions("bearded_man");
        using AT   = bm::ai::ActionType;
        using Mtrc = bm::ai::Metric;
        using CT   = bm::ai::MetricType;
        using V    = bm::ai::Value;

        bm_actions.push_back(bm::ai::ActionDef( AT::Move,
            // Requires
            { Mtrc(CT::HaveLeg, V(), V(true) /* needs subject */)
            },
            // Gives
            { Mtrc(CT::MeleeRange, V(), V(true)) /* needs pos */
            }));

        bm_actions.push_back(bm::ai::ActionDef( AT::MoveClose,
            // Requires
            { Mtrc(CT::HaveLeg, V(), V(true) /* needs subject */)
            },
            // Gives
            { Mtrc(CT::MeleeRangeDepth, V(), V(true)) /* needs pos */
            }));

        bm_actions.push_back(bm::ai::ActionDef( AT::Mine,
            // Requires
            { Mtrc(CT::MeleeRangeDepth, V(), V(true)), // needs arg (pos)
              Mtrc(CT::HaveHand, V(), V(true)), /* needs subject */
              Mtrc(CT::HaveMiningPick, V(), V(true) /* needs subject */)
            },
            // Gives
            { Mtrc(CT::BlockIsNotSolid, V(), V(true) /* needs arg=pos */)
            }));

        bm_actions.push_back(bm::ai::ActionDef( AT::CreateRamp,
            // Requires
            { Mtrc(CT::MeleeRangeDepth, V(), V(true)), // needs arg (pos)
              Mtrc(CT::HaveHand, V(), V(true)), /* needs subject */
              Mtrc(CT::HaveMiningPick, V(), V(true) /* needs subject */)
            },
            // Gives
            { Mtrc(CT::BlockIsRamp, V(), V(true) /* needs arg=pos */)
            }));

        is_initialized = true;
    }

    return bm_actions;
}
