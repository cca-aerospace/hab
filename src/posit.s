    .include "src/config.s"

    .global posit16_from_float

    .equ posit16_nar, 1 << 15
    .equ posit16_maxpos, 0x7FFF
    .equ posit16_minpos, 0x0001

    .equ posit16_max, 0x7FFF
    .equ posit16_min, 0x8001

    .equ posit16_useed_shift, 1 << 1

    .equ posit16_useed, 1 << posit16_useed_shift

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

posit16_from_float:
    /* prolouge */
    push    r16
    push    r17
    /* prolouge */

    /* set one */
    mov     r15,     1
    /* set bitsleft */
    /* bitsleft = 16 - 1 (sign bit) - 1 (regime end) */
    mov     r13,     14

    /* copy sign bit into r18 */
    mov     r18,     0
    bst     r25,     7
    bld     r18,     0

    /* shift exponent into r25 */
    /* shift fraction so it is aligned with the msb of r24 */
    lsl     r22
    rol     r23
    rol     r24
    rol     r25

    /* handle NaN and +/- infinity */
    cpi     r25,     0xFF
    breq    float_special_case

    /* set posit sign bit */
    mov     r16,     r18

    /* apply exponent bias */
    subi    r25,     127
    /* set klim to useed */
    mov     r20,     posit16_useed

    bst     r25,     7
    bld     r0,      0

    sub     r1,      r0

    /* r25 = abs(exponent) */
    eor     r25,     r1
    add     r25,     r0

    /* zero r20 if exponent is negative */
    com     r1
    and     r20,     r1

    mov     r21,     r0
    eor     r21,     r15

    /* restore the zero register */
    mov     r1,      0

    /* save exponent bit into T */
    bst     r25,     0
    /* set k to rbit */
    mov     r19,     r21

    /* skip loop if exponent is less than klim */
    cp      r25,     r20
    brlt    2f

1:
    add     r19,     r15
    sub     r13,     r15
    breq    end_regime
    sub     r25,     posit16_useed_shift
    /* continue loop while exponent is greater than or equal to klim */
    cp      r25,     r20
    brge    1b

2:
    lsl     r16
    rol     r17
    or      r16,     r21
    sub     r19,     r15
    brne    2b

end_regime:
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
1:
    lsl     r16
    rol     r17
    lsl     r22
    rol     r23
    rol     r24

    in      r0,      __io_sreg
    bst     r0,      __sreg_carry
    bld     r16,     0

    sub     r13,     r15
    brne    1b    

    /* epilouge */
0:
    pop     r17
    pop     r16
    /* epilouge */
    ret

float_special_case:
    /* or all the fraction bits together to check for 0 */
    or      r22,     r23
    or      r22,     r24

    cp      r22,     r1
    breq    float_infinity

float_nan:
    /* when exponent == 0xFF and fraction != 0 */
    /* NaN */
    ldi     r25,     hi8(posit16_nar)
    ldi     r24,     lo8(posit16_nar)
    jmp     0b

float_infinity:
    /* when exponent == 0xFF and fraction == 0 */
    cp      r18,     r1
    breq    clamp_maxpos

clamp_negative_infinity:
    /* when sign == 1 */
    /* -infinity */
    ldi     r25,     hi8(posit16_min)
    ldi     r24,     lo8(posit16_min)
    jmp     0b

clamp_positive_infinity:
    /* when sign == 0 */
    /* +infinity */
    ldi     r25,     hi8(posit16_max)
    ldi     r24,     lo8(posit16_max)
    jmp     0b

