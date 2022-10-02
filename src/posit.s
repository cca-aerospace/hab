    .global  posit_from_float

    .section .text

.macro posit16 es

/*!
* constructs a posit16 from a 32 bit float
* 
* float is passed in r22:r25
* 
* return value in r24:r25
*/

/* define sign r18 */

posit_from_float:
    /* prolouge */
    /* prolouge */

    mov     r18,     0
    bst     r25,     7
    bld     r18,     0

    bst     r24,     7
    lsr     r25
    bld     r25,     0

    /* epilouge */
    /* epilouge */
    ret

.endm

posit16 0
