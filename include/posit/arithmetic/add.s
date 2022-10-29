.macro namespace ns

    .include "src/posit/posit.s"

    .extern \ns\()_to_triple
    .extern \ns\()_internal_add

    .global \ns\()_add

/*
 * adds two posit16 numbers
 *
 * posit a in r24:r25
 * posit b in r22:r23
 */
\ns\()_add:
    /* prolouge */
    /* prolouge */

    /* handle zero and nar cases
     * case 0: 0 + b = b
     * case 1: a + 0 = a
     * case 2: nar + b = nar
     * case 3: a + nar = nar
     * case 4: nar + nar = nar
     */
    mov     r0,      r25
    mov     r18,     r24
    lsl     r0
    or      r18,     r0
    brne    \ns\()_add_check_b
    brcs    \ns\()_add_return_nar

    movw    r24,     r22
    rjmp    0f

\ns\()_add_check_b:
    mov     r0,      r23
    mov     r18,     r22
    lsl     r0
    or      r18,     r0
    brne    \ns\()_add_next
    brcs    \ns\()_add_return_nar

    /* returns a */
    rjmp    0f

\ns\()_add_next:
    call    \ns\()_internal_add

    /* epilouge */
0:
    /* epilouge */
    ret

\ns\()_add_return_nar:
    ldi     r24,     lo8(posit16_nar)
    ldi     r25,     hi8(posit16_nar)
    rjmp    0b

.endm

namespace posit16
