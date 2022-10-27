.macro namespace ns

    .include "src/posit/posit.s"
    
    .global \ns\()_from_triple

/* builds 16 bit posit from (sign, scale, fraction)
 *
 * fraction in r24:r25 (without leading one)
 * scale in r22
 * sign in r20
 *
 * returns posit in r24:r25
 */
\ns\()_from_triple:
    /* prolouge */
    push    r13
    push    r14
    push    r15
    push    r16
    push    r17
    /* prolouge */

    /* save fraciton in r18:r19 */
    movw    r18,     r24

    /* set one */
    ldi     r16,     1
    mov     r15,     r16
    /* set bitsleft */
    /* bitsleft = 16 - 1 (sign bit) */
    ldi     r16,     15
    mov     r13,     r16

    /* zero r24 (start building posit) */
    clr     r24

    ldi     r16,     posit16_useed_shift

    ldi     r17,     0
    clr     r0
    bst     r22,     7
    bld     r0,      0
    sub     r17,     r0
    /* r22 = abs(scale) */
    eor     r22,     r17
    add     r22,     r0

    /* zero r16 if scale is negative */
    com     r17
    and     r16,     r17
    add     r16,     r0

    mov     r21,     r0
    eor     r21,     r15

    /* save exponent bit into T */
    bst     r22,     0
    /* set k to rbit */
    mov     r17,     r21
    /* take into account the potential extra regime bit */
    sub     r13,     r21

    /* skip loop if exponent is less than klim */
    cp      r22,     r16
    brlt    \ns\()_from_triple_build_regime

\ns\()_from_triple_count_regime:
    add     r17,     r15
    sub     r13,     r15
    breq    \ns\()_from_triple_build_regime
    subi    r22,     posit16_useed_shift
    /* continue loop while exponent is greater than or equal to klim */
    cp      r22,     r16
    brge    \ns\()_from_triple_count_regime

\ns\()_from_triple_build_regime:
    lsl     r24
    rol     r25
    or      r24,     r21
    sub     r17,     r15
    brne    \ns\()_from_triple_build_regime

    tst     r13
    breq    \ns\()_from_triple_sign

\ns\()_from_triple_end_regime:
    eor     r21,     r15
    lsl     r24
    rol     r25
    or      r24,     r21

    sub     r13,     r15
    breq    \ns\()_from_triple_sign

    /* load exponent from T */
    lsl     r24
    rol     r25
    bld     r24,     0
    
    sub     r13,     r15
    breq    \ns\()_from_triple_sign

    /* calculate fraction */
\ns\()_from_triple_build_fraction:
    lsl     r18
    rol     r19

    rol     r24
    rol     r25

    sub     r13,     r15
    brne    \ns\()_from_triple_build_fraction

\ns\()_from_triple_sign:
    clr     r0
    sub     r0,      r20
    eor     r24,     r0
    eor     r25,     r0
    add     r24,     r20
    adc     r25,     r1

    /* epilouge */
0:
    pop     r17
    pop     r16
    pop     r15
    pop     r14
    pop     r13
    /* epilouge */
    ret

.endm

namespace posit16
