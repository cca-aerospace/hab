.macro namespace ns

    .include "src/posit/posit.s"

    .extern \ns\()_to_triple

    .global \ns\()_to_float

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
\ns\()_to_float:
    /* prolouge */
    push    r16
    /* prolouge */

    tst     r24
    brne    \ns\()_to_float_handle_normal

    mov     r0,      r25
    lsl     r0
    brne    \ns\()_to_float_handle_normal
    brcs    \ns\()_to_float_handle_nar

\ns\()_to_float_handle_zero:
    ldi      r23,     0
    ldi      r22,     0
    rjmp     0f

\ns\()_to_float_handle_normal:
    call    \ns\()_to_triple

    movw    r18,     r24

    mov     r23,     r20
    ldi     r16,     127
    add     r22,     r16

    ldi     r16,     23
\ns\()_to_float_build_fraction:
    lsl     r18
    rol     r19

    rol     r22
    rol     r23
    rol     r24
    rol     r25

    subi    r16,     1
    brne    \ns\()_to_float_build_fraction

    /* epilouge */
0:
    pop     r16
    /* epilouge */
    ret

\ns\()_to_float_handle_nar:
    ldi      r25,     float_nan3
    ldi      r24,     float_nan2
    ldi      r23,     float_nan1
    ldi      r22,     float_nan0
    rjmp     0b

.endm

namespace posit16
