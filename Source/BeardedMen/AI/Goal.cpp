#include "BeardedMen.h"
#include "AI/Goal.h"

//#include <QDebug>

#include "Game/World.h"
#include "AnimateObject.h"
#include "Game/WorldVolume.h"

namespace bm {
namespace ai {

uint64_t Order::last_id_ = 0;

Vec3i Value::get_pos() const
{
    check(is_position());
    return Vec3i(pos_.x, pos_.y, pos_.z);
}

/*
QDebug operator<<(QDebug d, const Value &v) {
    d.nospace();
    switch (v.get_type()) {
    case Value::Type::NoValue: d << "NoValue"; break;
    case Value::Type::Boolean: d << v.get_boolean(); break;
    case Value::Type::Position: d << v.get_pos(); break;
    }
    return d;
}

QDebug operator<<(QDebug d, const MetricVec &metrics) {
    d.nospace() << "Vec[";
    for(auto &m: metrics) {
        d << m << "; ";
    }
    d << "]";
    return d;
}

QDebug operator<<(QDebug d, const Metric &m) {
    d.nospace() << "Mtr{";
    d << m.type_ << "; arg=" << m.arg_ << "; rd=" << m.reading_
      << "}";
    return d;
}
*/

} // ns ai::bm
} // ns bm
