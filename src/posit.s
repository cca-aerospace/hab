    .include "src/config.s"

    .global posit16_from_float
    .global float_from_posit16

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
    breq    float_special_case

    /* apply exponent bias */
    subi    r25,     127
    /* set klim to useed */
    ldi     r20,     posit16_useed

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
    brlt    2f

1:
    add     r19,     r15
    sub     r13,     r15
    breq    end_regime
    subi    r25,     posit16_useed_shift
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
    lsl     r22
    rol     r23
    rol     r24

    rol     r16
    rol     r17

    sub     r13,     r15
    brne    1b

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

float_special_case:
    /* or all the fraction bits together to check for 0 */
    or      r22,     r23
    or      r22,     r24

    cp      r22,     r1
    breq    float_infinity

float_nan:
    /* when exponent == 0xFF and fraction != 0 */
    /* NaN */
    ldi     r17,     hi8(posit16_nar)
    ldi     r16,     lo8(posit16_nar)
    rjmp    0b

float_infinity:
    /* when exponent == 0xFF and fraction == 0 */
    cp      r18,     r1
    breq    clamp_positive_infinity

clamp_negative_infinity:
    /* when sign == 1 */
    /* -infinity */
    ldi     r17,     hi8(posit16_min)
    ldi     r16,     lo8(posit16_min)
    rjmp    0b

clamp_positive_infinity:
    /* when sign == 0 */
    /* +infinity */
    ldi     r17,     hi8(posit16_max)
    ldi     r16,     lo8(posit16_max)
    jmp     0b


    .equ float_nan3, 0b01111111
    .equ float_nan2, 0b10000000
    .equ float_nan1, 0b00000000
    .equ float_nan0, 0b00000001

/*
 * builds a 32 bit float from a 16 bit posit
 *
 * posit is passed in r24:r25
 *
 * float is returned in r22:r25
 */
float_from_posit16:
    /* prolouge */
    push    r16
    /* prolouge */

    tst     r24
    brne    1f

    tst     r25
    breq    2f
    brmi    3f

1:
    call    posit16_decode

    mov     r18,     r24
    mov     r0,      r22

    mov     r23,     r20
    mov     r22,     r0
    ldi     r16,     127
    add     r22,     r16

    mov     r16,     23
1:
    lsl     r18
    rol     r19

    rol     r22
    rol     r23
    rol     r24
    rol     r25

    subi    r16,     1
    brne    1b

    /* epilouge */
0:
    pop     r16
    /* epilouge */
    ret

2:
    mov      r23,     0
    mov      r22,     0
    rjmp     0b

3:
    ldi      r25,     float_nan3
    ldi      r24,     float_nan2
    ldi      r23,     float_nan1
    ldi      r22,     float_nan0
    rjmp     0b

/*
 * requires checking for NaR and zero first
 * returns the 4 parts of a 16 bit posit
 *
 * posit passed in r24:r25
 *
 * fraction is aligned to msb of r25
 *
 * | fraction | r24:r25 |
 * | scale    | r22     |
 * | sign     | r20     |
 */

/* let sign = r19 */
/* let mask = r18 */
/* let bitsleft = r17 */
posit16_decode:
    /* prolouge */
    push    r17
    /* prolouge */

    /* zero scale */
    ldi     r22,     0
    /* zero fraction */
    ldi     r21,     0
    ldi     r20,     0
    /* set bitsleft */
    ldi     r17,     15

    ldi     r19,     0
    bst     r25,     7
    bld     r19,     0

    ldi     r18,     0
    sub     r18,     r19

    eor     r24,     r18
    eor     r25,     r18
    add     r24,     r19
    adc     r25,     r1

    lsl     r24
    rol     r25

    mov     r18,     r25
    andi    r18,     0x80

1:
    add     r22,     posit16_useed_shift
    sub     r17,     1
    eor     r25,     r18
    lsl     r24
    rol     r25
    brcc    1b

    eor     r22,     r18
    add     r22,     r0

    tst     r17
    breq    0f

    lsl     r24
    rol     r25

    /* this only works because the carry bit is bit 0 */
    in      r17,     __io_sreg
    andi    r17,     1

    add     r22,     r0

    /* epilouge */
0:
    pop     r17
    /* epilouge */
    ret
