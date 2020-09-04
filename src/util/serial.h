
#pragma once

namespace util
{

    template <typename _Tp>
    struct serial_hash
    {
        size_t operator()(const _Tp& obj) const noexcept
        {
            const char* serial = reinterpret_cast<const char*>(&obj);
            size_t length = sizeof(_Tp);

            size_t hash = 0;
            for (size_t i = 0; i < length; ++i) {
                hash += serial[i];
            }
            return hash;
        }
    };

    template <typename _Tp>
    struct serial_equal_to
    {
        bool operator()(const _Tp& obj1, const _Tp& obj2)
        {
            const char* serial1 = reinterpret_cast<const char*>(&obj1);
            const char* serial2 = reinterpret_cast<const char*>(&obj2);
            size_t length = sizeof(_Tp);

            for (size_t i = 0; i < length; ++i) {
                if (serial1[i] != serial2[i]) return false;
            }
            return true; // serial1 == serial2
        }
    };

}