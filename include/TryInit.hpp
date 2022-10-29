#ifndef __TRY_INIT_HPP_
#define __TRY_INIT_HPP_

#include "Debug.hpp"

#define TryInit(retry, sensor, ...)                                            \
    for (int attempts = retry; attempts != 0; attempts--) {                    \
        if (!sensor.begin(__VA_ARGS__)) {                                      \
            Println(F("failed to initialize " #sensor));                       \
            delay(500);                                                        \
        } else {                                                               \
            Println(F("initialized " #sensor));                                \
            break;                                                             \
        }                                                                      \
    }

#endif