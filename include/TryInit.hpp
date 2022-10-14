#ifndef __TRY_INIT_HPP_
#define __TRY_INIT_HPP_

#define TryInit(retry, sensor, ...) \
    for (int attempts = retry; attempts != 0; attempts--) { \
        if (!sensor.begin(__VA_ARGS__)) { \
            Serial.println(F("failed to initialize "#sensor)); \
            delay(500); \
        } else { \
            Serial.println(F("initialized "#sensor)); \
        break; \
        } \
    }

#endif