    .ifndef __POSIT_S_
    .equ    __POSIT_S_, 1


    .include "src/config.s"

    .equ posit16_nar, 1 << 15
    .equ posit16_maxpos, 0x7FFF
    .equ posit16_minpos, 0x0001

    .equ posit16_max, 0x7FFF
    .equ posit16_min, 0x8001

    .equ posit16_useed_shift, 1 << 1

    .equ posit16_useed, 1 << posit16_useed_shift


    .endif /* __POSIT_S_ */
