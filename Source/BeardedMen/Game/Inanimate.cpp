#include "BeardedMen.h"
#include "Game/Inanimate.h"

namespace bm {

InanimateObject::InanimateObject(InanimateType t): type_(t) {
    switch (t) {
    case InanimateType::Boulder: model_ = ModelId::Boulder; break;
    }
}

} // ns bm
