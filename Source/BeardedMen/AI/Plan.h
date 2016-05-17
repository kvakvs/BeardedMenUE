#pragma once

#include "AI/Goal.h"
#include "AI/Action.h"
#include "Game/GameDefs.h"

namespace bm {

class World;
//class AAnimateObject;

namespace ai {

ActionVec propose_plan(World *wo, const MetricVec& from, const MetricVec& to,
                       const ai::Context& ctx);

namespace impl {
    bool check_requirements(const MetricVec& required,
                            const MetricVec& have);
    bool have_metric(const MetricVec& v, MetricType mt);
    Metric get_metric(const MetricVec& v, MetricType mt);
} // ns impl

} // ns bm::ai
} // ns bm
