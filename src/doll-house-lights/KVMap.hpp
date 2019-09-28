#pragma once

namespace dhl {
namespace kvmap {

template<typename Key, typename Value>
class KVMap {
    public:
        struct Entry {
            Key key;
            Value value;
        };

    public:
        constexpr KVMap(const Entry* const map, const uint32_t size) noexcept
           : m_map(map), m_size(size) {}

        const Value* value(const Key key) const noexcept {
            for (auto i = 0; i < m_size; i++) {
                if (m_map[i].key == key) {
                    return &(m_map[i].value);
                }
            }
            return nullptr;
        }
    private:
        const Entry* const m_map;
        uint32_t m_size;
};

}}  // end of namespace