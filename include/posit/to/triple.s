.macro namespace ns

    .include "src/posit/posit.s"

    .global \ns

/*
 * requires checking for NaR and zero first
 * returns the 4 parts of a 16 bit posit
 *
 * posit passed in r24:r25
 *
 * fraction is aligned to msb of r25
 * leading one is not automatically appended
 *
 * | fraction | r24:r25 |
 * | scale    | r22     |
 * | sign     | r20     |
 */

/* let sign = r19 */
/* let mask = r18 */
/* let bitsleft = r17 */
\ns\():
    /* prolouge */
    push    r16
    push    r17
    /* prolouge */

    /* zero scale */
    ldi     r23,     0
    ldi     r22,     0
    /* zero sign */
    movw    r20,     r22
    /* set bitsleft */
    ldi     r17,     15

    bst     r25,     7
    bld     r20,     0

    ldi     r18,     0
    sub     r18,     r20

    eor     r24,     r18
    eor     r25,     r18
    add     r24,     r20
    adc     r25,     r1

    lsl     r24
    rol     r25

    mov     r18,     r25
    andi    r18,     0x80

    ldi     r19,     posit16_useed_shift

\ns\()_count_scale:
    subi    r17,     1
    eor     r25,     r18
    lsl     r24
    rol     r25
    brcs    2f
    add     r22,     r19
    rjmp    \ns\()_count_scale

2:
    bst     r18,     7
    bld     r16,     0
    com     r16
    andi    r16,     1
    ldi     r18,     0
    sub     r18,     r16

    eor     r22,     r18
    add     r22,     r16

    com     r18
    and     r19,     r18
    sub     r22,     r19

    tst     r17
    breq    0f

    lsl     r24
    rol     r25

    /* this only works because the carry bit is bit 0 */
    in      r16,     __io_sreg
    andi    r16,     1
    add     r22,     r16

    /* epilouge */
0:
    pop     r17
    pop     r16
    /* epilouge */
    ret

.endm

namespace posit16_to_triple
