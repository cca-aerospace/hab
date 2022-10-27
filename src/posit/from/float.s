.macro namespace ns

    .include "src/posit/posit.s"

    .extern \ns\()_from_triple

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

    /* copy sign bit into r20 */
    ldi     r20,     0
    bst     r25,     7
    bld     r20,     0

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

    mov     r22,     r25
    mov     r25,     r24
    mov     r24,     r23
    call    \ns\()_from_triple

    /* epilouge */
0:
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

\ns\()_from_float_nan:
    /* when exponent == 0xFF and fraction != 0 */
    /* NaN */
    ldi     r25,     hi8(posit16_nar)
    ldi     r24,     lo8(posit16_nar)
    rjmp    0b

\ns\()_from_float_infinity:
    /* when exponent == 0xFF and fraction == 0 */
    cp      r18,     r1
    breq    \ns\()_from_float_positive_infinity

\ns\()_from_float_negative_infinity:
    /* when sign == 1 */
    /* -infinity */
    ldi     r25,     hi8(posit16_min)
    ldi     r24,     lo8(posit16_min)
    rjmp    0b

\ns\()_from_float_positive_infinity:
    /* when sign == 0 */
    /* +infinity */
    ldi     r25,     hi8(posit16_max)
    ldi     r24,     lo8(posit16_max)
    jmp     0b

.endm

namespace posit16
