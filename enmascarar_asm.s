	HEADER_SIZE	equ	512
	
	section .text
	global enmascarar_asm
	;; enmascarar_asm(char *a, char *b, char *mask, int cant)

enmascarar_asm:
	push	ebp
	mov	ebp, esp
	push	ebx		; preserved register

parametros:
	mov	eax, [esp + 12] ; parámetro a
	mov	ebx, [esp + 16]	; parámetro b
	mov 	ecx, [esp + 20]	; parámetro mask
	mov	edx, [esp + 24]	; parámetro cant
start:
	mov	esi, HEADER_SIZE
loop:	
	movdqu	xmm0, [eax + esi]
	movdqu	xmm1, [ebx + esi]
	movdqu	xmm2, [ecx + esi]

	pand	xmm1, xmm2	; xmm1(imagen2) = xmm1 and xmm2(mascara)
	pcmpeqb	xmm3, xmm3	; xmm3 = todos 1
	pxor	xmm2, xmm3	; xmm2 = mascara negada
	pand	xmm0, xmm2	; xmm0 = imagen1 and (not mascara)
	por	xmm0, xmm1	; xmm0 = xmm0 or xmm1
	
	movdqu	[eax + esi], xmm0
	add	esi, 16
	cmp	esi, edx
	jne	loop
exit:
	pop	ebx
	pop	ebp
	ret
