/* wrap_g.s */

/*
IRQ wrappers for DJGPP.
*/

.data
_IRQWrappers:
	.long	_IRQWrap0,  _IRQWrap1,  _IRQWrap2,  _IRQWrap3
	.long	_IRQWrap4,  _IRQWrap5,  _IRQWrap6,  _IRQWrap7
	.long	_IRQWrap8,  _IRQWrap9,  _IRQWrap10, _IRQWrap11
	.long	_IRQWrap12, _IRQWrap13, _IRQWrap14, _IRQWrap15

_IRQHandlers:
	.long	0, 0, 0, 0  	/* 0 - 3 */
	.long	0, 0, 0, 0	/* 4 - 7 */
	.long	0, 0, 0, 0	/* 8 - 11 */
	.long	0, 0, 0, 0	/* 12 - 15 */

	.globl	_IRQWrappers
	.globl	_IRQHandlers
	.globl	_IRQWrap
	.globl	_IRQWrap_End

/*
How many stacks to allocate for the irq wrappers. You could 
probably get away with fewer of these, if you want to save memory and
you are feeling brave...
Extracted from irqwrap.h: BOTH SHOULD BE THE SAME!
*/
#define IRQ_STACKS      8

.text
#define IRQWRAP(x)							; \
_IRQWrap##x:								; \
	pushw	%ds			/* save registers */		; \
	pushw	%es							; \
	pushw	%fs							; \
	pushw	%gs							; \
	pushal								; \
	/* __djgpp_ds_alias is irq sequred selector (see exceptn.h) */	; \
	movw	%cs:___djgpp_ds_alias, %ax				; \
	movw	%ax, %ds		/* set up selectors */		; \
	movw	%ax, %es						; \
	movw	%ax, %fs						; \
	movw	%ax, %gs						; \
									; \
	movl	$(IRQ_STACKS - 1), %ecx	/* look for a free stack */	; \
	/* Search from the last toward the first */			; \
StackSearchLoop##x:							; \
	leal	_IRQStacks(, %ecx, 4), %ebx				; \
	cmpl	$0, (%ebx)						; \
	jnz	FoundStack##x		/* found one! */		; \
									; \
	decl	%ecx			/* backward */			; \
	jnz	StackSearchLoop##x					; \
									; \
	jmp	GetOut##x		/* No free stack! */		; \
									; \
FoundStack##x:								; \
	movl	%esp, %ecx		/* save old stack in ecx:dx */	; \
	movw	%ss, %dx						; \
									; \
	movl	(%ebx), %esp		/* set up our stack */		; \
	movw	%ax, %ss						; \
									; \
	movl	$0, (%ebx)		/* flag the stack is in use */	; \
									; \
	pushl	%edx			/* push old stack onto new */	; \
	pushl	%ecx							; \
	pushl	%ebx							; \
									; \
	cld				/* clear the direction flag */	; \
									; \
	movl	_IRQHandlers + 4 * x, %eax				; \
	call	*%eax			/* call the C handler */	; \
									; \
	popl	%ebx			/* restore the old stack */	; \
	popl	%ecx							; \
	popl	%edx							; \
	movl	%esp, (%ebx)						; \
	movw	%dx, %ss						; \
	movl	%ecx, %esp						; \
									; \
	orl	%eax, %eax		/* check return value */	; \
	jz	GetOut##x						; \
									; \
	popal				/* chain to old handler */	; \
	popw	%gs							; \
	popw	%fs							; \
	popw	%es							; \
	popw	%ds							; \
					/* 8 = sizeof(__dpmi_paddr) */	; \
	ljmp	*%cs:_OldIRQVectors + 8 * x				; \
									; \
GetOut##x:								; \
	popal				/* iret */			; \
	popw	%gs							; \
	popw	%fs							; \
	popw	%es							; \
	popw	%ds							; \
	sti								; \
	iret

_IRQWrap:
	.byte	0

IRQWRAP(0);
IRQWRAP(1);
IRQWRAP(2);
IRQWRAP(3);
IRQWRAP(4);
IRQWRAP(5);
IRQWRAP(6);
IRQWRAP(7);
IRQWRAP(8);
IRQWRAP(9);
IRQWRAP(10);
IRQWRAP(11);
IRQWRAP(12);
IRQWRAP(13);
IRQWRAP(14);
IRQWRAP(15);

_IRQWrap_End:
	.byte	0

