#pragma once

#include <stdint.h>
#include <list>

#include "Util/Vec.h"
//#include "Gfx/Model.h"
#include "Tab/ModelId.h"
#include "Game/GameDefs.h"

class AAnimateObject;

namespace bm {

class World;
using EntityId = uint64_t;
using Route = std::list<Vec3i>;

//QDebug operator<< (QDebug d, const Route& r);

// Entity is anything which has a position in the world and is visible in
// some way (has model or a sprite etc).
// This includes both animate and inanimate objects (dropped resources,
// dead bodies etc)
class EntityComponent {
    AAnimateObject *parent_ = nullptr;
    //Vec3i pos_;
    EntityId id_;
    //ModelId model_id_;

    struct {
        //bool is_moving_ = false;
        Route planned_route;
        Vec3i dst;
    } movement_;

public:
    EntityComponent() {}
    void set_parent(AAnimateObject *p) { parent_ = p; }

    // Each entity has a position in 3d world (cell)
    virtual Vec3i get_pos() const;
    virtual void set_pos(const Vec3i &v);

    // Each entity has ID so that we don't have to use pointers everywhere
    // -- Entity --
    virtual EntityId get_id() const { return id_; }
    virtual void set_id(EntityId id) { id_ = id; }

    // Draw me like one of your cubic models
    //virtual ModelId get_model_id() { return model_id_; }
    AAnimateObject* get_parent() { return parent_; }

    // If route is planned, do a step. TODO: Respect movement speed
    void step();
    bool attempt_move(const Vec3i &new_pos);

    // Routing
    const Route& get_route() const { return movement_.planned_route; }
    // Move to dst or any adjacent
    bool move_to(const Vec3i& pos);
    // Move to any adjacent but NOT to dst
    bool move_to(const Vec3i &dst, MovePrecision mp);
    void clear_planned_route() { movement_.planned_route.clear(); }
    bool is_moving() const {
        return movement_.planned_route.empty() == false;
    }
    void set_planned_route(const Vec3i& dst, Route &r);
    Vec3i get_move_destination() const { return movement_.dst; }

private:
    // Find route ignoring walls only respecting ground
    Route find_relaxed_route(const Vec3i& dst);
    // Find strict route using ground
    Route find_route(const Vec3i& dst);
    // This is called several times in attempt to find a path to a block
    bool find_and_set_strict_route(const Vec3i &dst);
};

//QDebug operator<< (QDebug d, const EntityComponent& ent);

} // namespace bm
