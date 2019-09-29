#pragma once

#include "Keypad.hpp"
#include "KVMap.hpp"

namespace dhl {
namespace roomie {

// Using plain and not class enum since values are used are room array index
// and I don't want to cast it to int each and every time.
enum Room {
    All = -1,
    BL = 0,
    BR = 1,
    ML = 2,
    MR = 3,
    TL = 4,
    TR = 5,
    Eave = 6,
};

using keypad::Keys;
using RoomMap = kvmap::KVMap<Keys, Room>;

const RoomMap::Entry _rmap[] = {
    {Keys::RoomAll, Room::All},
    {Keys::RoomBL, Room::BL},
    {Keys::RoomBR, Room::BR},
    {Keys::RoomML, Room::ML},
    {Keys::RoomMR, Room::MR},
    {Keys::RoomTL, Room::TL},
    {Keys::RoomTR, Room::TR},
    {Keys::RoomEave, Room::Eave},
};
const RoomMap room_map(_rmap, sizeof(_rmap) / sizeof(_rmap[0]));

}} // end of namespace