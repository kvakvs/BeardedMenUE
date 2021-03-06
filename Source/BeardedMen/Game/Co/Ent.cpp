#include "BeardedMen.h"

#include <PolyVox/AStarPathfinder.h>

#include "Game/Co/Ent.h"
#include "Game/World.h"
#include "AnimateObject.h"

namespace bm {

bm::Vec3i EntityComponent::get_pos() const {
    auto v = parent_->GetActorLocation();
    return bm::Vec3i(v.X / 8.f, v.Y / 8.f, v.Z / 8.f);
}

void EntityComponent::set_pos(const Vec3i &v) {
    auto wo = get_parent()->get_world();
    wo->animate_position_changed_d(get_parent(), get_pos());
    //pos_ = v;
    parent_->SetActorLocation(
        FVector(v.getX() * 8.f, v.getY() * 8.f, v.getZ() * 8.f));
}

void EntityComponent::step() {
    if (movement_.planned_route.empty() == false) {
        if (attempt_move(movement_.planned_route.front())) {
            movement_.planned_route.pop_front();
        } else {
            movement_.planned_route.clear();
        }
    }
}

bool EntityComponent::attempt_move(const Vec3i &new_pos)
{
    auto wo = get_parent()->get_world();
    auto new_pos_v = wo->get_voxel(new_pos);

    if (not new_pos_v.is_not_air() || new_pos_v.is_ramp()) {
        // If can move horizontally or if there is ramp - can stand on it
        set_pos(new_pos);
        return true;
    }
    return false;
}

// Relaxed validator which ignores walls for routing
inline bool voxel_validate_relaxed(const VolumeType* vol,
                                   const Vec3i &p) {
    return vol->getVoxel(p + Vec3i(0,1,0)).is_not_air();
}

// Stricter validator which respects walls
static inline bool voxel_validate_strict(const VolumeType* vol,
                                         const Vec3i &p) {
    auto v_at    = vol->getVoxel(p);
    auto v_under = vol->getVoxel(p + Vec3i(0,1,0));

    // Standing on solid; or standing inside ramp; or standing on top of ramp
    return v_at.is_walkable_into() && v_under.is_walkable_on();
}

bool EntityComponent::move_to(const Vec3i &dst)
{
    clear_planned_route();
    // TODO: Try first sides closer to the worker
    if (find_and_set_strict_route(dst)) return true;
    if (find_and_set_strict_route(dst + Vec3i(1,0,0))) return true;
    if (find_and_set_strict_route(dst + Vec3i(-1,0,0))) return true;
    if (find_and_set_strict_route(dst + Vec3i(0,0,1))) return true;
    if (find_and_set_strict_route(dst + Vec3i(0,0,-1))) return true;
    return false;
}

bool EntityComponent::move_to(const Vec3i &dst, MovePrecision mp)
{
    clear_planned_route();
    switch (mp) {
    case MovePrecision::Exact: return find_and_set_strict_route(dst);
    case MovePrecision::Adjacent:
        // TODO: Try first sides closer to the worker
        return find_and_set_strict_route(dst + Vec3i( 1, 0, 0))
            || find_and_set_strict_route(dst + Vec3i(-1, 0, 0))
            || find_and_set_strict_route(dst + Vec3i( 0, 0, 1))
            || find_and_set_strict_route(dst + Vec3i( 0, 0,-1));
    case MovePrecision::AdjacentDepth:
        // TODO: Try first sides closer to the worker
        return find_and_set_strict_route(dst + Vec3i( 1, 0, 0))
            || find_and_set_strict_route(dst + Vec3i(-1, 0, 0))
            || find_and_set_strict_route(dst + Vec3i( 0, 0, 1))
            || find_and_set_strict_route(dst + Vec3i( 0, 0,-1))
            || find_and_set_strict_route(dst + Vec3i( 1,-1, 0))
            || find_and_set_strict_route(dst + Vec3i(-1,-1, 0))
            || find_and_set_strict_route(dst + Vec3i( 0,-1, 1))
            || find_and_set_strict_route(dst + Vec3i( 0,-1,-1));
    }
    unimplemented();
    return false;
}

bool EntityComponent::find_and_set_strict_route(const Vec3i& dst)
{
    // You are not going into the rock
    auto wo = get_parent()->get_world();
    auto dst_v = wo->get_voxel(dst);

    if (not dst_v.is_air() && not dst_v.is_ramp()) {
        return false;
    }

    Route path = find_route(dst);

    if (path.empty()) {
        return false;
    } else {
        set_planned_route(dst, path);
        return true;
    }
}

void EntityComponent::set_planned_route(const Vec3i &dst, Route &r) {
    movement_.dst = dst;
    movement_.planned_route = std::move(r);
}

Route EntityComponent::find_relaxed_route(const Vec3i &dst)
{
    Route result;
    auto wo = get_parent()->get_world();
    auto vol = wo->get_volume();

    // First find relaxed path over ground ignoring the walls
    pv::AStarPathfinderParams<VolumeType> pfpar(
                const_cast<VolumeType*>(vol),
                get_pos(), dst, & result,
                1.1 /*bias*/, 1000 /*maxlength*/,
                pv::SixConnected,
                & voxel_validate_relaxed);

    pv::AStarPathfinder<VolumeType> pf(pfpar);
    try {
        pf.execute();
        return result;
    } catch (std::runtime_error) {
        return Route();
    }
}

Route EntityComponent::find_route(const Vec3i &dst)
{
    Route result;
    auto wo = get_parent()->get_world();
    auto vol = wo->get_volume();

    // First find relaxed path over ground ignoring the walls
    pv::AStarPathfinderParams<VolumeType> pfpar(
                const_cast<VolumeType*>(vol),
                get_pos(), dst, & result,
                1.0 /*bias*/, 1000 /*maxlength*/,
                pv::SixConnected,
                & voxel_validate_strict);

    pv::AStarPathfinder<VolumeType> pf(pfpar);
    try {
        pf.execute();
        return result;
    } catch (std::runtime_error) {
        return Route();
    }
}

/*
QDebug operator<<(QDebug d, const EntityComponent &ent)
{
    d.nospace() << "Ent{";
    d << ent.get_id() << ";" << ent.get_pos();
    d << "}";
    return d;
}

QDebug operator<<(QDebug d, const Route &r)
{
    d.nospace() << "Route[";
    for (auto step: r) {
        d << step << "; ";
    }
    d << "]";
    return d;
}
*/

} // ns bm
