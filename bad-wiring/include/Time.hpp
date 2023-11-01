#ifndef __TIME_HPP_
#define __TIME_HPP_

typedef unsigned long time_t;

inline __attribute__((always_inline)) time_t seconds(time_t ms) {
    return ms * 1000;
}

inline __attribute__((always_inline)) time_t minutes(time_t ms) {
    return ms * 1000 * 60;
}

inline __attribute__((always_inline)) time_t hours(time_t ms) {
    return ms * 1000 * 60 * 60;
}

inline __attribute__((always_inline)) time_t days(time_t ms) {
    return ms * 1000 * 60 * 60 * 24;
}

#endif