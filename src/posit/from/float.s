.macro namespace ns

    .include "src/posit/posit.s"

    .global \ns\()_from_float

/*
 * constructs a posit16 with es = 1 from a 32 bit float
 * 
 * float is passed in r22:r25
 * 
 * return value in r24:r25
 */

/* let rbit = r21 */
/* let klim = r20 */

/* let k = r19 */
/* let sign = r18 */

/* let posit_high = r17 */
/* let posit_low = r16 */

/* let one = r15 */
/* let exponent = r14 */
/* let bitsleft = r13 */

\ns\()_from_float:
    /* prolouge */
    push    r13
    push    r14
    push    r15
    push    r16
    push    r17
    /* prolouge */

    /* set one */
    ldi     r16,     1
    mov     r15,     r16
    /* set bitsleft */
    /* bitsleft = 16 - 1 (sign bit) - 1 (regime end) */
    ldi     r16,     14
    mov     r13,     r16

    /* zero upper posit (unnecessary but useful for debugging) */
    ldi     r17,     0
    /* copy sign bit into r16 */
    ldi     r16,     0
    bst     r25,     7
    bld     r16,     0

    /* shift exponent into r25 */
    /* shift fraction so it is aligned with the msb of r24 */
    lsl     r22
    rol     r23
    rol     r24
    rol     r25

    /* handle NaN and +/- infinity */
    cpi     r25,     0xFF
    breq    \ns\()_from_float_not_normal

    /* apply exponent bias */
    subi    r25,     127
    /* set klim to useed_shift */
    ldi     r20,     posit16_useed_shift

    ldi     r18,     0
    clr     r0
    bst     r25,     7
    bld     r0,      0

    sub     r18,     r0

    /* r25 = abs(exponent) */
    eor     r25,     r18
    add     r25,     r0

    /* zero r20 if exponent is negative */
    com     r18
    and     r20,     r18

    mov     r21,     r0
    eor     r21,     r15

    /* save exponent bit into T */
    bst     r25,     0
    /* set k to rbit */
    mov     r19,     r21
    /* take into account the potential extra regime bit */
    sub     r13,     r21

    /* skip loop if exponent is less than klim */
    cp      r25,     r20
    brlt    \ns\()_from_float_build_regime

\ns\()_from_float_count_regime:
    add     r19,     r15
    sub     r13,     r15
    breq    \ns\()_from_float_end_regime
    subi    r25,     posit16_useed_shift
    /* continue loop while exponent is greater than or equal to klim */
    cp      r25,     r20
    brge    \ns\()_from_float_count_regime

\ns\()_from_float_build_regime:
    lsl     r16
    rol     r17
    or      r16,     r21
    sub     r19,     r15
    brne    \ns\()_from_float_build_regime

\ns\()_from_float_end_regime:
    eor     r21,     r15
    lsl     r16
    rol     r17
    or      r16,     r21

    tst     r13
    breq    0f

    /* load exponent from T */
    lsl     r16
    rol     r17
    bld     r16,     0
    
    sub     r13,     r15
    breq    0f

    /* calculate fraction */
\ns\()_from_float_build_fraction:
    lsl     r22
    rol     r23
    rol     r24

    rol     r16
    rol     r17

    sub     r13,     r15
    brne    \ns\()_from_float_build_fraction

    /* epilouge */
0:
    movw    r24,     r16
    pop     r17
    pop     r16
    pop     r15
    pop     r14
    pop     r13
    /* epilouge */
    ret

\ns\()_from_float_not_normal:
    /* or all the fraction bits together to check for 0 */
    or      r22,     r23
    or      r22,     r24

    cp      r22,     r1
    breq    \ns\()_from_float_infinity

\ns\()_float_nan:
    /* when exponent == 0xFF and fraction != 0 */
    /* NaN */
    ldi     r17,     hi8(posit16_nar)
    ldi     r16,     lo8(posit16_nar)
    rjmp    0b

\ns\()_float_infinity:
    /* when exponent == 0xFF and fraction == 0 */
    cp      r18,     r1
    breq    \ns\()_from_float_positive_infinity

\ns\()_from_float_negative_infinity:
    /* when sign == 1 */
    /* -infinity */
    ldi     r17,     hi8(posit16_min)
    ldi     r16,     lo8(posit16_min)
    rjmp    0b

\ns\()_from_float_positive_infinity:
    /* when sign == 0 */
    /* +infinity */
    ldi     r17,     hi8(posit16_max)
    ldi     r16,     lo8(posit16_max)
    jmp     0b

.endm

namespace posit16
