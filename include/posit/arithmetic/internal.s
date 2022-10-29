.macro namespace ns

    .extern \ns\()_to_triple

    .global \ns\()_internal_add

/*
 * posit a in r24:r25
 * posit b in r22:r23
 * signs must be the same
 */
\ns\()_internal_add:
    /* prolouge */
    push    r14
    push    r15
    push    r16
    push    r17
    /* prolouge */

    /* save b into r14:r15 */
    movw    r14,     r22

    /* convert a to triple */
    call    \ns\()_to_triple
    movw    r16,     r24
    movw    r24,     r14
    mov     r14,     r20
    mov     r15,     r22

    /* convert b to triple */
    call    \ns\()_to_triple
    mov     r21,     r22

    /* set upper a fraction */
    movw    r18,     r16

    /* set implicit leading one */
    /* this will never cause the fraction to round
     * because the fraction is at most 14 bits
     */
    sec
    ror     r19
    ror     r18
    sec
    ror     r25
    ror     r24

    /* r14 : a sign
     * r15 : a scale
     * r16:r19 : a frac
     *
     * r20 : b sign
     * r21 : b scale
     * r22:r25 : b frac
     */

    cp      r21,     r15
    brge    1f

    movw    r22,     r24
    movw    r24,     r18
    movw    r18,     r22
    movw    r16,     r20
    movw    r20,     r14
    movw    r14,     r16

1:
    /* b scale is guaranteed to be greater than or equal to a scale */
    /* zero the bottom parts of the fractions */
    clr     r0
    movw    r22,     r0
    movw    r16,     r0

    /* set r26 to one for use later */
    ldi     r26,     1

    cp      r21,     r15
    breq    \ns\()_internal_add_sum_fractions

\ns\()_internal_add_scale:
    add     r15,     r26
    lsr     r19
    ror     r18
    ror     r17
    ror     r16
    cp      r15,     r21
    brlt    \ns\()_internal_add_scale

\ns\()_internal_add_sum_fractions:
    add     r22,     r16
    adc     r23,     r17
    adc     r24,     r18
    adc     r25,     r19

    brcc \ns\()_internal_add_convert

    /* adjust fraction */
    ror     r25
    ror     r24
    ror     r23
    ror     r22

    /* increment scale by one */
    add     r21,     r26

\ns\()_internal_add_convert:
    lsl     r25
    rol     r24
    rol     r23
    rol     r22

    mov     r22,     r21

    /* move arguments into corresponding registers */
    call    \ns\()_from_triple

    /* epilouge */
0:
    pop     r17
    pop     r16
    pop     r15
    pop     r14
    /* epilouge */
    ret

.endm

namespace posit16
